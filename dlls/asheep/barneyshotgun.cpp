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

enum barneyshotgun_e {
	BARNEYSHOTGUN_IDLE = 0,
	BARNEYSHOTGUN_FIRE,
	BARNEYSHOTGUN_FIRE2,
	BARNEYSHOTGUN_RELOAD,
	BARNEYSHOTGUN_PUMP,
	BARNEYSHOTGUN_START_RELOAD,
	BARNEYSHOTGUN_DRAW,
	BARNEYSHOTGUN_HOLSTER,
	BARNEYSHOTGUN_IDLE4,
	BARNEYSHOTGUN_IDLE_DEEP
};

LINK_ENTITY_TO_CLASS(weapon_barneyshotgun, CBarneyShotgun);

const char* CBarneyShotgun::pReloadSounds[] =
{
	"weapons/reload1.wav",
	"weapons/reload3.wav",
};

int CBarneyShotgun::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "buckshot";
	p->iMaxAmmo1 = BARNEYSHOTGUN_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = BARNEYSHOTGUN_MAX_CLIP;
	p->iSlot = 2;
	p->iPosition = 3;
	p->iFlags = 0;
	p->iId = m_iId = GetWeaponID();
	p->iWeight = BARNEYSHOTGUN_WEIGHT;

	return 1;
}

int CBarneyShotgun::GetWeaponID()
{
	return WEAPON_BARNEYSHOTGUN;
}

const char* CBarneyShotgun::GetThirdpersonModel()
{
	return "models/p_shotgun.mdl";
}

const char* CBarneyShotgun::GetViewModel()
{
	return "models/v_barneyshotgun.mdl";
}

const char* CBarneyShotgun::GetWorldModel()
{
	return "models/w_shotgun.mdl";
}

void CBarneyShotgun::PrecacheModels()
{
	BaseClass::PrecacheModels();

	PRECACHE_MODEL("models/v_barneyshotgun.mdl");
	PRECACHE_MODEL("models/w_shotgun.mdl");
	PRECACHE_MODEL("models/p_shotgun.mdl");
}

void CBarneyShotgun::GiveFirstTimeSpawnDefaultAmmo()
{
	m_iDefaultAmmo = BARNEYSHOTGUN_DEFAULT_GIVE;
}

int CBarneyShotgun::GetDeploySequence()
{
	return BARNEYSHOTGUN_DRAW;
}

int CBarneyShotgun::GetHolsterSequence()
{
	return BARNEYSHOTGUN_HOLSTER;
}

float CBarneyShotgun::GetHolsterSequenceDuration()
{
	return 11.0f / 30.0f;
}

float CBarneyShotgun::PlayIdleAnimation()
{
	float animDuration = 0;
	int iAnim;
	float flRand = UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0, 1);
	if (flRand <= 0.8)
	{
		iAnim = BARNEYSHOTGUN_IDLE_DEEP;
		animDuration = 60.0 / 12.0;
	}
	else if (flRand <= 0.95)
	{
		iAnim = BARNEYSHOTGUN_IDLE;
		animDuration = 20.0 / 9.0;
	}
	else
	{
		iAnim = BARNEYSHOTGUN_IDLE4;
		animDuration = 20.0 / 9.0;
	}
	SendWeaponAnim(iAnim);

	return animDuration;
}

float CBarneyShotgun::PlayReloadAnimation()
{
	SendWeaponAnim(BARNEYSHOTGUN_RELOAD);
	return 0.5f;
}

float CBarneyShotgun::PlayStartReloadAnimation()
{
	SendWeaponAnim(BARNEYSHOTGUN_START_RELOAD);
	return 0.6f;
}

float CBarneyShotgun::PlayPumpAnimation()
{
	SendWeaponAnim(BARNEYSHOTGUN_PUMP);
	return 1.5;
}

void CBarneyShotgun::PlayReloadSound()
{
	EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_ITEM, RANDOM_SOUND_ARRAY(pReloadSounds), 1, ATTN_NORM, 0, 85 + RANDOM_LONG(0, 0x1f));
}

void CBarneyShotgun::PlayPumpSound()
{
	EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/scock1.wav", 1, ATTN_NORM, 0, 95 + RANDOM_LONG(0, 0x1f));
}
