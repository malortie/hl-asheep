#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "dead_actor.h"

#define ARRAY_LAST_INDEX(_array) ARRAYSIZE(_array) - 1

void CBaseDeadActor::Spawn(void)
{
	PrecacheModels();
	SetModel();
	SetSkinAndBodygroups();
	SetBloodColor();

	pev->effects = 0;
	pev->yaw_speed = 8;
	pev->sequence = 0;
	
	if (m_iPose > GetLastPoseIndex())
	{
		ALERT(at_console, "Pose index %d exceeds the number(%d) of available poses for this %s.\n",
			m_iPose, GetLastPoseIndex() + 1, STRING(pev->classname));

		UTIL_Remove(this);
		return;
	}

	pev->sequence = LookupSequence(GetPoseByIndex(m_iPose));

	if (pev->sequence == -1) {
		pev->sequence = 0;
		pev->effects = EF_BRIGHTFIELD;
		ALERT(at_console, "Dead %s with bad pose\n", STRING(pev->classname));
	}

	// Corpses have less health
	pev->health = 8;

	MonsterInitDead();
}

void CBaseDeadActor::KeyValue(KeyValueData *pkvd)
{
	if (FStrEq(pkvd->szKeyName, "pose"))
	{
		m_iPose = atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else
		CBaseMonster::KeyValue(pkvd);
}

void CBaseDeadActor::SetBloodColor()
{
	m_bloodColor = BLOOD_COLOR_RED;
}

void CBaseDeadActor::SetSkinAndBodygroups()
{
	pev->skin = 0;
	pev->body = 0;
}

//=======================================================
// CDeadActorWithBarneySequences
//=======================================================

int	CDeadActorWithBarneySequences::Classify(void)
{
	return CLASS_PLAYER_ALLY;
}

static const char* gBarneyPoses[] = 
{ 
	"lying_on_back",
	"lying_on_side",
	"lying_on_stomach"
};

const char* CDeadActorWithBarneySequences::GetPoseByIndex(int index)
{
	return gBarneyPoses[index];
}

int CDeadActorWithBarneySequences::GetLastPoseIndex()
{
	return ARRAY_LAST_INDEX(gBarneyPoses);
}

//=======================================================
// CDeadActorWithScientistSequences
//=======================================================

static const char* gScientistPoses[] =
{
	"lying_on_back",
	"lying_on_stomach",
	"dead_sitting",
	"dead_hang",
	"dead_table1",
	"dead_table2",
	"dead_table3"
};

int	CDeadActorWithScientistSequences::Classify(void)
{
	return CLASS_HUMAN_PASSIVE;
}

const char* CDeadActorWithScientistSequences::GetPoseByIndex(int index)
{
	return gScientistPoses[index];
}

int CDeadActorWithScientistSequences::GetLastPoseIndex()
{
	return ARRAY_LAST_INDEX(gScientistPoses);
}

//=======================================================
// CDeadActorWithHGruntSequences
//=======================================================

static const char* gHGruntPoses[] =
{
	"deadstomach",
	"deadside",
	"deadsitting",
};

int	CDeadActorWithHGruntSequences::Classify(void)
{
	return CLASS_HUMAN_MILITARY;
}

const char* CDeadActorWithHGruntSequences::GetPoseByIndex(int index)
{
	return gHGruntPoses[index];
}

int CDeadActorWithHGruntSequences::GetLastPoseIndex()
{
	return ARRAY_LAST_INDEX(gHGruntPoses);
}
