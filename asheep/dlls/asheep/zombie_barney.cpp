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
#include	"monsters.h"
#include	"schedule.h"
#include	"basemonsterwithzombieai.h"

class CZBarney : public CBaseMonsterWithZombieAI
{
protected:

	virtual void PrecacheModels();
	virtual void SetModel();
	virtual int GetFirstTimeSpawnHealth() const;

	virtual int GetOneSlashAttackDamage() const;
	virtual int GetBothSlashAttackDamage() const;
};

LINK_ENTITY_TO_CLASS(monster_zbarney, CZBarney);

void CZBarney::PrecacheModels()
{
	PRECACHE_MODEL("models/zbarney.mdl");
}

void CZBarney::SetModel()
{
	SET_MODEL(ENT(pev), "models/zbarney.mdl");
}

int CZBarney::GetFirstTimeSpawnHealth() const
{
	return gSkillData.zbarneyHealth;
}

int CZBarney::GetOneSlashAttackDamage() const
{
	return gSkillData.zbarneyDmgOneSlash;
}

int CZBarney::GetBothSlashAttackDamage() const
{
	return gSkillData.zbarneyDmgBothSlash;
}
