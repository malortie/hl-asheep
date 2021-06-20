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

enum handgrenade_e {
	HANDGRENADE_IDLE = 0,
	HANDGRENADE_FIDGET,
	HANDGRENADE_PINPULL,
	HANDGRENADE_THROW1,	// toss
	HANDGRENADE_THROW2,	// medium
	HANDGRENADE_THROW3,	// hard
	HANDGRENADE_HOLSTER,
	HANDGRENADE_DRAW
};

LINK_ENTITY_TO_CLASS(weapon_handgrenade, CHandGrenade);

int CHandGrenade::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "Hand Grenade";
	p->iMaxAmmo1 = HANDGRENADE_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 4;
	p->iPosition = 0;
	p->iId = m_iId = GetWeaponID();
	p->iWeight = HANDGRENADE_WEIGHT;
	p->iFlags = ITEM_FLAG_LIMITINWORLD | ITEM_FLAG_EXHAUSTIBLE;

	return 1;
}

int CHandGrenade::GetWeaponID()
{
	return WEAPON_HANDGRENADE;
}

const char* CHandGrenade::GetThirdpersonModel()
{
	return "models/p_grenade.mdl";
}

const char* CHandGrenade::GetViewModel()
{
	return "models/v_grenade.mdl";
}

const char* CHandGrenade::GetWorldModel()
{
	return "models/w_grenade.mdl";
}

void CHandGrenade::PrecacheModels()
{
	PRECACHE_MODEL("models/v_grenade.mdl");
	PRECACHE_MODEL("models/w_grenade.mdl");
	PRECACHE_MODEL("models/p_grenade.mdl");
}

void CHandGrenade::GiveFirstTimeSpawnDefaultAmmo()
{
	m_iDefaultAmmo = HANDGRENADE_DEFAULT_GIVE;
}

int CHandGrenade::GetDeploySequence()
{
	return HANDGRENADE_DRAW;
}

int CHandGrenade::GetHolsterSequence()
{
	return HANDGRENADE_HOLSTER;
}

float CHandGrenade::GetHolsterSequenceDuration()
{
	return 16.0f / 30.0f;
}

float CHandGrenade::PlayPinPullAnimation()
{
	SendWeaponAnim(HANDGRENADE_PINPULL);
	return 0.5f;
}

float CHandGrenade::PlayThrowAnimation(float tossVelocity)
{
	if (tossVelocity < 500)
	{
		SendWeaponAnim(HANDGRENADE_THROW1);
	}
	else if (tossVelocity < 1000)
	{
		SendWeaponAnim(HANDGRENADE_THROW2);
	}
	else
	{
		SendWeaponAnim(HANDGRENADE_THROW3);
	}

	return 0.5f;
}

float CHandGrenade::PlayIdleAnimation()
{
	int iAnim;
	float duration;
	float flRand = UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0, 1);
	if (flRand <= 0.75)
	{
		iAnim = HANDGRENADE_IDLE;
		duration = UTIL_SharedRandomFloat(m_pPlayer->random_seed, 10, 15);// how long till we do this again.
	}
	else
	{
		iAnim = HANDGRENADE_FIDGET;
		duration = 75.0 / 30.0;
	}

	SendWeaponAnim(iAnim);

	return duration;
}
