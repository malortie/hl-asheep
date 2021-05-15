/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "monsters.h"
#include "schedule.h"
#include "soundent.h"
#include "decals.h"
#include "weapons.h"
#include "toadweaponmonster.h"

float Square(float x) { return x * x; }

#define TOAD_LOOK_DISTANCE		200
#define TOAD_WALK_FRAMERATE		3.0f

#define TOAD_SCARED_STEP_SIZE	15.0f
#define TOAD_IDLE_STEP_SIZE		50.0f

#define	TOAD_IDLE				0
#define	TOAD_SCARED_BY_ENT		1

//=========================================================
// Monster's Anim Events Go Here
//=========================================================

LINK_ENTITY_TO_CLASS(toadweaponmonster, CToadWeaponMonster);

TYPEDESCRIPTION CToadWeaponMonster::m_SaveData[] = 
{
	DEFINE_FIELD(CToadWeaponMonster, m_iMode, FIELD_INTEGER),
	DEFINE_FIELD(CToadWeaponMonster, m_scaryEntity, FIELD_EHANDLE),
	DEFINE_FIELD(CToadWeaponMonster, m_flNextSoundTime, FIELD_TIME),
};

IMPLEMENT_SAVERESTORE(CToadWeaponMonster, CBaseMonster);

const char* CToadWeaponMonster::pHuntSounds[] =
{
	"toad/toad_hunt1.wav",
	"toad/toad_hunt2.wav",
	"toad/toad_hunt3.wav",
};

void CToadWeaponMonster::Spawn()
{
	Precache();

	SET_MODEL(ENT(pev), "models/toad_nest.mdl");
	UTIL_SetSize(pev, Vector(-5, -5, 0), Vector(5, 5, 4));

	pev->solid = SOLID_BBOX;
	pev->movetype = MOVETYPE_STEP;
	m_bloodColor = BLOOD_COLOR_YELLOW;
	pev->effects = 0;
	pev->health = 1;
	m_flFieldOfView = -1.0f;// indicates the width of this monster's forward view cone ( as a dotproduct result )
	m_MonsterState = MONSTERSTATE_NONE;

	MonsterInit();
	PlayIdleAnimation();

	pev->view_ofs = Vector(0, 0, 2);// position of the eyes relative to monster's origin.
	pev->takedamage = DAMAGE_NO;
	m_flDistLook = TOAD_LOOK_DISTANCE;
	m_iMode = TOAD_IDLE;
	m_scaryEntity = NULL;
	m_flNextSoundTime = gpGlobals->time + RANDOM_FLOAT(3, 4);
}

void CToadWeaponMonster::Precache()
{
	PRECACHE_MODEL("models/toad_nest.mdl");

	PRECACHE_SOUND_ARRAY(pHuntSounds);
}


//=========================================================
// ISoundMask - returns a bit mask indicating which types
// of sounds this monster regards. In the base class implementation,
// monsters care about all sounds, but no scents.
//=========================================================
int CToadWeaponMonster::ISoundMask(void)
{
	return	bits_SOUND_PLAYER;
}

//=========================================================
// Touch
//=========================================================
void CToadWeaponMonster::Touch(CBaseEntity *pOther)
{
	Vector		vecSpot;
	TraceResult	tr;

	if (pOther->pev->velocity == g_vecZero || !pOther->IsPlayer())
	{
		return;
	}

	((CBasePlayer*)pOther)->GiveNamedItem(GetClassnameOfWeaponToGiveToToucher(pOther));
	UTIL_Remove(this);
}

void CToadWeaponMonster::SetYawSpeed(void)
{
	int ys;

	ys = 0;

	switch (m_Activity)
	{
	default:
	case ACT_IDLE:
		ys = 120;
		break;
	case ACT_RUN:
	case ACT_WALK:
		ys = 360;
		break;
	}

	pev->yaw_speed = ys;
}

void CToadWeaponMonster::MonsterThink(void)
{
	if (FNullEnt(FIND_CLIENT_IN_PVS(edict())))
		pev->nextthink = gpGlobals->time + RANDOM_FLOAT(1, 1.5);
	else
		pev->nextthink = gpGlobals->time + 0.1;// keep monster thinking

	UpdateAnimation();

	float flInterval = StudioFrameAdvance(); // animate

	switch (m_iMode)
	{
	case TOAD_IDLE:
	{
		// if not moving, sample environment to see if anything scary is around. Do a radius search 'look' at random.

		Look(TOAD_LOOK_DISTANCE);
		if (HasConditions(bits_COND_SEE_FEAR))
		{
			// if see something scary
			//ALERT ( at_aiconsole, "toad scared\n" );
			PickNewDest(TOAD_SCARED_BY_ENT);

			// Play a random croak/hop sound.
			if (RANDOM_LONG(0, 1))
			{
				PlayHuntSound();
				UpdateNextSound(TRUE);
			}
			else
			{
				PlayHuntSound();
				UpdateNextSound(FALSE);
			}
		}
		Listen();
	}
	break;

	default:
		break;
	}

	if (m_iMode == TOAD_SCARED_BY_ENT && FNullEnt(m_scaryEntity))
		m_iMode = TOAD_IDLE;

	if (m_iMode != TOAD_IDLE)
	{
		if (pev->angles.y != pev->ideal_yaw)
		{
			ChangeYaw(pev->yaw_speed);
		}
		if (pev->angles.y == pev->ideal_yaw && !IsPlayingWalkSequence())
		{
			Advance();
		}
		else if (IsPlayingWalkSequence() && m_flGroundSpeed != 0)
		{
			Move(flInterval);
		}
	}


	if (m_flNextSoundTime <= gpGlobals->time)
	{
		// every once in a while, a toad will make a croak/hop sound.
		if (RANDOM_LONG(0, 1))
		{
			PlayHuntSound();
			UpdateNextSound(TRUE);
		}
		else
		{
			PlayHuntSound();
			UpdateNextSound(FALSE);
		}
	}
}

//=========================================================
// Picks a new spot for toad to run to.(
//=========================================================
void CToadWeaponMonster::PickNewDest(int iCondition)
{
	Vector	vecDest;

	m_iMode = iCondition;

	if (m_iMode == TOAD_SCARED_BY_ENT)
	{
		if (!FNullEnt(m_scaryEntity))
		{
			vecDest = ComputeRunawayPosition(m_scaryEntity->pev->origin, TOAD_SCARED_STEP_SIZE);

			m_Route[0].vecLocation.x = vecDest.x;
			m_Route[0].vecLocation.y = vecDest.y;
			m_Route[0].vecLocation.z = pev->origin.z;
			m_Route[0].iType = bits_MF_TO_LOCATION;
			m_movementGoal = RouteClassify(m_Route[0].iType);
			return;
		}
	}

	do
	{
		// picks a random spot, requiring that it be at least 128 units away
		// else, the roach will pick a spot too close to itself and run in 
		// circles. this is a hack but buys me time to work on the real monsters.
		vecDest = ComputeRandomPosition(TOAD_IDLE_STEP_SIZE);

	} while ((vecDest - pev->origin).Length2D() < TOAD_IDLE_STEP_SIZE);

	m_Route[0].vecLocation.x = vecDest.x;
	m_Route[0].vecLocation.y = vecDest.y;
	m_Route[0].vecLocation.z = pev->origin.z;
	m_Route[0].iType = bits_MF_TO_LOCATION;
	m_movementGoal = RouteClassify(m_Route[0].iType);
}

//=========================================================
// toad's move function
//=========================================================
void CToadWeaponMonster::Move(float flInterval)
{
	float		flWaypointDist;
	Vector		vecApex;

	// local move to waypoint.
	flWaypointDist = (m_Route[m_iRouteIndex].vecLocation - pev->origin).Length2D();
	MakeIdealYaw(m_Route[m_iRouteIndex].vecLocation);

	ChangeYaw(pev->yaw_speed);
	UTIL_MakeVectors(pev->angles);

	if (!WALK_MOVE(ENT(pev), pev->ideal_yaw, m_flGroundSpeed * flInterval, WALKMOVE_NORMAL) || flWaypointDist <= m_flGroundSpeed * flInterval)
	{
		Stop();
		m_iMode = TOAD_IDLE;
	}
}

//=========================================================
// Look - overriden for the toad, which can virtually see 
// 360 degrees.
//=========================================================
void CToadWeaponMonster::Look(int iDistance)
{
	CBaseEntity	*pSightEnt = NULL;// the current visible entity that we're dealing with
	CBaseEntity	*pPreviousEnt;// the last entity added to the link list 
	int			iSighted = 0;

	int threatPerceptionConditions = bits_COND_SEE_HATE | bits_COND_SEE_DISLIKE | bits_COND_SEE_ENEMY | bits_COND_SEE_FEAR;

	// DON'T let visibility information from last frame sit around!
	ClearConditions(threatPerceptionConditions);

	// don't let monsters outside of the player's PVS act up, or most of the interesting
	// things will happen before the player gets there!
	if (FNullEnt(FIND_CLIENT_IN_PVS(edict())))
	{
		return;
	}

	m_pLink = NULL;
	pPreviousEnt = this;

	// Does sphere also limit itself to PVS?
	// Examine all entities within a reasonable radius
	// !!!PERFORMANCE - let's trivially reject the ent list before radius searching!
	while ((pSightEnt = UTIL_FindEntityInSphere(pSightEnt, pev->origin, iDistance)) != NULL)
	{
		// only consider ents that can be damaged. !!!temporarily only considering other monsters and clients
		if (pSightEnt->IsPlayer() || FBitSet(pSightEnt->pev->flags, FL_MONSTER))
		{
			if ( !FBitSet(pSightEnt->pev->flags, FL_NOTARGET) && pSightEnt->pev->health > 0)
			{
				// NULL the Link pointer for each ent added to the link list. If other ents follow, the will overwrite
				// this value. If this ent happens to be the last, the list will be properly terminated.
				pPreviousEnt->m_pLink = pSightEnt;
				pSightEnt->m_pLink = NULL;
				pPreviousEnt = pSightEnt;

				// don't add the Enemy's relationship to the conditions. We only want to worry about conditions when
				// we see monsters other than the Enemy.
				switch (IRelationship(pSightEnt))
				{
				case	R_FR:
					iSighted |= bits_COND_SEE_FEAR;
					m_scaryEntity = pSightEnt;
					break;
				case	R_NO:
					break;
				default:
					ALERT(at_console, "%s can't asses %s\n", STRING(pev->classname), STRING(pSightEnt->pev->classname));
					break;
				}
			}
		}
	}

	if (iSighted == 0)
	{
		ClearConditions(threatPerceptionConditions);
		m_scaryEntity = NULL;
	}

	SetConditions(iSighted);
}

void CToadWeaponMonster::Advance()
{
	PlayWalkAnimation();
}

void CToadWeaponMonster::Stop(void)
{
	BaseClass::Stop();
	PlayIdleAnimation();
	m_iMode = TOAD_IDLE;
}

void CToadWeaponMonster::PlayIdleAnimation()
{
	SetSequenceByName("idle");
}

void CToadWeaponMonster::PlayWalkAnimation()
{
	SetSequenceByName("Walk");
}

BOOL CToadWeaponMonster::IsPlayingWalkSequence()
{
	return pev->sequence == LookupSequence("Walk");
}

void CToadWeaponMonster::UpdateAnimation()
{
	if (IsPlayingWalkSequence())
	{
		pev->framerate = TOAD_WALK_FRAMERATE;
	}
}

Vector CToadWeaponMonster::ComputeRunawayPosition(const Vector& enemyPosition, float stepSize)
{
	Vector	vecRunawayPosition, vecNewDir;
	float flDist;

	Vector escapeForward, escapeRight, escapeUp;
	Vector directionFromSelfToScaryEntity;

	directionFromSelfToScaryEntity = (pev->origin - m_scaryEntity->pev->origin);
	directionFromSelfToScaryEntity.z = 0;
	directionFromSelfToScaryEntity.Normalize();

	UTIL_MakeVectorsPrivate(UTIL_VecToAngles(directionFromSelfToScaryEntity),
		escapeForward, escapeRight, escapeUp);

	vecNewDir = (escapeForward * 0.8f + escapeRight * 0.2f * RANDOM_LONG(-1, 1)).Normalize();
	flDist = stepSize + (RANDOM_LONG(0, stepSize)); // TOAD_HOP_DISTANCE
	vecRunawayPosition = pev->origin + vecNewDir * flDist;

	TraceResult tr;
	TRACE_MONSTER_HULL(edict(), pev->origin, vecRunawayPosition, dont_ignore_monsters, edict(), &tr);

	if (tr.flFraction != 1.0f)
	{
		float n = -DotProduct(tr.vecPlaneNormal, vecNewDir);

		vecNewDir = 2.0 * tr.vecPlaneNormal * n + vecNewDir;

		vecRunawayPosition = tr.vecEndPos + vecNewDir * flDist;
	}

	return vecRunawayPosition;
}

Vector CToadWeaponMonster::ComputeRandomPosition(float stepSize)
{
	Vector randomDirection;
	Vector randomPosition;
	float flDist;

	randomDirection.x = RANDOM_FLOAT(-1, 1);
	randomDirection.y = RANDOM_FLOAT(-1, 1);
	flDist = stepSize + (RANDOM_LONG(0, stepSize));
	randomPosition = pev->origin + randomDirection * flDist;

	return randomPosition;
}

const char* CToadWeaponMonster::GetClassnameOfWeaponToGiveToToucher(CBaseEntity* toucher)
{
	return "weapon_toad";
}

void CToadWeaponMonster::PlayHuntSound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, RANDOM_SOUND_ARRAY(pHuntSounds), 1, ATTN_NORM, 0, 90 + RANDOM_LONG(0, 29));
}

void CToadWeaponMonster::UpdateNextSound(BOOL didCroakSound)
{
#define MAX_IDLE_SOUND_INTERVAL		6
#define MAX_SCARED_SOUND_INTERVAL	4

	if (didCroakSound)
	{
		if (m_iMode == TOAD_IDLE)
			m_flNextSoundTime = gpGlobals->time + RANDOM_FLOAT(2, MAX_IDLE_SOUND_INTERVAL);
		else // Croak more often when scared.
			m_flNextSoundTime = gpGlobals->time + RANDOM_FLOAT(2, MAX_SCARED_SOUND_INTERVAL);
	}
	else
	{
		if (m_iMode == TOAD_IDLE)
			m_flNextSoundTime = gpGlobals->time + RANDOM_FLOAT(1, MAX_IDLE_SOUND_INTERVAL);
		else // Croak more often when scared.
			m_flNextSoundTime = gpGlobals->time + RANDOM_FLOAT(1, MAX_SCARED_SOUND_INTERVAL);
	}
}
