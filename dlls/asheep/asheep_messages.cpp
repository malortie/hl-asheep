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
#include "asheep_serverside_utils.h"

static CBaseEntity* messageEntitySingleton = NULL;

void Message_CreateAndSetupSingleton()
{
	messageEntitySingleton = CBaseEntity::Create("env_message", g_vecZero, g_vecZero, NULL);

	if (!messageEntitySingleton)
		ALERT(at_console, "**Could not create env_message singleton!**\n");
	else
	{
		messageEntitySingleton->Precache();
		messageEntitySingleton->Spawn();
	}
}

void Message_ShowMessage(const char* message)
{
	if (!messageEntitySingleton)
	{
		ALERT(at_console, "**No env_message singleton!**\n");
		return;
	}
	
	messageEntitySingleton->SetThink(&CBaseEntity::SUB_CallUseToggle);
	messageEntitySingleton->pev->message = MAKE_STRING(message);
	messageEntitySingleton->pev->nextthink = gpGlobals->time;
	messageEntitySingleton->pev->spawnflags = 0;
}
