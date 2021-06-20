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
#if !defined( OEM_BUILD ) && !defined( HLDEMO_BUILD )

//=========================================================
// icthyosaur - evin, satan fish monster
//=========================================================

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

#define EYE_MAD		0
#define EYE_BASE	1
#define EYE_CLOSED	2
#define EYE_BACK	3
#define EYE_LOOK	4

class CIchthyosaur : public CBaseMonsterWithIchthyosaurAI
{
	static const char *pIdleSounds[];
	static const char *pAlertSounds[];
	static const char *pAttackSounds[];
	static const char *pBiteSounds[];
	static const char *pDieSounds[];
	static const char *pPainSounds[];

public:
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
	virtual void ResetEyeSkinAfterDeath();
	virtual int GetFirstTimeSpawnHealth() const;

	virtual float GetDamageShake() const;

	virtual BOOL CanBlink() const;
	virtual BOOL Blink();
	virtual void SetDeathSequence();
};

LINK_ENTITY_TO_CLASS(monster_ichthyosaur, CIchthyosaur);

const char *CIchthyosaur::pIdleSounds[] =
{
	"ichy/ichy_idle1.wav",
	"ichy/ichy_idle2.wav",
	"ichy/ichy_idle3.wav",
	"ichy/ichy_idle4.wav",
};

const char *CIchthyosaur::pAlertSounds[] =
{
	"ichy/ichy_alert2.wav",
	"ichy/ichy_alert3.wav",
};

const char *CIchthyosaur::pAttackSounds[] =
{
	"ichy/ichy_attack1.wav",
	"ichy/ichy_attack2.wav",
};

const char *CIchthyosaur::pBiteSounds[] =
{
	"ichy/ichy_bite1.wav",
	"ichy/ichy_bite2.wav",
};

const char *CIchthyosaur::pPainSounds[] =
{
	"ichy/ichy_pain2.wav",
	"ichy/ichy_pain3.wav",
	"ichy/ichy_pain5.wav",
};

const char *CIchthyosaur::pDieSounds[] =
{
	"ichy/ichy_die2.wav",
	"ichy/ichy_die4.wav",
};

#define EMIT_ICKY_SOUND( chan, array ) \
	EMIT_SOUND_DYN ( ENT(pev), chan , RANDOM_SOUND_ARRAY(array), 1.0, 0.6, 0, RANDOM_LONG(95,105) ); 


void CIchthyosaur::IdleSound(void)
{
	EMIT_ICKY_SOUND(CHAN_VOICE, pIdleSounds);
}

void CIchthyosaur::AlertSound(void)
{
	EMIT_ICKY_SOUND(CHAN_VOICE, pAlertSounds);
}

void CIchthyosaur::AttackSound(void)
{
	EMIT_ICKY_SOUND(CHAN_VOICE, pAttackSounds);
}

void CIchthyosaur::BiteSound(void)
{
	EMIT_ICKY_SOUND(CHAN_WEAPON, pBiteSounds);
}

void CIchthyosaur::DeathSound(void)
{
	EMIT_ICKY_SOUND(CHAN_VOICE, pDieSounds);
}

void CIchthyosaur::PainSound(void)
{
	EMIT_ICKY_SOUND(CHAN_VOICE, pPainSounds);
}

void CIchthyosaur::PrecacheModels()
{
	PRECACHE_MODEL("models/icky.mdl");
}

void CIchthyosaur::PrecacheSounds()
{
	PRECACHE_SOUND_ARRAY(pIdleSounds);
	PRECACHE_SOUND_ARRAY(pAlertSounds);
	PRECACHE_SOUND_ARRAY(pAttackSounds);
	PRECACHE_SOUND_ARRAY(pBiteSounds);
	PRECACHE_SOUND_ARRAY(pDieSounds);
	PRECACHE_SOUND_ARRAY(pPainSounds);
}

void CIchthyosaur::SetModel()
{
	SET_MODEL(ENT(pev), "models/icky.mdl");
}

void CIchthyosaur::SetMonsterCollisionBounds()
{
	UTIL_SetSize(pev, Vector(-32, -32, -32), Vector(32, 32, 32));
}

void CIchthyosaur::SetDefaultViewOffset()
{
	pev->view_ofs = Vector(0, 0, 16);
}

void CIchthyosaur::ResetEyeSkinAfterDeath()
{
	pev->skin = EYE_BASE;
}

int CIchthyosaur::GetFirstTimeSpawnHealth() const
{
	return gSkillData.ichthyosaurHealth;
}

float CIchthyosaur::GetDamageShake() const
{
	return gSkillData.ichthyosaurDmgShake;
}

BOOL CIchthyosaur::CanBlink() const
{
	return TRUE;
}

BOOL CIchthyosaur::Blink()
{
	pev->skin = EYE_CLOSED;

	if (m_flBlink + 0.2 < gpGlobals->time)
	{
		if (m_bOnAttack)
			pev->skin = EYE_MAD;
		else
			pev->skin = EYE_BASE;

		return TRUE;
	}

	return FALSE;
}

void CIchthyosaur::SetDeathSequence()
{
	SetSequenceByName("bellyup");
}

#endif
