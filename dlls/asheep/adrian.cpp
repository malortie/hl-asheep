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


class CAdrianCombatTactics : public CBaseCombatTactics
{
	typedef CBaseCombatTactics BaseClass;
public:
	CAdrianCombatTactics(CBaseMonster* outer);

	virtual float GetKickDamage() const;
	virtual float GetPunchDamage() const { return 0.0f; }

	virtual void KickHitSound();
	virtual void KickMissSound();

	static const char* pKickHitSounds[];
	static const char* pKickMissSounds[];
};

const char *CAdrianCombatTactics::pKickHitSounds[] =
{
	"common/kick.wav",
};

const char *CAdrianCombatTactics::pKickMissSounds[] =
{
	"zombie/claw_miss1.wav",
	"zombie/claw_miss2.wav",
};

CAdrianCombatTactics::CAdrianCombatTactics(CBaseMonster* outer) : BaseClass(outer)
{
}

float CAdrianCombatTactics::GetKickDamage() const
{
	return gSkillData.adrianDmgKick;
}

void CAdrianCombatTactics::KickHitSound()
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

void CAdrianCombatTactics::KickMissSound()
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

#define ADRIAN_MELEE_RANGE			60
#define ADRIAN_ALLIES_CLASSNAME		"monster_human_grunt"

//=========================================================
// Monster's Anim Events Go Here
//=========================================================
#define	ADRIAN_AE_MELEEATTACK ( 6 )

class CAdrian : public CTalkMonsterWithBarneyAI
{
	typedef CTalkMonsterWithBarneyAI BaseClass;
public:
	virtual int Classify(void);
	void HandleAnimEvent(MonsterEvent_t *pEvent);
	virtual int IRelationship(CBaseEntity *pTarget);
	virtual int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType);
	void OnRemove();

protected:
	virtual void PrecacheModels();
	virtual void PrecacheSounds();
	virtual void SetModel();
	virtual void SetSkinAndBodyGroups() {}
	virtual int GetFirstTimeSpawnHealth() const;

	virtual BOOL CanKick() { return TRUE; }
	virtual void MeleeAttack();

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
	virtual void GunFireSound();

	virtual void NotifyAlliesOfDeath(CBaseEntity* pAttacker);

	virtual CBaseCombatTactics* GetCombatTactics();

private:

	static const char* pAttackSounds[];
	static const char* pPainSounds[];
	static const char* pDeathSounds[];
};

LINK_ENTITY_TO_CLASS(monster_adrian, CAdrian);

const char* CAdrian::pAttackSounds[] =
{
	"gordon/go_attack1.wav",
	"gordon/go_attack2.wav",
};

const char* CAdrian::pPainSounds[] = 
{
	"gordon/go_pain1.wav",
	"gordon/go_pain2.wav",
	"gordon/go_pain3.wav",
};

const char* CAdrian::pDeathSounds[] =
{
	"gordon/go_die1.wav",
	"gordon/go_die2.wav",
	"gordon/go_die3.wav",
};

int CAdrian::Classify(void)
{
	return CLASS_PLAYER_ALLY;
}

void CAdrian::HandleAnimEvent(MonsterEvent_t *pEvent)
{
	switch (pEvent->event)
	{
	case ADRIAN_AE_MELEEATTACK:
		if ( m_hEnemy != NULL )
			MeleeAttack();
		break;
	default:
		BaseClass::HandleAnimEvent(pEvent);
	}
}

int CAdrian::IRelationship(CBaseEntity *pTarget)
{
	// Adrian should be friend with military.
	if (pTarget->Classify() == CLASS_HUMAN_MILITARY)
		return R_AL;

	return BaseClass::IRelationship(pTarget);
}

int CAdrian::TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType)
{
	int result = BaseClass::TakeDamage(pevInflictor, pevAttacker, flDamage, bitsDamageType);

	if (result > 0)
	{
		if ((m_afMemory & bits_MEMORY_PROVOKED) != 0 && (pevAttacker->flags & FL_CLIENT) != 0)
			UTIL_MakeMonstersWithClassnameEnemyOfPlayer(ADRIAN_ALLIES_CLASSNAME);
	}

	return result;
}

void CAdrian::OnRemove()
{
	UTIL_MakeMonstersWithClassnameEnemyOfPlayer(ADRIAN_ALLIES_CLASSNAME);
}

void CAdrian::PrecacheModels()
{
	PRECACHE_MODEL("models/adrian.mdl");
}

void CAdrian::PrecacheSounds()
{
	PRECACHE_SOUND_ARRAY(pAttackSounds);
	PRECACHE_SOUND_ARRAY(pPainSounds);
	PRECACHE_SOUND_ARRAY(pDeathSounds);

	PRECACHE_SOUND_ARRAY(CAdrianCombatTactics::pKickHitSounds);
	PRECACHE_SOUND_ARRAY(CAdrianCombatTactics::pKickMissSounds);
}

void CAdrian::SetModel()
{
	SET_MODEL(ENT(pev), "models/adrian.mdl");
}

int CAdrian::GetFirstTimeSpawnHealth() const
{
	return gSkillData.adrianHealth;
}

void CAdrian::MeleeAttack()
{
	GetCombatTactics()->Kick();
}

void CAdrian::InitSentenceGroup()
{
	m_szGrp[TLK_ANSWER] = "GO_ANSWER";
	m_szGrp[TLK_QUESTION] = "GO_QUESTION";
	m_szGrp[TLK_IDLE] = "GO_IDLE";
	m_szGrp[TLK_STARE] = "GO_STARE";
	m_szGrp[TLK_USE] = "GO_OK";
	m_szGrp[TLK_UNUSE] = "GO_WAIT";
	m_szGrp[TLK_STOP] = "GO_STOP";

	m_szGrp[TLK_NOSHOOT] = "GO_SCARED";
	m_szGrp[TLK_HELLO] = "GO_HELLO";

	m_szGrp[TLK_PLHURT1] = "!GO_CUREA";
	m_szGrp[TLK_PLHURT2] = "!GO_CUREB";
	m_szGrp[TLK_PLHURT3] = "!GO_CUREC";

	m_szGrp[TLK_PHELLO] = NULL;
	m_szGrp[TLK_PIDLE] = NULL;
	m_szGrp[TLK_PQUESTION] = "GO_PQUEST";

	m_szGrp[TLK_SMELL] = "GO_SMELL";

	m_szGrp[TLK_WOUND] = "GO_WOUND";
	m_szGrp[TLK_MORTAL] = "GO_MORTAL";
}

void CAdrian::SetDefaultVoicePitch()
{
	// get voice for head - just one barney voice for now
	m_voicePitch = 100;
}

void CAdrian::SpeakAttack()
{
	PlaySentence("GO_ATTACK", RANDOM_FLOAT(2.8, 3.2), VOL_NORM, ATTN_IDLE);
}

void CAdrian::SpeakMadAtYou()
{
	PlaySentence("GO_MAD", 4, VOL_NORM, ATTN_NORM);
}

void CAdrian::SpeakIHaveBeenShot()
{
	PlaySentence("GO_SHOT", 4, VOL_NORM, ATTN_NORM);
}

void CAdrian::SpeakHeyBeCareful()
{
	PlaySentence("GO_SHOT", 4, VOL_NORM, ATTN_NORM);
}

void CAdrian::SpeakAlrightIAmPissed()
{
	PlaySentence("GO_SHOT", 4, VOL_NORM, ATTN_NORM);
}

void CAdrian::SpeakKilledOneEnemy()
{
	PlaySentence("GO_KILL", 4, VOL_NORM, ATTN_NORM);
}

void CAdrian::SpeakDeclineFollowing()
{
	PlaySentence("GO_POK", 2, VOL_NORM, ATTN_NORM);
}

void CAdrian::DoPainSound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, RANDOM_SOUND_ARRAY(pPainSounds), 1, ATTN_NORM, 0, GetVoicePitch());
}

void CAdrian::DoDeathSound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, RANDOM_SOUND_ARRAY(pDeathSounds), 1, ATTN_NORM, 0, GetVoicePitch());
}

void CAdrian::GunFireSound()
{
	int pitchShift = RANDOM_LONG(0, 20);

	// Only shift about half the time
	if (pitchShift > 10)
		pitchShift = 0;
	else
		pitchShift -= 5;

	EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, "gordon/go_attack2.wav", 1, ATTN_NORM, 0, 100 + pitchShift);
}

void CAdrian::NotifyAlliesOfDeath(CBaseEntity* pAttacker)
{
	UTIL_MakeMonstersWithClassnameEnemyOfPlayer(ADRIAN_ALLIES_CLASSNAME);
}

CBaseCombatTactics* CAdrian::GetCombatTactics()
{
	static CAdrianCombatTactics combatTacticsSingleton( this );
	return &combatTacticsSingleton;
}
