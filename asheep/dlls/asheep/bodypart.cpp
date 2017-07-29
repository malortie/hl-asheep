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
#include "studio.h"

class CBodyPart : public CBaseAnimating
{
public:
	void Spawn(void);
	virtual int	ObjectCaps(void) { return (CBaseAnimating::ObjectCaps() & ~FCAP_ACROSS_TRANSITION); }

	virtual void SetSkinAndBodygroups();
};

LINK_ENTITY_TO_CLASS(monster_bodypart, CBodyPart);

void CBodyPart::Spawn()
{
	PRECACHE_MODEL("models/hgibs.mdl");
	SET_MODEL(ENT(pev), "models/hgibs.mdl");

	pev->movetype = MOVETYPE_NONE;
	pev->solid = SOLID_NOT;
	pev->health = 1;
	pev->takedamage = DAMAGE_NO;
	pev->effects = 0;
	pev->yaw_speed = 0;
	pev->sequence = 0;
	pev->frame = 0;

	ResetSequenceInfo();
	pev->frame = 0;
	SetSkinAndBodygroups();

	if (pev->body < 0)
		pev->body = 0;

	SetThink(&CBodyPart::SUB_DoNothing);

	DROP_TO_FLOOR(ENT(pev));
}

int GetModelCountForBodyGroup(edict_t* edict, int iBodyGroup) 
{
	studiohdr_t* pstudiohdr = (studiohdr_t*)GET_MODEL_PTR(edict);
	mstudiobodyparts_t *pbodypart = (mstudiobodyparts_t *)((byte *)pstudiohdr + pstudiohdr->bodypartindex) + iBodyGroup;

	return pbodypart->nummodels;
}

void CBodyPart::SetSkinAndBodygroups()
{
	pev->skin = 0;

	int numberOfAvailableSubModels = GetModelCountForBodyGroup(ENT(pev), 0);
	if (numberOfAvailableSubModels > 0)
		SetBodygroup(0, RANDOM_LONG(0, numberOfAvailableSubModels - 1));
}