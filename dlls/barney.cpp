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

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"talkmonster.h"
#include	"schedule.h"
#include	"defaultai.h"
#include	"scripted.h"
#include	"weapons.h"
#include	"soundent.h"
#include "talkmonsterwithbarneyai.h"

#define SF_BARNEY_LAZY_BARNEY	32
#define SF_BARNEY_CARDS_BARNEY	64

#define BARNEY_CARDS_GROUP		1
#define BARNEY_CARDS_VISIBLE	1

class CBarney : public CTalkMonsterWithBarneyAI
{
	typedef CTalkMonsterWithBarneyAI BaseClass;

public:
	void Spawn(void);
protected:
	virtual void PrecacheModels();
	virtual void SetModel();
	virtual void SetSkinAndBodyGroups();
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

	virtual const char* GetDefaultBarneyModel() { return "models/barney.mdl"; }
	virtual const char* GetCardsBarneyModel() { return "models/cardsbarney.mdl"; }
	virtual const char* GetLazyBarneyModel() { return "models/lazybarney.mdl"; }

	BOOL IsLazyBarney() { return (pev->spawnflags & SF_BARNEY_LAZY_BARNEY) != 0; }
	BOOL IsCardsBarney() { return (pev->spawnflags & SF_BARNEY_CARDS_BARNEY) != 0; }
};

LINK_ENTITY_TO_CLASS(monster_barney, CBarney);

void CBarney::Spawn(void)
{
	BaseClass::Spawn();

	// Since this type of monster uses a different model,
	// remove all defense mechanism since this model lacks
	// firing+melee sequences.
	if (IsLazyBarney() || IsCardsBarney())
	{
		m_afCapability &= ~(bits_CAP_MELEE_ATTACK1 | bits_CAP_MELEE_ATTACK2 | 
			bits_CAP_RANGE_ATTACK1 | bits_CAP_RANGE_ATTACK2);
	}
}

void CBarney::PrecacheModels()
{
	PRECACHE_MODEL((char*)GetDefaultBarneyModel());
	PRECACHE_MODEL((char*)GetCardsBarneyModel());
	PRECACHE_MODEL((char*)GetLazyBarneyModel());
}

void CBarney::SetModel()
{
	if(IsCardsBarney())
		SET_MODEL(ENT(pev), GetCardsBarneyModel());
	else if (IsLazyBarney())
		SET_MODEL(ENT(pev), GetLazyBarneyModel());
	else
		SET_MODEL(ENT(pev), GetDefaultBarneyModel());
}

void CBarney::SetSkinAndBodyGroups()
{
	if (IsCardsBarney()) 
		SetBodygroup(BARNEY_CARDS_GROUP, BARNEY_CARDS_VISIBLE);
}

int CBarney::GetFirstTimeSpawnHealth() const
{
	return gSkillData.barneyHealth;
}

void CBarney::InitSentenceGroup()
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

void CBarney::SetDefaultVoicePitch()
{
	// get voice for head - just one barney voice for now
	m_voicePitch = 100;
}

void CBarney::SpeakAttack()
{
	PlaySentence("BA_ATTACK", RANDOM_FLOAT(2.8, 3.2), VOL_NORM, ATTN_IDLE);
}

void CBarney::SpeakMadAtYou()
{
	PlaySentence("BA_MAD", 4, VOL_NORM, ATTN_NORM);
}

void CBarney::SpeakIHaveBeenShot()
{
	PlaySentence("BA_SHOT", 4, VOL_NORM, ATTN_NORM);
}

void CBarney::SpeakHeyBeCareful()
{
	PlaySentence("BA_SHOT", 4, VOL_NORM, ATTN_NORM);
}

void CBarney::SpeakAlrightIAmPissed()
{
	PlaySentence("BA_SHOT", 4, VOL_NORM, ATTN_NORM);
}

void CBarney::SpeakKilledOneEnemy()
{
	PlaySentence("BA_KILL", 4, VOL_NORM, ATTN_NORM);
}

void CBarney::SpeakDeclineFollowing()
{
	PlaySentence("BA_POK", 2, VOL_NORM, ATTN_NORM);
}

void CBarney::DoPainSound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, BarneySounds::GetRandomPainSound(), 1, ATTN_NORM, 0, GetVoicePitch());
}

void CBarney::DoDeathSound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, BarneySounds::GetRandomDeathSound(), 1, ATTN_NORM, 0, GetVoicePitch());
}

void CBarney::GunFireSound()
{
	int pitchShift = RANDOM_LONG(0, 20);

	// Only shift about half the time
	if (pitchShift > 10)
		pitchShift = 0;
	else
		pitchShift -= 5;

	EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, "barney/ba_attack2.wav", 1, ATTN_NORM, 0, 100 + pitchShift);
}

