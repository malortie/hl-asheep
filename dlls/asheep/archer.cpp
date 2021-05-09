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
#include    "flyingmonster.h"
#include	"nodes.h"
#include	"soundent.h"
#include	"animation.h"
#include	"effects.h"
#include	"weapons.h"
#include	"basemonsterwithichthyosaurai.h"

#define ARCHER_PROBE_LENGTH			75.0f

#define ARCHER_SWIM_SPEED_IDLE		80.0f
#define ARCHER_SWIM_SPEED_ALERT		150.0f
#define ARCHER_SWIM_SPEED_MAX		200.0f

class CArcherConstants : public CBaseMonsterWithIchthyosaurAIConstants
{
public:
	virtual float GetFirstTimeSpawnDefaultSpeed();
	virtual float GetFirstTimeSpawnMinimalSpeed();
	virtual float GetFirstTimeSpawnMaximalSpeed();

	virtual float GetSpeedAfterStopping();
	virtual float GetSpeedAfterEnteringIdleState();
	virtual float GetSpeedAfterEnteringAlertState();
	virtual float GetMaximalSpeedAfterEnteringCombatState();

	virtual float GetProbeLength();
};

float CArcherConstants::GetFirstTimeSpawnDefaultSpeed()
{
	return ARCHER_SWIM_SPEED_ALERT;
}

float CArcherConstants::GetFirstTimeSpawnMinimalSpeed()
{
	return ARCHER_SWIM_SPEED_IDLE;
}

float CArcherConstants::GetFirstTimeSpawnMaximalSpeed()
{
	return 175;
}

float CArcherConstants::GetSpeedAfterStopping()
{
	return ARCHER_SWIM_SPEED_IDLE;
}

float CArcherConstants::GetSpeedAfterEnteringIdleState()
{
	return ARCHER_SWIM_SPEED_IDLE;
}

float CArcherConstants::GetSpeedAfterEnteringAlertState()
{
	return ARCHER_SWIM_SPEED_ALERT;
}

float CArcherConstants::GetMaximalSpeedAfterEnteringCombatState()
{
	return ARCHER_SWIM_SPEED_MAX;
}

float CArcherConstants::GetProbeLength()
{
	return ARCHER_PROBE_LENGTH;
}

class CArcher : public CBaseMonsterWithIchthyosaurAI
{
	static const char *pIdleSounds[];
	static const char *pAlertSounds[];
	static const char *pAttackSounds[];
	static const char *pBiteSounds[];
	static const char *pDieSounds[];
	static const char *pPainSounds[];

public:
	int Classify(void);

	virtual void IdleSound(void);
	virtual void AlertSound(void);
	virtual void AttackSound(void);
	virtual void BiteSound(void);
	virtual void DeathSound(void);
	virtual void PainSound(void);

protected:
	virtual void PrecacheModels();
	virtual void PrecacheSounds();
	virtual void SetModel();
	virtual void SetMonsterCollisionBounds();
	virtual void SetDefaultViewOffset();
	virtual int GetFirstTimeSpawnHealth() const;

	virtual float GetDamageShake() const;
	virtual void SetDeathSequence();

	CBaseMonsterWithIchthyosaurAIConstants* GetSharedConstants() const;
};

LINK_ENTITY_TO_CLASS(monster_archer, CArcher);

const char *CArcher::pIdleSounds[] =
{
	"archer/arch_idle1.wav",
	"archer/arch_idle2.wav",
	"archer/arch_idle3.wav",
};

const char *CArcher::pAlertSounds[] =
{
	"archer/arch_alert1.wav",
	"archer/arch_alert2.wav",
};

const char *CArcher::pAttackSounds[] =
{
	"archer/arch_attack1.wav",
	"archer/arch_attack2.wav",
};

const char *CArcher::pBiteSounds[] =
{
	"archer/arch_bite1.wav",
	"archer/arch_bite2.wav",
};

const char *CArcher::pPainSounds[] =
{
	"archer/arch_pain1.wav",
	"archer/arch_pain2.wav",
	"archer/arch_pain3.wav",
	"archer/arch_pain4.wav",
};

const char *CArcher::pDieSounds[] =
{
	"archer/arch_die1.wav",
	"archer/arch_die2.wav",
	"archer/arch_die3.wav",
};

int CArcher::Classify(void)
{
	return CLASS_ARCHER;
}

#define EMIT_ARCHER_SOUND( chan, array ) \
	EMIT_SOUND_DYN ( ENT(pev), chan , RANDOM_SOUND_ARRAY(array), 1.0, 0.6, 0, RANDOM_LONG(95,105) ); 


void CArcher::IdleSound(void)
{
	EMIT_ARCHER_SOUND(CHAN_VOICE, pIdleSounds);
}

void CArcher::AlertSound(void)
{
	EMIT_ARCHER_SOUND(CHAN_VOICE, pAlertSounds);
}

void CArcher::AttackSound(void)
{
	EMIT_ARCHER_SOUND(CHAN_VOICE, pAttackSounds);
}

void CArcher::BiteSound(void)
{
	EMIT_ARCHER_SOUND(CHAN_WEAPON, pBiteSounds);
}

void CArcher::DeathSound(void)
{
	EMIT_ARCHER_SOUND(CHAN_VOICE, pDieSounds);
}

void CArcher::PainSound(void)
{
	EMIT_ARCHER_SOUND(CHAN_VOICE, pPainSounds);
}


void CArcher::PrecacheModels()
{
	PRECACHE_MODEL("models/archer.mdl");
}

void CArcher::PrecacheSounds()
{
	PRECACHE_SOUND_ARRAY(pIdleSounds);
	PRECACHE_SOUND_ARRAY(pAlertSounds);
	PRECACHE_SOUND_ARRAY(pAttackSounds);
	PRECACHE_SOUND_ARRAY(pBiteSounds);
	PRECACHE_SOUND_ARRAY(pDieSounds);
	PRECACHE_SOUND_ARRAY(pPainSounds);
}

void CArcher::SetModel()
{
	SET_MODEL(ENT(pev), "models/archer.mdl");
}

void CArcher::SetMonsterCollisionBounds()
{
	UTIL_SetSize(pev, Vector(-16, -16, -16), Vector(16, 16, 16));
}

void CArcher::SetDefaultViewOffset()
{
	pev->view_ofs = Vector(0, 0, 4);
}

int CArcher::GetFirstTimeSpawnHealth() const
{
	return gSkillData.archerHealth;
}

float CArcher::GetDamageShake() const
{
	return gSkillData.archerDmgShake;
}

void CArcher::SetDeathSequence()
{
	SetSequenceByName("dead_float");
}

CBaseMonsterWithIchthyosaurAIConstants* CArcher::GetSharedConstants() const
{
	static CArcherConstants constantsSingleton;
	return &constantsSingleton;
}
