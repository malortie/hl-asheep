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

#define GARBAGE_DEFAULT_MODEL	"models/garbagegibs.mdl"
#define GARBAGE_CAN_MODEL		"models/can.mdl"

#define RANDOM_ARRAY_ELEMENT(_array) _array[RANDOM_LONG(0, ARRAYSIZE(_array) - 1)]

enum GarbageParts
{
	StrainedPaper1 = 0,
	StrainedPaper2,
	StrainedPaper3,
	Unknown1,
	Glass,
	Sandwich,
	Apple,
	Banana,
	Magazine,
	BrownPaper,
	Unknown2,
	BlueCan
};

class CGarbage : public CBaseAnimating
{
public:
	void Spawn(void);
	virtual int	ObjectCaps(void) { return (CBaseAnimating::ObjectCaps() & ~FCAP_ACROSS_TRANSITION); }

protected:
	const char* ChooseModel();
	void SetSkinAndBodygroups();
	int ChooseRandomSkin();
	void RandomizeYaw();

	BOOL IsSodaCan() const;
};

LINK_ENTITY_TO_CLASS(monster_garbage, CGarbage);

void CGarbage::Spawn()
{
	char* model = (char*)ChooseModel();

	if (!*model || *model == '\0')
		model = GARBAGE_DEFAULT_MODEL;

	PRECACHE_MODEL(model);
	SET_MODEL(ENT(pev), model);

	SetSkinAndBodygroups();

	if (pev->body < 0)
		pev->body = 0;

	// Add random angles.
	RandomizeYaw();

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
	SetThink(&CGarbage::SUB_DoNothing);
}

int GetModelCountForBodyGroup(edict_t* edict, int iBodyGroup);

const char* CGarbage::ChooseModel()
{
	switch (pev->body)
	{
	case 4:
		return GARBAGE_CAN_MODEL; 
	default:
		{
			switch (RANDOM_LONG(0, 5))
			{
			case 1:
				return GARBAGE_CAN_MODEL; // Use a can 1/6th of the time.
			default:
				return GARBAGE_DEFAULT_MODEL;
			}
		}
	}
}

void CGarbage::SetSkinAndBodygroups()
{
	if (IsSodaCan()) // Soda can model share the same model but has different skins.
		pev->skin = ChooseRandomSkin();
	else if (pev->body == 3) // Predefined sub bodygroup.
		SetBodygroup(0, Magazine);
	else
	{
		// Pick a random sub bodygroup.
		static int randomSubmodelIndices[] = 
		{ 
			Glass,
			Sandwich,
			Apple,
			Banana,
			BlueCan
		};
		SetBodygroup(0, RANDOM_ARRAY_ELEMENT(randomSubmodelIndices));
	}
}

BOOL CGarbage::IsSodaCan() const
{
	return FStrEq(STRING(pev->model), GARBAGE_CAN_MODEL);
}

int GetModelSkinCount(edict_t* edict)
{
	studiohdr_t* pstudiohdr = (studiohdr_t*)GET_MODEL_PTR(edict);
	return pstudiohdr->numskinref;
}

int CGarbage::ChooseRandomSkin()
{
	int skinChosenRandomly;

	int numberOfAvailableSkins = GetModelSkinCount(ENT(pev));
	if (numberOfAvailableSkins > 0)
		skinChosenRandomly = RANDOM_LONG(0, numberOfAvailableSkins - 1);
	else
		skinChosenRandomly = 0;

	return skinChosenRandomly;
}

void CGarbage::RandomizeYaw()
{
	pev->angles.y = RANDOM_FLOAT(2, 6) *  RANDOM_FLOAT(2, 6) * RANDOM_FLOAT(2, 8);
}