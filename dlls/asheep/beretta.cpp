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

enum beretta_e {
	BERETTA_IDLE1 = 0,
	BERETTA_IDLE2,
	BERETTA_IDLE3,
	BERETTA_SHOOT,
	BERETTA_SHOOT_EMPTY,
	BERETTA_RELOAD,
	BERETTA_RELOAD_NOT_EMPTY,
	BERETTA_DRAW,
	BERETTA_HOLSTER,
	BERETTA_ADD_SILENCER
};

LINK_ENTITY_TO_CLASS(weapon_beretta, CBeretta);

int CBeretta::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "9mm";
	p->iMaxAmmo1 = BERETTA_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = BERETTA_MAX_CLIP;
	p->iSlot = 1;
	p->iPosition = 2;
	p->iFlags = 0;
	p->iId = m_iId = GetWeaponID();
	p->iWeight = BERETTA_WEIGHT;

	return 1;
}

int CBeretta::GetWeaponID()
{
	return WEAPON_BERETTA;
}

const char* CBeretta::GetThirdpersonModel()
{
	return "models/p_9mmberetta.mdl";
}

const char* CBeretta::GetViewModel()
{
	return "models/v_9mmberetta.mdl";
}

const char* CBeretta::GetWorldModel()
{
	return "models/w_9mmberetta.mdl";
}

void CBeretta::PrecacheModels()
{
	BaseClass::PrecacheModels();

	PRECACHE_MODEL("models/v_9mmberetta.mdl");
	PRECACHE_MODEL("models/w_9mmberetta.mdl");
	PRECACHE_MODEL("models/p_9mmberetta.mdl");
}

void CBeretta::PrecacheEvents()
{
	m_usFireBeretta1 = PRECACHE_EVENT(1, "events/beretta1.sc");
	m_usFireBeretta2 = PRECACHE_EVENT(1, "events/beretta2.sc");
}

int CBeretta::GetBulletType()
{
	return BULLET_PLAYER_BERETTA;
}

void CBeretta::GiveFirstTimeSpawnDefaultAmmo()
{
	m_iDefaultAmmo = BERETTA_DEFAULT_GIVE;
}

const unsigned short& CBeretta::GetPrimaryFireEvent() const
{
	return m_usFireBeretta1;
}

const unsigned short& CBeretta::GetSecondaryFireEvent() const
{
	return m_usFireBeretta2;
}

int CBeretta::GetDeploySequence()
{
	return BERETTA_DRAW;
}

int CBeretta::GetReloadSequence()
{
	return BERETTA_RELOAD_NOT_EMPTY;
}

int CBeretta::GetReloadEmptySequence()
{
	return BERETTA_RELOAD;
}

int CBeretta::GetHolsterSequence()
{
	return BERETTA_HOLSTER;
}

float CBeretta::GetReloadSequenceDuration()
{
	return 1.5f;
}

float CBeretta::GetReloadEmptySequenceDuration()
{
	return 1.5f;
}

float CBeretta::GetHolsterSequenceDuration()
{
	return 1;
}

float CBeretta::PlayIdleAnimation()
{
	int iAnim;
	float duration;
	float flRand = UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0.0, 1.0);

	if (flRand <= 0.3 + 0 * 0.75)
	{
		iAnim = BERETTA_IDLE3;
		duration = 49.0 / 16;
	}
	else if (flRand <= 0.6 + 0 * 0.875)
	{
		iAnim = BERETTA_IDLE1;
		duration = 60.0 / 16.0;
	}
	else
	{
		iAnim = BERETTA_IDLE2;
		duration = 40.0 / 16.0;
	}
	SendWeaponAnim(iAnim, 1);

	return duration;
}
