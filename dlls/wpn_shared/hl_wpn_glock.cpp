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

enum glock_e {
	GLOCK_IDLE1 = 0,
	GLOCK_IDLE2,
	GLOCK_IDLE3,
	GLOCK_SHOOT,
	GLOCK_SHOOT_EMPTY,
	GLOCK_RELOAD,
	GLOCK_RELOAD_NOT_EMPTY,
	GLOCK_DRAW,
	GLOCK_HOLSTER,
	GLOCK_ADD_SILENCER
};

LINK_ENTITY_TO_CLASS(weapon_glock, CGlock);
LINK_ENTITY_TO_CLASS(weapon_9mmhandgun, CGlock);

void CGlock::Spawn(void)
{
	pev->classname = MAKE_STRING("weapon_9mmhandgun"); // hack to allow for old names
	BaseClass::Spawn();
}

int CGlock::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "9mm";
	p->iMaxAmmo1 = _9MM_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = GLOCK_MAX_CLIP;
	p->iSlot = 1;
	p->iPosition = 0;
	p->iFlags = 0;
	p->iId = m_iId = GetWeaponID();
	p->iWeight = GLOCK_WEIGHT;

	return 1;
}

int CGlock::GetWeaponID()
{
	return WEAPON_GLOCK;
}

const char* CGlock::GetThirdpersonModel()
{
	return "models/p_9mmhandgun.mdl";
}

const char* CGlock::GetViewModel()
{
	return "models/v_9mmhandgun.mdl";
}

const char* CGlock::GetWorldModel()
{
	return "models/w_9mmhandgun.mdl";
}

void CGlock::PrecacheModels()
{
	BaseClass::PrecacheModels();

	PRECACHE_MODEL("models/v_9mmhandgun.mdl");
	PRECACHE_MODEL("models/w_9mmhandgun.mdl");
	PRECACHE_MODEL("models/p_9mmhandgun.mdl");
}

void CGlock::PrecacheEvents()
{
	m_usFireGlock1 = PRECACHE_EVENT(1, "events/glock1.sc");
	m_usFireGlock2 = PRECACHE_EVENT(1, "events/glock2.sc");
}

int CGlock::GetBulletType()
{
	return BULLET_PLAYER_9MM;
}

void CGlock::GiveFirstTimeSpawnDefaultAmmo()
{
	m_iDefaultAmmo = GLOCK_DEFAULT_GIVE;
}

const unsigned short& CGlock::GetPrimaryFireEvent() const
{
	return m_usFireGlock1;
}

const unsigned short& CGlock::GetSecondaryFireEvent() const
{
	return m_usFireGlock2;
}

int CGlock::GetDeploySequence()
{
	return GLOCK_DRAW;
}

int CGlock::GetReloadSequence()
{
	return GLOCK_RELOAD_NOT_EMPTY;
}

int CGlock::GetReloadEmptySequence()
{
	return GLOCK_RELOAD;
}

int CGlock::GetHolsterSequence()
{
	return GLOCK_HOLSTER;
}

float CGlock::GetReloadSequenceDuration()
{
	return 1.5f;
}

float CGlock::GetReloadEmptySequenceDuration()
{
	return 1.5f;
}

float CGlock::GetHolsterSequenceDuration()
{
	return 1;
}

float CGlock::PlayIdleAnimation()
{
	int iAnim;
	float duration;
	float flRand = UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0.0, 1.0);

	if (flRand <= 0.3 + 0 * 0.75)
	{
		iAnim = GLOCK_IDLE3;
		duration = 49.0 / 16.0;
	}
	else if (flRand <= 0.6 + 0 * 0.875)
	{
		iAnim = GLOCK_IDLE1;
		duration = 60.0 / 16.0;
	}
	else
	{
		iAnim = GLOCK_IDLE2;
		duration = 40.0 / 16.0;
	}
	SendWeaponAnim(iAnim, 1);

	return duration;
}


class CGlockAmmo : public CBasePlayerAmmo
{
	void Spawn( void )
	{ 
		Precache( );
		SET_MODEL(ENT(pev), "models/w_9mmclip.mdl");
		CBasePlayerAmmo::Spawn( );
	}
	void Precache( void )
	{
		PRECACHE_MODEL ("models/w_9mmclip.mdl");
		PRECACHE_SOUND("items/9mmclip1.wav");
	}
	BOOL AddAmmo( CBaseEntity *pOther ) 
	{ 
		if (pOther->GiveAmmo( AMMO_GLOCKCLIP_GIVE, "9mm", _9MM_MAX_CARRY ) != -1)
		{
			EMIT_SOUND(ENT(pev), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);
			return TRUE;
		}
		return FALSE;
	}
};
LINK_ENTITY_TO_CLASS( ammo_glockclip, CGlockAmmo );
LINK_ENTITY_TO_CLASS( ammo_9mmclip, CGlockAmmo );















