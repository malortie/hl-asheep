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
#if !defined ( CLIENT_DLL )
#include "toadweaponmonster.h"
#endif // !defined ( CLIENT_DLL )

enum toadweapon_e {
	TOADWEAPON_IDLE1 = 0,
	TOADWEAPON_IDLE2,
	TOADWEAPON_FIDGETFIT,
	TOADWEAPON_FIDGETNIP,
	TOADWEAPON_DOWN,
	TOADWEAPON_UP,
	TOADWEAPON_THROW
};

LINK_ENTITY_TO_CLASS(weapon_toad, CToadWeapon);

const char* CToadWeapon::pDeploySounds[] =
{
	"toad/toad_hunt1.wav",
	"toad/toad_hunt2.wav",
	"toad/toad_hunt3.wav",
};

const char* CToadWeapon::pHuntSounds[] =
{
	"toad/toad_hunt1.wav",
	"toad/toad_hunt2.wav",
	"toad/toad_hunt3.wav",
};

void CToadWeapon::Spawn(void)
{
	BaseClass::Spawn();

#if !defined ( CLIENT_DLL )
	if ( m_doNotSpawnToad == FALSE )
		SpawnMonsterAndRemoveWeapon();
#endif // !defined ( CLIENT_DLL )
}

void CToadWeapon::Precache(void)
{
	BaseClass::Precache();

	UTIL_PrecacheOther("toadweaponmonster");
}

int CToadWeapon::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "Toads";
	p->iMaxAmmo1 = TOAD_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 4;
	p->iPosition = 5;
	p->iId = m_iId = GetWeaponID();
	p->iWeight = TOAD_WEIGHT;
	p->iFlags = ITEM_FLAG_LIMITINWORLD | ITEM_FLAG_EXHAUSTIBLE;

	return 1;
}

int CToadWeapon::GetWeaponID()
{
	return WEAPON_TOAD;
}

const char* CToadWeapon::GetThirdpersonModel()
{
	return "models/p_toad.mdl";
}

const char* CToadWeapon::GetViewModel()
{
	return "models/v_toad.mdl";
}

const char* CToadWeapon::GetWorldModel()
{
	return "models/toad_nest.mdl";
}

void CToadWeapon::PrecacheModels()
{
	PRECACHE_MODEL("models/toad_nest.mdl");
	PRECACHE_MODEL("models/v_toad.mdl");
	PRECACHE_MODEL("models/p_toad.mdl");
}

void CToadWeapon::PrecacheSounds()
{
	PRECACHE_SOUND_ARRAY(pDeploySounds);
	PRECACHE_SOUND_ARRAY(pHuntSounds);
}

void CToadWeapon::PrecacheEvents()
{
	m_usToad = PRECACHE_EVENT(1, "events/toad.sc");
}

void CToadWeapon::GiveFirstTimeSpawnDefaultAmmo()
{
	m_iDefaultAmmo = TOAD_DEFAULT_GIVE;
}

const unsigned short& CToadWeapon::GetThrowEvent() const
{
	return m_usToad;
}

const char* CToadWeapon::GetClassnameOfMonsterToThrow()
{
	return "monster_toad";
}

int CToadWeapon::GetDeploySequence()
{
	return TOADWEAPON_UP;
}

int CToadWeapon::GetHolsterSequence()
{
	return TOADWEAPON_DOWN;
}

float CToadWeapon::GetHolsterSequenceDuration()
{
	return 21.0f / 16.0f;
}

float CToadWeapon::PlayIdleAnimation()
{
	int iAnim;
	float duration;

	float flRand = UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0, 1);
	if (flRand <= 0.75)
	{
		iAnim = TOADWEAPON_IDLE1;
		duration = 30.0 / 16 * (2);
	}
	else if (flRand <= 0.875)
	{
		iAnim = TOADWEAPON_FIDGETFIT;
		duration = 70.0 / 16.0;
	}
	else
	{
		iAnim = TOADWEAPON_FIDGETNIP;
		duration = 80.0 / 16.0;
	}
	SendWeaponAnim(iAnim);
	return duration;
}

void CToadWeapon::PlayDeploySound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, RANDOM_SOUND_ARRAY(pDeploySounds), 1, ATTN_NORM, 0, 100);
}

void CToadWeapon::PlayHuntSound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, RANDOM_SOUND_ARRAY(pHuntSounds), 1, ATTN_NORM, 0, 105);
}

#if !defined ( CLIENT_DLL )
void CToadWeapon::SpawnMonsterAndRemoveWeapon()
{
	// Spawn a toad weapon monster and remove this weapon instance.
	// Players can pick up the monster and get this weapon back.
	edict_t* toadEdict = CREATE_NAMED_ENTITY(MAKE_STRING(GetClassnameOfMonsterToCreate()));
	CToadWeaponMonster* toadWeaponMonster = (CToadWeaponMonster*)CBaseEntity::Instance(toadEdict);

	if (toadWeaponMonster != NULL)
	{
		toadWeaponMonster->pev->origin = pev->origin;
		toadWeaponMonster->pev->angles = Vector(0, pev->angles.y, 0);
		toadWeaponMonster->Precache();
		DispatchSpawn(toadWeaponMonster->edict());
		DROP_TO_FLOOR(toadWeaponMonster->edict());
	}

	UTIL_Remove(this);
}
const char* CToadWeapon::GetClassnameOfMonsterToCreate()
{
	return "toadweaponmonster";
}

#endif // !defined ( CLIENT_DLL )
