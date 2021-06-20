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
// Zombie
//=========================================================

// UNDONE: Don't flinch every time you get hit

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"schedule.h"
#include	"basemonsterwithzombieai.h"

class CZombie : public CBaseMonsterWithZombieAI
{
protected:

	virtual void PrecacheModels();
	virtual void SetModel();
	virtual int GetFirstTimeSpawnHealth() const;

	virtual int GetOneSlashAttackDamage() const;
	virtual int GetBothSlashAttackDamage() const;
};

LINK_ENTITY_TO_CLASS(monster_zombie, CZombie);

void CZombie::PrecacheModels()
{
	PRECACHE_MODEL("models/zombie.mdl");
}

void CZombie::SetModel()
{
	SET_MODEL(ENT(pev), "models/zombie.mdl");
}

int CZombie::GetFirstTimeSpawnHealth() const
{
	return gSkillData.zombieHealth;
}

int CZombie::GetOneSlashAttackDamage() const
{
	return gSkillData.zombieDmgOneSlash;
}

int CZombie::GetBothSlashAttackDamage() const
{
	return gSkillData.zombieDmgBothSlash;
}
