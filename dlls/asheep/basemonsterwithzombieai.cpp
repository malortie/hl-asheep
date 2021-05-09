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
#include	"schedule.h"
#include	"basemonsterwithzombieai.h"

//=========================================================
// Monster's Anim Events Go Here
//=========================================================
#define	ZOMBIE_AE_ATTACK_RIGHT		0x01
#define	ZOMBIE_AE_ATTACK_LEFT		0x02
#define	ZOMBIE_AE_ATTACK_BOTH		0x03

#define ZOMBIE_FLINCH_DELAY			2		// at most one flinch every n secs

//=========================================================
// Classify - indicates this monster's place in the 
// relationship table.
//=========================================================
int	CBaseMonsterWithZombieAI::Classify(void)
{
	return	CLASS_ALIEN_MONSTER;
}

//=========================================================
// SetYawSpeed - allows each sequence to have a different
// turn rate associated with it.
//=========================================================
void CBaseMonsterWithZombieAI::SetYawSpeed(void)
{
	pev->yaw_speed = 120;
}

int CBaseMonsterWithZombieAI::TakeDamage(entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType)
{
	// Take 30% damage from bullets
	if (bitsDamageType == DMG_BULLET)
	{
		Vector vecDir = pev->origin - (pevInflictor->absmin + pevInflictor->absmax) * 0.5;
		vecDir = vecDir.Normalize();
		float flForce = DamageForce(flDamage);
		pev->velocity = pev->velocity + vecDir * flForce;
		flDamage *= 0.3;
	}

	// HACK HACK -- until we fix this.
	if (IsAlive())
		PainSound();
	return CBaseMonster::TakeDamage(pevInflictor, pevAttacker, flDamage, bitsDamageType);
}

//=========================================================
// HandleAnimEvent - catches the monster-specific messages
// that occur when tagged animation frames are played.
//=========================================================
void CBaseMonsterWithZombieAI::HandleAnimEvent(MonsterEvent_t *pEvent)
{
	switch (pEvent->event)
	{
	case ZOMBIE_AE_ATTACK_RIGHT:
	{
		// do stuff for this event.
		//		ALERT( at_console, "Slash right!\n" );
		CBaseEntity *pHurt = CheckTraceHullAttack(70, GetOneSlashAttackDamage(), DMG_SLASH);
		if (pHurt)
		{
			if (pHurt->pev->flags & (FL_MONSTER | FL_CLIENT))
			{
				pHurt->pev->punchangle.z = -18;
				pHurt->pev->punchangle.x = 5;
				pHurt->pev->velocity = pHurt->pev->velocity - gpGlobals->v_right * 100;
			}
			// Play a random attack hit sound
			AttackHitSound();
		}
		else // Play a random attack miss sound
			AttackMissSound();

		if (RANDOM_LONG(0, 1))
			AttackSound();
	}
	break;

	case ZOMBIE_AE_ATTACK_LEFT:
	{
		// do stuff for this event.
		//		ALERT( at_console, "Slash left!\n" );
		CBaseEntity *pHurt = CheckTraceHullAttack(70, GetOneSlashAttackDamage(), DMG_SLASH);
		if (pHurt)
		{
			if (pHurt->pev->flags & (FL_MONSTER | FL_CLIENT))
			{
				pHurt->pev->punchangle.z = 18;
				pHurt->pev->punchangle.x = 5;
				pHurt->pev->velocity = pHurt->pev->velocity + gpGlobals->v_right * 100;
			}
			AttackHitSound();
		}
		else
			AttackMissSound();

		if (RANDOM_LONG(0, 1))
			AttackSound();
	}
	break;

	case ZOMBIE_AE_ATTACK_BOTH:
	{
		// do stuff for this event.
		CBaseEntity *pHurt = CheckTraceHullAttack(70, GetBothSlashAttackDamage(), DMG_SLASH);
		if (pHurt)
		{
			if (pHurt->pev->flags & (FL_MONSTER | FL_CLIENT))
			{
				pHurt->pev->punchangle.x = 5;
				pHurt->pev->velocity = pHurt->pev->velocity + gpGlobals->v_forward * -100;
			}
			AttackHitSound();
		}
		else
			AttackMissSound();

		if (RANDOM_LONG(0, 1))
			AttackSound();
	}
	break;

	default:
		CBaseMonster::HandleAnimEvent(pEvent);
		break;
	}
}

//=========================================================
// Spawn
//=========================================================
void CBaseMonsterWithZombieAI::Spawn()
{
	Precache();

	SetModel();
	UTIL_SetSize(pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX);

	pev->solid = SOLID_SLIDEBOX;
	pev->movetype = MOVETYPE_STEP;
	m_bloodColor = BLOOD_COLOR_GREEN;
	pev->health = GetFirstTimeSpawnHealth();
	pev->view_ofs = VEC_VIEW;// position of the eyes relative to monster's origin.
	m_flFieldOfView = 0.5;// indicates the width of this monster's forward view cone ( as a dotproduct result )
	m_MonsterState = MONSTERSTATE_NONE;
	m_afCapability = bits_CAP_DOORS_GROUP;

	SetSkinAndBodygroups();

	MonsterInit();
}

//=========================================================
// Precache - precaches all resources this monster needs
//=========================================================
void CBaseMonsterWithZombieAI::Precache()
{
	PrecacheModels();
	PrecacheSounds();
}

//=========================================================
// AI Schedules Specific to this monster
//=========================================================

int CBaseMonsterWithZombieAI::IgnoreConditions(void)
{
	int iIgnore = CBaseMonster::IgnoreConditions();

	if ((m_Activity == ACT_MELEE_ATTACK1) || (m_Activity == ACT_MELEE_ATTACK1))
	{		
			if (m_flNextFlinch >= gpGlobals->time)
				iIgnore |= (bits_COND_LIGHT_DAMAGE | bits_COND_HEAVY_DAMAGE);
	}

	if ((m_Activity == ACT_SMALL_FLINCH) || (m_Activity == ACT_BIG_FLINCH))
	{
		if (m_flNextFlinch < gpGlobals->time)
			m_flNextFlinch = gpGlobals->time + ZOMBIE_FLINCH_DELAY;
	}

	return iIgnore;

}

void CBaseMonsterWithZombieAI::PrecacheSounds()
{
	GetSharedUtils()->PrecacheSharedSounds();
}

void CBaseMonsterWithZombieAI::SetSkinAndBodygroups()
{
	pev->skin = 0;
	pev->body = 0;
}

void CBaseMonsterWithZombieAI::PainSound(void)
{
	GetSharedUtils()->PlayPainSound(this);
}

void CBaseMonsterWithZombieAI::AlertSound(void)
{
	GetSharedUtils()->PlayAlertSound(this);
}

void CBaseMonsterWithZombieAI::IdleSound(void)
{
	GetSharedUtils()->PlayIdleSound(this);
}

void CBaseMonsterWithZombieAI::AttackSound(void)
{
	GetSharedUtils()->PlayAttackSound(this);
}

void CBaseMonsterWithZombieAI::AttackHitSound()
{
	GetSharedUtils()->PlayAttackHitSound(this);
}

void CBaseMonsterWithZombieAI::AttackMissSound()
{
	GetSharedUtils()->PlayAttackMissSound(this);
}

CBaseMonsterWithZombieAIUtil* CBaseMonsterWithZombieAI::GetSharedUtils() const
{
	static CBaseMonsterWithZombieAIUtil utilitySingleton;
	return &utilitySingleton;
}
