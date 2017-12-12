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

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "gamerules.h"
#include "monsters.h"
#include "talkmonsterwithbarneyai.h"
#include "combat_tactics.h"

#define		KATE_MELEE_DISTANCE	60
#define		KATE_KICK_FACTOR	15
#define		KATE_PUNCH_FACTOR	15
#define		KATE_KICK_ADDITIONAL_POWER	RANDOM_FLOAT(4, 5)
#define		KATE_PUNCH_ADDITIONAL_POWER	RANDOM_FLOAT(4, 5)


class CKateCombatTactics : public CBaseCombatTactics
{
	typedef CBaseCombatTactics BaseClass;
public:
	CKateCombatTactics(CBaseMonster* outer);

	virtual float GetKickDamage() const;
	virtual float GetPunchDamage() const;

	virtual void PunchHitSound();
	virtual void PunchMissSound();
	virtual void KickHitSound();
	virtual void KickMissSound();

	static const char* pKickHitSounds[];
	static const char* pKickMissSounds[];
	static const char* pPunchHitSounds[];
	static const char* pPunchMissSounds[];
};

const char *CKateCombatTactics::pKickHitSounds[] =
{
	"common/kick.wav",
};

const char *CKateCombatTactics::pKickMissSounds[] =
{
	"zombie/claw_miss1.wav",
	"zombie/claw_miss2.wav",
};

const char *CKateCombatTactics::pPunchHitSounds[] =
{
	"common/punch.wav",
};

const char *CKateCombatTactics::pPunchMissSounds[] =
{
	"zombie/claw_miss1.wav",
	"zombie/claw_miss2.wav",
};

CKateCombatTactics::CKateCombatTactics(CBaseMonster* outer) : BaseClass(outer)
{
}

float CKateCombatTactics::GetKickDamage() const
{
	return gSkillData.kateDmgKick;
}

float CKateCombatTactics::GetPunchDamage() const
{
	return gSkillData.kateDmgPunch;
}

void CKateCombatTactics::KickHitSound()
{
	EMIT_SOUND_DYN(
		ENT(GetOuter()->pev), 
		CHAN_WEAPON, 
		RANDOM_SOUND_ARRAY(pKickHitSounds), 
		1.0,
		ATTN_NORM, 
		0, 
		RANDOM_LONG(95, 105));
}

void CKateCombatTactics::KickMissSound()
{
	EMIT_SOUND_DYN(
		ENT(GetOuter()->pev),
		CHAN_WEAPON,
		RANDOM_SOUND_ARRAY(pKickMissSounds), 
		1.0, 
		ATTN_NORM, 
		0, 
		RANDOM_LONG(95, 105));
}

void CKateCombatTactics::PunchHitSound()
{
	EMIT_SOUND_DYN(
		ENT(GetOuter()->pev),
		CHAN_WEAPON, 
		RANDOM_SOUND_ARRAY(pPunchHitSounds), 
		1.0, 
		ATTN_NORM,
		0, 
		RANDOM_LONG(95, 105));
}

void CKateCombatTactics::PunchMissSound()
{
	EMIT_SOUND_DYN(
		ENT(GetOuter()->pev), 
		CHAN_WEAPON, 
		RANDOM_SOUND_ARRAY(pPunchMissSounds),
		1.0, 
		ATTN_NORM, 
		0, 
		RANDOM_LONG(95, 105));
}

//=========================================================
// Monster's Anim Events Go Here
//=========================================================
#define	KATE_AE_MELEEATTACK ( 6 )

class CKate : public CTalkMonsterWithBarneyAI
{
	typedef CTalkMonsterWithBarneyAI BaseClass;

	int	m_karateStrikeCount;
public:
	virtual void Spawn(void);
	int Classify(void);

	virtual int		Save(CSave &save);
	virtual int		Restore(CRestore &restore);
	static	TYPEDESCRIPTION m_SaveData[];

	void HandleAnimEvent(MonsterEvent_t *pEvent);
	Schedule_t *GetSchedule(void);

	virtual void MeleeAttack();
	virtual BOOL IsPlayingKarateBigHitSequence();

	virtual BOOL CanKick() { return TRUE; }
	virtual BOOL CanPunch() { return TRUE; }

	virtual BOOL IsWearingHelmet();
protected:
	virtual void PrecacheModels();
	virtual void PrecacheSounds();
	virtual void SetModel();
	virtual void SetSkinAndBodyGroups() {}
	virtual int GetFirstTimeSpawnHealth() const;

	virtual void InitSentenceGroup();
	virtual void SetDefaultVoicePitch();
	virtual void SpeakAttack();
	virtual void SpeakMadAtYou();
	virtual void SpeakIHaveBeenShot();
	virtual void SpeakHeyBeCareful();
	virtual void SpeakAlrightIAmPissed();
	virtual void SpeakKilledOneEnemy();
	virtual void SpeakDeclineFollowing();

	virtual void DoDeathSound();
	virtual void DoPainSound();
	virtual void GunFireSound(void);

	virtual void TraceAttack(entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType);

	virtual CBaseCombatTactics* GetCombatTactics();
private:

	static const char* pAttackSounds[];
	static const char* pPainSounds[];
	static const char* pDeathSounds[];
};

LINK_ENTITY_TO_CLASS(monster_kate, CKate);

TYPEDESCRIPTION	CKate::m_SaveData[] =
{
	DEFINE_FIELD(CKate, m_karateStrikeCount, FIELD_INTEGER),
};

IMPLEMENT_SAVERESTORE(CKate, CTalkMonsterWithBarneyAI);

const char* CKate::pAttackSounds[] =
{
	"kate/ka_attack1.wav",
	"kate/ka_attack2.wav",
};

const char* CKate::pPainSounds[] =
{
	"kate/ka_pain1.wav",
	"kate/ka_pain2.wav",
};

const char* CKate::pDeathSounds[] =
{
	"kate/ka_die1.wav",
};

void CKate::Spawn(void)
{
	m_karateStrikeCount = 0;
	BaseClass::Spawn();
}

int CKate::Classify(void)
{
	return CLASS_KATE;
}

//=========================================================
// HandleAnimEvent
//=========================================================
void CKate::HandleAnimEvent(MonsterEvent_t *pEvent)
{
	switch (pEvent->event)
	{
	case KATE_AE_MELEEATTACK:
		if ( m_hEnemy != NULL )
			MeleeAttack();
		break;
	default:
		BaseClass::HandleAnimEvent(pEvent);
	}

}

Schedule_t *CKate::GetSchedule(void)
{
	switch (m_MonsterState)
	{
	case MONSTERSTATE_COMBAT:
		{
			if ((HasConditions(bits_COND_SEE_ENEMY) && !HasConditions(bits_COND_ENEMY_OCCLUDED)) && 
				HasConditions(bits_COND_CAN_MELEE_ATTACK1))
			{
				return GetScheduleOfType(SCHED_MELEE_ATTACK1);
			}
		}
		break;
	default:
		break;
	}

	return BaseClass::GetSchedule();
}

void CKate::MeleeAttack()
{
	if (!IsPlayingKarateBigHitSequence())
	{
		// kate is only kicking once.
		GetCombatTactics()->Kick();
		m_karateStrikeCount = 0;
	}
	else
	{
		if (m_karateStrikeCount++ == 2)
		{
			// Perform a kick on last karate strike.
			GetCombatTactics()->Kick();
			m_karateStrikeCount = 0;
		}
		else
			GetCombatTactics()->Punch();
	}
}

BOOL CKate::IsPlayingKarateBigHitSequence()
{
	return pev->sequence == LookupSequence("karate_bighit");
}

void CKate::PrecacheModels()
{
	PRECACHE_MODEL("models/kate.mdl");
}

void CKate::PrecacheSounds()
{
	PRECACHE_SOUND_ARRAY(pAttackSounds);
	PRECACHE_SOUND_ARRAY(pPainSounds);
	PRECACHE_SOUND_ARRAY(pDeathSounds);

	PRECACHE_SOUND_ARRAY(CKateCombatTactics::pKickHitSounds);
	PRECACHE_SOUND_ARRAY(CKateCombatTactics::pKickMissSounds);

	PRECACHE_SOUND_ARRAY(CKateCombatTactics::pPunchHitSounds);
	PRECACHE_SOUND_ARRAY(CKateCombatTactics::pPunchMissSounds);
}

void CKate::SetModel()
{
	SET_MODEL(ENT(pev), "models/kate.mdl");
}

int CKate::GetFirstTimeSpawnHealth() const
{
	return gSkillData.kateHealth;
}

void CKate::InitSentenceGroup()
{
	m_szGrp[TLK_ANSWER] = "KA_ANSWER";
	m_szGrp[TLK_QUESTION] = "KA_QUESTION";
	m_szGrp[TLK_IDLE] = "KA_IDLE";
	m_szGrp[TLK_STARE] = "KA_STARE";
	m_szGrp[TLK_USE] = "KA_OK";
	m_szGrp[TLK_UNUSE] = "KA_WAIT";
	m_szGrp[TLK_STOP] = "KA_STOP";

	m_szGrp[TLK_NOSHOOT] = "KA_SCARED";
	m_szGrp[TLK_HELLO] = "KA_HELLO";

	m_szGrp[TLK_PLHURT1] = "!KA_CUREA";
	m_szGrp[TLK_PLHURT2] = "!KA_CUREB";
	m_szGrp[TLK_PLHURT3] = "!KA_CUREC";

	m_szGrp[TLK_PHELLO] = NULL;
	m_szGrp[TLK_PIDLE] = NULL;
	m_szGrp[TLK_PQUESTION] = "KA_PQUEST";

	m_szGrp[TLK_SMELL] = "KA_SMELL";

	m_szGrp[TLK_WOUND] = "KA_WOUND";
	m_szGrp[TLK_MORTAL] = "KA_MORTAL";
}

void CKate::SetDefaultVoicePitch()
{
	// get voice for head - just one barney voice for now
	m_voicePitch = 100;
}

void CKate::SpeakAttack()
{
	PlaySentence("KA_ATTACK", RANDOM_FLOAT(2.8, 3.2), VOL_NORM, ATTN_IDLE);
}

void CKate::SpeakMadAtYou()
{
	PlaySentence("KA_MAD", 4, VOL_NORM, ATTN_NORM);
}

void CKate::SpeakIHaveBeenShot()
{
	PlaySentence("KA_SHOT", 4, VOL_NORM, ATTN_NORM);
}

void CKate::SpeakHeyBeCareful()
{
	PlaySentence("KA_SHOT", 4, VOL_NORM, ATTN_NORM);
}

void CKate::SpeakAlrightIAmPissed()
{
	PlaySentence("KA_SHOT", 4, VOL_NORM, ATTN_NORM);
}

void CKate::SpeakKilledOneEnemy()
{
	PlaySentence("KA_KILL", 4, VOL_NORM, ATTN_NORM);
}

void CKate::SpeakDeclineFollowing()
{
	PlaySentence("KA_POK", 2, VOL_NORM, ATTN_NORM);
}

void CKate::DoPainSound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, RANDOM_SOUND_ARRAY(pPainSounds), 1, ATTN_NORM, 0, GetVoicePitch());
}

void CKate::DoDeathSound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, RANDOM_SOUND_ARRAY(pDeathSounds), 1, ATTN_NORM, 0, GetVoicePitch());
}

void CKate::GunFireSound()
{
	int pitchShift = RANDOM_LONG(0, 20);

	// Only shift about half the time
	if (pitchShift > 10)
		pitchShift = 0;
	else
		pitchShift -= 5;

	EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, "kate/ka_attack2.wav", 1, ATTN_NORM, 0, 100 + pitchShift);
}

void CKate::TraceAttack(entvars_t *pevAttacker, float flDamage,
	Vector vecDir, TraceResult *ptr, int bitsDamageType)
{
	GetSharedUtils()->TraceAttackNormal(this, pevAttacker,
		vecDir, ptr, bitsDamageType, flDamage);

	CTalkMonster::TraceAttack(pevAttacker, flDamage, vecDir, ptr, bitsDamageType);
}

BOOL CKate::IsWearingHelmet()
{
	return FALSE;
}

CBaseCombatTactics* CKate::GetCombatTactics()
{
	static CKateCombatTactics combatTacticsSingleton( this );
	return &combatTacticsSingleton;
}