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
#include "squadmonsterwithhgruntai.h"

const char* HumanGruntSounds::pHGruntMP5FireSounds[] =
{
	"hgrunt/gr_mgun1.wav",
	"hgrunt/gr_mgun2.wav",
};

const char* HumanGruntSounds::pHGruntMP5GrenadeLaunchSounds[] =
{
	"weapons/glauncher.wav",
};

const char* HumanGruntSounds::pHGruntMP5ReloadSounds[] =
{
	"hgrunt/gr_reload1.wav",
};

const char* HumanGruntSounds::pHGruntShotgunFireSounds[] =
{
	"weapons/sbarrel1.wav",
};

const char* HumanGruntSounds::pPainSounds[] =
{
	"hgrunt/gr_pain1.wav",
	"hgrunt/gr_pain2.wav",
	"hgrunt/gr_pain3.wav",
	"hgrunt/gr_pain4.wav",
	"hgrunt/gr_pain5.wav",
};

const char* HumanGruntSounds::pDeathSounds[] =
{
	"hgrunt/gr_die1.wav",
	"hgrunt/gr_die2.wav",
	"hgrunt/gr_die3.wav",
};

const char *HumanGruntSounds::GetRandomHGruntMP5FireSound()
{
	return RANDOM_SOUND_ARRAY(pHGruntMP5FireSounds);
}

const char *HumanGruntSounds::GetRandomHGruntMP5GrenadeLaunchSound()
{
	return RANDOM_SOUND_ARRAY(pHGruntMP5GrenadeLaunchSounds);
}

const char *HumanGruntSounds::GetRandomHGruntMP5ReloadSound()
{
	return RANDOM_SOUND_ARRAY(pHGruntMP5ReloadSounds);
}

const char *HumanGruntSounds::GetRandomHGruntShotgunFireSound()
{
	return RANDOM_SOUND_ARRAY(pHGruntShotgunFireSounds);
}

const char *HumanGruntSounds::GetRandomPainSound()
{
	return RANDOM_SOUND_ARRAY(pPainSounds);
}

const char *HumanGruntSounds::GetRandomDeathSound()
{
	return RANDOM_SOUND_ARRAY(pDeathSounds);
}

void CSquadMonsterWithHGruntAIUtil::PrecacheSharedSounds()
{
	PRECACHE_SOUND_ARRAY(HumanGruntSounds::pPainSounds);
	PRECACHE_SOUND_ARRAY(HumanGruntSounds::pDeathSounds);

	PRECACHE_SOUND("zombie/claw_miss2.wav");// because we use the basemonster SWIPE animation event
	PRECACHE_SOUND("common/kick.wav");
}

void CSquadMonsterWithHGruntAIUtil::PrecacheHGruntWeaponSounds()
{
	PRECACHE_SOUND_ARRAY(HumanGruntSounds::pHGruntMP5FireSounds);
	PRECACHE_SOUND_ARRAY(HumanGruntSounds::pHGruntMP5GrenadeLaunchSounds);
	PRECACHE_SOUND_ARRAY(HumanGruntSounds::pHGruntMP5ReloadSounds);
	PRECACHE_SOUND_ARRAY(HumanGruntSounds::pHGruntShotgunFireSounds);
}

void CSquadMonsterWithHGruntAIUtil::PlayHGruntPainSound(CBaseEntity* outer)
{
	edict_t* edict = ENT(outer->pev);

	EMIT_SOUND(edict, CHAN_VOICE, HumanGruntSounds::GetRandomPainSound(), 1, ATTN_NORM);
}

void CSquadMonsterWithHGruntAIUtil::PlayHGruntDeathSound(CBaseEntity* outer)
{
	edict_t* edict = ENT(outer->pev);

	EMIT_SOUND(edict, CHAN_VOICE, HumanGruntSounds::GetRandomDeathSound(), 1, ATTN_IDLE);
}

void CSquadMonsterWithHGruntAIUtil::PlayHGruntMP5FireSound(CBaseEntity* outer)
{
	// the first round of the three round burst plays the sound and puts a sound in the world sound list.
	EMIT_SOUND(ENT(outer->pev), CHAN_WEAPON, HumanGruntSounds::GetRandomHGruntMP5FireSound(), 1, ATTN_NORM);
}

void CSquadMonsterWithHGruntAIUtil::PlayHGruntShotgunFireSound(CBaseEntity* outer)
{
	EMIT_SOUND(ENT(outer->pev), CHAN_WEAPON, HumanGruntSounds::GetRandomHGruntShotgunFireSound(), 1, ATTN_NORM);
}

void CSquadMonsterWithHGruntAIUtil::PlayHGruntLaunchGrenadeSound(CBaseEntity* outer)
{
	EMIT_SOUND(ENT(outer->pev), CHAN_WEAPON, HumanGruntSounds::GetRandomHGruntMP5GrenadeLaunchSound(), 0.8, ATTN_NORM);
}

void CSquadMonsterWithHGruntAIUtil::PlayHGruntReloadSound(CBaseEntity* outer)
{
	EMIT_SOUND(ENT(outer->pev), CHAN_WEAPON, "hgrunt/gr_reload1.wav", 1, ATTN_NORM);
}

void CSquadMonsterWithHGruntAIUtil::PlayHGruntKickSound(CBaseEntity* outer)
{
	EMIT_SOUND_DYN(ENT(outer->pev),
		CHAN_WEAPON,
		"common/kick.wav",
		1.0,
		ATTN_NORM,
		0,
		RANDOM_LONG(95, 105));
}

void CSquadMonsterWithHGruntAIUtil::TraceAttackNormal(CSquadMonsterWithHGruntAI* outer, entvars_t *pevAttacker, Vector vecDir, TraceResult *ptr, int bitsDamageType, float& flDamage)
{
	// check for helmet shot
	if (ptr->iHitgroup == 11)
	{
		// make sure we're wearing one
		if (outer->IsWearingHelmet() && (bitsDamageType & (DMG_BULLET | DMG_SLASH | DMG_BLAST | DMG_CLUB)))
		{
			// absorb damage
			flDamage -= 20;
			if (flDamage <= 0)
			{
				UTIL_Ricochet(ptr->vecEndPos, 1.0);
				flDamage = 0.01;
			}
		}
		// it's head shot anyways
		ptr->iHitgroup = HITGROUP_HEAD;
	}
}

CBaseEntity* CSquadMonsterWithHGruntAIUtil::DropMP5(CBaseMonster* outer)
{
	Vector	vecGunPos;
	Vector	vecGunAngles;

	outer->GetAttachment(0, vecGunPos, vecGunAngles);

	CBaseEntity* pGun;
	
	if (UTIL_IsPlayerEquippedWithArmor(UTIL_GetLocalPlayer()))
		pGun = outer->DropItem("weapon_barney9mmar", vecGunPos, vecGunAngles);
	else
		pGun = outer->DropItem("weapon_9mmAR", vecGunPos, vecGunAngles);

	return pGun;
}

CBaseEntity* CSquadMonsterWithHGruntAIUtil::DropShotgun(CBaseMonster* outer)
{
	Vector	vecGunPos;
	Vector	vecGunAngles;

	outer->GetAttachment(0, vecGunPos, vecGunAngles);

	CBaseEntity* pGun;
	if (UTIL_IsPlayerEquippedWithArmor(UTIL_GetLocalPlayer()))
		pGun = outer->DropItem("weapon_barneyshotgun", vecGunPos, vecGunAngles);
	else
		pGun = outer->DropItem("weapon_shotgun", vecGunPos, vecGunAngles);

	return pGun;
}

CBaseEntity* CSquadMonsterWithHGruntAIUtil::DropLauncherGrenade(CBaseMonster* outer)
{
	Vector	vecGunPos;
	Vector	vecGunAngles;

	outer->GetAttachment(0, vecGunPos, vecGunAngles);

	CBaseEntity* pGrenade = outer->DropItem("ammo_ARgrenades", outer->BodyTarget(outer->pev->origin), vecGunAngles);
	return pGrenade;
}
