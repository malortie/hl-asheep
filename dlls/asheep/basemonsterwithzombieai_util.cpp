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
#include "basemonsterwithzombieai.h"

const char *ZombieSounds::pAttackHitSounds[] =
{
	"zombie/claw_strike1.wav",
	"zombie/claw_strike2.wav",
	"zombie/claw_strike3.wav",
};

const char *ZombieSounds::pAttackMissSounds[] =
{
	"zombie/claw_miss1.wav",
	"zombie/claw_miss2.wav",
};

const char *ZombieSounds::pAttackSounds[] =
{
	"zombie/zo_attack1.wav",
	"zombie/zo_attack2.wav",
};

const char *ZombieSounds::pIdleSounds[] =
{
	"zombie/zo_idle1.wav",
	"zombie/zo_idle2.wav",
	"zombie/zo_idle3.wav",
	"zombie/zo_idle4.wav",
};

const char *ZombieSounds::pAlertSounds[] =
{
	"zombie/zo_alert10.wav",
	"zombie/zo_alert20.wav",
	"zombie/zo_alert30.wav",
};

const char *ZombieSounds::pPainSounds[] =
{
	"zombie/zo_pain1.wav",
	"zombie/zo_pain2.wav",
};


const char *ZombieSounds::GetRandomAttackHitSound()
{
	return RANDOM_SOUND_ARRAY(pAttackHitSounds);
}

const char *ZombieSounds::GetRandomAttackMissSound()
{
	return RANDOM_SOUND_ARRAY(pAttackMissSounds);
}

const char *ZombieSounds::GetRandomAttackSound()
{
	return RANDOM_SOUND_ARRAY(pAttackSounds);
}

const char *ZombieSounds::GetRandomIdleSound()
{
	return RANDOM_SOUND_ARRAY(pIdleSounds);
}

const char *ZombieSounds::GetRandomAlertSound()
{
	return RANDOM_SOUND_ARRAY(pAlertSounds);
}

const char *ZombieSounds::GetRandomPainSound()
{
	return RANDOM_SOUND_ARRAY(pPainSounds);
}

void CBaseMonsterWithZombieAIUtil::PrecacheSharedSounds()
{
	PRECACHE_SOUND_ARRAY(ZombieSounds::pAttackSounds);
	PRECACHE_SOUND_ARRAY(ZombieSounds::pIdleSounds);
	PRECACHE_SOUND_ARRAY(ZombieSounds::pAlertSounds);
	PRECACHE_SOUND_ARRAY(ZombieSounds::pPainSounds);
	PRECACHE_SOUND_ARRAY(ZombieSounds::pAttackHitSounds);
	PRECACHE_SOUND_ARRAY(ZombieSounds::pAttackMissSounds);
}

void CBaseMonsterWithZombieAIUtil::PlayPainSound(CBaseMonsterWithZombieAI* outer)
{
	int pitch = 95 + RANDOM_LONG(0, 9);

	if (RANDOM_LONG(0, 5) < 2)
		EMIT_SOUND_DYN(ENT(outer->pev), CHAN_VOICE, ZombieSounds::GetRandomPainSound(), 1.0, ATTN_NORM, 0, pitch);
}

void CBaseMonsterWithZombieAIUtil::PlayAlertSound(CBaseMonsterWithZombieAI* outer)
{
	int pitch = 95 + RANDOM_LONG(0, 9);

	EMIT_SOUND_DYN(ENT(outer->pev), CHAN_VOICE, ZombieSounds::GetRandomAlertSound(), 1.0, ATTN_NORM, 0, pitch);
}

void CBaseMonsterWithZombieAIUtil::PlayIdleSound(CBaseMonsterWithZombieAI* outer)
{
	int pitch = 100 + RANDOM_LONG(-5, 5);

	// Play a random idle sound
	EMIT_SOUND_DYN(ENT(outer->pev), CHAN_VOICE, ZombieSounds::GetRandomIdleSound(), 1.0, ATTN_NORM, 0, pitch);
}

void CBaseMonsterWithZombieAIUtil::PlayAttackSound(CBaseMonsterWithZombieAI* outer)
{
	int pitch = 100 + RANDOM_LONG(-5, 5);

	// Play a random attack sound
	EMIT_SOUND_DYN(ENT(outer->pev), CHAN_VOICE, ZombieSounds::GetRandomAttackSound(), 1.0, ATTN_NORM, 0, pitch);
}

void CBaseMonsterWithZombieAIUtil::PlayAttackHitSound(CBaseMonsterWithZombieAI* outer)
{
	EMIT_SOUND_DYN(ENT(outer->pev), CHAN_WEAPON, ZombieSounds::GetRandomAttackHitSound(), 1.0, ATTN_NORM, 0, 100 + RANDOM_LONG(-5, 5));
}

void CBaseMonsterWithZombieAIUtil::PlayAttackMissSound(CBaseMonsterWithZombieAI* outer)
{
	EMIT_SOUND_DYN(ENT(outer->pev), CHAN_WEAPON, ZombieSounds::GetRandomAttackMissSound(), 1.0, ATTN_NORM, 0, 100 + RANDOM_LONG(-5, 5));
}
