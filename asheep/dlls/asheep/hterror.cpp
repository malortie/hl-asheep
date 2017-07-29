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
#include	"plane.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"schedule.h"
#include	"animation.h"
#include	"squadmonster.h"
#include	"weapons.h"
#include	"talkmonster.h"
#include	"soundent.h"
#include	"effects.h"
#include	"customentity.h"
#include	"squadmonsterwithhgruntai.h"

#define	TERROR_WEAPON1_CLIP_SIZE	36 // how many bullets in a clip? - NOTE: 3 round burst sound, so keep as 3 * x!
#define TERROR_WEAPON2_CLIP_SIZE	8

#define TERROR_9MMAR			(1 << 0)
#define TERROR_HANDGRENADE		(1 << 1)
#define TERROR_GRENADELAUNCHER	(1 << 2)
#define TERROR_SHOTGUN			(1 << 3)

enum HTerrorBodygroup
{
	HEAD_GROUP = 1,
	GUN_GROUP = 2,
};

enum HTerrorHead
{
	HEAD_HOOD = 0,
};

enum HTerrorGun
{
	GUN_MP5 = 0,
	GUN_SHOTGUN,
	GUN_NONE,
};

class CHTerror : public CSquadMonsterWithHGruntAI
{
	typedef CSquadMonsterWithHGruntAI BaseClass;
public:
	virtual int GetKickDamage() const;
	virtual BOOL IsWearingHelmet();

	BOOL	CanDropWeapons() { return FALSE; }

	virtual BOOL IsUsingSecondaryWeapon() const;
	virtual BOOL IsUsingHandGrenade() const;
	virtual BOOL IsUsingGrenadeLauncher() const;

protected:

	virtual void FireWeapon1(
		const Vector& shootOrigin,
		const Vector& shootDirection,
		const Vector& forward,
		const Vector& right,
		const Vector& up);

	virtual void FireWeapon2(
		const Vector& shootOrigin,
		const Vector& shootDirection,
		const Vector& forward,
		const Vector& right,
		const Vector& up);
		
	virtual void ChangeWeaponVisibility(BOOL visible);
	virtual void GiveDefaultWeapons();

	virtual void PrecacheModels();
	virtual void PrecacheSounds();
	virtual void SetModel();
	virtual void SetSkinAndBodyGroups();
	virtual int GetFirstTimeSpawnHealth() const;
	virtual int GetWeapon1ClipSize() const;
	virtual int GetWeapon2ClipSize() const;

	virtual void SpeakQuestion(int questionToAskID);
	virtual void SpeakAnswer(int questionToAnswerID);
	virtual void SpeakCaughtEnemy();
	virtual void SpeakCaughtMonster();
	virtual void SpeakWatchOutGrenade();
	virtual void SpeakReadyToThrowGrenade();
	virtual void SpeakTaunt();
};

LINK_ENTITY_TO_CLASS(monster_human_terror, CHTerror);

int CHTerror::GetKickDamage() const
{
	return gSkillData.terrorDmgKick;
}

BOOL CHTerror::IsWearingHelmet()
{
	return FALSE;
}

BOOL CHTerror::IsUsingSecondaryWeapon() const
{
	return HasWeapons(TERROR_SHOTGUN);
}

BOOL CHTerror::IsUsingHandGrenade() const
{
	return HasWeapons(TERROR_HANDGRENADE);
}

BOOL CHTerror::IsUsingGrenadeLauncher() const
{
	return HasWeapons(TERROR_GRENADELAUNCHER);
}

void CHTerror::PrecacheModels()
{
	PRECACHE_MODEL("models/terror.mdl");
}

void CHTerror::PrecacheSounds()
{
	BaseClass::PrecacheSounds();

	GetSharedUtils()->PrecacheHGruntWeaponSounds();
}

void CHTerror::SetModel()
{
	SET_MODEL(ENT(pev), "models/terror.mdl");
}

void CHTerror::SetSkinAndBodyGroups()
{
	pev->skin = 0;
	SetBodygroup(HEAD_GROUP, HEAD_HOOD);
}

int CHTerror::GetFirstTimeSpawnHealth() const
{
	return gSkillData.terrorHealth;
}

int CHTerror::GetWeapon1ClipSize() const
{
	return TERROR_WEAPON1_CLIP_SIZE;
}

int CHTerror::GetWeapon2ClipSize() const
{
	return TERROR_WEAPON2_CLIP_SIZE;
}

void CHTerror::SpeakQuestion(int questionToAskID)
{
	switch (questionToAskID)
	{
	case 0: // check in
		SENTENCEG_PlayRndSz(ENT(pev), "TR_CHECK", GetVoiceVolume(), ATTN_NORM, 0, GetVoicePitch());
		break;
	case 1: // question
		SENTENCEG_PlayRndSz(ENT(pev), "TR_QUEST", GetVoiceVolume(), ATTN_NORM, 0, GetVoicePitch());
		break;
	case 2: // statement
		SENTENCEG_PlayRndSz(ENT(pev), "TR_IDLE", GetVoiceVolume(), ATTN_NORM, 0, GetVoicePitch());
		break;
	}
}

void CHTerror::SpeakAnswer(int questionToAnswerID)
{
	switch (questionToAnswerID)
	{
	case 1: // check in
		SENTENCEG_PlayRndSz(ENT(pev), "TR_CLEAR", GetVoiceVolume(), ATTN_NORM, 0, GetVoicePitch());
		break;
	case 2: // question 
		SENTENCEG_PlayRndSz(ENT(pev), "TR_ANSWER", GetVoiceVolume(), ATTN_NORM, 0, GetVoicePitch());
		break;
	}
}

void CHTerror::SpeakCaughtEnemy()
{
	SENTENCEG_PlayRndSz(ENT(pev), "TR_ALERT", GetVoiceVolume(), GetVoiceAttenuation(), 0, GetVoicePitch());
}

void CHTerror::SpeakCaughtMonster()
{
	SENTENCEG_PlayRndSz(ENT(pev), "TR_MONST", GetVoiceVolume(), GetVoiceAttenuation(), 0, GetVoicePitch());
}

void CHTerror::SpeakWatchOutGrenade()
{
	SENTENCEG_PlayRndSz(ENT(pev), "TR_GREN", GetVoiceVolume(), GetVoiceAttenuation(), 0, GetVoicePitch());
}

void CHTerror::SpeakReadyToThrowGrenade()
{
	SENTENCEG_PlayRndSz(ENT(pev), "TR_THROW", GetVoiceVolume(), GetVoiceAttenuation(), 0, GetVoicePitch());
}

void CHTerror::SpeakTaunt()
{
	SENTENCEG_PlayRndSz(ENT(pev), "TR_TAUNT", GetVoiceVolume(), GetVoiceAttenuation(), 0, GetVoicePitch());
}

void CHTerror::FireWeapon1(const Vector& shootOrigin, const Vector& shootDirection, const Vector& forward, const Vector& right, const Vector& up)
{
	Vector	vecShellVelocity = right * RANDOM_FLOAT(40, 90) + up * RANDOM_FLOAT(75, 200) + forward * RANDOM_FLOAT(-40, 40);
	EjectBrass(shootOrigin - shootDirection * 24, vecShellVelocity, pev->angles.y, m_iShell[SHELL_SLOT1], TE_BOUNCE_SHELL);
	FireBullets(1, shootOrigin, shootDirection, VECTOR_CONE_10DEGREES, 2048, BULLET_MONSTER_MP5); // shoot +-5 degrees
}

void CHTerror::FireWeapon2(const Vector& shootOrigin, const Vector& shootDirection, const Vector& forward, const Vector& right, const Vector& up)
{
	Vector	vecShellVelocity = right * RANDOM_FLOAT(40, 90) + up * RANDOM_FLOAT(75, 200) + forward * RANDOM_FLOAT(-40, 40);
	EjectBrass(shootOrigin - shootDirection * 24, vecShellVelocity, pev->angles.y, m_iShell[SHELL_SLOT2], TE_BOUNCE_SHOTSHELL);
	FireBullets(gSkillData.hgruntShotgunPellets, shootOrigin, shootDirection, VECTOR_CONE_15DEGREES, 2048, BULLET_PLAYER_BUCKSHOT, 0); // shoot +-7.5 degrees
}

void CHTerror::ChangeWeaponVisibility(BOOL visible)
{
	if (!visible)
		SetBodygroup(GUN_GROUP, GUN_NONE);
	else 
	{
		if (IsUsingSecondaryWeapon())
			SetBodygroup(GUN_GROUP, GUN_SHOTGUN);
		else 
			SetBodygroup(GUN_GROUP, GUN_MP5);
	}
}

void CHTerror::GiveDefaultWeapons()
{
	pev->weapons = TERROR_9MMAR;
}
