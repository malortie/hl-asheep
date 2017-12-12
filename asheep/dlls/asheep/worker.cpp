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
#include	"talkmonster.h"
#include	"schedule.h"
#include	"defaultai.h"
#include	"scripted.h"
#include	"animation.h"
#include	"soundent.h"
#include	"talkmonsterwithscientistai.h"

class CWorker : public CTalkMonsterWithScientistAI
{
protected:
	virtual void PrecacheModels();
	virtual void PrecacheSounds();
	virtual void SetModel();
	virtual int GetFirstTimeSpawnHealth() const;

	virtual void SetFriendsSpeechOrder();
	virtual void InitSentenceGroup();
	virtual void SetDefaultVoicePitch();

	virtual void SpeakDeclineFollowing();
	virtual void SpeakScream();
	virtual void SpeakFearEnemy();
	virtual void SpeakFearPlayer();
};

LINK_ENTITY_TO_CLASS(monster_worker, CWorker);

void CWorker::PrecacheModels()
{
	PRECACHE_MODEL("models/gus.mdl");
}

void CWorker::PrecacheSounds()
{
	PRECACHE_SOUND("scientist/sci_pain1.wav");
	PRECACHE_SOUND("scientist/sci_pain2.wav");
	PRECACHE_SOUND("scientist/sci_pain3.wav");
	PRECACHE_SOUND("scientist/sci_pain4.wav");
	PRECACHE_SOUND("scientist/sci_pain5.wav");
}

void CWorker::SetModel()
{
	SET_MODEL(ENT(pev), "models/gus.mdl");
}

int CWorker::GetFirstTimeSpawnHealth() const
{
	return gSkillData.workerHealth;
}

void CWorker::SetFriendsSpeechOrder()
{
	// worker will try to talk to friends in this order:

	m_szFriends[0] = "monster_worker";
	m_szFriends[1] = "monster_barney";
	m_szFriends[2] = "monster_scientist";
}

void CWorker::InitSentenceGroup()
{
	m_szGrp[TLK_ANSWER] = "WK_ANSWER";
	m_szGrp[TLK_QUESTION] = "WK_QUESTION";
	m_szGrp[TLK_IDLE] = "WK_IDLE";
	m_szGrp[TLK_STARE] = "WK_STARE";
	m_szGrp[TLK_USE] = "WK_OK";
	m_szGrp[TLK_UNUSE] = "WK_WAIT";
	m_szGrp[TLK_STOP] = "WK_STOP";
	m_szGrp[TLK_NOSHOOT] = "WK_SCARED";
	m_szGrp[TLK_HELLO] = "WK_HELLO";

	m_szGrp[TLK_PLHURT1] = "!WK_CUREA";
	m_szGrp[TLK_PLHURT2] = "!WK_CUREB";
	m_szGrp[TLK_PLHURT3] = "!WK_CUREC";

	m_szGrp[TLK_PHELLO] = "SC_PHELLO";
	m_szGrp[TLK_PIDLE] = "SC_PIDLE";
	m_szGrp[TLK_PQUESTION] = "SC_PQUEST";
	m_szGrp[TLK_SMELL] = "WK_SMELL";

	m_szGrp[TLK_WOUND] = "WK_WOUND";
	m_szGrp[TLK_MORTAL] = "WK_MORTAL";
}

void CWorker::SetDefaultVoicePitch()
{
	m_voicePitch = 90;
}

void CWorker::SpeakDeclineFollowing()
{
	PlaySentence("SC_POK", 2, VOL_NORM, ATTN_NORM);
}

void CWorker::SpeakScream()
{
	PlaySentence("WK_SCREAM", RANDOM_FLOAT(3, 6), VOL_NORM, ATTN_NORM);
}

void CWorker::SpeakFearEnemy()
{
	PlaySentence("WK_FEAR", 5, VOL_NORM, ATTN_NORM);
}

void CWorker::SpeakFearPlayer()
{
	PlaySentence("WK_PLFEAR", 5, VOL_NORM, ATTN_NORM);
}