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

enum handgrenade_e {
	BARNEYHANDGRENADE_IDLE = 0,
	BARNEYHANDGRENADE_FIDGET,
	BARNEYHANDGRENADE_PINPULL,
	BARNEYHANDGRENADE_THROW1,	// toss
	BARNEYHANDGRENADE_THROW2,	// medium
	BARNEYHANDGRENADE_THROW3,	// hard
	BARNEYHANDGRENADE_HOLSTER,
	BARNEYHANDGRENADE_DRAW
};

LINK_ENTITY_TO_CLASS(weapon_barneyhandgrenade, CBarneyHandGrenade);

int CBarneyHandGrenade::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "Hand Grenade";
	p->iMaxAmmo1 = BARNEYHANDGRENADE_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 4;
	p->iPosition = 1;
	p->iId = m_iId = GetWeaponID();
	p->iWeight = BARNEYHANDGRENADE_WEIGHT;
	p->iFlags = ITEM_FLAG_LIMITINWORLD | ITEM_FLAG_EXHAUSTIBLE;

	return 1;
}

int CBarneyHandGrenade::GetWeaponID()
{
	return WEAPON_BARNEYHANDGRENADE;
}

const char* CBarneyHandGrenade::GetThirdpersonModel()
{
	return "models/p_grenade.mdl";
}

const char* CBarneyHandGrenade::GetViewModel()
{
	return "models/v_barneygrenade.mdl";
}

const char* CBarneyHandGrenade::GetWorldModel()
{
	return "models/w_grenade.mdl";
}

void CBarneyHandGrenade::PrecacheModels()
{
	PRECACHE_MODEL("models/v_barneygrenade.mdl");
	PRECACHE_MODEL("models/w_grenade.mdl");
	PRECACHE_MODEL("models/p_grenade.mdl");
}

void CBarneyHandGrenade::GiveFirstTimeSpawnDefaultAmmo()
{
	m_iDefaultAmmo = BARNEYHANDGRENADE_DEFAULT_GIVE;
}

int CBarneyHandGrenade::GetDeploySequence()
{
	return BARNEYHANDGRENADE_DRAW;
}

int CBarneyHandGrenade::GetHolsterSequence()
{
	return BARNEYHANDGRENADE_HOLSTER;
}

float CBarneyHandGrenade::GetHolsterSequenceDuration()
{
	return 16.0f / 30.0f;
}

float CBarneyHandGrenade::PlayPinPullAnimation()
{
	SendWeaponAnim(BARNEYHANDGRENADE_PINPULL);
	return 0.5f;
}

float CBarneyHandGrenade::PlayThrowAnimation(float tossVelocity)
{
	if (tossVelocity < 500)
	{
		SendWeaponAnim(BARNEYHANDGRENADE_THROW1);
	}
	else if (tossVelocity < 1000)
	{
		SendWeaponAnim(BARNEYHANDGRENADE_THROW2);
	}
	else
	{
		SendWeaponAnim(BARNEYHANDGRENADE_THROW3);
	}

	return 0.5f;
}

float CBarneyHandGrenade::PlayIdleAnimation()
{
	int iAnim;
	float duration;
	float flRand = UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0, 1);
	if (flRand <= 0.75)
	{
		iAnim = BARNEYHANDGRENADE_IDLE;
		duration = UTIL_SharedRandomFloat(m_pPlayer->random_seed, 10, 15);// how long till we do this again.
	}
	else
	{
		iAnim = BARNEYHANDGRENADE_FIDGET;
		duration = 75.0 / 30.0;
	}

	SendWeaponAnim(iAnim);

	return duration;
}
