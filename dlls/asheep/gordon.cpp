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

class CGordon : public CTalkMonsterWithBarneyAI
{
	typedef CTalkMonsterWithBarneyAI BaseClass;

	virtual BOOL IsWearingHEVSuit();
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
	virtual void GunFireSound();

private:

	static const char* pAttackSounds[];
	static const char* pPainSounds[];
	static const char* pDeathSounds[];
};

LINK_ENTITY_TO_CLASS(monster_gordon, CGordon);

const char* CGordon::pAttackSounds[] =
{
	"gordon/go_attack1.wav",
	"gordon/go_attack2.wav",
};

const char* CGordon::pPainSounds[] =
{
	"gordon/go_pain1.wav",
	"gordon/go_pain2.wav",
	"gordon/go_pain3.wav",
};

const char* CGordon::pDeathSounds[] =
{
	"gordon/go_die1.wav",
	"gordon/go_die2.wav",
	"gordon/go_die3.wav",
};

void CGordon::PrecacheModels()
{
	PRECACHE_MODEL("models/freeman.mdl");
}

void CGordon::PrecacheSounds()
{
	PRECACHE_SOUND_ARRAY(pAttackSounds);
	PRECACHE_SOUND_ARRAY(pPainSounds);
	PRECACHE_SOUND_ARRAY(pDeathSounds);
}

void CGordon::SetModel()
{
	SET_MODEL(ENT(pev), "models/freeman.mdl");
}

int CGordon::GetFirstTimeSpawnHealth() const
{
	return gSkillData.freemanHealth;
}

void CGordon::InitSentenceGroup()
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

void CGordon::SetDefaultVoicePitch()
{
	// get voice for head - just one barney voice for now
	m_voicePitch = 100;
}

void CGordon::SpeakAttack()
{
	PlaySentence("GO_ATTACK", RANDOM_FLOAT(2.8, 3.2), VOL_NORM, ATTN_IDLE);
}

void CGordon::SpeakMadAtYou()
{
	PlaySentence("GO_MAD", 4, VOL_NORM, ATTN_NORM);
}

void CGordon::SpeakIHaveBeenShot()
{
	PlaySentence("GO_SHOT", 4, VOL_NORM, ATTN_NORM);
}

void CGordon::SpeakHeyBeCareful()
{
	PlaySentence("GO_SHOT", 4, VOL_NORM, ATTN_NORM);
}

void CGordon::SpeakAlrightIAmPissed()
{
	PlaySentence("GO_SHOT", 4, VOL_NORM, ATTN_NORM);
}

void CGordon::SpeakKilledOneEnemy()
{
	PlaySentence("GO_KILL", 4, VOL_NORM, ATTN_NORM);
}

void CGordon::SpeakDeclineFollowing()
{
	PlaySentence("GO_POK", 2, VOL_NORM, ATTN_NORM);
}

void CGordon::DoPainSound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, RANDOM_SOUND_ARRAY(pPainSounds), 1, ATTN_NORM, 0, GetVoicePitch());
}

void CGordon::DoDeathSound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, RANDOM_SOUND_ARRAY(pDeathSounds), 1, ATTN_NORM, 0, GetVoicePitch());
}

void CGordon::GunFireSound()
{
	int pitchShift = RANDOM_LONG(0, 20);

	// Only shift about half the time
	if (pitchShift > 10)
		pitchShift = 0;
	else
		pitchShift -= 5;

	EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, "gordon/go_attack2.wav", 1, ATTN_NORM, 0, 100 + pitchShift);
}

BOOL CGordon::IsWearingHEVSuit()
{
	return TRUE;
}

BOOL CGordon::IsWearingHelmet()
{
	return FALSE;
}
