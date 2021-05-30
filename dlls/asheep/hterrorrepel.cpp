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

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"effects.h"
#include	"customentity.h"
#include	"basemonsterrepel.h"

class CHTerrorRepel : public CBaseMonsterRepel
{
public:
	const char* GetClassNameOfMonsterToSpawn() const;
};

LINK_ENTITY_TO_CLASS(monster_terror_repel, CHTerrorRepel);

const char* CHTerrorRepel::GetClassNameOfMonsterToSpawn() const
{
	return "monster_human_terror";
}
