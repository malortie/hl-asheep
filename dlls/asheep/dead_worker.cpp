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

#define ARRAY_LAST_INDEX(_array) ARRAYSIZE(_array) - 1

//=========================================================
// Dead Worker PROP
//=========================================================
class CDeadWorker : public CBaseDeadActor
{
public:
	int	Classify(void);
	virtual void PrecacheModels();
	virtual void SetModel();
	const char* GetPoseByIndex(int index);
	int GetLastPoseIndex();
};

int	CDeadWorker::Classify(void)
{
	return CLASS_PLAYER_ALLY;
}

// Dead workers use the same underlying sequences
// as barney, but we treat those separately.
static const char* gWorkerPoses[] =
{
	"lying_on_back",
	"lying_on_side",
	"lying_on_stomach"
};

const char* CDeadWorker::GetPoseByIndex(int index)
{
	return gWorkerPoses[index];
}

int CDeadWorker::GetLastPoseIndex()
{
	return ARRAY_LAST_INDEX(gWorkerPoses);
}

void CDeadWorker::PrecacheModels()
{
	PRECACHE_MODEL("models/construction.mdl");
}

void CDeadWorker::SetModel()
{
	SET_MODEL(ENT(pev), "models/construction.mdl");
}

LINK_ENTITY_TO_CLASS(monster_worker_dead, CDeadWorker);
