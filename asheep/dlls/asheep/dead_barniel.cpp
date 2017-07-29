/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
//=========================================================
// monster template
//=========================================================
// UNDONE: Holster weapon?

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "gamerules.h"
#include "monsters.h"
#include "dead_actor.h"

class CDeadBarniel : public CDeadActorWithBarneySequences
{
public:
	virtual void PrecacheModels();
	virtual void SetModel();
};

void CDeadBarniel::PrecacheModels()
{
	PRECACHE_MODEL("models/barniel.mdl");
}

void CDeadBarniel::SetModel()
{
	SET_MODEL(ENT(pev), "models/barniel.mdl");
}

LINK_ENTITY_TO_CLASS(monster_barniel_dead, CDeadBarniel);
