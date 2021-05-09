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
#include "scientist.h"

//=========================================================
// Dead Scientist PROP
//=========================================================
class CDeadScientist : public CDeadActorWithScientistSequences
{
public:
	virtual void PrecacheModels();
	virtual void SetModel();
	virtual void SetSkinAndBodygroups();
};

void CDeadScientist::PrecacheModels()
{
	PRECACHE_MODEL("models/scientist.mdl");
}

void CDeadScientist::SetModel()
{
	SET_MODEL(ENT(pev), "models/scientist.mdl");
}

void CDeadScientist::SetSkinAndBodygroups()
{
	if (pev->body == -1)
	{// -1 chooses a random head
		pev->body = RANDOM_LONG(0, NUM_SCIENTIST_HEADS - 1);// pick a head, any head
	}
	// Luther is black, make his hands black
	if (pev->body == HEAD_LUTHER)
		pev->skin = 1;
	else
		pev->skin = 0;
}

LINK_ENTITY_TO_CLASS(monster_scientist_dead, CDeadScientist);
