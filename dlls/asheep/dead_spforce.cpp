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

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "gamerules.h"
#include "monsters.h"
#include "dead_actor.h"
#include "spforce.h"

class CDeadSpForce : public CDeadActorWithHGruntSequences
{
public:
	virtual int	Classify(void);

	virtual void PrecacheModels();
	virtual void SetModel();
	virtual void SetSkinAndBodygroups();
};

int	CDeadSpForce::Classify(void)
{
	return CLASS_SPECIAL_FORCE;
}

void CDeadSpForce::PrecacheModels()
{
	PRECACHE_MODEL("models/spforce.mdl");
}

void CDeadSpForce::SetModel()
{
	SET_MODEL(ENT(pev), "models/spforce.mdl");
}

void CDeadSpForce::SetSkinAndBodygroups()
{
	// map old bodies onto new bodies
	switch (pev->body)
	{
	case 0: // SpForce with Gun
		pev->body = 0;
		pev->skin = 0;
		SetBodygroup(HEAD_GROUP, HEAD_GASMASK);
		SetBodygroup(GUN_GROUP, GUN_9MMM41A);
		break;
	case 1: // Commander with Gun
		pev->body = 0;
		pev->skin = 0;
		SetBodygroup(HEAD_GROUP, HEAD_COMMANDER);
		SetBodygroup(GUN_GROUP, GUN_9MMM41A);
		break;
	case 2: // Grunt no Gun
		pev->body = 0;
		pev->skin = 0;
		SetBodygroup(HEAD_GROUP, HEAD_GASMASK);
		SetBodygroup(GUN_GROUP, GUN_NONE);
		break;
	case 3: // Commander no Gun
		pev->body = 0;
		pev->skin = 0;
		SetBodygroup(HEAD_GROUP, HEAD_COMMANDER);
		SetBodygroup(GUN_GROUP, GUN_NONE);
		break;
	}
}

LINK_ENTITY_TO_CLASS(monster_spforce_dead, CDeadSpForce);
