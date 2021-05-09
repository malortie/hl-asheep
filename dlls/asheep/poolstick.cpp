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
#include "gamerules.h"

enum poolstick_e {
	POOLSTICK_IDLE = 0,
	POOLSTICK_DRAW,
	POOLSTICK_HOLSTER,
	POOLSTICK_ATTACK1HIT,
	POOLSTICK_ATTACK1MISS,
	POOLSTICK_ATTACK2MISS,
	POOLSTICK_ATTACK2HIT,
	POOLSTICK_ATTACK3MISS,
	POOLSTICK_ATTACK3HIT
};

LINK_ENTITY_TO_CLASS(weapon_poolstick, CPoolstick);

const char* CPoolstick::pMissSounds[] =
{
	"weapons/pstk_miss1.wav",
};

const char* CPoolstick::pHitBodySounds[] =
{
	"weapons/pstk_hitbod1.wav",
	"weapons/pstk_hitbod2.wav",
	"weapons/pstk_hitbod3.wav",
};

const char* CPoolstick::pHitWorldSounds[] =
{
	"weapons/pstk_hit1.wav",
	"weapons/pstk_hit2.wav",
};

int CPoolstick::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = NULL;
	p->iMaxAmmo1 = -1;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 0;
	p->iPosition = 1;
	p->iId = GetWeaponID();
	p->iWeight = POOLSTICK_WEIGHT;
	return 1;
}

int CPoolstick::GetWeaponID()
{
	return WEAPON_POOLSTICK;
}

const char* CPoolstick::GetThirdpersonModel()
{
	return "models/p_poolstick.mdl";
}

const char* CPoolstick::GetViewModel()
{
	return "models/v_poolstick.mdl";
}

const char* CPoolstick::GetWorldModel()
{
	return "models/w_poolstick.mdl";
}

float CPoolstick::GetMeleeDamage()
{
	return gSkillData.plrDmgPoolstick;
}

int CPoolstick::GetBulletType() 
{
	return BULLET_PLAYER_POOLSTICK;
}

int CPoolstick::GetDeploySequence()
{
	return POOLSTICK_DRAW;
}

int CPoolstick::GetHolsterSequence()
{
	return POOLSTICK_HOLSTER;
}

float CPoolstick::GetHolsterSequenceDuration()
{
	return 13.0f / 24.0f;
}

const unsigned short& CPoolstick::GetSwingEvent() const
{
	return m_usPoolstick;
}

void CPoolstick::PrecacheModels()
{
	PRECACHE_MODEL("models/v_poolstick.mdl");
	PRECACHE_MODEL("models/w_poolstick.mdl");
	PRECACHE_MODEL("models/p_poolstick.mdl");
}

void CPoolstick::PrecacheSounds()
{
	PRECACHE_SOUND_ARRAY(pMissSounds);
	PRECACHE_SOUND_ARRAY(pHitBodySounds);
	PRECACHE_SOUND_ARRAY(pHitWorldSounds);
}

void CPoolstick::PrecacheEvents()
{
	m_usPoolstick = PRECACHE_EVENT(1, "events/poolstick.sc");
}

void CPoolstick::PlayMissAnimation()
{
	switch ((m_iSwing++) % 3)
	{
	case 0:
		SendWeaponAnim(POOLSTICK_ATTACK1MISS); break;
	case 1:
		SendWeaponAnim(POOLSTICK_ATTACK2MISS); break;
	case 2:
		SendWeaponAnim(POOLSTICK_ATTACK3MISS); break;
	}
}

void CPoolstick::PlayMissSound()
{
	EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/pstk_miss1.wav", 1, ATTN_NORM, 0, 94 + RANDOM_LONG(0, 0xF));
}

void CPoolstick::PlayHitAnimation()
{
	switch (((m_iSwing++) % 2) + 1)
	{
	case 0:
		SendWeaponAnim(POOLSTICK_ATTACK1HIT); break;
	case 1:
		SendWeaponAnim(POOLSTICK_ATTACK2HIT); break;
	case 2:
		SendWeaponAnim(POOLSTICK_ATTACK3HIT); break;
	}
}

void CPoolstick::PlayHitBodySound()
{
	EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_ITEM, RANDOM_SOUND_ARRAY(pHitBodySounds), 1, ATTN_NORM);
}

void CPoolstick::PlayHitWorldSound(float fvolbar)
{
	EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_ITEM, RANDOM_SOUND_ARRAY(pHitWorldSounds), fvolbar, ATTN_NORM);
}
