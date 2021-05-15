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
#include "player.h"
#include "saverestore.h"
#include "trains.h"
#include "gamerules.h"

class CTriggerDie : public CPointEntity
{
public:
	void KeyValue(KeyValueData *pkvd);
	void Spawn(void);
	void Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);

	int ObjectCaps(void) { return CPointEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }
	virtual int		Save(CSave &save);
	virtual int		Restore(CRestore &restore);
	static	TYPEDESCRIPTION m_SaveData[];

private:
	string_t	monsterName;
};
LINK_ENTITY_TO_CLASS(trigger_die, CTriggerDie);

TYPEDESCRIPTION	CTriggerDie::m_SaveData[] =
{
	DEFINE_FIELD(CTriggerDie, monsterName, FIELD_STRING),
};

IMPLEMENT_SAVERESTORE(CTriggerDie, CPointEntity);

void CTriggerDie::KeyValue(KeyValueData *pkvd)
{
	if (FStrEq(pkvd->szKeyName, "monstername"))
	{
		monsterName = ALLOC_STRING(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else
		CPointEntity::KeyValue(pkvd);
}

void CTriggerDie::Spawn(void)
{
}

void CTriggerDie::Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value)
{
	if (!FStringNull(monsterName))
	{
		CBaseEntity* monster = NULL;
		while ((monster = UTIL_FindEntityByTargetname(monster, STRING(monsterName))) != NULL)
		{
			if(pActivator != NULL)
				monster->Killed(pActivator->pev, GIB_NEVER);
			else if (pCaller != NULL)
				monster->Killed(pCaller->pev, GIB_NEVER);
		}
	}
	else
		ALERT(at_error, "%s has invalid monster name.", STRING(pev->classname));
}


class CTriggerLoadHazard : public CPointEntity
{
public:
	void Spawn(void);
	void Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);

	int ObjectCaps(void) { return CPointEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }

private:

};

LINK_ENTITY_TO_CLASS(trigger_loadhazard, CTriggerLoadHazard);

void CTriggerLoadHazard::Spawn(void)
{
}

void CTriggerLoadHazard::Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value)
{
	CHANGE_LEVEL("t0a0", 0);
}


class CTriggerKateHealth : public CPointEntity
{
public:
	void Spawn(void);
	void KeyValue(KeyValueData* pkvd);
	void Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);

	int ObjectCaps(void) { return CPointEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }

	virtual int		Save(CSave &save);
	virtual int		Restore(CRestore &restore);
	static	TYPEDESCRIPTION m_SaveData[];

	void HealKate();

private:
	string_t m_kateName;
};

LINK_ENTITY_TO_CLASS(trigger_katehealth, CTriggerKateHealth);

TYPEDESCRIPTION	CTriggerKateHealth::m_SaveData[] =
{
	DEFINE_FIELD(CTriggerKateHealth, m_kateName, FIELD_STRING),
};

IMPLEMENT_SAVERESTORE(CTriggerKateHealth, CPointEntity);

void CTriggerKateHealth::Spawn(void)
{
}

void CTriggerKateHealth::KeyValue(KeyValueData* pkvd)
{
	if (FStrEq(pkvd->szKeyName, "katename"))
	{
		m_kateName = ALLOC_STRING(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else
		CPointEntity::KeyValue(pkvd);
}

void CTriggerKateHealth::Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value)
{
	HealKate();

	pev->nextthink = gpGlobals->time + 0.1;
	SetThink(&CBaseEntity::SUB_Remove);
}

void CTriggerKateHealth::HealKate()
{
	if (FStringNull(m_kateName))
	{
		ALERT(at_error, "Entity %s, has no valid katename.\n", STRING(pev->classname));
		return;
	}

	CBaseEntity* kate = UTIL_FindEntityByTargetname(NULL, STRING(m_kateName));

	if (kate)
		kate->TakeHealth(kate->pev->max_health, DMG_GENERIC);
	else
		ALERT(at_error, "Failed to find entity named %s.\n", STRING(m_kateName));
}
