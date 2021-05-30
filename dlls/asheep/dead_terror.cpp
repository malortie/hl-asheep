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
#include "hterror.h"

//=========================================================
// Dead terror PROP
//=========================================================
class CDeadHTerror : public CDeadActorWithHGruntSequences
{
public:
	virtual void PrecacheModels();
	virtual void SetModel();
	virtual void SetSkinAndBodygroups();
};

void CDeadHTerror::PrecacheModels()
{
	PRECACHE_MODEL("models/terror.mdl");
}

void CDeadHTerror::SetModel()
{
	SET_MODEL(ENT(pev), "models/terror.mdl");
}

void CDeadHTerror::SetSkinAndBodygroups()
{
	// map old bodies onto new bodies
	switch (pev->body)
	{
	case 0: // Hood with MP5
		pev->body = 0;
		pev->skin = 0;
		SetBodygroup(HEAD_GROUP, HEAD_HOOD);
		SetBodygroup(GUN_GROUP, GUN_MP5);
		break;
	case 1: // Hood with Shotgun ?
		pev->body = 0;
		pev->skin = 0;
		SetBodygroup(HEAD_GROUP, HEAD_HOOD);
		SetBodygroup(GUN_GROUP, GUN_SHOTGUN);
		break;
	case 2: // Hood with MP5 ?
		pev->body = 0;
		pev->skin = 0;
		SetBodygroup(HEAD_GROUP, HEAD_HOOD);
		SetBodygroup(GUN_GROUP, GUN_MP5);
		break;
	case 3: // Hood with Shotgun ?
		pev->body = 0;
		pev->skin = 0;
		SetBodygroup(HEAD_GROUP, HEAD_HOOD);
		SetBodygroup(GUN_GROUP, GUN_SHOTGUN);
		break;
	}
}

LINK_ENTITY_TO_CLASS(monster_terror_dead, CDeadHTerror);
