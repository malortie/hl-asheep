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
#include "monsters.h"
#include "talkmonster.h"
#include "schedule.h"
#include "defaultai.h"
#include "scripted.h"
#include "weapons.h"
#include "soundent.h"
#include "talkmonsterwithbarneyai.h"

const char* BarneySounds::pBarneyAttackSounds[] =
{
	"barney/ba_attack1.wav",
	"barney/ba_attack2.wav",
};

const char* BarneySounds::pBarneyPainSounds[] =
{
	"barney/ba_pain1.wav",
	"barney/ba_pain2.wav",
	"barney/ba_pain3.wav",
};

const char* BarneySounds::pBarneyDeathSounds[] =
{
	"barney/ba_die1.wav",
	"barney/ba_die2.wav",
	"barney/ba_die3.wav",
};

const char* BarneySounds::GetRandomAttackSound()
{
	return RANDOM_SOUND_ARRAY(pBarneyAttackSounds);
}

const char* BarneySounds::GetRandomPainSound()
{
	return RANDOM_SOUND_ARRAY(pBarneyPainSounds);
}

const char* BarneySounds::GetRandomDeathSound()
{
	return RANDOM_SOUND_ARRAY(pBarneyDeathSounds);
}

void CTalkMonsterWithBarneyAIUtil::PrecacheSharedSounds()
{
	PRECACHE_SOUND_ARRAY(BarneySounds::pBarneyAttackSounds);
	PRECACHE_SOUND_ARRAY(BarneySounds::pBarneyPainSounds);
	PRECACHE_SOUND_ARRAY(BarneySounds::pBarneyDeathSounds);
}

void CTalkMonsterWithBarneyAIUtil::TraceAttackNormal(CTalkMonsterWithBarneyAI* outer, entvars_t *pevAttacker, Vector vecDir,
	TraceResult *ptr, int bitsDamageType, float& flDamage)
{
	switch (ptr->iHitgroup)
	{
	case HITGROUP_CHEST:
	case HITGROUP_STOMACH:
		if ((outer->IsWearingArmor() || outer->IsWearingHEVSuit()) && ShouldReduceDamageForDamageType(bitsDamageType))
			AdjustBodyShotDamage(flDamage);
		break;

	case 10:
		// Helmet hitbox ID.
		if (outer->IsWearingHelmet() && ShouldReduceDamageForDamageType(bitsDamageType))
		{
			AdjustHeadShotDamage(flDamage);
			if (flDamage <= 0)
				UTIL_Ricochet(ptr->vecEndPos, 1.0);
		}
		// always a head shot
		ptr->iHitgroup = HITGROUP_HEAD;
		break;

	default:
		// With the HEV suit, the entire body is fully protected.
		// Therefore, reduce damage at any spot.
		if (outer->IsWearingHEVSuit() && ShouldReduceDamageForDamageType(bitsDamageType))
			AdjustBodyShotDamage(flDamage);
		break;
	}
}

CBaseEntity* CTalkMonsterWithBarneyAIUtil::DropHandGun(CBaseMonster* outer)
{
	Vector vecGunPos;
	Vector vecGunAngles;

	outer->GetAttachment(0, vecGunPos, vecGunAngles);

	CBaseEntity *pGun;
	if (UTIL_IsPlayerEquippedWithArmor(UTIL_GetLocalPlayer()))
		pGun = outer->DropItem("weapon_barney9mmhg", vecGunPos, vecGunAngles);
	else
		pGun = outer->DropItem("weapon_9mmhandgun", vecGunPos, vecGunAngles);

	return pGun;
}

BOOL CTalkMonsterWithBarneyAIUtil::ShouldReduceDamageForDamageType(int bitsDamageType) const
{
	return (bitsDamageType & (DMG_BULLET | DMG_SLASH | DMG_BLAST)) != 0;
}

void CTalkMonsterWithBarneyAIUtil::AdjustHeadShotDamage(float& flDamage)
{
	flDamage -= 20;
	if (flDamage <= 0)
		flDamage = 0.01f;
}

void CTalkMonsterWithBarneyAIUtil::AdjustBodyShotDamage(float& flDamage)
{
	flDamage = flDamage / 2;
}