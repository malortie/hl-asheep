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
#include "items.h"
#include "player.h"
#include "soundent.h"
#include "items.h"

#define DEFAULT_ARMOR_VALUE		15

class CItemArmor : public CItem
{
	void Spawn(void)
	{
		Precache();
		SET_MODEL(ENT(pev), "models/w_suit.mdl");
		CItem::Spawn();
	}
	void Precache(void)
	{
		PRECACHE_MODEL("models/w_suit.mdl");
		PRECACHE_SOUND("armor/pickup.wav");
	}
	BOOL MyTouch(CBasePlayer *pPlayer)
	{
		// if the player already has an armor, deny
		// pickup.
		if (UTIL_IsPlayerEquippedWithArmor(pPlayer))
			return FALSE;

		PlayerMediator_AddArmorToPlayer(pPlayer);

		// By default, give a small amount of armor value.
		pPlayer->pev->armorvalue = DEFAULT_ARMOR_VALUE;

		EMIT_SOUND_DYN(ENT(pPlayer->pev), CHAN_AUTO, "armor/pickup.wav", VOL_NORM, ATTN_NORM, 0, 100);

		return TRUE;
	}
};

LINK_ENTITY_TO_CLASS(item_armor, CItemArmor);
