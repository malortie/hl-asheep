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
// headcrab.cpp - tiny, jumpy alien parasite
//=========================================================

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"schedule.h"
#include	"game.h"
#include	"basemonsterwithheadcrabai.h"

class CHeadCrab : public CBaseMonsterWithHeadCrabAI
{
	static const char *pIdleSounds[];
	static const char *pAlertSounds[];
	static const char *pPainSounds[];
	static const char *pAttackSounds[];
	static const char *pDeathSounds[];
	static const char *pBiteSounds[];

public:
	virtual void PainSound(void);
	virtual void DeathSound(void);
	virtual void IdleSound(void);
	virtual void AlertSound(void);
	virtual void AttackSound(void);
	virtual void BiteSound();

	virtual int Classify(void);
	virtual float GetDamageAmount(void);
protected:
	virtual void PrecacheModels();
	virtual void PrecacheSounds();
	virtual void SetModel();
	virtual void SetMonsterCollisionBounds();
	virtual int GetFirstTimeSpawnHealth() const;
	virtual void SetBloodColor();
};
LINK_ENTITY_TO_CLASS(monster_headcrab, CHeadCrab);

const char *CHeadCrab::pIdleSounds[] =
{
	"headcrab/hc_idle1.wav",
	"headcrab/hc_idle2.wav",
	"headcrab/hc_idle3.wav",
};
const char *CHeadCrab::pAlertSounds[] =
{
	"headcrab/hc_alert1.wav",
};
const char *CHeadCrab::pPainSounds[] =
{
	"headcrab/hc_pain1.wav",
	"headcrab/hc_pain2.wav",
	"headcrab/hc_pain3.wav",
};
const char *CHeadCrab::pAttackSounds[] =
{
	"headcrab/hc_attack1.wav",
	"headcrab/hc_attack2.wav",
	"headcrab/hc_attack3.wav",
};

const char *CHeadCrab::pDeathSounds[] =
{
	"headcrab/hc_die1.wav",
	"headcrab/hc_die2.wav",
};

const char *CHeadCrab::pBiteSounds[] =
{
	"headcrab/hc_headbite.wav",
};

//=========================================================
// Classify - indicates this monster's place in the 
// relationship table.
//=========================================================
int	CHeadCrab::Classify(void)
{
	return	CLASS_ALIEN_PREY;
}

void CHeadCrab::PrecacheModels()
{
	PRECACHE_MODEL("models/headcrab.mdl");
}

void CHeadCrab::PrecacheSounds()
{
	PRECACHE_SOUND_ARRAY(pIdleSounds);
	PRECACHE_SOUND_ARRAY(pAlertSounds);
	PRECACHE_SOUND_ARRAY(pPainSounds);
	PRECACHE_SOUND_ARRAY(pAttackSounds);
	PRECACHE_SOUND_ARRAY(pDeathSounds);
	PRECACHE_SOUND_ARRAY(pBiteSounds);
}

void CHeadCrab::SetModel()
{
	SET_MODEL(ENT(pev), "models/headcrab.mdl");
}

void CHeadCrab::SetMonsterCollisionBounds()
{
	UTIL_SetSize(pev, Vector(-12, -12, 0), Vector(12, 12, 24));
}

int CHeadCrab::GetFirstTimeSpawnHealth() const
{
	return gSkillData.headcrabHealth;
}

void CHeadCrab::SetBloodColor()
{
	m_bloodColor = BLOOD_COLOR_GREEN;
}

//=========================================================
// IdleSound
//=========================================================
#define CRAB_ATTN_IDLE (float)1.5
void CHeadCrab::IdleSound(void)
{
	EMIT_SOUND_DYN(edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pIdleSounds), GetSoundVolue(), ATTN_IDLE, 0, GetVoicePitch());
}

//=========================================================
// AlertSound 
//=========================================================
void CHeadCrab::AlertSound(void)
{
	EMIT_SOUND_DYN(edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pAlertSounds), GetSoundVolue(), ATTN_IDLE, 0, GetVoicePitch());
}

//=========================================================
// AlertSound 
//=========================================================
void CHeadCrab::PainSound(void)
{
	EMIT_SOUND_DYN(edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pPainSounds), GetSoundVolue(), ATTN_IDLE, 0, GetVoicePitch());
}

//=========================================================
// DeathSound 
//=========================================================
void CHeadCrab::DeathSound(void)
{
	EMIT_SOUND_DYN(edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pDeathSounds), GetSoundVolue(), ATTN_IDLE, 0, GetVoicePitch());
}

void CHeadCrab::AttackSound(void)
{
	EMIT_SOUND_DYN(edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pAttackSounds), GetSoundVolue(), ATTN_IDLE, 0, GetVoicePitch());
}

void CHeadCrab::BiteSound()
{
	EMIT_SOUND_DYN(edict(), CHAN_WEAPON, RANDOM_SOUND_ARRAY(pBiteSounds), GetSoundVolue(), ATTN_IDLE, 0, GetVoicePitch());
}

float CHeadCrab::GetDamageAmount()
{
	return gSkillData.headcrabDmgBite;
}


class CBabyCrab : public CHeadCrab
{
public:
	void SetYawSpeed(void);
	float GetDamageAmount(void) { return gSkillData.headcrabDmgBite * 0.3; }
	BOOL CheckRangeAttack1(float flDot, float flDist);
	Schedule_t* GetScheduleOfType(int Type);
	virtual int GetVoicePitch(void) { return PITCH_NORM + RANDOM_LONG(40, 50); }
	virtual float GetSoundVolue(void) { return 0.8; }

	CUSTOM_SCHEDULES;

protected:

	virtual void PrecacheModels();
	virtual void SetModel();
	virtual void SetMonsterCollisionBounds();
	virtual void SetSkinAndBodygroups();
	virtual int GetFirstTimeSpawnHealth() const;
};
LINK_ENTITY_TO_CLASS(monster_babycrab, CBabyCrab);

Task_t	tlHCRangeAttack1Fast[] =
{
	{ TASK_STOP_MOVING,			(float)0 },
	{ TASK_FACE_IDEAL,			(float)0 },
	{ TASK_RANGE_ATTACK1,		(float)0 },
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
};

Schedule_t	slHCRangeAttack1Fast[] =
{
	{
		tlHCRangeAttack1Fast,
		ARRAYSIZE(tlHCRangeAttack1Fast),
		bits_COND_ENEMY_OCCLUDED |
		bits_COND_NO_AMMO_LOADED,
	0,
	"HCRAFast"
	},
};

DEFINE_CUSTOM_SCHEDULES(CBabyCrab)
{
	slHCRangeAttack1Fast,
};

IMPLEMENT_CUSTOM_SCHEDULES(CBabyCrab, CHeadCrab);

void CBabyCrab::SetYawSpeed(void)
{
	pev->yaw_speed = 120;
}

BOOL CBabyCrab::CheckRangeAttack1(float flDot, float flDist)
{
	if (pev->flags & FL_ONGROUND)
	{
		if (pev->groundentity && (pev->groundentity->v.flags & (FL_CLIENT | FL_MONSTER)))
			return TRUE;

		// A little less accurate, but jump from closer
		if (flDist <= 180 && flDot >= 0.55)
			return TRUE;
	}

	return FALSE;
}

Schedule_t* CBabyCrab::GetScheduleOfType(int Type)
{
	switch (Type)
	{
	case SCHED_FAIL:	// If you fail, try to jump!
		if (m_hEnemy != NULL)
			return slHCRangeAttack1Fast;
		break;

	case SCHED_RANGE_ATTACK1:
	{
		return slHCRangeAttack1Fast;
	}
	break;
	}

	return CHeadCrab::GetScheduleOfType(Type);
}

void CBabyCrab::PrecacheModels()
{
	PRECACHE_MODEL("models/baby_headcrab.mdl");
}

void CBabyCrab::SetModel()
{
	SET_MODEL(ENT(pev), "models/baby_headcrab.mdl");
}

void CBabyCrab::SetMonsterCollisionBounds()
{
	UTIL_SetSize(pev, Vector(-12, -12, 0), Vector(12, 12, 24));
}

void CBabyCrab::SetSkinAndBodygroups()
{
	pev->rendermode = kRenderTransTexture;
	pev->renderamt = 192;
}

int CBabyCrab::GetFirstTimeSpawnHealth() const
{
	return gSkillData.headcrabHealth * 0.25; // less health than full grown
}
