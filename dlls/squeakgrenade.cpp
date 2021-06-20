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
#if !defined( OEM_BUILD ) && !defined( HLDEMO_BUILD )

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

enum w_squeak_e {
	WSQUEAK_IDLE1 = 0,
	WSQUEAK_FIDGET,
	WSQUEAK_JUMP,
	WSQUEAK_RUN,
};

enum squeak_e {
	SQUEAK_IDLE1 = 0,
	SQUEAK_FIDGETFIT,
	SQUEAK_FIDGETNIP,
	SQUEAK_DOWN,
	SQUEAK_UP,
	SQUEAK_THROW
};

#ifndef CLIENT_DLL

class CSqueakGrenade : public CBaseMonsterWithSnarkAI
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

LINK_ENTITY_TO_CLASS(monster_snark, CSqueakGrenade);

void CSqueakGrenade::PrecacheModels()
{
	BaseClass::PrecacheModels();

	PRECACHE_MODEL("models/w_squeak.mdl");
}

void CSqueakGrenade::PrecacheSounds()
{
	BaseClass::PrecacheSounds();

	PRECACHE_SOUND("squeek/sqk_blast1.wav");
	PRECACHE_SOUND("squeek/sqk_die1.wav");
	PRECACHE_SOUND("squeek/sqk_hunt1.wav");
	PRECACHE_SOUND("squeek/sqk_hunt2.wav");
	PRECACHE_SOUND("squeek/sqk_hunt3.wav");
	PRECACHE_SOUND("squeek/sqk_deploy1.wav");
}
void CSqueakGrenade::SetModel()
{
	SET_MODEL(ENT(pev), "models/w_squeak.mdl");
}

void CSqueakGrenade::SetVoicePitch()
{
	m_minVoicePitch = 60;
	m_maxVoicePitch = 155;
}

float CSqueakGrenade::GetFirstTimeSpawnHealth()
{
	return gSkillData.snarkHealth;
}

float CSqueakGrenade::GetBlastDamage()
{
	return gSkillData.snarkDmgPop;
}

float CSqueakGrenade::GetBiteDamage()
{
	return gSkillData.snarkDmgBite;
}

#define SQUEEK_DETONATE_DELAY 15.0

float CSqueakGrenade::GetDetonateDelay()
{
	return SQUEEK_DETONATE_DELAY;
}

void CSqueakGrenade::DeathSound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "squeek/sqk_die1.wav", 1, ATTN_NORM, 0, 100 + RANDOM_LONG(0, 0x3F));
}

void CSqueakGrenade::BlastSound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_ITEM, "squeek/sqk_blast1.wav", 1, 0.5, 0, PITCH_NORM);
}

void CSqueakGrenade::HuntSound()
{
	float flpitch = GetVoicePitch();
	if (flpitch < 80)
		flpitch = 80;

	float flRndSound = RANDOM_FLOAT(0, 1);

	if (flRndSound <= 0.33)
		EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "squeek/sqk_hunt1.wav", 1, ATTN_NORM, 0, (int)flpitch);
	else if (flRndSound <= 0.66)
		EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "squeek/sqk_hunt2.wav", 1, ATTN_NORM, 0, (int)flpitch);
	else
		EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "squeek/sqk_hunt3.wav", 1, ATTN_NORM, 0, (int)flpitch);
}

void CSqueakGrenade::BiteSound()
{
	float flpitch = GetVoicePitch();
	if (flpitch < 80)
		flpitch = 80;

	EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, "squeek/sqk_deploy1.wav", 1.0, ATTN_NORM, 0, (int)flpitch);
}

int CSqueakGrenade::GetMoveSequence()
{
	return WSQUEAK_RUN;
}

#endif

LINK_ENTITY_TO_CLASS(weapon_snark, CSqueak);

const char* CSqueak::pDeploySounds[] =
{
	"squeek/sqk_hunt2.wav",
	"squeek/sqk_hunt3.wav",
};

const char* CSqueak::pHuntSounds[] =
{
	"squeek/sqk_hunt2.wav",
	"squeek/sqk_hunt3.wav",
};

int CSqueak::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "Snarks";
	p->iMaxAmmo1 = SNARK_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 4;
	p->iPosition = 4;
	p->iId = m_iId = GetWeaponID();
	p->iWeight = SNARK_WEIGHT;
	p->iFlags = ITEM_FLAG_LIMITINWORLD | ITEM_FLAG_EXHAUSTIBLE;

	return 1;
}

int CSqueak::GetWeaponID()
{
	return WEAPON_SNARK;
}

const char* CSqueak::GetThirdpersonModel()
{
	return "models/p_squeak.mdl";
}

const char* CSqueak::GetViewModel()
{
	return "models/v_squeak.mdl";
}

const char* CSqueak::GetWorldModel()
{
	return "models/w_sqknest.mdl";
}

void CSqueak::PrecacheModels()
{
	PRECACHE_MODEL("models/w_sqknest.mdl");
	PRECACHE_MODEL("models/v_squeak.mdl");
	PRECACHE_MODEL("models/p_squeak.mdl");
}

void CSqueak::PrecacheSounds()
{
	PRECACHE_SOUND_ARRAY(pDeploySounds);
	PRECACHE_SOUND_ARRAY(pHuntSounds);
}

void CSqueak::PrecacheEvents()
{
	m_usSnarkFire = PRECACHE_EVENT ( 1, "events/snarkfire.sc" );
}

void CSqueak::GiveFirstTimeSpawnDefaultAmmo()
{
	m_iDefaultAmmo = SNARK_DEFAULT_GIVE;
}

const unsigned short& CSqueak::GetThrowEvent() const
{
	return m_usSnarkFire;
}

const char* CSqueak::GetClassnameOfMonsterToThrow()
{
	return "monster_snark";
}

int CSqueak::GetDeploySequence()
{
	return SQUEAK_UP;
}

int CSqueak::GetHolsterSequence()
{
	return SQUEAK_DOWN;
}

float CSqueak::GetHolsterSequenceDuration()
{
	return 21.0f / 16.0f;
}

float CSqueak::PlayIdleAnimation()
{
	int iAnim;
	float duration;

	float flRand = UTIL_SharedRandomFloat( m_pPlayer->random_seed, 0, 1 );
	if (flRand <= 0.75)
	{
		iAnim = SQUEAK_IDLE1;
		duration = 30.0 / 16.0;
	}
	else if (flRand <= 0.875)
	{
		iAnim = SQUEAK_FIDGETFIT;
		duration = 70.0 / 16.0;
	}
	else
	{
		iAnim = SQUEAK_FIDGETNIP;
		duration = 80.0 / 16.0;
	}
	SendWeaponAnim( iAnim );
	return duration;
}

void CSqueak::PlayDeploySound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, RANDOM_SOUND_ARRAY(pDeploySounds), 1, ATTN_NORM, 0, 100);
}

void CSqueak::PlayHuntSound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, RANDOM_SOUND_ARRAY(pHuntSounds), 1, ATTN_NORM, 0, 105);
}

#endif
