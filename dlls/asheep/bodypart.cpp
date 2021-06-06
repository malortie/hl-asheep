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

#define GIB_HUMAN_MODEL		"models/hgibs.mdl"
#define GIB_HGRUNT_MODEL	"models/gib_hgrunt.mdl"

enum BodypartType
{
	BP_ANY = 0,
	BP_HGIBS_ONLY = 1,
	BP_HGRUNTGIBS_ONLY = 2,
	BP_NUM_TYPES, // Must be last.
};

int GetModelCountForBodyGroup(edict_t* edict, int iBodyGroup);

class CBodyPart : public CBaseAnimating
{
public:
	void Spawn(void);
	virtual int	ObjectCaps(void) { return (CBaseAnimating::ObjectCaps() & ~FCAP_ACROSS_TRANSITION); }
};

LINK_ENTITY_TO_CLASS(monster_bodypart, CBodyPart);

void CBodyPart::Spawn()
{
	// Ensure body is non negative.
	if (pev->body < 0)
		pev->body = 0;

	const char* model_name = NULL;

	if (pev->body == BodypartType::BP_ANY)
	{
		if (RANDOM_LONG(0, 15) > 0)
			model_name = GIB_HUMAN_MODEL;
		else
			model_name = GIB_HGRUNT_MODEL;
	}
	else if (pev->body == BodypartType::BP_HGIBS_ONLY)
	{
		model_name = GIB_HUMAN_MODEL;
	}
	else if (pev->body == BodypartType::BP_HGRUNTGIBS_ONLY)
	{
		model_name = GIB_HGRUNT_MODEL;
	}
	else
	{
		// Undefined setup. Use a fallback model.
		model_name = GIB_HUMAN_MODEL;
	}

	PRECACHE_MODEL((char*)model_name);
	SET_MODEL(ENT(pev), model_name);

	if (pev->body < BodypartType::BP_NUM_TYPES)
	{
		// Reset pev->body.
		pev->body = 0;

		// Choose a random bodygroup if multiple are available.
		int numberOfAvailableSubModels = GetModelCountForBodyGroup(ENT(pev), 0);
		if (numberOfAvailableSubModels > 0)
			SetBodygroup(0, RANDOM_LONG(0, numberOfAvailableSubModels - 1));
	}
	else
	{
		// Convert from body type to bodygroup index.
		pev->body -= BodypartType::BP_NUM_TYPES;

		// Undefined setup. Set the bodygroup as it is, but ensure it is less than the max available.
		int numberOfAvailableSubModels = GetModelCountForBodyGroup(ENT(pev), 0);
		if (numberOfAvailableSubModels > 0)
		{
			pev->body = std::min(pev->body, numberOfAvailableSubModels - 1);
			SetBodygroup(0, pev->body);
		}
		else
		{
			// Reset pev->body.
			pev->body = 0;
		}
	}

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

	pev->skin = 0;

	SetThink(&CBodyPart::SUB_DoNothing);

	DROP_TO_FLOOR(ENT(pev));
}

int GetModelCountForBodyGroup(edict_t* edict, int iBodyGroup) 
{
	studiohdr_t* pstudiohdr = (studiohdr_t*)GET_MODEL_PTR(edict);
	mstudiobodyparts_t *pbodypart = (mstudiobodyparts_t *)((byte *)pstudiohdr + pstudiohdr->bodypartindex) + iBodyGroup;

	return pbodypart->nummodels;
}
