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

enum mp5_e
{
	_9MMM41A_LONGIDLE = 0,
	_9MMM41A_IDLE1,
	_9MMM41A_LAUNCH,
	_9MMM41A_RELOAD,
	_9MMM41A_DEPLOY,
	_9MMM41A_FIRE1,
	_9MMM41A_FIRE2,
	_9MMM41A_FIRE3,
	_9MMM41A_HOLSTER,
};

LINK_ENTITY_TO_CLASS(weapon_9mmm41a, C9MMM41A);

int C9MMM41A::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "9mm";
	p->iMaxAmmo1 = _9MMM41A_MAX_CARRY;
	p->pszAmmo2 = "ARgrenades";
	p->iMaxAmmo2 = M203_GRENADE_MAX_CARRY;
	p->iMaxClip = _9MMM41A_MAX_CLIP;
	p->iSlot = 2;
	p->iPosition = 5;
	p->iFlags = 0;
	p->iId = m_iId = GetWeaponID();
	p->iWeight = _9MMM41A_WEIGHT;

	return 1;
}

int C9MMM41A::GetWeaponID()
{
	return WEAPON_9MMM41A;
}

const char* C9MMM41A::GetThirdpersonModel()
{
	return "models/p_9mmm41a.mdl";
}

const char* C9MMM41A::GetViewModel()
{
	return "models/v_9mmm41a.mdl";
}

const char* C9MMM41A::GetWorldModel()
{
	return "models/w_9mmm41a.mdl";
}

void C9MMM41A::PrecacheModels()
{
	BaseClass::PrecacheModels();

	PRECACHE_MODEL("models/v_9mmm41a.mdl");
	PRECACHE_MODEL("models/w_9mmm41a.mdl");
	PRECACHE_MODEL("models/p_9mmm41a.mdl");
}

void C9MMM41A::PrecacheEvents()
{
	m_us9MMM41A = PRECACHE_EVENT(1, "events/m41a.sc");
	m_us9MMM41A2 = PRECACHE_EVENT(1, "events/m41a2.sc");
}

void C9MMM41A::GiveFirstTimeSpawnDefaultAmmo()
{
	m_iDefaultAmmo = _9MMM41A_DEFAULT_GIVE;
}

const unsigned short& C9MMM41A::GetFireEvent() const
{
	return m_us9MMM41A;
}

const unsigned short& C9MMM41A::GetGrenadeLaunchEvent() const
{
	return m_us9MMM41A2;
}

int C9MMM41A::GetBulletType()
{
	return BULLET_PLAYER_9MMM41A;
}

int C9MMM41A::GetDeploySequence()
{
	return _9MMM41A_DEPLOY;
}

int C9MMM41A::GetHolsterSequence()
{
	return _9MMM41A_HOLSTER;
}

int C9MMM41A::GetReloadSequence()
{
	return _9MMM41A_RELOAD;
}

float C9MMM41A::GetHolsterSequenceDuration()
{
	return 16.0f / 30.0f;
}

float C9MMM41A::GetReloadSequenceDuration()
{
	return 1.5;
}

float C9MMM41A::PlayIdleAnimation()
{
	int iAnim;
	switch (RANDOM_LONG(0, 1))
	{
	case 0:
		iAnim = _9MMM41A_LONGIDLE;
		break;

	default:
	case 1:
		iAnim = _9MMM41A_IDLE1;
		break;
	}

	SendWeaponAnim(iAnim);
	return UTIL_SharedRandomFloat(m_pPlayer->random_seed, 10, 15);
}
