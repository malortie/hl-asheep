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
	BARNEY9MMAR_LONGIDLE = 0,
	BARNEY9MMAR_IDLE1,
	BARNEY9MMAR_LAUNCH,
	BARNEY9MMAR_RELOAD,
	BARNEY9MMAR_DEPLOY,
	BARNEY9MMAR_FIRE1,
	BARNEY9MMAR_FIRE2,
	BARNEY9MMAR_FIRE3,
	BARNEY9MMAR_HOLSTER,
};

LINK_ENTITY_TO_CLASS(weapon_barney9mmar, CBarney9MMAR);

int CBarney9MMAR::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "9mm";
	p->iMaxAmmo1 = BARNEY9MMAR_MAX_CARRY;
	p->pszAmmo2 = "ARgrenades";
	p->iMaxAmmo2 = M203_GRENADE_MAX_CARRY;
	p->iMaxClip = BARNEY9MMAR_MAX_CLIP;
	p->iSlot = 2;
	p->iPosition = 1;
	p->iFlags = 0;
	p->iId = m_iId = GetWeaponID();
	p->iWeight = BARNEY9MMAR_WEIGHT;

	return 1;
}

int CBarney9MMAR::GetWeaponID()
{
	return WEAPON_BARNEY9MMAR;
}

const char* CBarney9MMAR::GetThirdpersonModel()
{
	return "models/p_9mmAR.mdl";
}

const char* CBarney9MMAR::GetViewModel()
{
	return "models/v_barney9mmar.mdl";
}

const char* CBarney9MMAR::GetWorldModel()
{
	return "models/w_9mmAR.mdl";
}

void CBarney9MMAR::PrecacheModels()
{
	BaseClass::PrecacheModels();

	PRECACHE_MODEL("models/v_barney9mmar.mdl");
	PRECACHE_MODEL("models/w_9mmAR.mdl");
	PRECACHE_MODEL("models/p_9mmAR.mdl");
}

void CBarney9MMAR::PrecacheEvents()
{
	m_usBarney9MMAR = PRECACHE_EVENT(1, "events/mp5.sc");
	m_usBarney9MMAR2 = PRECACHE_EVENT(1, "events/mp52.sc");
}

void CBarney9MMAR::GiveFirstTimeSpawnDefaultAmmo()
{
	m_iDefaultAmmo = MP5_DEFAULT_GIVE;
}

const unsigned short& CBarney9MMAR::GetFireEvent() const
{
	return m_usBarney9MMAR;
}

const unsigned short& CBarney9MMAR::GetGrenadeLaunchEvent() const
{
	return m_usBarney9MMAR2;
}

int CBarney9MMAR::GetBulletType()
{
	return BULLET_PLAYER_MP5;
}

int CBarney9MMAR::GetDeploySequence()
{
	return BARNEY9MMAR_DEPLOY;
}

int CBarney9MMAR::GetHolsterSequence()
{
	return BARNEY9MMAR_HOLSTER;
}

int CBarney9MMAR::GetReloadSequence()
{
	return BARNEY9MMAR_RELOAD;
}

float CBarney9MMAR::GetHolsterSequenceDuration()
{
	return 16.0f / 30.0f;
}

float CBarney9MMAR::GetReloadSequenceDuration()
{
	return 1.5;
}

float CBarney9MMAR::PlayIdleAnimation()
{
	int iAnim;
	switch (RANDOM_LONG(0, 1))
	{
	case 0:
		iAnim = BARNEY9MMAR_LONGIDLE;
		break;

	default:
	case 1:
		iAnim = BARNEY9MMAR_IDLE1;
		break;
	}

	SendWeaponAnim(iAnim);
	return UTIL_SharedRandomFloat(m_pPlayer->random_seed, 10, 15);
}
