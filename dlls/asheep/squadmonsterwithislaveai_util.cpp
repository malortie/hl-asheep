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

const char *AlienSlaveSounds::pAttackHitSounds[] =
{
	"zombie/claw_strike1.wav",
	"zombie/claw_strike2.wav",
	"zombie/claw_strike3.wav",
};

const char *AlienSlaveSounds::pAttackMissSounds[] =
{
	"zombie/claw_miss1.wav",
	"zombie/claw_miss2.wav",
};

const char *AlienSlaveSounds::pPainSounds[] =
{
	"aslave/slv_pain1.wav",
	"aslave/slv_pain2.wav",
};

const char *AlienSlaveSounds::pDeathSounds[] =
{
	"aslave/slv_die1.wav",
	"aslave/slv_die2.wav",
};

const char *AlienSlaveSounds::GetRandomAttackHitSound()
{
	return RANDOM_SOUND_ARRAY(pAttackHitSounds);
}

const char *AlienSlaveSounds::GetRandomAttackMissSound()
{
	return RANDOM_SOUND_ARRAY(pAttackMissSounds);
}

const char *AlienSlaveSounds::GetRandomPainSound()
{
	return RANDOM_SOUND_ARRAY(pPainSounds);
}

const char *AlienSlaveSounds::GetRandomDeathSound()
{
	return RANDOM_SOUND_ARRAY(pDeathSounds);
}

void CSquadMonsterWithISlaveAIUtil::PrecacheSharedSounds()
{
	PRECACHE_SOUND_ARRAY(AlienSlaveSounds::pAttackHitSounds);
	PRECACHE_SOUND_ARRAY(AlienSlaveSounds::pAttackMissSounds);
	PRECACHE_SOUND_ARRAY(AlienSlaveSounds::pPainSounds);
	PRECACHE_SOUND_ARRAY(AlienSlaveSounds::pDeathSounds);
}

void CSquadMonsterWithISlaveAIUtil::PlayPainSound(CSquadMonsterWithISlaveAI* outer)
{
	EMIT_SOUND_DYN(ENT(outer->pev), CHAN_WEAPON, AlienSlaveSounds::GetRandomPainSound(), 1.0, ATTN_NORM, 0, outer->GetVoicePitch());
}

void CSquadMonsterWithISlaveAIUtil::PlayAlertSound(CSquadMonsterWithISlaveAI* outer)
{
	SENTENCEG_PlayRndSz(ENT(outer->pev), "SLV_ALERT", 0.85, ATTN_NORM, 0, outer->GetVoicePitch());
}

void CSquadMonsterWithISlaveAIUtil::PlayIdleSound(CSquadMonsterWithISlaveAI* outer)
{
	SENTENCEG_PlayRndSz(ENT(outer->pev), "SLV_IDLE", 0.85, ATTN_NORM, 0, outer->GetVoicePitch());
}

void CSquadMonsterWithISlaveAIUtil::PlayDeathSound(CSquadMonsterWithISlaveAI* outer)
{
	EMIT_SOUND_DYN(ENT(outer->pev), CHAN_WEAPON, AlienSlaveSounds::GetRandomDeathSound(), 1.0, ATTN_NORM, 0, outer->GetVoicePitch());
}

void CSquadMonsterWithISlaveAIUtil::PlayAttackHitSound(CSquadMonsterWithISlaveAI* outer)
{
	EMIT_SOUND_DYN(ENT(outer->pev), CHAN_WEAPON, AlienSlaveSounds::GetRandomAttackHitSound(), 1.0, ATTN_NORM, 0, outer->GetVoicePitch());
}

void CSquadMonsterWithISlaveAIUtil::PlayAttackMissSound(CSquadMonsterWithISlaveAI* outer)
{
	EMIT_SOUND_DYN(ENT(outer->pev), CHAN_WEAPON, AlienSlaveSounds::GetRandomAttackMissSound(), 1.0, ATTN_NORM, 0, outer->GetVoicePitch());
}
