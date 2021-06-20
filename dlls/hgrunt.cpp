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
// hgrunt
//=========================================================

//=========================================================
// Hit groups!	
//=========================================================
/*

  1 - Head
  2 - Stomach
  3 - Gun

*/


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
#include	"hgrunt.h"

#define	GRUNT_MP5_CLIP_SIZE			36 // how many bullets in a clip? - NOTE: 3 round burst sound, so keep as 3 * x!
#define	GRUNT_SHOTGUN_CLIP_SIZE		8


#define HGRUNT_9MMAR				(1 << 0)
#define HGRUNT_HANDGRENADE			(1 << 1)
#define HGRUNT_GRENADELAUNCHER		(1 << 2)
#define HGRUNT_SHOTGUN				(1 << 3)

#define SF_HGRUNT_PLAYER_FRIENDLY		64	// Spawn as the player ally until the 
											// player kills one of his friends.

class CHGrunt : public CSquadMonsterWithHGruntAI
{
	typedef CSquadMonsterWithHGruntAI BaseClass;
public:
	int IRelationship(CBaseEntity* pTarget);

	int TakeDamage(entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType);

	virtual int GetKickDamage() const;
	
	virtual BOOL IsWearingHelmet();

	virtual BOOL IsUsingSecondaryWeapon() const;
	virtual BOOL IsUsingHandGrenade() const;
	virtual BOOL IsUsingGrenadeLauncher() const;

	BOOL CanDropWeapons() { return GetBodygroup(GUN_GROUP) != GUN_NONE; }

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
	void SpeakCover();
	void SpeakCharge();

	BOOL IsPlayerAlly();
	virtual void BecomeEnemyOfPlayer();
};

LINK_ENTITY_TO_CLASS(monster_human_grunt, CHGrunt);

int CHGrunt::IRelationship(CBaseEntity* pTarget)
{
	if (pTarget->IsPlayer() && IsPlayerAlly())
		return R_AL;

	// Grunts are friend with Adrian.
	if (FClassnameIs(pTarget->pev, "monster_adrian"))
		return R_AL;

	// Enemy of spforce.
	if ( FClassnameIs( pTarget->pev, MONSTER_HUMAN_SPFORCE_CLASSNAME ) )
	{
		return R_NM;
	}

	return BaseClass::IRelationship(pTarget);
}

int CHGrunt::TakeDamage(entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType)
{
	int ret = BaseClass::TakeDamage(pevInflictor, pevAttacker, flDamage, bitsDamageType);
	if (ret > 0)
	{
		// If the player attacked us, treat him as an enemy.
		if (IsPlayerAlly() && !FNullEnt(pevAttacker) && (pevAttacker->flags & FL_CLIENT) != 0)
			UTIL_MakeMonstersWithClassnameEnemyOfPlayer(STRING(pev->classname));
	}

	return ret;
}

int CHGrunt::GetKickDamage() const
{
	return gSkillData.hgruntDmgKick;
}

BOOL CHGrunt::IsWearingHelmet()
{
	return GetBodygroup(HEAD_GROUP) == HEAD_GRUNT;
}

BOOL CHGrunt::IsUsingSecondaryWeapon() const
{
	return HasWeapons(HGRUNT_SHOTGUN);
}

BOOL CHGrunt::IsUsingHandGrenade() const
{
	return HasWeapons(HGRUNT_HANDGRENADE);
}

BOOL CHGrunt::IsUsingGrenadeLauncher() const
{
	return HasWeapons(HGRUNT_GRENADELAUNCHER);
}

void CHGrunt::PrecacheModels()
{
	PRECACHE_MODEL("models/hgrunt.mdl");
}

void CHGrunt::PrecacheSounds()
{
	BaseClass::PrecacheSounds();

	GetSharedUtils()->PrecacheHGruntWeaponSounds();
}

void CHGrunt::SetModel()
{
	SET_MODEL(ENT(pev), "models/hgrunt.mdl");
}

void CHGrunt::SetSkinAndBodyGroups()
{
	if (RANDOM_LONG(0, 99) >= 80) 
		pev->skin = 1;	// dark skin
	else
		pev->skin = 0;	// light skin

	if (IsUsingSecondaryWeapon())
		SetBodygroup(HEAD_GROUP, HEAD_SHOTGUN);
	else if (IsUsingGrenadeLauncher()) 
	{
		SetBodygroup(HEAD_GROUP, HEAD_M203);
		pev->skin = 1; // alway dark skin
	}
}

int CHGrunt::GetFirstTimeSpawnHealth() const
{
	return gSkillData.hgruntHealth;
}

int CHGrunt::GetWeapon1ClipSize() const
{
	return GRUNT_MP5_CLIP_SIZE;
}

int CHGrunt::GetWeapon2ClipSize() const
{
	return GRUNT_SHOTGUN_CLIP_SIZE;
}

void CHGrunt::SpeakQuestion(int questionToAskID)
{
	switch (questionToAskID)
	{
	case 0: // check in
		SENTENCEG_PlayRndSz(ENT(pev), "HG_CHECK", GetVoiceVolume(), ATTN_NORM, 0, GetVoicePitch());
		break;
	case 1: // question
		SENTENCEG_PlayRndSz(ENT(pev), "HG_QUEST", GetVoiceVolume(), ATTN_NORM, 0, GetVoicePitch());
		break;
	case 2: // statement
		SENTENCEG_PlayRndSz(ENT(pev), "HG_IDLE", GetVoiceVolume(), ATTN_NORM, 0, GetVoicePitch());
		break;
	}
}

void CHGrunt::SpeakAnswer(int questionToAnswerID)
{
	switch (questionToAnswerID)
	{
	case 1: // check in
		SENTENCEG_PlayRndSz(ENT(pev), "HG_CLEAR", GetVoiceVolume(), ATTN_NORM, 0, GetVoicePitch());
		break;
	case 2: // question 
		SENTENCEG_PlayRndSz(ENT(pev), "HG_ANSWER", GetVoiceVolume(), ATTN_NORM, 0, GetVoicePitch());
		break;
	}
}

void CHGrunt::SpeakCaughtEnemy()
{
	SENTENCEG_PlayRndSz(ENT(pev), "HG_ALERT", GetVoiceVolume(), GetVoiceAttenuation(), 0, GetVoicePitch());
}

void CHGrunt::SpeakCaughtMonster()
{
	SENTENCEG_PlayRndSz(ENT(pev), "HG_MONST", GetVoiceVolume(), GetVoiceAttenuation(), 0, GetVoicePitch());
}

void CHGrunt::SpeakWatchOutGrenade()
{
	SENTENCEG_PlayRndSz(ENT(pev), "HG_GREN", GetVoiceVolume(), GetVoiceAttenuation(), 0, GetVoicePitch());
}

void CHGrunt::SpeakReadyToThrowGrenade()
{
	SENTENCEG_PlayRndSz(ENT(pev), "HG_THROW", GetVoiceVolume(), GetVoiceAttenuation(), 0, GetVoicePitch());
}

void CHGrunt::SpeakTaunt()
{
	SENTENCEG_PlayRndSz(ENT(pev), "HG_TAUNT", GetVoiceVolume(), GetVoiceAttenuation(), 0, GetVoicePitch());
}

void CHGrunt::SpeakCover()
{
	SENTENCEG_PlayRndSz(ENT(pev), "HG_COVER", GetVoiceVolume(), GetVoiceAttenuation(), 0, GetVoicePitch());
}

void CHGrunt::SpeakCharge()
{
	SENTENCEG_PlayRndSz(ENT(pev), "HG_CHARGE", GetVoiceVolume(), GetVoiceAttenuation(), 0, GetVoicePitch());
}

void CHGrunt::FireWeapon1(const Vector& shootOrigin, const Vector& shootDirection, const Vector& forward, const Vector& right, const Vector& up)
{
	Vector	vecShellVelocity = right * RANDOM_FLOAT(40, 90) + up * RANDOM_FLOAT(75, 200) + forward * RANDOM_FLOAT(-40, 40);
	EjectBrass(shootOrigin - shootDirection * 24, vecShellVelocity, pev->angles.y, m_iShell[SHELL_SLOT1], TE_BOUNCE_SHELL);
	FireBullets(1, shootOrigin, shootDirection, VECTOR_CONE_10DEGREES, 2048, BULLET_MONSTER_MP5); // shoot +-5 degrees
}

void CHGrunt::FireWeapon2(const Vector& shootOrigin, const Vector& shootDirection, const Vector& forward, const Vector& right, const Vector& up)
{
	Vector	vecShellVelocity = right * RANDOM_FLOAT(40, 90) + up * RANDOM_FLOAT(75, 200) + forward * RANDOM_FLOAT(-40, 40);
	EjectBrass(shootOrigin - shootDirection * 24, vecShellVelocity, pev->angles.y, m_iShell[SHELL_SLOT2], TE_BOUNCE_SHOTSHELL);
	FireBullets(gSkillData.hgruntShotgunPellets, shootOrigin, shootDirection, VECTOR_CONE_15DEGREES, 2048, BULLET_PLAYER_BUCKSHOT, 0); // shoot +-7.5 degrees
}

void CHGrunt::ChangeWeaponVisibility(BOOL visible)
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

void CHGrunt::GiveDefaultWeapons()
{
	pev->weapons = HGRUNT_9MMAR | HGRUNT_HANDGRENADE;
}

BOOL CHGrunt::IsPlayerAlly()
{
	return (pev->spawnflags & SF_HGRUNT_PLAYER_FRIENDLY) != 0;
}

void CHGrunt::BecomeEnemyOfPlayer()
{
	pev->spawnflags &= ~SF_HGRUNT_PLAYER_FRIENDLY;
}
