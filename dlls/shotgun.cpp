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

enum shotgun_e {
	SHOTGUN_IDLE = 0,
	SHOTGUN_FIRE,
	SHOTGUN_FIRE2,
	SHOTGUN_RELOAD,
	SHOTGUN_PUMP,
	SHOTGUN_START_RELOAD,
	SHOTGUN_DRAW,
	SHOTGUN_HOLSTER,
	SHOTGUN_IDLE4,
	SHOTGUN_IDLE_DEEP
};

LINK_ENTITY_TO_CLASS(weapon_shotgun, CShotgun);

const char* CShotgun::pReloadSounds[] = 
{
	"weapons/reload1.wav",
	"weapons/reload3.wav",
};

int CShotgun::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "buckshot";
	p->iMaxAmmo1 = BUCKSHOT_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = SHOTGUN_MAX_CLIP;
	p->iSlot = 2;
	p->iPosition = 2;
	p->iFlags = 0;
	p->iId = m_iId = GetWeaponID();
	p->iWeight = SHOTGUN_WEIGHT;

	return 1;
}

int CShotgun::GetWeaponID()
{
	return WEAPON_SHOTGUN;
}

const char* CShotgun::GetThirdpersonModel()
{
	return "models/p_shotgun.mdl";
}

const char* CShotgun::GetViewModel()
{
	return "models/v_shotgun.mdl";
}

const char* CShotgun::GetWorldModel()
{
	return "models/w_shotgun.mdl";
}

void CShotgun::PrecacheModels()
{
	BaseClass::PrecacheModels();

	PRECACHE_MODEL("models/v_shotgun.mdl");
	PRECACHE_MODEL("models/w_shotgun.mdl");
	PRECACHE_MODEL("models/p_shotgun.mdl");
}

void CShotgun::GiveFirstTimeSpawnDefaultAmmo()
{
	m_iDefaultAmmo = SHOTGUN_DEFAULT_GIVE;
}

int CShotgun::GetDeploySequence()
{
	return SHOTGUN_DRAW;
}

int CShotgun::GetHolsterSequence()
{
	return SHOTGUN_HOLSTER;
}

float CShotgun::GetHolsterSequenceDuration()
{
	return 11.0f / 30.0f;
}

float CShotgun::PlayIdleAnimation()
{
	float animDuration = 0;
	int iAnim;
	float flRand = UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0, 1);
	if (flRand <= 0.8)
	{
		iAnim = SHOTGUN_IDLE_DEEP;
		animDuration = 60.0 / 12.0; // * RANDOM_LONG(2, 5);
	}
	else if (flRand <= 0.95)
	{
		iAnim = SHOTGUN_IDLE;
		animDuration = 20.0 / 9.0;
	}
	else
	{
		iAnim = SHOTGUN_IDLE4;
		animDuration = 20.0 / 9.0;
	}
	SendWeaponAnim(iAnim);

	return animDuration;
}

float CShotgun::PlayReloadAnimation()
{
	SendWeaponAnim(SHOTGUN_RELOAD);
	return 0.5f;
}

float CShotgun::PlayStartReloadAnimation()
{
	SendWeaponAnim(SHOTGUN_START_RELOAD);
	return 0.6f;
}

float CShotgun::PlayPumpAnimation()
{
	SendWeaponAnim(SHOTGUN_PUMP);
	return 1.5;
}

void CShotgun::PlayReloadSound()
{
	EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_ITEM, RANDOM_SOUND_ARRAY(pReloadSounds), 1, ATTN_NORM, 0, 85 + RANDOM_LONG(0, 0x1f));
}

void CShotgun::PlayPumpSound() 
{
	EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/scock1.wav", 1, ATTN_NORM, 0, 95 + RANDOM_LONG(0, 0x1f));
}

class CShotgunAmmo : public CBasePlayerAmmo
{
	void Spawn( void )
	{ 
		Precache( );
		SET_MODEL(ENT(pev), "models/w_shotbox.mdl");
		CBasePlayerAmmo::Spawn( );
	}
	void Precache( void )
	{
		PRECACHE_MODEL ("models/w_shotbox.mdl");
		PRECACHE_SOUND("items/9mmclip1.wav");
	}
	BOOL AddAmmo( CBaseEntity *pOther ) 
	{ 
		if (pOther->GiveAmmo( AMMO_BUCKSHOTBOX_GIVE, "buckshot", BUCKSHOT_MAX_CARRY ) != -1)
		{
			EMIT_SOUND(ENT(pev), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);
			return TRUE;
		}
		return FALSE;
	}
};
LINK_ENTITY_TO_CLASS( ammo_buckshot, CShotgunAmmo );


