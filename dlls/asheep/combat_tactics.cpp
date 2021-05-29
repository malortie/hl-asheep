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
#include "gamerules.h"
#include "monsters.h"
#include "combat_tactics.h"

#define		DEFAULT_KICK_FACTOR		15
#define		DEFAULT_PUNCH_FACTOR	15
#define		DEFAULT_KICK_ADDITIONAL_POWER	RANDOM_FLOAT(4, 5)
#define		DEFAULT_PUNCH_ADDITIONAL_POWER	RANDOM_FLOAT(4, 5)

CBaseEntity* CBaseCombatTactics::CheckTraceAttackHull(CBaseMonster* const outer, float distance, float damage, Vector punchAngles, Vector impulse)
{
	CBaseEntity *pHurt = outer->CheckTraceHullAttack(distance, damage, DMG_CLUB);
	if (pHurt && (pHurt->pev->flags & (FL_MONSTER | FL_CLIENT)) != 0)
	{
		if (pHurt->IsPlayer())
		{
			pHurt->pev->punchangle.x = punchAngles.x;
			pHurt->pev->punchangle.y = punchAngles.y;
			pHurt->pev->punchangle.z = punchAngles.z;
		}

		pHurt->pev->velocity = pHurt->pev->velocity + impulse;
	}

	return pHurt;
}

float CBaseCombatTactics::GetKickImpulseForce() const
{
	return DEFAULT_KICK_ADDITIONAL_POWER * DEFAULT_KICK_FACTOR;
}

float CBaseCombatTactics::GetPunchImpulseForce() const
{
	return DEFAULT_PUNCH_ADDITIONAL_POWER * DEFAULT_PUNCH_FACTOR;
}

void CBaseCombatTactics::Punch(CBaseMonster* const outer)
{
	//ALERT(at_console, "Punch!");
	Vector punchAngles = GetVictimPunchAngles();
	Vector impulse = gpGlobals->v_forward * GetPunchImpulseForce();

	CBaseEntity *pHurt = CheckTraceAttackHull(outer, GetDefaultDistanceForTraceHullAttack(), 
		GetPunchDamage(), punchAngles, impulse);
	if (pHurt)
		PunchHitSound(outer);
	else
		PunchMissSound(outer);
}

void CBaseCombatTactics::Kick(CBaseMonster* const outer)
{
	//ALERT(at_console, "Kick!");
	Vector punchAngles = GetVictimPunchAngles();
	Vector impulse = gpGlobals->v_forward * GetKickImpulseForce();

	CBaseEntity *pHurt = CheckTraceAttackHull(outer, GetDefaultDistanceForTraceHullAttack(), 
		GetKickDamage(), punchAngles, impulse);
	if (pHurt)
		KickHitSound(outer);
	else
		KickMissSound(outer);
}

float CBaseCombatTactics::GetDefaultDistanceForTraceHullAttack()
{
	return 70;
}

Vector CBaseCombatTactics::GetVictimPunchAngles()
{
	return Vector(RANDOM_FLOAT(3, 5), 0, 0);
}
