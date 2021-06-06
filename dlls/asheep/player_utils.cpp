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
#include "player_utils.h"

class CPlayerMediator
{
public:
	virtual BOOL IsPlayerEquippedWithArmor(CBasePlayer* player);
	virtual BOOL IsPlayerEquippedWithSuit(CBasePlayer* player);

	virtual void AddArmorToPlayer(CBasePlayer* player);
};

static CPlayerMediator gPlayerMediator;

BOOL CPlayerMediator::IsPlayerEquippedWithArmor(CBasePlayer* player)
{
	return (player->pev->weapons & (1 << WEAPON_ARMOR)) != 0;
}

BOOL CPlayerMediator::IsPlayerEquippedWithSuit(CBasePlayer* player)
{
	return (player->pev->weapons & (1 << WEAPON_SUIT)) != 0;
}

void CPlayerMediator::AddArmorToPlayer(CBasePlayer* player)
{
	player->pev->weapons |= (1 << WEAPON_ARMOR);
}

CBasePlayer* UTIL_GetLocalPlayer()
{
	if (!g_pGameRules->IsMultiplayer())
	{
		return dynamic_cast<CBasePlayer*>(UTIL_FindEntityByClassname(NULL, "player"));
	}
	else
		return NULL;
}

BOOL UTIL_IsPlayerEquippedWithArmor(CBasePlayer* player)
{
	if (player == NULL)
		return FALSE;

	return gPlayerMediator.IsPlayerEquippedWithArmor(player);
}

BOOL UTIL_IsPlayerEquippedWithSuit(CBasePlayer* player)
{
	if (player == NULL)
		return FALSE;

	return gPlayerMediator.IsPlayerEquippedWithSuit(player);
}

void PlayerMediator_AddArmorToPlayer(CBasePlayer* player)
{
	if (player == NULL)
		return;

	gPlayerMediator.AddArmorToPlayer(player);
}

#define BATTERY_DENY_MESSAGE_DELAY	4.0f // In seconds.

void UTIL_DisplayBatteryDenyMessage(CBasePlayer* pPlayer, PlayerBatteryDenyMessageType messageType)
{
	if (pPlayer->m_flNextBatteryDenyMessageTime > gpGlobals->time)
		return;

	pPlayer->m_flNextBatteryDenyMessageTime = gpGlobals->time + BATTERY_DENY_MESSAGE_DELAY;

	switch (messageType)
	{
	case BATTERYDENY_BATTERY:
		UTIL_ShowMessage("NOHEVBATTERY", pPlayer);
		EMIT_SOUND_DYN(ENT(pPlayer->pev), CHAN_VOICE, "barney/nope.wav", 1.0, ATTN_NORM, 0, 100);
		break;
	case BATTERYDENY_SUITCHARGER:
		UTIL_ShowMessage("NOHEVRECHARGE", pPlayer);
		break;
	default:
		// Undefined deny message type.
		break;
	}
}
