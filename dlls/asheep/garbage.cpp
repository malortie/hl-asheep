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

#define RANDOM_SKIN			(-1)

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

enum GarbageType
{
	GT_ANY = 0,
	GT_TRASHPAPER = 1,
	GT_FOOD_OR_SODACAN = 2,
	GT_MAGAZINE_OR_BROWNPAPER = 3,
	GT_SODACAN_ONLY = 4,

	// Specific garbage submodel.
	GT_GARBAGEGIB_TRASHPAPER_1 = 5,
	GT_GARBAGEGIB_TRASHPAPER_2 = 6,
	GT_GARBAGEGIB_TRASHPAPER_3 = 7,
	GT_GARBAGEGIB_GLASS = 8,
	GT_GARBAGEGIB_SANDWICH = 9,
	GT_GARBAGEGIB_APPLE = 10,
	GT_GARBAGEGIB_BANANA = 11,

	// Specific sodacan.
	GT_SODACAN_1 = 12,
	GT_SODACAN_2 = 13,
	GT_SODACAN_3 = 14,
	GT_SODACAN_4 = 15,
	GT_SODACAN_5 = 16,
	GT_SODACAN_6 = 17,

	// Specific garbage submodel.
	GT_GARBAGEGIB_SODACAN = 18,
	GT_GARBAGEGIB_MAGAZINE = 19,
	GT_GARBAGEGIB_BROWNPAPER = 20,

	GT_NUM_TYPES, // Must be last.
};

int GetModelCountForBodyGroup(edict_t* edict, int iBodyGroup);

class CGarbage : public CBaseAnimating
{
public:
	void Spawn(void);
	virtual int	ObjectCaps(void) { return (CBaseAnimating::ObjectCaps() & ~FCAP_ACROSS_TRANSITION); }

protected:
	int ChooseRandomSkin();

	void SetGarbageModel(int bodygroup_value);
	void SetSodaModel(int skin = RANDOM_SKIN);
};

LINK_ENTITY_TO_CLASS(monster_garbage, CGarbage);

void CGarbage::Spawn()
{
	// Ensure body is non negative.
	if (pev->body < 0)
		pev->body = 0;

	if (pev->body == GarbageType::GT_ANY)
	{
		if (RANDOM_LONG(0, 1))
		{
			static const int random_indices[] =
			{
				StrainedPaper1,
				StrainedPaper2,
				Glass,
				Apple,
				Banana,
				Magazine,
				BrownPaper,
				BlueCan,
			};
			SetGarbageModel(RANDOM_ARRAY_ELEMENT(random_indices));
		}
		else
		{
			SetSodaModel();
		}
	}
	else if (pev->body == GarbageType::GT_TRASHPAPER)
	{
		static const int random_indices[] =
		{
			StrainedPaper1,
			StrainedPaper2,
			StrainedPaper3,
		};
		SetGarbageModel(RANDOM_ARRAY_ELEMENT(random_indices));
	}
	else if (pev->body == GarbageType::GT_FOOD_OR_SODACAN)
	{
		if (RANDOM_LONG(0, 1))
		{
			static const int random_indices[] =
			{
				Glass,
				Sandwich,
				Apple,
				Banana,
				BlueCan,
			};
			SetGarbageModel(RANDOM_ARRAY_ELEMENT(random_indices));
		}
		else
		{
			SetSodaModel();
		}
	}
	else if (pev->body == GarbageType::GT_MAGAZINE_OR_BROWNPAPER)
	{
		static const int random_indices[] =
		{
			Magazine,
			BrownPaper,
		};
		SetGarbageModel(RANDOM_ARRAY_ELEMENT(random_indices));
	}
	else if (pev->body == GarbageType::GT_SODACAN_ONLY)
	{
		if (RANDOM_LONG(0, 1))
		{
			SetGarbageModel(BlueCan);
		}
		else
		{
			SetSodaModel();
		}
	}
	else if (pev->body == GarbageType::GT_GARBAGEGIB_TRASHPAPER_1)
	{
		SetGarbageModel(StrainedPaper1);
	}
	else if (pev->body == GarbageType::GT_GARBAGEGIB_TRASHPAPER_2)
	{
		SetGarbageModel(StrainedPaper2);
	}
	else if (pev->body == GarbageType::GT_GARBAGEGIB_TRASHPAPER_3)
	{
		SetGarbageModel(StrainedPaper2);
	}
	else if (pev->body == GarbageType::GT_GARBAGEGIB_GLASS)
	{
		SetGarbageModel(Glass);
	}
	else if (pev->body == GarbageType::GT_GARBAGEGIB_SANDWICH)
	{
		SetGarbageModel(Sandwich);
	}
	else if (pev->body == GarbageType::GT_GARBAGEGIB_APPLE)
	{
		SetGarbageModel(Apple);
	}
	else if (pev->body == GarbageType::GT_GARBAGEGIB_BANANA)
	{
		SetGarbageModel(Banana);
	}
	else if (pev->body == GarbageType::GT_SODACAN_1)
	{
		SetSodaModel(0);
	}
	else if (pev->body == GarbageType::GT_SODACAN_2)
	{
		SetSodaModel(1);
	}
	else if (pev->body == GarbageType::GT_SODACAN_3)
	{
		SetSodaModel(2);
	}
	else if (pev->body == GarbageType::GT_SODACAN_4)
	{
		SetSodaModel(3);
	}
	else if (pev->body == GarbageType::GT_SODACAN_5)
	{
		SetSodaModel(4);
	}
	else if (pev->body == GarbageType::GT_SODACAN_6)
	{
		SetSodaModel(5);
	}
	else if (pev->body == GarbageType::GT_GARBAGEGIB_SODACAN)
	{
		SetGarbageModel(BlueCan);
	}
	else if (pev->body == GarbageType::GT_GARBAGEGIB_MAGAZINE)
	{
		SetGarbageModel(Magazine);
	}
	else if (pev->body == GarbageType::GT_GARBAGEGIB_BROWNPAPER)
	{
		SetGarbageModel(BrownPaper);
	}
	else
	{
		ALERT(at_aiconsole, "%s Unknown setup (%d). Removing entity.\n", STRING(pev->classname), pev->body);
		UTIL_Remove(this);
		return;
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
	SetThink(&CGarbage::SUB_DoNothing);

	if (DROP_TO_FLOOR(ENT(pev)) == 0)
	{
		ALERT(at_error, "%s fell out of level at %f,%f,%f\n", STRING(pev->classname), pev->origin.x, pev->origin.y, pev->origin.z);
		UTIL_Remove(this);
		return;
	}
}

int GetModelSkinCount(edict_t* edict)
{
	studiohdr_t* pstudiohdr = (studiohdr_t*)GET_MODEL_PTR(edict);
	return pstudiohdr->numskinfamilies;
}

int CGarbage::ChooseRandomSkin()
{
	int skinChosenRandomly = 0;

	int numberOfAvailableSkins = GetModelSkinCount(ENT(pev));
	if (numberOfAvailableSkins > 0)
		skinChosenRandomly = RANDOM_LONG(0, numberOfAvailableSkins - 1);

	return skinChosenRandomly;
}

void CGarbage::SetGarbageModel(int bodygroup_value)
{
	PRECACHE_MODEL(GARBAGE_DEFAULT_MODEL);
	SET_MODEL(ENT(pev), GARBAGE_DEFAULT_MODEL);

	pev->body = 0;
	pev->skin = 0;

	// Set the bodygroup as it is, but ensure it is less than the max available.
	int numberOfAvailableSubModels = GetModelCountForBodyGroup(ENT(pev), 0);
	if (numberOfAvailableSubModels > 0)
	{
		bodygroup_value = std::min(bodygroup_value, numberOfAvailableSubModels - 1);
		SetBodygroup(0, bodygroup_value);
	}
}

void CGarbage::SetSodaModel(int skin)
{
	PRECACHE_MODEL(GARBAGE_CAN_MODEL);
	SET_MODEL(ENT(pev), GARBAGE_CAN_MODEL);

	pev->body = 0;
	pev->skin = 0;

	if (skin == -1) // (-1) means a random skin.
		pev->skin = ChooseRandomSkin();
	else
	{
		// Set the skin as it is, but ensure it is less than the max available.
		int numberOfAvailableSkins = GetModelSkinCount(ENT(pev));
		if (numberOfAvailableSkins > 0)
		{
			pev->skin = std::min(skin, numberOfAvailableSkins - 1);
		}
	}
}
