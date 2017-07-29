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
#include "monsters.h"
#include "schedule.h"
#include "nodes.h"
#include "soundent.h"
#include "animation.h"
#include "effects.h"
#include "weapons.h"
#include "asheep_globals.h"
#if defined ( ASHEEP_MAPFIXES )
#include "asheep_map_fixes.h"

#define IsTriggerTargetName(_entity, _str) FStrEq(STRING(_entity->pev->targetname), _str)
#define IsTriggerTarget(_entity, _str) FStrEq(STRING(_entity->pev->target), _str)
#define IsTriggerOnce(_entity) FStrEq(STRING(_entity->pev->classname), "trigger_once")
#define IsTriggerMultiple(_entity) FStrEq(STRING(_entity->pev->classname), "trigger_multiple")

#define KATE_CLASSNAME	"monster_kate"

class MapFixes;
class MapFixLadderWithTooHighVelocity;
class MapFixKateOnlyTriggers;
class MapFixGruntsShootAtJeepInOutroLevel;
class MapFixKateHealingOnAsmap06c;
class MapFixMapMusicNotPlaying;
class MapFixIntroMusicNotPlaying;
class MapFixBarneyNotDyingUponApacheCrash;
class MapFixUnstickRatFromWall;
class MapFixRemoveTrainingMessages;

//================================================
//
// class MapFixes
//
//================================================

class MapFixes
{
public:
	virtual void ApplyFixesForMapT0a0();
	virtual void ApplyFixesForMapIntro();
	virtual void ApplyFixesForMapAsmap00b();
	virtual void ApplyFixesForMapAsmap05b();
	virtual void ApplyFixesForMapAsmap11c();
	virtual void ApplyFixesForMapCredits();
	virtual BOOL CheckIfOnlyKateShouldActivateTrigger(CBaseEntity* trigger);
	virtual BOOL CheckIfEntityIsOutroJeep(CBaseEntity* entity);
	virtual BOOL CheckIfShouldTeleportKateToHealingSequence(CBaseEntity* trigger);
	virtual void FixupTriggerForAsmap06KateHealingSequence(CBaseEntity* trigger);
	virtual void TeleportKateToHealingSequence();

protected:

	MapFixLadderWithTooHighVelocity* GetLadderWithTooHighVelocitySingleton();
	MapFixKateOnlyTriggers* GetKateOnlyTriggersSingleton();
	MapFixGruntsShootAtJeepInOutroLevel* GetGruntsShootAtJeepInOutroLevelSingleton();
	MapFixKateHealingOnAsmap06c* GetKateHealingOnAsmap06cSingleton();
	MapFixMapMusicNotPlaying* GetMapMusicNotPlayingSingleton();
	MapFixIntroMusicNotPlaying* GetIntroMusicNotPlayingSingleton();
	MapFixBarneyNotDyingUponApacheCrash* GetBarneyNotDyingUponApacheCrashSingleton();
	MapFixUnstickRatFromWall* GetUnstickRatFromWallSingleton();
};

static MapFixes* GetMapFixesSingleton()
{
	static MapFixes gMapFixes;
	return &gMapFixes;
}

//=======================================================
//
// Fix for map asmap05b.
//
// Players find themselves in a small cockpit where
// it's possible to control brush based machine gun
// entities.
//
// When attempting to leave the cockpit by climbing
// the ladder, a bug that I wasn't able to identity
// would restrain players from climbing the ladder.
// Think of it as if players were pushed down the cabin.
//
//=======================================================

// Volume 1 is for entering the cockpit area with the machine gun.
// Volumes 2, 3, 4, 5, 6 are for escaping the cockpit area with the machine gun.
#define ASMAP05B_TELEPORT_VOLUME_COUNT	6
#define ASMAP05B_TELEPORT_DESTINATION_COUNT	2

class MapFixLadderWithTooHighVelocity
{
	CBaseEntity* m_teleportVolumes[ASMAP05B_TELEPORT_VOLUME_COUNT];
	CBaseEntity* m_teleportDestinations[ASMAP05B_TELEPORT_DESTINATION_COUNT];
public:
	void ApplyFix();

protected:

	void CreateAndSpawnVolumes();
	void CreateAndSpawnDestinations();
	void SetVolumesDestination();

	CBaseEntity* CreateAndPlaceTeleportVolumeAlongLadder(
		const Vector& ladderTipStartFromTopView,
		const Vector& ladderTipEndFromTopView,
		float interpolation,
		const Vector& mins,
		const Vector& maxs);

	CBaseEntity* CreateTeleportVolume(const Vector& origin, const Vector& mins, const Vector& maxs);
	CBaseEntity* CreateTeleportDestination(const Vector& origin);

private:
	CBaseEntity* GetTeleportEntryDestination() { return m_teleportDestinations[0]; }
	CBaseEntity* GetTeleportExitDestination() { return m_teleportDestinations[1]; }
	CBaseEntity* GetTeleportVolume(int index) { return m_teleportVolumes[index]; }
};

//================================================
//
// class MapFixKateOnlyTriggers
//
//================================================
class MapFixKateOnlyTriggers
{
public:
	BOOL ShouldOnlyKateActivateTrigger(CBaseEntity* trigger);
protected:
	BOOL ShouldOnlyKateActivateTriggerAsmap05(CBaseEntity* trigger);
	BOOL ShouldOnlyKateActivateTriggerAsmap05b(CBaseEntity* trigger);
	BOOL ShouldOnlyKateActivateTriggerAsmap06c(CBaseEntity* trigger);
	BOOL ShouldOnlyKateActivateTriggerAsmap08b(CBaseEntity* trigger);
	BOOL ShouldOnlyKateActivateTriggerAsmap09c(CBaseEntity* trigger);
	BOOL ShouldOnlyKateActivateTriggerAsmap10b(CBaseEntity* trigger);
};

//================================================
//
// class MapFixGruntsShootAtJeepInOutroLevel
//
//================================================

class MapFixGruntsShootAtJeepInOutroLevel
{
public:
	BOOL IsEntityOutroJeep(CBaseEntity* entity);
};

//================================================
//
// class MapFixKateHealingOnAsmap06c
//
//================================================

class MapFixKateHealingOnAsmap06c
{
	string_t m_kateTargetname;
	string_t m_kateClassname;
	string_t m_sequenceTargetname;

public:
	MapFixKateHealingOnAsmap06c();

	void ApplyFixToTrigger(CBaseEntity* trigger);

	BOOL CheckIfKateTargetnameValueIsValid() { return !FStringNull(m_kateTargetname); }
	BOOL CheckIfKateClassnameValueIsValid() { return !FStringNull(m_kateClassname); }
	BOOL CheckIfSequenceTargetnameValueIsValid() { return !FStringNull(m_sequenceTargetname); }

	void SetKateTargetnameToSearch(const char* targetname);
	void SetKateClassnameToSearch(const char* classname);
	void SetSequenceTargetnameToSearch(const char* targetname);
	BOOL CheckIfShouldTeleportKateToHealingSequence(CBaseEntity* trigger);
	void TeleportKateToHealingSequence();

protected:

	const char* GetKateTargetnameToSearch() { return STRING(m_kateTargetname); }
	const char* GetKateClassnameToSearch() { return STRING(m_kateClassname); }
	const char* GetSequenceTargetnameToSearch() { return STRING(m_sequenceTargetname); }
};

//================================================
//
// class CKateHealingSequenceTeleporter
//
//================================================
class CKateHealingSequenceTeleporter : public CBaseEntity
{
public:
	void Spawn(void) {}

	void Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);
};

//================================================
//
// class CCDTrackPlayer
//
//================================================

class CCDTrackPlayer : public CBaseEntity
{
public:
	void Spawn(void);
	int	ObjectCaps(void) { return (CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION) | FCAP_DONT_SAVE; }
	void EXPORT WaitAndPlay(void);

	void SetCDTrackToPlay(int trackId);

protected:
	edict_t* GetClientToWaitFor();
	void Play();
};

//================================================
//
// class MapFixMapMusicNotPlaying
//
//================================================

class MapFixMapMusicNotPlaying
{
public:
	void ApplyFix();
};

//================================================
//
// class MapFixIntroMusicNotPlaying
//
//================================================

class MapFixIntroMusicNotPlaying
{
public:
	void ApplyFix();
};


//================================================
//
// class MapFixBarneyNotDyingUponApacheCrash
//
//================================================

class MapFixBarneyNotDyingUponApacheCrash
{
public:
	void ApplyFix();
};


//================================================
//
// class CKillBarneyUponApacheCrash
//
//================================================

class CKillBarneyUponApacheCrash : public CBaseEntity
{
public:
	virtual int		Save(CSave &save);
	virtual int		Restore(CRestore &restore);
	static	TYPEDESCRIPTION m_SaveData[];

	void Spawn(void);
	int	ObjectCaps(void) { return (CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION); }
	void EXPORT WaitAndSetupScriptedSequenceToCallThisEntity(void);
	void EXPORT FindAndKillBarney(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);
protected:
	virtual void KillBarney();

	virtual CBaseEntity* FindScriptedSequenceByName(const char* targetname);
	virtual CBaseEntity* FindBarneyToKillByName(const char* targetname);
	void SetScriptedSequenceTargetToThisEntity();

private:

	CBaseEntity* m_scriptedSequence;
};

LINK_ENTITY_TO_CLASS(MapFix_KillBarneyUponApacheCrash, CKillBarneyUponApacheCrash);

//================================================
//
// class MapFixRatStuckInWallOnT0a0
//
//================================================

class MapFixUnstickRatFromWall
{
public:
	void ApplyFix();
};

//================================================
//
// class CRatWallUnstick
//
//================================================

class CRatUnstickFromWall : public CBaseEntity
{
public:
	void Spawn(void);
	int	ObjectCaps(void) { return (CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION); }
	void EXPORT WaitAndUnstickRatFromWall(void);
protected:

	virtual CBaseMonster* FindRatToUnstickFromWall();

	void UnstickRatFromWall();

private:

	CBaseMonster* m_ratMonster;
};

//================================================
// MapFixLadderWithTooHighVelocity
//================================================

void MapFixLadderWithTooHighVelocity::ApplyFix()
{
	memset(m_teleportDestinations, 0, ASMAP05B_TELEPORT_DESTINATION_COUNT);
	memset(m_teleportVolumes, 0, ASMAP05B_TELEPORT_VOLUME_COUNT);

	CreateAndSpawnVolumes();
	CreateAndSpawnDestinations();
	SetVolumesDestination();
}

void MapFixLadderWithTooHighVelocity::CreateAndSpawnVolumes()
{
	const int EXIT_VOLUME_COUNT = ASMAP05B_TELEPORT_VOLUME_COUNT - 1;
	const int LAST_EXIT_VOLUME_INDEX = EXIT_VOLUME_COUNT - 1;

	Vector mins, maxs;

	// Create one entry volume.
	mins = Vector(-40, -40, 0);
	maxs = Vector(40, 40, 2);

	if (m_teleportVolumes[0] == NULL)
		m_teleportVolumes[0] = CreateTeleportVolume(Vector(896, -64, -191), mins, maxs);


	// Create five exit volumes.
	mins = Vector(-2, -2, 0);
	maxs = Vector(2, 2, 72);

	Vector ladderStart(922, -22, -327);
	Vector ladderEnd(938, -38, -327);

	if (m_teleportVolumes[1] == NULL)
		m_teleportVolumes[1] = CreateAndPlaceTeleportVolumeAlongLadder(ladderStart, ladderEnd, 0, mins, maxs);

	if (m_teleportVolumes[2] == NULL)
		m_teleportVolumes[2] = CreateAndPlaceTeleportVolumeAlongLadder(ladderStart, ladderEnd, 1 / (float)LAST_EXIT_VOLUME_INDEX, mins, maxs);

	if (m_teleportVolumes[3] == NULL)
		m_teleportVolumes[3] = CreateAndPlaceTeleportVolumeAlongLadder(ladderStart, ladderEnd, 2 / (float)LAST_EXIT_VOLUME_INDEX, mins, maxs);

	if (m_teleportVolumes[4] == NULL)
		m_teleportVolumes[4] = CreateAndPlaceTeleportVolumeAlongLadder(ladderStart, ladderEnd, 3 / (float)LAST_EXIT_VOLUME_INDEX, mins, maxs);

	if (m_teleportVolumes[5] == NULL)
		m_teleportVolumes[5] = CreateAndPlaceTeleportVolumeAlongLadder(ladderStart, ladderEnd, 1, mins, maxs);
}

void MapFixLadderWithTooHighVelocity::CreateAndSpawnDestinations()
{
	// Create the destination entity when entering the cockpit area.
	if (m_teleportDestinations[0] == NULL)
		m_teleportDestinations[0] = CreateTeleportDestination(Vector(896, -64, -355));

	// Create the destination entity when leaving the cockpit area.
	if (m_teleportDestinations[1] == NULL)
		m_teleportDestinations[1] = CreateTeleportDestination(Vector(960, 0, -183));
}

void MapFixLadderWithTooHighVelocity::SetVolumesDestination()
{
	GetTeleportEntryDestination()->pev->targetname = MAKE_STRING("_asmap05bladderwithtoohighvelocity_tele_entry");
	GetTeleportExitDestination()->pev->targetname = MAKE_STRING("_asmap05bladderwithtoohighvelocity_tele_exit");

	// Setup destination for the entry volume.
	GetTeleportVolume(0)->pev->target = GetTeleportEntryDestination()->pev->targetname;

	// Setup destination for the exit volumes.
	for (int i = 1; i < ASMAP05B_TELEPORT_VOLUME_COUNT; i++)
		GetTeleportVolume(i)->pev->target = GetTeleportExitDestination()->pev->targetname;
}

CBaseEntity* MapFixLadderWithTooHighVelocity::CreateAndPlaceTeleportVolumeAlongLadder(
	const Vector& ladderTipStartFromTopView,
	const Vector& ladderTipEndFromTopView,
	float interpolation,
	const Vector& mins,
	const Vector& maxs)
{
	CBaseEntity* volume = CreateTeleportVolume(g_vecZero, mins, maxs);
	if (volume != NULL)
	{
		Vector direction = (ladderTipEndFromTopView - ladderTipStartFromTopView).Normalize();
		float distance = (ladderTipEndFromTopView - ladderTipStartFromTopView).Length2D();

		Vector resultPosition = ladderTipStartFromTopView + direction * distance * interpolation;

		UTIL_SetOrigin(volume->pev, resultPosition);
	}

	return volume;
}

CBaseEntity* MapFixLadderWithTooHighVelocity::CreateTeleportVolume(const Vector& origin, const Vector& mins, const Vector& maxs)
{
	CBaseEntity* volume = CBaseEntity::Create("trigger_teleportuseplayerangles", origin, g_vecZero, NULL);

	if (volume != NULL)
	{
		volume->pev->spawnflags = 0;
		volume->Precache();
		volume->Spawn();
		UTIL_SetSize(volume->pev, mins, maxs);
	}

	return volume;
}

CBaseEntity* MapFixLadderWithTooHighVelocity::CreateTeleportDestination(const Vector& origin)
{
	CBaseEntity* destination = CBaseEntity::Create("info_teleport_destination", origin, g_vecZero, NULL);

	if (destination != NULL)
	{
		destination->Spawn();
	}

	return destination;
}


//================================================
// MapFixKateOnlyTriggers
//================================================

BOOL MapFixKateOnlyTriggers::ShouldOnlyKateActivateTrigger(CBaseEntity* trigger)
{
	if (!FStringNull(trigger->pev->target))
	{
		if (GetAsheepGlobalsSingleton()->IsMapAsmap05())
			return ShouldOnlyKateActivateTriggerAsmap05(trigger);
		if (GetAsheepGlobalsSingleton()->IsMapAsmap05b())
			return ShouldOnlyKateActivateTriggerAsmap05b(trigger);
		if (GetAsheepGlobalsSingleton()->IsMapAsmap06c())
			return ShouldOnlyKateActivateTriggerAsmap06c(trigger);
		if (GetAsheepGlobalsSingleton()->IsMapAsmap08b())
			return ShouldOnlyKateActivateTriggerAsmap08b(trigger);
		if (GetAsheepGlobalsSingleton()->IsMapAsmap09c())
			return ShouldOnlyKateActivateTriggerAsmap09c(trigger);
		if (GetAsheepGlobalsSingleton()->IsMapAsmap10b())
			return ShouldOnlyKateActivateTriggerAsmap10b(trigger);
	}

	return FALSE;
}

BOOL MapFixKateOnlyTriggers::ShouldOnlyKateActivateTriggerAsmap05(CBaseEntity* trigger)
{
	return IsTriggerOnce(trigger) &&
		IsTriggerTarget(trigger, "05retsecmm0101");
}

BOOL MapFixKateOnlyTriggers::ShouldOnlyKateActivateTriggerAsmap05b(CBaseEntity* trigger)
{
	return IsTriggerMultiple(trigger) &&
		IsTriggerTargetName(trigger, "05bretsecmmtrg0101");
}
BOOL MapFixKateOnlyTriggers::ShouldOnlyKateActivateTriggerAsmap06c(CBaseEntity* trigger)
{
	return IsTriggerOnce(trigger) &&
		(IsTriggerTarget(trigger, "06cretsecmm0101") ||
			IsTriggerTarget(trigger, "06cretsecmm0201"));
}
BOOL MapFixKateOnlyTriggers::ShouldOnlyKateActivateTriggerAsmap08b(CBaseEntity* trigger)
{
	return  IsTriggerOnce(trigger) &&
		(IsTriggerTarget(trigger, "08bretsecmm0201") ||
			IsTriggerTarget(trigger, "08bkatetrseq0101"));
}
BOOL MapFixKateOnlyTriggers::ShouldOnlyKateActivateTriggerAsmap09c(CBaseEntity* trigger)
{
	return IsTriggerOnce(trigger) &&
		IsTriggerTarget(trigger, "09cretsecmm0101");
}
BOOL MapFixKateOnlyTriggers::ShouldOnlyKateActivateTriggerAsmap10b(CBaseEntity* trigger)
{
	return (IsTriggerOnce(trigger) &&
		(IsTriggerTarget(trigger, "10bretsecmm0101") ||
			IsTriggerTarget(trigger, "10bretsecmm0201"))) ||

			(IsTriggerMultiple(trigger) && IsTriggerTarget(trigger, "10bknogoseq0101"));
}

//================================================
// MapFixGruntsShootAtJeepInOutroLevel
//================================================

BOOL MapFixGruntsShootAtJeepInOutroLevel::IsEntityOutroJeep(CBaseEntity* entity)
{
	return GetAsheepGlobalsSingleton()->IsMapOutro() &&
		FStrEq(STRING(entity->pev->targetname), "11cjeep0101");
}

//================================================
// MapFixKateHealingOnAsmap06c
//================================================

MapFixKateHealingOnAsmap06c::MapFixKateHealingOnAsmap06c() :
	m_kateTargetname(iStringNull),
	m_kateClassname(iStringNull),
	m_sequenceTargetname(iStringNull)
{
}

void MapFixKateHealingOnAsmap06c::ApplyFixToTrigger(CBaseEntity* trigger)
{
	edict_t* pevMediator = CREATE_NAMED_ENTITY(MAKE_STRING("katehealingsequenceteleporter"));
	if (FNullEnt(pevMediator))
	{
		ALERT(at_console, "** Could not create Kate healing sequence teleporter! **\n");
		return;
	}

	CBaseEntity* mediator = CBaseEntity::Instance(pevMediator);
	if (mediator == NULL)
	{
		ALERT(at_console, "** Failed to retreive Kate healing sequence teleporter instance! **\n");
		return;
	}

	mediator->pev->origin = g_vecZero;

	mediator->pev->targetname = ALLOC_STRING("06katehealseqteleport01");
	mediator->pev->target = trigger->pev->target;
	trigger->pev->target = mediator->pev->targetname;

	DispatchSpawn(pevMediator);
}

void MapFixKateHealingOnAsmap06c::SetKateTargetnameToSearch(const char* targetname)
{
	if (CheckIfKateTargetnameValueIsValid() && FStrEq(STRING(m_kateTargetname), targetname))
		return;

	m_kateTargetname = ALLOC_STRING(targetname);
}

void MapFixKateHealingOnAsmap06c::SetKateClassnameToSearch(const char* classname)
{
	if (CheckIfKateClassnameValueIsValid() & FStrEq(STRING(m_kateClassname), classname))
		return;

	m_kateClassname = ALLOC_STRING(classname);
}

void MapFixKateHealingOnAsmap06c::SetSequenceTargetnameToSearch(const char* targetname)
{
	if (CheckIfSequenceTargetnameValueIsValid() && FStrEq(STRING(m_sequenceTargetname), targetname))
		return;

	m_sequenceTargetname = ALLOC_STRING(targetname);
}

BOOL MapFixKateHealingOnAsmap06c::CheckIfShouldTeleportKateToHealingSequence(CBaseEntity* trigger)
{
	if (!GetAsheepGlobalsSingleton()->IsMapAsmap06c())
		return FALSE;

	return IsTriggerOnce(trigger) &&
			IsTriggerTarget(trigger, "06cdr0301");
}

void MapFixKateHealingOnAsmap06c::TeleportKateToHealingSequence()
{
	if ((!CheckIfKateTargetnameValueIsValid() && !CheckIfKateClassnameValueIsValid()) ||
		!CheckIfSequenceTargetnameValueIsValid())
	{
		ALERT(at_console, "One or more Kate healing sequence search values is/are invalid!\n");
		return;
	}

	CBaseEntity* kateMonster = UTIL_FindEntityByTargetname(NULL, GetKateTargetnameToSearch());
	if (kateMonster == NULL)
	{
		kateMonster = UTIL_FindEntityByClassname(NULL, GetKateClassnameToSearch());
		if (kateMonster == NULL)
		{
			ALERT(at_console, "Could not find named entity %s or any %s instances!\n",
				GetKateTargetnameToSearch(), GetKateClassnameToSearch());
			return;
		}

		// Set kate's targetname to allow itself to be used as
		// Target Monster by scripted sequence entity.
		kateMonster->pev->targetname = ALLOC_STRING(GetKateTargetnameToSearch());
	}

	CBaseEntity* scriptedSequence = UTIL_FindEntityByTargetname(NULL, GetSequenceTargetnameToSearch());
	if (scriptedSequence == NULL)
	{
		ALERT(at_console, "Could not find Kate healing sequence entity!\n");
		return;
	}

	// Teleport Kate monster entity to it's healing sequence.
	UTIL_SetOrigin(kateMonster->pev, scriptedSequence->pev->origin + Vector(0, 0, 1));
	kateMonster->pev->ideal_yaw = scriptedSequence->pev->angles.y;
	kateMonster->pev->avelocity = Vector(0, 0, 0);
	kateMonster->pev->velocity = Vector(0, 0, 0);
	kateMonster->pev->effects |= EF_NOINTERP;
	kateMonster->pev->angles.y = scriptedSequence->pev->angles.y;
}

//================================================
// CKateHealingSequenceTeleporter
//================================================

LINK_ENTITY_TO_CLASS(katehealingsequenceteleporter, CKateHealingSequenceTeleporter);

void CKateHealingSequenceTeleporter::Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value)
{
	GetMapFixesSingleton()->TeleportKateToHealingSequence();
	SUB_UseTargets(pActivator, useType, value);
	UTIL_Remove(this);
}

//================================================
// COutroCDTrackPlayer
//================================================

void PlayCDTrack(int iTrack);

void CCDTrackPlayer::Spawn(void)
{
	if (!FNullEnt(GetClientToWaitFor()))
	{
		Play();
		UTIL_Remove(this);
	}
	else
	{
		SetThink(&CCDTrackPlayer::WaitAndPlay);
		pev->nextthink = gpGlobals->time + 1.0f;
	}
}

void CCDTrackPlayer::WaitAndPlay(void)
{
	if (!FNullEnt(GetClientToWaitFor()))
	{
		Play();
		SetThink(&CBaseEntity::SUB_Remove);
		pev->nextthink = gpGlobals->time;
	}
	else
		pev->nextthink = gpGlobals->time + 0.05f;
}

void CCDTrackPlayer::SetCDTrackToPlay(int trackId)
{
	pev->health = trackId;
}

edict_t* CCDTrackPlayer::GetClientToWaitFor()
{
	return g_engfuncs.pfnPEntityOfEntIndex(1);
}

void CCDTrackPlayer::Play()
{
	CLIENT_COMMAND(GetClientToWaitFor(), "cd stop\n");

	// pev->health is cd audio track to play.
	::PlayCDTrack(pev->health);
}

//================================================
// MapFixMapMusicNotPlaying
//================================================

void MapFixMapMusicNotPlaying::ApplyFix()
{
	CCDTrackPlayer* audioTrackPlayer = GetClassPtr((CCDTrackPlayer*)NULL);
	if (audioTrackPlayer != NULL)
	{
		audioTrackPlayer->SetCDTrackToPlay(gpGlobals->cdAudioTrack);
		audioTrackPlayer->Spawn();
	}
}

//================================================
// MapFixIntroMusicNotPlaying
//================================================

void MapFixIntroMusicNotPlaying::ApplyFix()
{
	CCDTrackPlayer* audioTrackPlayer = GetClassPtr((CCDTrackPlayer*)NULL);
	if (audioTrackPlayer != NULL)
	{
		audioTrackPlayer->SetCDTrackToPlay(3);
		audioTrackPlayer->Spawn();
	}
}

//================================================
// MapFixUnstickRatFromWall
//================================================

void MapFixBarneyNotDyingUponApacheCrash::ApplyFix()
{
	CKillBarneyUponApacheCrash* pBarneyKillerEntity = GetClassPtr((CKillBarneyUponApacheCrash*)NULL);
	if (pBarneyKillerEntity)
		pBarneyKillerEntity->Spawn();
}

//================================================
// CRatUnstickFromWall
//================================================

TYPEDESCRIPTION CKillBarneyUponApacheCrash::m_SaveData[] = 
{
	DEFINE_FIELD(CKillBarneyUponApacheCrash, m_scriptedSequence, FIELD_CLASSPTR)
};
IMPLEMENT_SAVERESTORE(CKillBarneyUponApacheCrash, CBaseEntity);

void CKillBarneyUponApacheCrash::Spawn(void)
{
	m_scriptedSequence = NULL;

	SetUse(&CKillBarneyUponApacheCrash::FindAndKillBarney);
	SetThink(&CKillBarneyUponApacheCrash::WaitAndSetupScriptedSequenceToCallThisEntity);
	pev->nextthink = gpGlobals->time + 0.1f;
}

void CKillBarneyUponApacheCrash::WaitAndSetupScriptedSequenceToCallThisEntity(void)
{
	if (m_scriptedSequence == NULL)
		m_scriptedSequence = FindScriptedSequenceByName("00onpbarnseq0108");

	if (m_scriptedSequence == NULL)
	{
		pev->nextthink = gpGlobals->time + 1.0f;
	}
	else
	{
		SetScriptedSequenceTargetToThisEntity();
		m_scriptedSequence = NULL;

		SetThink(&CBaseEntity::SUB_DoNothing);
		pev->nextthink = gpGlobals->time + 0.2f;
	}
}

void CKillBarneyUponApacheCrash::FindAndKillBarney(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	KillBarney();

	SetUse(NULL);
	SetThink(&CBaseEntity::SUB_Remove);
	pev->nextthink = gpGlobals->time + 0.2f;
}

void CKillBarneyUponApacheCrash::KillBarney()
{
	CBaseEntity* barney = FindBarneyToKillByName("00onpbarn01");
	if (barney != NULL)
		barney->Killed(pev, GIB_ALWAYS);
}

CBaseEntity* CKillBarneyUponApacheCrash::FindScriptedSequenceByName(const char* targetname)
{
	return UTIL_FindEntityByTargetname(NULL, targetname);
}

CBaseEntity* CKillBarneyUponApacheCrash::FindBarneyToKillByName(const char* targetname)
{
	return UTIL_FindEntityByTargetname(NULL, targetname);
}

void CKillBarneyUponApacheCrash::SetScriptedSequenceTargetToThisEntity()
{
	pev->targetname = ALLOC_STRING("00killbarneyuponapachecrash");
	m_scriptedSequence->pev->target = pev->targetname;
}

//================================================
// MapFixUnstickRatFromWall
//================================================

void MapFixUnstickRatFromWall::ApplyFix()
{
	CRatUnstickFromWall* pRatUnstickFromWall = GetClassPtr((CRatUnstickFromWall*)NULL);
	if (pRatUnstickFromWall)
		pRatUnstickFromWall->Spawn();
}

//================================================
// CRatUnstickFromWall
//================================================

void CRatUnstickFromWall::Spawn(void)
{
	m_ratMonster = NULL;

	SetThink(&CRatUnstickFromWall::WaitAndUnstickRatFromWall);
	pev->nextthink = gpGlobals->time + 0.1f;
}

void CRatUnstickFromWall::WaitAndUnstickRatFromWall(void)
{
	if (m_ratMonster == NULL)
		m_ratMonster = FindRatToUnstickFromWall();

	if (m_ratMonster == NULL)
	{
		pev->nextthink = gpGlobals->time + 1.0f;
	}
	else
	{
		UnstickRatFromWall();
		m_ratMonster = NULL;

		SetThink(&CBaseEntity::SUB_Remove);
		pev->nextthink = gpGlobals->time + 0.2f;
	}
}

CBaseMonster* CRatUnstickFromWall::FindRatToUnstickFromWall()
{
	BOOL foundRatMonster = FALSE;
	CBaseMonster* ratMonster = NULL;
	
	while (!foundRatMonster && (ratMonster = (CBaseMonster*)UTIL_FindEntityByClassname(ratMonster, "monster_rat")) != NULL)
	{
		// The monster we are trying to find has no targetname.
		if (ratMonster && FStringNull(ratMonster->pev->targetname))
			foundRatMonster = TRUE;
	}

	if (!foundRatMonster)
		ratMonster = NULL;

	return ratMonster;
}

void CRatUnstickFromWall::UnstickRatFromWall()
{
	edict_t* pevMonster = CREATE_NAMED_ENTITY(MAKE_STRING("monster_rat"));
	if (FNullEnt(pevMonster))
	{
		ALERT(at_console, "** Could not create monster_rat entity! **\n");
		return;
	}

	CBaseMonster* newRatMonster = (CBaseMonster*)CBaseEntity::Instance(pevMonster);
	if (!newRatMonster)
	{
		ALERT(at_console, "** Could not retrieve monster_rat entity instance! **\n");
		return;
	}

	newRatMonster->pev->targetname = m_ratMonster->pev->targetname;
	//newRatMonster->pev->target = m_ratMonster->pev->target;
	newRatMonster->pev->origin = m_ratMonster->pev->origin;
	newRatMonster->pev->angles.y = m_ratMonster->pev->angles.y;
	newRatMonster->pev->velocity = newRatMonster->pev->avelocity = g_vecZero;
	newRatMonster->pev->spawnflags = m_ratMonster->pev->spawnflags | SF_MONSTER_FALL_TO_GROUND;
	newRatMonster->Precache();

	UTIL_Remove(m_ratMonster);

	DispatchSpawn(newRatMonster->edict());

	DROP_TO_FLOOR(newRatMonster->edict());
}

//================================================
// MapFixes
//================================================

MapFixLadderWithTooHighVelocity* MapFixes::GetLadderWithTooHighVelocitySingleton()
{
	static MapFixLadderWithTooHighVelocity singleton;
	return &singleton;
}

MapFixKateOnlyTriggers* MapFixes::GetKateOnlyTriggersSingleton()
{
	static MapFixKateOnlyTriggers singleton;
	return &singleton;
}

MapFixGruntsShootAtJeepInOutroLevel* MapFixes::GetGruntsShootAtJeepInOutroLevelSingleton()
{
	static MapFixGruntsShootAtJeepInOutroLevel singleton;
	return &singleton;
}

MapFixKateHealingOnAsmap06c* MapFixes::GetKateHealingOnAsmap06cSingleton()
{
	static MapFixKateHealingOnAsmap06c singleton;
	return &singleton;
}

MapFixMapMusicNotPlaying* MapFixes::GetMapMusicNotPlayingSingleton()
{
	static MapFixMapMusicNotPlaying singleton;
	return &singleton;
}

MapFixIntroMusicNotPlaying* MapFixes::GetIntroMusicNotPlayingSingleton()
{
	static MapFixIntroMusicNotPlaying singleton;
	return &singleton;
}

MapFixBarneyNotDyingUponApacheCrash* MapFixes::GetBarneyNotDyingUponApacheCrashSingleton()
{
	static MapFixBarneyNotDyingUponApacheCrash singleton;
	return &singleton;
}

MapFixUnstickRatFromWall* MapFixes::GetUnstickRatFromWallSingleton()
{
	static MapFixUnstickRatFromWall singleton;
	return &singleton;
}

void MapFixes::ApplyFixesForMapT0a0()
{
	// ALERT(at_aiconsole, "Applying map fixes for t0a0.\n");
	GetUnstickRatFromWallSingleton()->ApplyFix();
}

void MapFixes::ApplyFixesForMapIntro()
{
	// ALERT(at_aiconsole, "Applying map fixes for asmapin.\n");
	GetIntroMusicNotPlayingSingleton()->ApplyFix();
}

void MapFixes::ApplyFixesForMapAsmap00b()
{
	// ALERT(at_aiconsole, "Applying map fixes for asmap00b.\n");
	GetBarneyNotDyingUponApacheCrashSingleton()->ApplyFix();
}

void MapFixes::ApplyFixesForMapAsmap05b()
{
	// ALERT(at_aiconsole, "Applying map fixes for asmap05b.\n");
	GetLadderWithTooHighVelocitySingleton()->ApplyFix();
}

void MapFixes::ApplyFixesForMapAsmap11c()
{
	// ALERT(at_aiconsole, "Applying map fixes for asmap11c.\n");
	GetMapMusicNotPlayingSingleton()->ApplyFix();
}

void MapFixes::ApplyFixesForMapCredits()
{
	// ALERT(at_aiconsole, "Applying map fixes for asmapend.\n");
	GetMapMusicNotPlayingSingleton()->ApplyFix();
}

BOOL MapFixes::CheckIfOnlyKateShouldActivateTrigger(CBaseEntity* trigger)
{
	return GetKateOnlyTriggersSingleton()->ShouldOnlyKateActivateTrigger(trigger);
}

BOOL MapFixes::CheckIfEntityIsOutroJeep(CBaseEntity* entity)
{
	return GetGruntsShootAtJeepInOutroLevelSingleton()->IsEntityOutroJeep(entity);
}

BOOL MapFixes::CheckIfShouldTeleportKateToHealingSequence(CBaseEntity* trigger)
{
	return GetKateHealingOnAsmap06cSingleton()->CheckIfShouldTeleportKateToHealingSequence(trigger);
}

void MapFixes::FixupTriggerForAsmap06KateHealingSequence(CBaseEntity* trigger)
{
	GetKateHealingOnAsmap06cSingleton()->ApplyFixToTrigger(trigger);
}

void MapFixes::TeleportKateToHealingSequence()
{
	MapFixKateHealingOnAsmap06c* const singleton = GetKateHealingOnAsmap06cSingleton();
	singleton->SetKateTargetnameToSearch("katemm");
	singleton->SetKateClassnameToSearch(KATE_CLASSNAME);
	singleton->SetSequenceTargetnameToSearch("06ckateseq0101");
	singleton->TeleportKateToHealingSequence();
}

void MapFixes_ApplyAllPossibleFixes()
{
	if (GetAsheepGlobalsSingleton()->IsMapT0a0())
	{
		GetMapFixesSingleton()->ApplyFixesForMapT0a0();
	}
	else if (GetAsheepGlobalsSingleton()->IsMapIntro())
	{
		GetMapFixesSingleton()->ApplyFixesForMapIntro();
	}
	else if (GetAsheepGlobalsSingleton()->IsMapAsmap00b())
	{
		GetMapFixesSingleton()->ApplyFixesForMapAsmap00b();
	}
	else if (GetAsheepGlobalsSingleton()->IsMapAsmap05b())
	{
		GetMapFixesSingleton()->ApplyFixesForMapAsmap05b();
	}
	else if (GetAsheepGlobalsSingleton()->IsMapOutro())
	{
		GetMapFixesSingleton()->ApplyFixesForMapAsmap11c();
	}
	else if (GetAsheepGlobalsSingleton()->IsMapCredits())
	{
		GetMapFixesSingleton()->ApplyFixesForMapCredits();
	}
}

BOOL MapFixes_CheckIfOnlyKateShouldActivateTrigger(CBaseEntity* trigger)
{
	return GetMapFixesSingleton()->CheckIfOnlyKateShouldActivateTrigger(trigger);
}

BOOL MapFixes_CheckIfEntityIsOutroJeep(CBaseEntity* entity)
{
	return GetMapFixesSingleton()->CheckIfEntityIsOutroJeep(entity);
}

BOOL MapFixes_CheckIfShouldTeleportKateToHealingSequence(CBaseEntity* trigger)
{
	return GetMapFixesSingleton()->CheckIfShouldTeleportKateToHealingSequence(trigger);
}

void MapFixes_FixupTriggerForAsmap06KateHealingSequence(CBaseEntity* trigger)
{
	GetMapFixesSingleton()->FixupTriggerForAsmap06KateHealingSequence(trigger);
}

#endif // defined ( ASHEEP_MAPFIXES )