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

class CHEVBarney : public CTalkMonsterWithBarneyAI
{
	typedef CTalkMonsterWithBarneyAI BaseClass;
public:

	virtual BOOL IsWearingHEVSuit();

protected:
	virtual void PrecacheModels();
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

LINK_ENTITY_TO_CLASS(monster_hevbarn, CHEVBarney);

void CHEVBarney::PrecacheModels()
{
	PRECACHE_MODEL("models/hev_barney.mdl");
}

void CHEVBarney::SetModel()
{
	SET_MODEL(ENT(pev), "models/hev_barney.mdl");
}

int CHEVBarney::GetFirstTimeSpawnHealth() const
{
	return gSkillData.hevbarneyHealth;
}

void CHEVBarney::InitSentenceGroup()
{
	m_szGrp[TLK_ANSWER] = "BA_ANSWER";
	m_szGrp[TLK_QUESTION] = "BA_QUESTION";
	m_szGrp[TLK_IDLE] = "BA_IDLE";
	m_szGrp[TLK_STARE] = "BA_STARE";
	m_szGrp[TLK_USE] = "BA_OK";
	m_szGrp[TLK_UNUSE] = "BA_WAIT";
	m_szGrp[TLK_STOP] = "BA_STOP";

	m_szGrp[TLK_NOSHOOT] = "BA_SCARED";
	m_szGrp[TLK_HELLO] = "BA_HELLO";

	m_szGrp[TLK_PLHURT1] = "!BA_CUREA";
	m_szGrp[TLK_PLHURT2] = "!BA_CUREB";
	m_szGrp[TLK_PLHURT3] = "!BA_CUREC";

	m_szGrp[TLK_PHELLO] = NULL;	//"BA_PHELLO";		// UNDONE
	m_szGrp[TLK_PIDLE] = NULL;	//"BA_PIDLE";			// UNDONE
	m_szGrp[TLK_PQUESTION] = "BA_PQUEST";		// UNDONE

	m_szGrp[TLK_SMELL] = "BA_SMELL";

	m_szGrp[TLK_WOUND] = "BA_WOUND";
	m_szGrp[TLK_MORTAL] = "BA_MORTAL";
}

void CHEVBarney::SetDefaultVoicePitch()
{
	// get voice for head - just one barney voice for now
	m_voicePitch = 100;
}

void CHEVBarney::SpeakAttack()
{
	PlaySentence("BA_ATTACK", RANDOM_FLOAT(2.8, 3.2), VOL_NORM, ATTN_IDLE);
}

void CHEVBarney::SpeakMadAtYou()
{
	PlaySentence("BA_MAD", 4, VOL_NORM, ATTN_NORM);
}

void CHEVBarney::SpeakIHaveBeenShot()
{
	PlaySentence("BA_SHOT", 4, VOL_NORM, ATTN_NORM);
}

void CHEVBarney::SpeakHeyBeCareful()
{
	PlaySentence("BA_SHOT", 4, VOL_NORM, ATTN_NORM);
}

void CHEVBarney::SpeakAlrightIAmPissed()
{
	PlaySentence("BA_SHOT", 4, VOL_NORM, ATTN_NORM);
}

void CHEVBarney::SpeakKilledOneEnemy()
{
	PlaySentence("BA_KILL", 4, VOL_NORM, ATTN_NORM);
}

void CHEVBarney::SpeakDeclineFollowing()
{
	PlaySentence("BA_POK", 2, VOL_NORM, ATTN_NORM);
}

void CHEVBarney::DoPainSound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, BarneySounds::GetRandomPainSound(), 1, ATTN_NORM, 0, GetVoicePitch());
}

void CHEVBarney::DoDeathSound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, BarneySounds::GetRandomDeathSound(), 1, ATTN_NORM, 0, GetVoicePitch());
}

void CHEVBarney::GunFireSound()
{
	int pitchShift = RANDOM_LONG(0, 20);

	// Only shift about half the time
	if (pitchShift > 10)
		pitchShift = 0;
	else
		pitchShift -= 5;

	EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, "barney/ba_attack2.wav", 1, ATTN_NORM, 0, 100 + pitchShift);
}

BOOL CHEVBarney::IsWearingHEVSuit()
{
	return TRUE;
}
