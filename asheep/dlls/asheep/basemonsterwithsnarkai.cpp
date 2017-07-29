/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "soundent.h"
#include "gamerules.h"
#include "basemonsterwithsnarkai.h"

float CBaseMonsterWithSnarkAI::m_flNextBounceSoundTime = 0;

TYPEDESCRIPTION	CBaseMonsterWithSnarkAI::m_SaveData[] =
{
	DEFINE_FIELD(CBaseMonsterWithSnarkAI, m_flDie, FIELD_TIME),
	DEFINE_FIELD(CBaseMonsterWithSnarkAI, m_vecTarget, FIELD_VECTOR),
	DEFINE_FIELD(CBaseMonsterWithSnarkAI, m_flNextHunt, FIELD_TIME),
	DEFINE_FIELD(CBaseMonsterWithSnarkAI, m_flNextHit, FIELD_TIME),
	DEFINE_FIELD(CBaseMonsterWithSnarkAI, m_posPrev, FIELD_POSITION_VECTOR),
	DEFINE_FIELD(CBaseMonsterWithSnarkAI, m_hOwner, FIELD_EHANDLE),
	DEFINE_FIELD(CBaseMonsterWithSnarkAI, m_minVoicePitch, FIELD_INTEGER),
	DEFINE_FIELD(CBaseMonsterWithSnarkAI, m_maxVoicePitch, FIELD_INTEGER),
};

IMPLEMENT_SAVERESTORE(CBaseMonsterWithSnarkAI, CGrenade);

int CBaseMonsterWithSnarkAI::Classify(void)
{
	if (m_iMyClass != 0)
		return m_iMyClass; // protect against recursion

	if (m_hEnemy != NULL)
	{
		m_iMyClass = CLASS_INSECT; // no one cares about it
		switch (m_hEnemy->Classify())
		{
		case CLASS_PLAYER:
		case CLASS_HUMAN_PASSIVE:
		case CLASS_HUMAN_MILITARY:
#if defined ( ASHEEP_DLL )
		case CLASS_KATE:
		case CLASS_SPECIAL_FORCE:
#endif // defined ( ASHEEP_DLL )
			m_iMyClass = 0;
			return CLASS_ALIEN_MILITARY; // barney's get mad, grunts get mad at it
		}
		m_iMyClass = 0;
	}

	return CLASS_ALIEN_BIOWEAPON;
}

void CBaseMonsterWithSnarkAI::Spawn(void)
{
	Precache();
	// motor
	pev->movetype = MOVETYPE_BOUNCE;
	pev->solid = SOLID_BBOX;

	SetModel();
	UTIL_SetSize(pev, Vector(-4, -4, 0), Vector(4, 4, 8));
	UTIL_SetOrigin(pev, pev->origin);

	SetTouch(&CBaseMonsterWithSnarkAI::SuperBounceTouch);
	SetThink(&CBaseMonsterWithSnarkAI::HuntThink);
	pev->nextthink = gpGlobals->time + 0.1;
	m_flNextHunt = gpGlobals->time + 1E6;

	pev->flags |= FL_MONSTER;
	pev->takedamage = DAMAGE_AIM;
	pev->health = GetFirstTimeSpawnHealth();
	pev->gravity = 0.5;
	pev->friction = 0.5;

	pev->dmg = GetBlastDamage();

	m_flDie = gpGlobals->time + GetDetonateDelay();

	m_flFieldOfView = 0; // 180 degrees

	if (pev->owner)
		m_hOwner = Instance(pev->owner);

	m_flNextBounceSoundTime = gpGlobals->time;// reset each time a snark is spawned.

	pev->sequence = GetMoveSequence();
	ResetSequenceInfo();

	SetVoicePitch();
}

void CBaseMonsterWithSnarkAI::Precache(void)
{
	PrecacheModels();
	PrecacheSounds();
}

void CBaseMonsterWithSnarkAI::Killed(entvars_t *pevAttacker, int iGib)
{
	pev->model = iStringNull;// make invisible
	SetThink(&CBaseMonsterWithSnarkAI::SUB_Remove);
	SetTouch(NULL);
	pev->nextthink = gpGlobals->time + 0.1;

	// since squeak grenades never leave a body behind, clear out their takedamage now.
	// Squeaks do a bit of radius damage when they pop, and that radius damage will
	// continue to call this function unless we acknowledge the Squeak's death now. (sjb)
	pev->takedamage = DAMAGE_NO;

	// play squeek blast
	BlastSound();

	CSoundEnt::InsertSound(bits_SOUND_COMBAT, pev->origin, SMALL_EXPLOSION_VOLUME, 3.0);

	UTIL_BloodDrips(pev->origin, g_vecZero, BloodColor(), 80);

	if (m_hOwner != NULL)
		RadiusDamage(pev, m_hOwner->pev, pev->dmg, CLASS_NONE, DMG_BLAST);
	else
		RadiusDamage(pev, pev, pev->dmg, CLASS_NONE, DMG_BLAST);

	// reset owner so death message happens
	if (m_hOwner != NULL)
		pev->owner = m_hOwner->edict();

	CBaseMonster::Killed(pevAttacker, GIB_ALWAYS);
}

void CBaseMonsterWithSnarkAI::GibMonster(void)
{
	GibSound();
}

void CBaseMonsterWithSnarkAI::HuntThink(void)
{
	// ALERT( at_console, "think\n" );

	if (!IsInWorld())
	{
		SetTouch(NULL);
		UTIL_Remove(this);
		return;
	}

	StudioFrameAdvance();
	pev->nextthink = gpGlobals->time + 0.1;

	// explode when ready
	if (gpGlobals->time >= m_flDie)
	{
		g_vecAttackDir = pev->velocity.Normalize();
		pev->health = -1;
		Killed(pev, 0);
		return;
	}

	// float
	if (pev->waterlevel != 0)
	{
		if (pev->movetype == MOVETYPE_BOUNCE)
		{
			pev->movetype = MOVETYPE_FLY;
		}
		pev->velocity = pev->velocity * 0.9;
		pev->velocity.z += 8.0;
	}
	else if (pev->movetype = MOVETYPE_FLY)
	{
		pev->movetype = MOVETYPE_BOUNCE;
	}

	// return if not time to hunt
	if (m_flNextHunt > gpGlobals->time)
		return;

	m_flNextHunt = gpGlobals->time + 2.0;

	CBaseEntity *pOther = NULL;
	Vector vecDir;
	TraceResult tr;

	Vector vecFlat = pev->velocity;
	vecFlat.z = 0;
	vecFlat = vecFlat.Normalize();

	UTIL_MakeVectors(pev->angles);

	if (m_hEnemy == NULL || !m_hEnemy->IsAlive())
	{
		// find target, bounce a bit towards it.
		Look(512);
		m_hEnemy = BestVisibleEnemy();
	}

	// squeek if it's about time blow up
	if ((m_flDie - gpGlobals->time <= 0.5) && (m_flDie - gpGlobals->time >= 0.3))
	{
		DeathSound();
		CSoundEnt::InsertSound(bits_SOUND_COMBAT, pev->origin, 256, 0.25);
	}

	if (m_hEnemy != NULL)
	{
		if (FVisible(m_hEnemy))
		{
			vecDir = m_hEnemy->EyePosition() - pev->origin;
			m_vecTarget = vecDir.Normalize();
		}

		float flVel = pev->velocity.Length();
		float flAdj = 50.0 / (flVel + 10.0);

		if (flAdj > 1.2)
			flAdj = 1.2;

		// ALERT( at_console, "think : enemy\n");

		// ALERT( at_console, "%.0f %.2f %.2f %.2f\n", flVel, m_vecTarget.x, m_vecTarget.y, m_vecTarget.z );

		pev->velocity = pev->velocity * flAdj + m_vecTarget * 300;
	}

	if (pev->flags & FL_ONGROUND)
	{
		pev->avelocity = Vector(0, 0, 0);
	}
	else
	{
		if (pev->avelocity == Vector(0, 0, 0))
		{
			pev->avelocity.x = RANDOM_FLOAT(-100, 100);
			pev->avelocity.z = RANDOM_FLOAT(-100, 100);
		}
	}

	if ((pev->origin - m_posPrev).Length() < 1.0)
	{
		pev->velocity.x = RANDOM_FLOAT(-100, 100);
		pev->velocity.y = RANDOM_FLOAT(-100, 100);
	}
	m_posPrev = pev->origin;

	pev->angles = UTIL_VecToAngles(pev->velocity);
	pev->angles.z = 0;
	pev->angles.x = 0;
}


void CBaseMonsterWithSnarkAI::SuperBounceTouch(CBaseEntity *pOther)
{
	TraceResult tr = UTIL_GetGlobalTrace();

	// don't hit the guy that launched this grenade
	if (pev->owner && pOther->edict() == pev->owner)
		return;

	// at least until we've bounced once
	pev->owner = NULL;

	pev->angles.x = 0;
	pev->angles.z = 0;

	// avoid bouncing too much
	if (m_flNextHit > gpGlobals->time)
		return;

	if (pOther->pev->takedamage && m_flNextAttack < gpGlobals->time)
	{
		// attack!

		// make sure it's me who has touched them
		if (tr.pHit == pOther->edict())
		{
			// and it's not another squeakgrenade
			if (tr.pHit->v.modelindex != pev->modelindex)
			{
				// ALERT( at_console, "hit enemy\n");
				ClearMultiDamage();
				pOther->TraceAttack(pev, GetBiteDamage(), gpGlobals->v_forward, &tr, DMG_SLASH);
				if (m_hOwner != NULL)
					ApplyMultiDamage(pev, m_hOwner->pev);
				else
					ApplyMultiDamage(pev, pev);

				pev->dmg += GetBlastDamage(); // add more explosion damage
													// m_flDie += 2.0; // add more life

													// make bite sound
				BiteSound();
				m_flNextAttack = gpGlobals->time + 0.5;
			}
		}
		else
		{
			// ALERT( at_console, "been hit\n");
		}
	}

	m_flNextHit = gpGlobals->time + 0.1;
	m_flNextHunt = gpGlobals->time;

	if (g_pGameRules->IsMultiplayer())
	{
		// in multiplayer, we limit how often snarks can make their bounce sounds to prevent overflows.
		if (gpGlobals->time < m_flNextBounceSoundTime)
		{
			// too soon!
			return;
		}
	}

	if (!(pev->flags & FL_ONGROUND))
	{
		// play bounce sound
		HuntSound();
		CSoundEnt::InsertSound(bits_SOUND_COMBAT, pev->origin, 256, 0.25);
	}
	else
	{
		// skittering sound
		CSoundEnt::InsertSound(bits_SOUND_COMBAT, pev->origin, 100, 0.1);
	}

	m_flNextBounceSoundTime = gpGlobals->time + 0.5;// half second.
}

float CBaseMonsterWithSnarkAI::GetDetonateDelay()
{
	return 15.0f;
}

void CBaseMonsterWithSnarkAI::PrecacheModels()
{
}

void CBaseMonsterWithSnarkAI::PrecacheSounds()
{
	PRECACHE_SOUND("common/bodysplat.wav");
}

void CBaseMonsterWithSnarkAI::SetVoicePitch()
{
	m_minVoicePitch = 80;
	m_maxVoicePitch = 100;
}

float CBaseMonsterWithSnarkAI::GetVoicePitch()
{
	// higher pitch as squeeker gets closer to detonation time
	return fabs(m_maxVoicePitch - m_minVoicePitch * ((m_flDie - gpGlobals->time) / GetDetonateDelay()));
}

void CBaseMonsterWithSnarkAI::GibSound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "common/bodysplat.wav", 0.75, ATTN_NORM, 0, 200);
}