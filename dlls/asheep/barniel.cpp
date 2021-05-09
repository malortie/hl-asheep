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
#include "talkmonsterwithbarneyai.h"

class CBarniel : public CTalkMonsterWithBarneyAI
{
	typedef CTalkMonsterWithBarneyAI BaseClass;
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
};

LINK_ENTITY_TO_CLASS(monster_barniel, CBarniel);

void CBarniel::PrecacheModels()
{
	PRECACHE_MODEL("models/barniel.mdl");
}

void CBarniel::PrecacheSounds()
{
	PRECACHE_SOUND("barniel/bn_attack1.wav");
	PRECACHE_SOUND("barniel/bn_attack2.wav");

	PRECACHE_SOUND("barniel/bn_pain1.wav");

	PRECACHE_SOUND("barniel/bn_die1.wav");
}

void CBarniel::SetModel()
{
	SET_MODEL(ENT(pev), "models/barniel.mdl");
}

int CBarniel::GetFirstTimeSpawnHealth() const
{
	return gSkillData.barnielHealth;
}

void CBarniel::InitSentenceGroup()
{
	m_szGrp[TLK_ANSWER] = "BN_ANSWER";
	m_szGrp[TLK_QUESTION] = "BN_QUESTION";
	m_szGrp[TLK_IDLE] = "BN_IDLE";
	m_szGrp[TLK_STARE] = "BN_STARE";
	m_szGrp[TLK_USE] = "BN_OK";
	m_szGrp[TLK_UNUSE] = "BN_WAIT";
	m_szGrp[TLK_STOP] = "BN_STOP";

	m_szGrp[TLK_NOSHOOT] = "BN_SCARED";
	m_szGrp[TLK_HELLO] = "BN_HELLO";

	m_szGrp[TLK_PLHURT1] = "!BN_CUREA";
	m_szGrp[TLK_PLHURT2] = "!BN_CUREB";
	m_szGrp[TLK_PLHURT3] = "!BN_CUREC";

	m_szGrp[TLK_PHELLO] = NULL;
	m_szGrp[TLK_PIDLE] = NULL;
	m_szGrp[TLK_PQUESTION] = "BN_PQUEST";

	m_szGrp[TLK_SMELL] = "BN_SMELL";

	m_szGrp[TLK_WOUND] = "BN_WOUND";
	m_szGrp[TLK_MORTAL] = "BN_MORTAL";
}

void CBarniel::SetDefaultVoicePitch()
{
	m_voicePitch = 100;
}

void CBarniel::SpeakAttack()
{
	PlaySentence("BN_ATTACK", RANDOM_FLOAT(2.8, 3.2), VOL_NORM, ATTN_IDLE);
}

void CBarniel::SpeakMadAtYou()
{
	PlaySentence("BN_MAD", 4, VOL_NORM, ATTN_NORM);
}

void CBarniel::SpeakIHaveBeenShot()
{
	PlaySentence("BN_SHOT", 4, VOL_NORM, ATTN_NORM);
}

void CBarniel::SpeakHeyBeCareful()
{
	PlaySentence("BN_SHOT", 4, VOL_NORM, ATTN_NORM);
}

void CBarniel::SpeakAlrightIAmPissed()
{
	PlaySentence("BN_SHOT", 4, VOL_NORM, ATTN_NORM);
}

void CBarniel::SpeakKilledOneEnemy()
{
	PlaySentence("BN_KILL", 4, VOL_NORM, ATTN_NORM);
}

void CBarniel::SpeakDeclineFollowing()
{
	PlaySentence("BN_POK", 2, VOL_NORM, ATTN_NORM);
}

void CBarniel::DoPainSound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "barniel/bn_pain1.wav", 1, ATTN_NORM, 0, GetVoicePitch());
}

void CBarniel::DoDeathSound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "barniel/bn_die1.wav", 1, ATTN_NORM, 0, GetVoicePitch());
}

void CBarniel::GunFireSound()
{
	int pitchShift = RANDOM_LONG(0, 20);

	// Only shift about half the time
	if (pitchShift > 10)
		pitchShift = 0;
	else
		pitchShift -= 5;

	EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, "barniel/bn_attack2.wav", 1, ATTN_NORM, 0, 100 + pitchShift);
}
