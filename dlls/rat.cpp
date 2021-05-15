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
// rat - environmental monster
//=========================================================

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"schedule.h"
#if defined ( ASHEEP_DLL )
#include "basemonsterwithheadcrabai.h"

class CRat : public CBaseMonsterWithHeadCrabAI
{
	typedef CBaseMonsterWithHeadCrabAI BaseClass;
public:
	int	Classify() { return CLASS_RAT; }
	int IRelationship(CBaseEntity *pTarget);

	virtual void PainSound(void);
	virtual void DeathSound(void);
	virtual void IdleSound(void);
	virtual void AlertSound(void);
	virtual void AttackSound(void);
	virtual void BiteSound();

	virtual float GetDamageAmount(void);
protected:
	virtual void PrecacheModels();
	virtual void PrecacheSounds();
	virtual void SetModel();
	virtual void SetMonsterCollisionBounds();
	virtual int GetFirstTimeSpawnHealth() const;
	virtual void SetBloodColor();
};
LINK_ENTITY_TO_CLASS(monster_rat, CRat);

int CRat::IRelationship(CBaseEntity *pTarget)
{
	// Ignore apaches since they share the same classification
	// as human grunts, spforces - CLASS_HUMAN_MILITARY.
	if (FClassnameIs(pTarget->pev, "monster_apache"))
		return R_NO;

	return BaseClass::IRelationship(pTarget);
}

void CRat::PrecacheModels()
{
	PRECACHE_MODEL("models/bigrat.mdl");
}

void CRat::PrecacheSounds()
{
	PRECACHE_SOUND("rat/rat_alert.wav");
	PRECACHE_SOUND("rat/rat_attack.wav");
	PRECACHE_SOUND("rat/rat_die.wav");
	PRECACHE_SOUND("rat/rat_headbite.wav");
	PRECACHE_SOUND("rat/rat_idle.wav");
	PRECACHE_SOUND("rat/rat_pain.wav");
}

void CRat::SetModel()
{
	SET_MODEL(ENT(pev), "models/bigrat.mdl");
}

void CRat::SetMonsterCollisionBounds()
{
	UTIL_SetSize(pev, Vector(-1, -1, 0), Vector(1, 1, 1));
}

int CRat::GetFirstTimeSpawnHealth() const
{
	return gSkillData.ratHealth;
}

void CRat::SetBloodColor()
{
	m_bloodColor = BLOOD_COLOR_RED;
}

//=========================================================
// IdleSound
//=========================================================
void CRat::IdleSound(void)
{
	EMIT_SOUND_DYN(edict(), CHAN_VOICE, "rat/rat_idle.wav", GetSoundVolue(), ATTN_IDLE, 0, GetVoicePitch());
}

//=========================================================
// AlertSound 
//=========================================================
void CRat::AlertSound(void)
{
	EMIT_SOUND_DYN(edict(), CHAN_VOICE, "rat/rat_alert.wav", GetSoundVolue(), ATTN_IDLE, 0, GetVoicePitch());
}

//=========================================================
// AlertSound 
//=========================================================
void CRat::PainSound(void)
{
	EMIT_SOUND_DYN(edict(), CHAN_VOICE, "rat/rat_pain.wav", GetSoundVolue(), ATTN_IDLE, 0, GetVoicePitch());
}

//=========================================================
// DeathSound 
//=========================================================
void CRat::DeathSound(void)
{
	EMIT_SOUND_DYN(edict(), CHAN_VOICE, "rat/rat_die.wav", GetSoundVolue(), ATTN_IDLE, 0, GetVoicePitch());
}

void CRat::AttackSound(void)
{
	EMIT_SOUND_DYN(edict(), CHAN_VOICE, "rat/rat_attack.wav", GetSoundVolue(), ATTN_IDLE, 0, GetVoicePitch());
}

void CRat::BiteSound()
{
	EMIT_SOUND_DYN(edict(), CHAN_WEAPON, "rat/rat_headbite.wav", GetSoundVolue(), ATTN_IDLE, 0, GetVoicePitch());
}

float CRat::GetDamageAmount()
{
	return gSkillData.ratDmgBite;
}
#else
//=========================================================
// Monster's Anim Events Go Here
//=========================================================

class CRat : public CBaseMonster
{
public:
	void Spawn( void );
	void Precache( void );
	void SetYawSpeed( void );
	int  Classify ( void );
};
LINK_ENTITY_TO_CLASS( monster_rat, CRat );

//=========================================================
// Classify - indicates this monster's place in the 
// relationship table.
//=========================================================
int	CRat :: Classify ( void )
{
	return	CLASS_INSECT;
}

//=========================================================
// SetYawSpeed - allows each sequence to have a different
// turn rate associated with it.
//=========================================================
void CRat :: SetYawSpeed ( void )
{
	int ys;

	switch ( m_Activity )
	{
	case ACT_IDLE:
	default:
		ys = 45;
		break;
	}

	pev->yaw_speed = ys;
}

//=========================================================
// Spawn
//=========================================================
void CRat :: Spawn()
{
	Precache( );

	SET_MODEL(ENT(pev), "models/bigrat.mdl");
	UTIL_SetSize( pev, Vector( 0, 0, 0 ), Vector( 0, 0, 0 ) );

	pev->solid			= SOLID_SLIDEBOX;
	pev->movetype		= MOVETYPE_STEP;
	m_bloodColor		= BLOOD_COLOR_RED;
	pev->health			= 8;
	pev->view_ofs		= Vector ( 0, 0, 6 );// position of the eyes relative to monster's origin.
	m_flFieldOfView		= 0.5;// indicates the width of this monster's forward view cone ( as a dotproduct result )
	m_MonsterState		= MONSTERSTATE_NONE;

	MonsterInit();
}

//=========================================================
// Precache - precaches all resources this monster needs
//=========================================================
void CRat :: Precache()
{
	PRECACHE_MODEL("models/bigrat.mdl");
}	

//=========================================================
// AI Schedules Specific to this monster
//=========================================================
#endif // defined ( ASHEEP_DLL )
