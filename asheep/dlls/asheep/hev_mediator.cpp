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
#include "nodes.h"
#include "soundent.h"
#include "client.h"
#include "decals.h"
#include "skill.h"
#include "effects.h"
#include "player.h"
#include "weapons.h"
#include "gamerules.h"
#include "player_utils.h"
#include "hev_mediator.h"
#include "asheep_messages.h"

#define HEV_EFFECT_USE_FREQUENCY	4.0f // In seconds.

enum HevMediatorUseType
{
	MEDIATOR_USE_BATTERY = 0,
	MEDIATOR_USE_SUITCHARGER,
};

class CHevMediator : public CBaseEntity
{
public:
	void Precache(void);
	void Spawn(void);
	void EXPORT MediatorUse(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);
	void EXPORT WaitThink(void);

	BOOL IsPlayerAllowedToPickupBattery(CBasePlayer* player);
	BOOL IsPlayerAllowedToUseSuitCharger(CBasePlayer* player);

	void UseMediatorToPlayEffects(CBaseEntity* pActivator, CBaseEntity* pCaller, HevMediatorUseType effectType);

protected:
	BOOL IsPlayerSuitEquipped(CBasePlayer* player);

	void EmitBatteryPickupDeniedSound(CBaseEntity* toucher);
	void EmitSuitChargerDeniedSound(CBaseEntity* toucher);

	void ShowBatteryPickupDeniedMessage(CBaseEntity* toucher);
	void ShowSuitChargerDeniedMessage(CBaseEntity* toucher);
};

LINK_ENTITY_TO_CLASS(asheep_hevmediator, CHevMediator);

void CHevMediator::Precache(void)
{
	PRECACHE_SOUND("barney/nope.wav");
}

void CHevMediator::Spawn(void)
{
	SetThink(NULL);
	SetUse(&CHevMediator::MediatorUse);
}

void CHevMediator::MediatorUse(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	if (useType == USE_ON)
	{
		switch ((int)value)
		{
		case MEDIATOR_USE_BATTERY:
			EmitBatteryPickupDeniedSound(pActivator);
			ShowBatteryPickupDeniedMessage(pActivator);
			break;
		case MEDIATOR_USE_SUITCHARGER:
			EmitSuitChargerDeniedSound(pActivator);
			ShowSuitChargerDeniedMessage(pActivator);
			break;
		default:
			ALERT(at_console, "Unsupported value %d in MediatorUse.\n", (int)value);
			return;
		}

		SetUse(NULL);
		SetThink(&CHevMediator::WaitThink);
		pev->nextthink = gpGlobals->time + HEV_EFFECT_USE_FREQUENCY;
	}
}

void CHevMediator::WaitThink(void)
{
	SetThink(NULL);
	SetUse(&CHevMediator::MediatorUse);
}

BOOL CHevMediator::IsPlayerAllowedToPickupBattery(CBasePlayer* player)
{
	if (!IsPlayerSuitEquipped(player))
		return FALSE;

	if (player->pev->armorvalue >= MAX_NORMAL_BATTERY)
		return FALSE;

	return TRUE;
}

BOOL CHevMediator::IsPlayerAllowedToUseSuitCharger(CBasePlayer* player)
{
	if (!IsPlayerSuitEquipped(player))
		return FALSE;

	if (player->pev->armorvalue >= MAX_NORMAL_BATTERY)
		return FALSE;

	return TRUE;
}

void CHevMediator::UseMediatorToPlayEffects(CBaseEntity* pActivator, CBaseEntity* pCaller, HevMediatorUseType effectType)
{
	Use(pActivator, pCaller, USE_ON, effectType);
}

BOOL CHevMediator::IsPlayerSuitEquipped(CBasePlayer* player)
{
	return UTIL_IsPlayerEquippedWithSuit(player);
}

void CHevMediator::EmitBatteryPickupDeniedSound(CBaseEntity* toucher)
{
	EMIT_SOUND_DYN(ENT(toucher->pev), CHAN_VOICE, "barney/nope.wav", 1.0, ATTN_NORM, 0, 100);
}

void CHevMediator::ShowBatteryPickupDeniedMessage(CBaseEntity* toucher)
{
	Message_ShowMessage("NOHEVBATTERY");
}

void CHevMediator::EmitSuitChargerDeniedSound(CBaseEntity* toucher)
{
	// In case modders wish to add extra sounds. 
}

void CHevMediator::ShowSuitChargerDeniedMessage(CBaseEntity* toucher)
{
	Message_ShowMessage("NOHEVRECHARGE");
}

static CHevMediator* hevMediatorSingleton;

void HevMediator_CreateAndSetupSingleton()
{
	hevMediatorSingleton = (CHevMediator*)CBaseEntity::Create("asheep_hevmediator", g_vecZero, g_vecZero, NULL);

	if (!hevMediatorSingleton)
		ALERT(at_console, "**Could not create asheep_hevmediator singleton!**\n");
	else
	{
		hevMediatorSingleton->Precache();
		hevMediatorSingleton->Spawn();
	}
}

BOOL HevMediator_PlayerAttemptToPickupSuitBattery(CBasePlayer* player, BOOL playEffects)
{
	if (!hevMediatorSingleton)
	{
		ALERT(at_console, "**No asheep_hevmediator singleton!**\n");
		return TRUE;
	}

	BOOL result = hevMediatorSingleton->IsPlayerAllowedToPickupBattery(player);

	if (!result && playEffects)
	{
		hevMediatorSingleton->UseMediatorToPlayEffects(player, player, MEDIATOR_USE_BATTERY);
	}

	return result;
}

BOOL HevMediator_PlayerAttemptToUseSuitCharger(CBasePlayer* player, BOOL playEffects)
{
	if (!hevMediatorSingleton)
	{
		ALERT(at_console, "**No asheep_hevmediator singleton!**\n");
		return TRUE;
	}

	BOOL result = hevMediatorSingleton->IsPlayerAllowedToUseSuitCharger(player);

	if (!result && playEffects)
	{
		hevMediatorSingleton->UseMediatorToPlayEffects(player, player, MEDIATOR_USE_SUITCHARGER);
	}

	return result;
}