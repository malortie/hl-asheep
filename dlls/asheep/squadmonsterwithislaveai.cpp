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

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"squadmonster.h"
#include	"schedule.h"
#include	"effects.h"
#include	"weapons.h"
#include	"soundent.h"
#include	"squadmonsterwithislaveai.h"

extern DLL_GLOBAL int		g_iSkillLevel;

//=========================================================
// Monster's Anim Events Go Here
//=========================================================
#define		ISLAVE_AE_CLAW		( 1 )
#define		ISLAVE_AE_CLAWRAKE	( 2 )
#define		ISLAVE_AE_ZAP_POWERUP	( 3 )
#define		ISLAVE_AE_ZAP_SHOOT		( 4 )
#define		ISLAVE_AE_ZAP_DONE		( 5 )

TYPEDESCRIPTION	CSquadMonsterWithISlaveAI::m_SaveData[] =
{
	DEFINE_FIELD(CSquadMonsterWithISlaveAI, m_iBravery, FIELD_INTEGER),

	DEFINE_ARRAY(CSquadMonsterWithISlaveAI, m_pBeam, FIELD_CLASSPTR, ISLAVE_MAX_BEAMS),
	DEFINE_FIELD(CSquadMonsterWithISlaveAI, m_iBeams, FIELD_INTEGER),
	DEFINE_FIELD(CSquadMonsterWithISlaveAI, m_flNextAttack, FIELD_TIME),

	DEFINE_FIELD(CSquadMonsterWithISlaveAI, m_voicePitch, FIELD_INTEGER),

	DEFINE_FIELD(CSquadMonsterWithISlaveAI, m_hDead, FIELD_EHANDLE),

};

IMPLEMENT_SAVERESTORE(CSquadMonsterWithISlaveAI, CSquadMonster);

//=========================================================
// Classify - indicates this monster's place in the 
// relationship table.
//=========================================================
int	CSquadMonsterWithISlaveAI::Classify(void)
{
	return	CLASS_ALIEN_MILITARY;
}

int CSquadMonsterWithISlaveAI::IRelationship(CBaseEntity *pTarget)
{
	if ((pTarget->IsPlayer()))
		if ((pev->spawnflags & SF_MONSTER_WAIT_UNTIL_PROVOKED) && !(m_afMemory & bits_MEMORY_PROVOKED))
			return R_NO;
	return CBaseMonster::IRelationship(pTarget);
}


void CSquadMonsterWithISlaveAI::CallForHelp(char *szClassname, float flDist, EHANDLE hEnemy, Vector &vecLocation)
{
	// ALERT( at_aiconsole, "help " );

	// skip ones not on my netname
	if (FStringNull(pev->netname))
		return;

	CBaseEntity *pEntity = NULL;

	while ((pEntity = UTIL_FindEntityByString(pEntity, "netname", STRING(pev->netname))) != NULL)
	{
		float d = (pev->origin - pEntity->pev->origin).Length();
		if (d < flDist)
		{
			CBaseMonster *pMonster = pEntity->MyMonsterPointer();
			if (pMonster)
			{
				pMonster->m_afMemory |= bits_MEMORY_PROVOKED;
				pMonster->PushEnemy(hEnemy, vecLocation);
			}
		}
	}
}


//=========================================================
// ALertSound - scream
//=========================================================
void CSquadMonsterWithISlaveAI::AlertSound(void)
{
	if (m_hEnemy != NULL)
	{
		DoAlertSound();
		CallForHelp((char*)STRING(pev->classname), 512, m_hEnemy, m_vecEnemyLKP);
	}
}

//=========================================================
// IdleSound
//=========================================================
void CSquadMonsterWithISlaveAI::IdleSound(void)
{
	if (RANDOM_LONG(0, 2) == 0)
	{
		DoIdleSound();
	}
}

//=========================================================
// PainSound
//=========================================================
void CSquadMonsterWithISlaveAI::PainSound(void)
{
	if (RANDOM_LONG(0, 2) == 0)
	{
		DoPainSound();
	}
}

//=========================================================
// DieSound
//=========================================================

void CSquadMonsterWithISlaveAI::DeathSound(void)
{
	DoDeathSound();
}


//=========================================================
// ISoundMask - returns a bit mask indicating which types
// of sounds this monster regards. 
//=========================================================
int CSquadMonsterWithISlaveAI::ISoundMask(void)
{
	return	bits_SOUND_WORLD |
		bits_SOUND_COMBAT |
		bits_SOUND_DANGER |
		bits_SOUND_PLAYER;
}


void CSquadMonsterWithISlaveAI::Killed(entvars_t *pevAttacker, int iGib)
{
	ClearBeams();
	CSquadMonster::Killed(pevAttacker, iGib);
}

//=========================================================
// SetYawSpeed - allows each sequence to have a different
// turn rate associated with it.
//=========================================================
void CSquadMonsterWithISlaveAI::SetYawSpeed(void)
{
	int ys;

	switch (m_Activity)
	{
	case ACT_WALK:
		ys = 50;
		break;
	case ACT_RUN:
		ys = 70;
		break;
	case ACT_IDLE:
		ys = 50;
		break;
	default:
		ys = 90;
		break;
	}

	pev->yaw_speed = ys;
}

//=========================================================
// HandleAnimEvent - catches the monster-specific messages
// that occur when tagged animation frames are played.
//
// Returns number of events handled, 0 if none.
//=========================================================
void CSquadMonsterWithISlaveAI::HandleAnimEvent(MonsterEvent_t *pEvent)
{
	// ALERT( at_console, "event %d : %f\n", pEvent->event, pev->frame );
	switch (pEvent->event)
	{
	case ISLAVE_AE_CLAW:
	{
		// SOUND HERE!
		CBaseEntity *pHurt = CheckTraceHullAttack(70, GetClawDamage(), DMG_SLASH);
		if (pHurt)
		{
			if (pHurt->pev->flags & (FL_MONSTER | FL_CLIENT))
			{
				pHurt->pev->punchangle.z = -18;
				pHurt->pev->punchangle.x = 5;
			}
			// Play a random attack hit sound
			AttackHitSound();
		}
		else
		{
			// Play a random attack miss sound
			AttackMissSound();
		}
	}
	break;

	case ISLAVE_AE_CLAWRAKE:
	{
		CBaseEntity *pHurt = CheckTraceHullAttack(70, GetClawRakeDamage(), DMG_SLASH);
		if (pHurt)
		{
			if (pHurt->pev->flags & (FL_MONSTER | FL_CLIENT))
			{
				pHurt->pev->punchangle.z = -18;
				pHurt->pev->punchangle.x = 5;
			}
			AttackHitSound();
		}
		else
		{
			AttackMissSound();
		}
	}
	break;

	case ISLAVE_AE_ZAP_POWERUP:
	{
		// In Azure Sheep, alien slaves shoot
		// as fast as in Vanilla HL on hard.
		pev->framerate = 1.5;

		UTIL_MakeAimVectors(pev->angles);

		if (m_iBeams == 0)
		{
			Vector vecSrc = pev->origin + gpGlobals->v_forward * 2;
			SpawnDynamicLight(vecSrc);
		}
		if (m_hDead != NULL)
		{
			WackBeam(-1, m_hDead);
			WackBeam(1, m_hDead);
		}
		else
		{
			ArmBeam(-1);
			ArmBeam(1);
			BeamGlow();
		}

		BeamPowerupSound();
		pev->skin = m_iBeams / 2;
	}
	break;

	case ISLAVE_AE_ZAP_SHOOT:
	{
		ClearBeams();

		if (m_hDead != NULL)
		{
			Vector vecDest = m_hDead->pev->origin + Vector(0, 0, 38);
			TraceResult trace;
			UTIL_TraceHull(vecDest, vecDest, dont_ignore_monsters, human_hull, m_hDead->edict(), &trace);

			if (!trace.fStartSolid)
			{
				CBaseEntity *pNew = Create((char*)STRING(pev->classname), m_hDead->pev->origin, m_hDead->pev->angles);
				CBaseMonster *pNewMonster = pNew->MyMonsterPointer();
				pNew->pev->spawnflags |= 1;
				WackBeam(-1, pNew);
				WackBeam(1, pNew);
				UTIL_Remove(m_hDead);
				BeamZapSound();
				break;
			}
		}
		ClearMultiDamage();

		UTIL_MakeAimVectors(pev->angles);

		ZapBeam(-1);
		ZapBeam(1);

		BeamZapSound();
		ApplyMultiDamage(pev, pev);

		m_flNextAttack = gpGlobals->time + RANDOM_FLOAT(0.5, 4.0);
	}
	break;

	case ISLAVE_AE_ZAP_DONE:
	{
		ClearBeams();
	}
	break;

	default:
		CSquadMonster::HandleAnimEvent(pEvent);
		break;
	}
}

//=========================================================
// CheckRangeAttack1 - normal beam attack 
//=========================================================
BOOL CSquadMonsterWithISlaveAI::CheckRangeAttack1(float flDot, float flDist)
{
	if (m_flNextAttack > gpGlobals->time)
	{
		return FALSE;
	}

	return CSquadMonster::CheckRangeAttack1(flDot, flDist);
}

//=========================================================
// CheckRangeAttack2 - check bravery and try to resurect dead comrades
//=========================================================
BOOL CSquadMonsterWithISlaveAI::CheckRangeAttack2(float flDot, float flDist)
{
	return FALSE;

	if (m_flNextAttack > gpGlobals->time)
	{
		return FALSE;
	}

	m_hDead = NULL;
	m_iBravery = 0;

	CBaseEntity *pEntity = NULL;
	while ((pEntity = UTIL_FindEntityByClassname(pEntity, "monster_alien_slave")) != NULL)
	{
		TraceResult tr;

		UTIL_TraceLine(EyePosition(), pEntity->EyePosition(), ignore_monsters, ENT(pev), &tr);
		if (tr.flFraction == 1.0 || tr.pHit == pEntity->edict())
		{
			if (pEntity->pev->deadflag == DEAD_DEAD)
			{
				float d = (pev->origin - pEntity->pev->origin).Length();
				if (d < flDist)
				{
					m_hDead = pEntity;
					flDist = d;
				}
				m_iBravery--;
			}
			else
			{
				m_iBravery++;
			}
		}
	}
	if (m_hDead != NULL)
		return TRUE;
	else
		return FALSE;
}


//=========================================================
// StartTask
//=========================================================
void CSquadMonsterWithISlaveAI::StartTask(Task_t *pTask)
{
	ClearBeams();

	CSquadMonster::StartTask(pTask);
}


//=========================================================
// Spawn
//=========================================================
void CSquadMonsterWithISlaveAI::Spawn()
{
	Precache();

	SetModel();
	UTIL_SetSize(pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX);

	pev->solid = SOLID_SLIDEBOX;
	pev->movetype = MOVETYPE_STEP;
	SetBloodColor();
	pev->effects = 0;
	pev->health = GetFirstTimeSpawnHealth();
	pev->view_ofs = Vector(0, 0, 64);// position of the eyes relative to monster's origin.
	m_flFieldOfView = VIEW_FIELD_WIDE; // NOTE: we need a wide field of view so npc will notice player and say hello
	m_MonsterState = MONSTERSTATE_NONE;
	m_afCapability = bits_CAP_HEAR | bits_CAP_TURN_HEAD | bits_CAP_RANGE_ATTACK2 | bits_CAP_DOORS_GROUP;

	SetVoicePitch();

	MonsterInit();
}

//=========================================================
// Precache - precaches all resources this monster needs
//=========================================================
void CSquadMonsterWithISlaveAI::Precache()
{
	PrecacheModels();
	PrecacheSounds();
}


//=========================================================
// TakeDamage - get provoked when injured
//=========================================================

int CSquadMonsterWithISlaveAI::TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType)
{
	// don't slash one of your own
	if ((bitsDamageType & DMG_SLASH) && pevAttacker && IRelationship(Instance(pevAttacker)) < R_DL)
		return 0;

	m_afMemory |= bits_MEMORY_PROVOKED;
	return CSquadMonster::TakeDamage(pevInflictor, pevAttacker, flDamage, bitsDamageType);
}


void CSquadMonsterWithISlaveAI::TraceAttack(entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType)
{
	if (bitsDamageType & DMG_SHOCK)
		return;

	CSquadMonster::TraceAttack(pevAttacker, flDamage, vecDir, ptr, bitsDamageType);
}


//=========================================================
// AI Schedules Specific to this monster
//=========================================================



// primary range attack
Task_t	tlSlaveAttack1[] =
{
	{ TASK_STOP_MOVING,			0 },
	{ TASK_FACE_IDEAL,			(float)0 },
	{ TASK_RANGE_ATTACK1,		(float)0 },
};

Schedule_t	slSlaveAttack1[] =
{
	{
		tlSlaveAttack1,
		ARRAYSIZE(tlSlaveAttack1),
		bits_COND_CAN_MELEE_ATTACK1 |
		bits_COND_HEAR_SOUND |
	bits_COND_HEAVY_DAMAGE,

	bits_SOUND_DANGER,
	"Slave Range Attack1"
	},
};


DEFINE_CUSTOM_SCHEDULES(CSquadMonsterWithISlaveAI)
{
	slSlaveAttack1,
};

IMPLEMENT_CUSTOM_SCHEDULES(CSquadMonsterWithISlaveAI, CSquadMonster);


//=========================================================
//=========================================================
Schedule_t *CSquadMonsterWithISlaveAI::GetSchedule(void)
{
	ClearBeams();

	if (HasConditions(bits_COND_HEAR_SOUND))
	{
		CSound *pSound;
		pSound = PBestSound();

		ASSERT(pSound != NULL);

		if (pSound && (pSound->m_iType & bits_SOUND_DANGER))
			return GetScheduleOfType(SCHED_TAKE_COVER_FROM_BEST_SOUND);
		if (pSound->m_iType & bits_SOUND_COMBAT)
			m_afMemory |= bits_MEMORY_PROVOKED;
	}

	switch (m_MonsterState)
	{
	case MONSTERSTATE_COMBAT:
		// dead enemy
		if (HasConditions(bits_COND_ENEMY_DEAD))
		{
			// call base class, all code to handle dead enemies is centralized there.
			return CBaseMonster::GetSchedule();
		}

		if (pev->health < 20 || m_iBravery < 0)
		{
			if (!HasConditions(bits_COND_CAN_MELEE_ATTACK1))
			{
				m_failSchedule = SCHED_CHASE_ENEMY;
				if (HasConditions(bits_COND_LIGHT_DAMAGE | bits_COND_HEAVY_DAMAGE))
				{
					return GetScheduleOfType(SCHED_TAKE_COVER_FROM_ENEMY);
				}
				if (HasConditions(bits_COND_SEE_ENEMY) && HasConditions(bits_COND_ENEMY_FACING_ME))
				{
					// ALERT( at_console, "exposed\n");
					return GetScheduleOfType(SCHED_TAKE_COVER_FROM_ENEMY);
				}
			}
		}
		break;
	}
	return CSquadMonster::GetSchedule();
}


Schedule_t *CSquadMonsterWithISlaveAI::GetScheduleOfType(int Type)
{
	switch (Type)
	{
	case SCHED_FAIL:
		if (HasConditions(bits_COND_CAN_MELEE_ATTACK1))
		{
			return CSquadMonster::GetScheduleOfType(SCHED_MELEE_ATTACK1); ;
		}
		break;
	case SCHED_RANGE_ATTACK1:
		return slSlaveAttack1;
	case SCHED_RANGE_ATTACK2:
		return slSlaveAttack1;
	}
	return CSquadMonster::GetScheduleOfType(Type);
}


//=========================================================
// ArmBeam - small beam from arm to nearby geometry
//=========================================================

void CSquadMonsterWithISlaveAI::ArmBeam(int side)
{
	TraceResult tr;
	float flDist = 1.0;

	if (m_iBeams >= ISLAVE_MAX_BEAMS)
		return;

	UTIL_MakeAimVectors(pev->angles);
	Vector vecSrc = pev->origin + gpGlobals->v_up * 36 + gpGlobals->v_right * side * 16 + gpGlobals->v_forward * 32;

	for (int i = 0; i < 3; i++)
	{
		Vector vecAim = gpGlobals->v_right * side * RANDOM_FLOAT(0, 1) + gpGlobals->v_up * RANDOM_FLOAT(-1, 1);
		TraceResult tr1;
		UTIL_TraceLine(vecSrc, vecSrc + vecAim * 512, dont_ignore_monsters, ENT(pev), &tr1);
		if (flDist > tr1.flFraction)
		{
			tr = tr1;
			flDist = tr.flFraction;
		}
	}

	// Couldn't find anything close enough
	if (flDist == 1.0)
		return;

	DecalGunshot(&tr, BULLET_PLAYER_CROWBAR);

	m_pBeam[m_iBeams] = CreateArmBeam(side, tr.vecEndPos);
	if (!m_pBeam[m_iBeams])
		return;

	m_iBeams++;
}


//=========================================================
// BeamGlow - brighten all beams
//=========================================================
void CSquadMonsterWithISlaveAI::BeamGlow()
{
	int b = m_iBeams * 32;
	if (b > 255)
		b = 255;

	for (int i = 0; i < m_iBeams; i++)
	{
		if (m_pBeam[i]->GetBrightness() != 255)
		{
			m_pBeam[i]->SetBrightness(b);
		}
	}
}


//=========================================================
// WackBeam - regenerate dead colleagues
//=========================================================
void CSquadMonsterWithISlaveAI::WackBeam(int side, CBaseEntity *pEntity)
{
	Vector vecDest;
	float flDist = 1.0;

	if (m_iBeams >= ISLAVE_MAX_BEAMS)
		return;

	if (pEntity == NULL)
		return;

	m_pBeam[m_iBeams] = CBeam::BeamCreate("sprites/lgtning.spr", 30);
	if (!m_pBeam[m_iBeams])
		return;

	m_pBeam[m_iBeams]->PointEntInit(pEntity->Center(), entindex());
	m_pBeam[m_iBeams]->SetEndAttachment(side < 0 ? 2 : 1);
	m_pBeam[m_iBeams]->SetColor(180, 255, 96);
	m_pBeam[m_iBeams]->SetBrightness(255);
	m_pBeam[m_iBeams]->SetNoise(80);
	m_iBeams++;
}

//=========================================================
// ZapBeam - heavy damage directly forward
//=========================================================
void CSquadMonsterWithISlaveAI::ZapBeam(int side)
{
	Vector vecSrc, vecAim;
	TraceResult tr;
	CBaseEntity *pEntity;

	if (m_iBeams >= ISLAVE_MAX_BEAMS)
		return;

	vecSrc = pev->origin + gpGlobals->v_up * 36;
	vecAim = ShootAtEnemy(vecSrc);
	float deflection = 0.01;
	vecAim = vecAim + side * gpGlobals->v_right * RANDOM_FLOAT(0, deflection) + gpGlobals->v_up * RANDOM_FLOAT(-deflection, deflection);
	UTIL_TraceLine(vecSrc, vecSrc + vecAim * 1024, dont_ignore_monsters, ENT(pev), &tr);

	m_pBeam[m_iBeams] = CreateZapBeam(side, tr.vecEndPos);
	if (!m_pBeam[m_iBeams])
		return;

	m_iBeams++;

	pEntity = CBaseEntity::Instance(tr.pHit);
	if (pEntity != NULL && pEntity->pev->takedamage)
	{
		pEntity->TraceAttack(pev, GetZapBeamDamage(), vecAim, &tr, DMG_SHOCK);
	}

	BeamElectroSound(tr.vecEndPos);
}


//=========================================================
// ClearBeams - remove all beams
//=========================================================
void CSquadMonsterWithISlaveAI::ClearBeams()
{
	for (int i = 0; i < ISLAVE_MAX_BEAMS; i++)
	{
		if (m_pBeam[i])
		{
			UTIL_Remove(m_pBeam[i]);
			m_pBeam[i] = NULL;
		}
	}
	m_iBeams = 0;
	pev->skin = 0;

	StopBeamSounds();
}

int CSquadMonsterWithISlaveAI::GetVoicePitch()
{
	return m_voicePitch;
}

void CSquadMonsterWithISlaveAI::PrecacheSounds()
{
	GetSharedUtils()->PrecacheSharedSounds();
}

void CSquadMonsterWithISlaveAI::SetSkinAndBodygroups()
{
	pev->skin = 0;
	pev->body = 0;
}

void CSquadMonsterWithISlaveAI::SetBloodColor()
{
	m_bloodColor = BLOOD_COLOR_GREEN;
}

void CSquadMonsterWithISlaveAI::DoDeathSound()
{
	GetSharedUtils()->PlayDeathSound(this);
}

void CSquadMonsterWithISlaveAI::DoPainSound()
{
	GetSharedUtils()->PlayPainSound(this);
}

void CSquadMonsterWithISlaveAI::DoAlertSound()
{
	GetSharedUtils()->PlayAlertSound(this);
}

void CSquadMonsterWithISlaveAI::DoIdleSound()
{
	GetSharedUtils()->PlayIdleSound(this);
}

void CSquadMonsterWithISlaveAI::AttackHitSound()
{
	GetSharedUtils()->PlayAttackHitSound(this);
}

void CSquadMonsterWithISlaveAI::AttackMissSound()
{
	GetSharedUtils()->PlayAttackMissSound(this);
}

CSquadMonsterWithISlaveAIUtil* CSquadMonsterWithISlaveAI::GetSharedUtils() const
{
	static CSquadMonsterWithISlaveAIUtil utilitySingleton;
	return &utilitySingleton;
}
