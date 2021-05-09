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
#include "player.h"
#include "weapons.h"
#include "gamerules.h"
#include "soundent.h"
#include "basemonsterwithsnarkai.h"

enum w_toad_e {
	TOAD_IDLE1 = 0,
	TOAD_FIDGET,
	TOAD_JUMP,
	TOAD_RUN,
};

class CToad : public CBaseMonsterWithSnarkAI
{
	typedef CBaseMonsterWithSnarkAI BaseClass;
public:
protected:
	virtual float GetBlastDamage();
	virtual float GetBiteDamage();

	virtual float GetDetonateDelay();

	virtual void PrecacheModels();
	virtual void PrecacheSounds();
	virtual void SetModel();
	virtual void SetVoicePitch();
	virtual float GetFirstTimeSpawnHealth();

	virtual void DeathSound();
	virtual void BlastSound();
	virtual void HuntSound();
	virtual void BiteSound();

	virtual int GetMoveSequence();
};

LINK_ENTITY_TO_CLASS(monster_toad, CToad);

void CToad::PrecacheModels()
{
	BaseClass::PrecacheModels();

	PRECACHE_MODEL("models/w_toad.mdl");
}

void CToad::PrecacheSounds()
{
	BaseClass::PrecacheSounds();

	PRECACHE_SOUND("toad/toad_blast1.wav");
	PRECACHE_SOUND("toad/toad_die1.wav");
	PRECACHE_SOUND("toad/toad_hunt1.wav");
	PRECACHE_SOUND("toad/toad_hunt2.wav");
	PRECACHE_SOUND("toad/toad_hunt3.wav");
	PRECACHE_SOUND("toad/toad_deploy1.wav");
}
void CToad::SetModel()
{
	SET_MODEL(ENT(pev), "models/w_toad.mdl");
}

void CToad::SetVoicePitch()
{
	m_minVoicePitch = 60.0;
	m_maxVoicePitch = 155.0;
}

float CToad::GetFirstTimeSpawnHealth()
{
	return gSkillData.toadHealth;
}

float CToad::GetBlastDamage()
{
	return gSkillData.toadDmgPop;
}

float CToad::GetBiteDamage()
{
	return gSkillData.toadDmgBite;
}

#define TOAD_DETONATE_DELAY 15.0

float CToad::GetDetonateDelay()
{
	return TOAD_DETONATE_DELAY;
}

void CToad::DeathSound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "toad/toad_die1.wav", 1, ATTN_NORM, 0, 100 + RANDOM_LONG(0, 0x3F));
}

void CToad::BlastSound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_ITEM, "toad/toad_blast1.wav", 1, 0.5, 0, PITCH_NORM);
}

void CToad::HuntSound()
{
	float flpitch = GetVoicePitch();
	if (flpitch < 80)
		flpitch = 80;

	float flRndSound = RANDOM_FLOAT(0, 1);

	if (flRndSound <= 0.33)
		EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "toad/toad_hunt1.wav", 1, ATTN_NORM, 0, (int)flpitch);
	else if (flRndSound <= 0.66)
		EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "toad/toad_hunt2.wav", 1, ATTN_NORM, 0, (int)flpitch);
	else
		EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "toad/toad_hunt3.wav", 1, ATTN_NORM, 0, (int)flpitch);
}

void CToad::BiteSound()
{
	float flpitch = GetVoicePitch();
	if (flpitch < 80)
		flpitch = 80;

	EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, "toad/toad_deploy1.wav", 1.0, ATTN_NORM, 0, (int)flpitch);
}

int CToad::GetMoveSequence()
{
	return TOAD_RUN;
}
