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

enum barney9mmhg_e {
	BARNEY9MMHG_IDLE1 = 0,
	BARNEY9MMHG_IDLE2,
	BARNEY9MMHG_IDLE3,
	BARNEY9MMHG_SHOOT,
	BARNEY9MMHG_SHOOT_EMPTY,
	BARNEY9MMHG_RELOAD,
	BARNEY9MMHG_RELOAD_NOT_EMPTY,
	BARNEY9MMHG_DRAW,
	BARNEY9MMHG_HOLSTER,
	BARNEY9MMHG_ADD_SILENCER
};

LINK_ENTITY_TO_CLASS(weapon_barney9mmhg, CBarney9MMHandgun);

int CBarney9MMHandgun::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "9mm";
	p->iMaxAmmo1 = BARNEY9MMHG_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = BARNEY9MMHG_MAX_CLIP;
	p->iSlot = 1;
	p->iPosition = 1;
	p->iFlags = 0;
	p->iId = m_iId = GetWeaponID();
	p->iWeight = BARNEY9MMHG_WEIGHT;

	return 1;
}

int CBarney9MMHandgun::GetWeaponID()
{
	return WEAPON_BARNEY9MMHG;
}

const char* CBarney9MMHandgun::GetThirdpersonModel()
{
	return "models/p_9mmhandgun.mdl";
}

const char* CBarney9MMHandgun::GetViewModel()
{
	return "models/v_barney9mmhg.mdl";
}

const char* CBarney9MMHandgun::GetWorldModel()
{
	return "models/w_9mmhandgun.mdl";
}

void CBarney9MMHandgun::PrecacheModels()
{
	BaseClass::PrecacheModels();

	PRECACHE_MODEL("models/v_barney9mmhg.mdl");
	PRECACHE_MODEL("models/w_9mmhandgun.mdl");
	PRECACHE_MODEL("models/p_9mmhandgun.mdl");
}

void CBarney9MMHandgun::PrecacheEvents()
{
	m_usFireHandgun1 = PRECACHE_EVENT(1, "events/glock1.sc");
	m_usFireHandgun2 = PRECACHE_EVENT(1, "events/glock2.sc");
}

int CBarney9MMHandgun::GetBulletType()
{
	return BULLET_PLAYER_9MM;
}

void CBarney9MMHandgun::GiveFirstTimeSpawnDefaultAmmo()
{
	m_iDefaultAmmo = BARNEY9MMHG_DEFAULT_GIVE;
}

const unsigned short& CBarney9MMHandgun::GetPrimaryFireEvent() const
{
	return m_usFireHandgun1;
}

const unsigned short& CBarney9MMHandgun::GetSecondaryFireEvent() const
{
	return m_usFireHandgun2;
}

int CBarney9MMHandgun::GetDeploySequence()
{
	return BARNEY9MMHG_DRAW;
}

int CBarney9MMHandgun::GetReloadSequence()
{
	return BARNEY9MMHG_RELOAD_NOT_EMPTY;
}

int CBarney9MMHandgun::GetReloadEmptySequence()
{
	return BARNEY9MMHG_RELOAD;
}

int CBarney9MMHandgun::GetHolsterSequence()
{
	return BARNEY9MMHG_HOLSTER;
}

float CBarney9MMHandgun::GetReloadSequenceDuration()
{
	return 1.5f;
}

float CBarney9MMHandgun::GetReloadEmptySequenceDuration()
{
	return 1.5f;
}

float CBarney9MMHandgun::GetHolsterSequenceDuration()
{
	return 1;
}

float CBarney9MMHandgun::PlayIdleAnimation()
{
	int iAnim;
	float duration;
	float flRand = UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0.0, 1.0);

	if (flRand <= 0.3 + 0 * 0.75)
	{
		iAnim = BARNEY9MMHG_IDLE3;
		duration = 49.0 / 16.0;
	}
	else if (flRand <= 0.6 + 0 * 0.875)
	{
		iAnim = BARNEY9MMHG_IDLE1;
		duration = 60.0 / 16.0;
	}
	else
	{
		iAnim = BARNEY9MMHG_IDLE2;
		duration = 40.0 / 16.0;
	}
	SendWeaponAnim(iAnim, 1);

	return duration;
}
