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

class CRemoveEnvMessageEntities : public CBaseEntity
{
public:
	void Spawn(void);
	void EXPORT WaitAndRemoveEnvMessages(void);

protected:

	virtual CBaseEntity* FindNextMessageEntityToRemove();
	void ReplaceMessageEntityWithNullEntity(CBaseEntity* messageEntity);
};

void CRemoveEnvMessageEntities::Spawn(void)
{
	SetThink(&CRemoveEnvMessageEntities::WaitAndRemoveEnvMessages);
	pev->nextthink = gpGlobals->time + 0.1f;
}

void CRemoveEnvMessageEntities::WaitAndRemoveEnvMessages(void)
{
	CBaseEntity* messageEntity = FindNextMessageEntityToRemove();

	if (messageEntity != NULL)
	{
		ReplaceMessageEntityWithNullEntity(messageEntity);
		pev->nextthink = gpGlobals->time + 0.1f;
	}
	else
	{
		SetThink(&CBaseEntity::SUB_Remove);
		pev->nextthink = gpGlobals->time + 0.2f;
	}
}

CBaseEntity* CRemoveEnvMessageEntities::FindNextMessageEntityToRemove()
{
	return UTIL_FindEntityByClassname(NULL, "env_message");
}

void CRemoveEnvMessageEntities::ReplaceMessageEntityWithNullEntity(CBaseEntity* messageEntity)
{
	CBaseEntity* nullEntity = CBaseEntity::Create("info_null", g_vecZero, g_vecZero, NULL);
	if (nullEntity)
		nullEntity->pev->targetname = messageEntity->pev->targetname;

	UTIL_Remove(messageEntity);
}

void Message_RemoveTrainingMapMessages()
{
	CRemoveEnvMessageEntities* pRemoveMessageEntities = GetClassPtr((CRemoveEnvMessageEntities*)NULL);
	if (pRemoveMessageEntities)
		pRemoveMessageEntities->Spawn();
}

