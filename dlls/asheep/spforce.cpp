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
#include	"spforce.h"

#define	SPFORCE_WEAPON1_CLIP_SIZE 36
#define	SPFORCE_WEAPON2_CLIP_SIZE 15

#define SPFORCE_VOL						0.35		// volume of grunt sounds
#define SPFORCE_ATTN					ATTN_NORM	// attenutation of grunt sentences
#define SPFORCE_LIMP_HEALTH				20
#define SPFORCE_DMG_HEADSHOT				( DMG_BULLET | DMG_CLUB )	// damage types that can kill a grunt with a single headshot.
#define SPFORCE_NUM_HEADS				2 // how many grunt heads are there? 
#define SPFORCE_MINIMUM_HEADSHOT_DAMAGE	15 // must do at least this much damage in one shot to head to score a headshot kill
#define	SPFORCE_SENTENCE_VOLUME			(float)0.35 // volume of grunt sentences

#define SPFORCE_9MMM41A				( 1 << 0)
#define SPFORCE_HANDGRENADE			( 1 << 1)
#define SPFORCE_GRENADELAUNCHER		( 1 << 2)
#define	SPFORCE_BERETTA				( 1 << 3)

class CSpForce : public CSquadMonsterWithHGruntAI
{
	typedef CSquadMonsterWithHGruntAI BaseClass;
public:
	virtual int Classify();
	virtual int IRelationship(CBaseEntity *pTarget);

	virtual int GetKickDamage() const;
	float GetLaunchedGrenadeDamage() const;
	virtual BOOL IsWearingHelmet();

	virtual BOOL IsUsingSecondaryWeapon() const;
	virtual BOOL IsUsingHandGrenade() const;
	virtual BOOL IsUsingGrenadeLauncher() const;

	BOOL CanDropWeapons() { return GetBodygroup(GUN_GROUP) != GUN_NONE; }

protected:
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

	virtual void DoDeathSound();
	virtual void DoPainSound();
	virtual void Weapon1FireSound();
	virtual void Weapon2FireSound();
	virtual void LaunchGrenadeSound();
	virtual void ReloadSound();

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

	virtual CBaseEntity* DropWeapon1();
	virtual CBaseEntity* DropWeapon2();
	virtual CBaseEntity* DropLauncherGrenade();

private:
	static const char* pWeapon1FireSounds[];
	static const char* pWeapon2FireSounds[];
	static const char* pLaunchGrenadeSounds[];
	static const char* pReloadSounds[];
};

LINK_ENTITY_TO_CLASS(monster_human_spforce, CSpForce);
const char* CSpForce::pWeapon1FireSounds[] =
{
	"weapons/m41ahks1.wav",
	"weapons/m41ahks2.wav",
};

const char* CSpForce::pWeapon2FireSounds[] =
{
	"weapons/beretta_fire1.wav",
};

const char* CSpForce::pLaunchGrenadeSounds[] =
{
	"weapons/m41aglauncher.wav",
	"weapons/m41aglauncher2.wav",
};

const char* CSpForce::pReloadSounds[] =
{
	"spforce/spf_reload1.wav",
	"spforce/spf_reload2.wav",
};

int	CSpForce::Classify(void)
{
	return	CLASS_SPECIAL_FORCE;
}

int CSpForce::IRelationship(CBaseEntity *pTarget)
{

	return BaseClass::IRelationship(pTarget);
}

int CSpForce::GetKickDamage() const
{
	return gSkillData.spforceDmgKick;
}

float CSpForce::GetLaunchedGrenadeDamage() const
{
	return gSkillData.plrDmg9mmM41AGrenade;
}

BOOL CSpForce::IsWearingHelmet()
{
	return GetBodygroup(HEAD_GROUP) == HEAD_GASMASK;
}

BOOL CSpForce::IsUsingSecondaryWeapon() const
{
	return HasWeapons(SPFORCE_BERETTA);
}

BOOL CSpForce::IsUsingHandGrenade() const
{
	return HasWeapons(SPFORCE_HANDGRENADE);
}

BOOL CSpForce::IsUsingGrenadeLauncher() const
{
	return HasWeapons(SPFORCE_GRENADELAUNCHER);
}

void CSpForce::PrecacheModels()
{
	PRECACHE_MODEL("models/spforce.mdl");
}

void CSpForce::PrecacheSounds()
{
	BaseClass::PrecacheSounds();

	PRECACHE_SOUND_ARRAY(pWeapon1FireSounds);
	PRECACHE_SOUND_ARRAY(pWeapon2FireSounds);
	PRECACHE_SOUND_ARRAY(pLaunchGrenadeSounds);
	PRECACHE_SOUND_ARRAY(pReloadSounds);
}

void CSpForce::SetModel()
{
	SET_MODEL(ENT(pev), "models/spforce.mdl");
}

void CSpForce::SetSkinAndBodyGroups()
{
	pev->skin = 0;

	if (IsUsingSecondaryWeapon())
	{
		SetBodygroup(GUN_GROUP, GUN_BERETTA);
		SetBodygroup(HEAD_GROUP, HEAD_BERETTA);
	}
	else if (IsUsingGrenadeLauncher())
		SetBodygroup(HEAD_GROUP, HEAD_M203);
}

int CSpForce::GetFirstTimeSpawnHealth() const
{
	return gSkillData.spforceHealth;
}

int CSpForce::GetWeapon1ClipSize() const
{
	return SPFORCE_WEAPON1_CLIP_SIZE;
}

int CSpForce::GetWeapon2ClipSize() const
{
	return SPFORCE_WEAPON2_CLIP_SIZE;
}

void CSpForce::SpeakQuestion(int questionToAskID)
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

void CSpForce::SpeakAnswer(int questionToAnswerID)
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

void CSpForce::SpeakCaughtEnemy()
{
	SENTENCEG_PlayRndSz(ENT(pev), "HG_ALERT", GetVoiceVolume(), GetVoiceAttenuation(), 0, GetVoicePitch());
}

void CSpForce::SpeakCaughtMonster()
{
	SENTENCEG_PlayRndSz(ENT(pev), "HG_MONST", GetVoiceVolume(), GetVoiceAttenuation(), 0, GetVoicePitch());
}

void CSpForce::SpeakWatchOutGrenade()
{
	SENTENCEG_PlayRndSz(ENT(pev), "HG_GREN", GetVoiceVolume(), GetVoiceAttenuation(), 0, GetVoicePitch());
}

void CSpForce::SpeakReadyToThrowGrenade()
{
	SENTENCEG_PlayRndSz(ENT(pev), "HG_THROW", GetVoiceVolume(), GetVoiceAttenuation(), 0, GetVoicePitch());
}

void CSpForce::SpeakTaunt()
{
	SENTENCEG_PlayRndSz(ENT(pev), "HG_TAUNT", GetVoiceVolume(), GetVoiceAttenuation(), 0, GetVoicePitch());
}

void CSpForce::SpeakCover()
{
	SENTENCEG_PlayRndSz(ENT(pev), "HG_COVER", GetVoiceVolume(), GetVoiceAttenuation(), 0, GetVoicePitch());
}

void CSpForce::SpeakCharge()
{
	SENTENCEG_PlayRndSz(ENT(pev), "HG_CHARGE", GetVoiceVolume(), GetVoiceAttenuation(), 0, GetVoicePitch());
}

void CSpForce::DoDeathSound(void)
{
	GetSharedUtils()->PlayHGruntDeathSound(this);
}

void CSpForce::DoPainSound(void)
{
	GetSharedUtils()->PlayHGruntPainSound(this);
}

void CSpForce::Weapon1FireSound()
{
	// the first round of the three round burst plays the sound and puts a sound in the world sound list.
	EMIT_SOUND(ENT(pev), CHAN_WEAPON, RANDOM_SOUND_ARRAY(pWeapon1FireSounds), 1, ATTN_NORM);
}

void CSpForce::Weapon2FireSound()
{
	EMIT_SOUND(ENT(pev), CHAN_WEAPON, RANDOM_SOUND_ARRAY(pWeapon2FireSounds), 1, ATTN_NORM);
}

void CSpForce::LaunchGrenadeSound()
{
	EMIT_SOUND(ENT(pev), CHAN_WEAPON, RANDOM_SOUND_ARRAY(pLaunchGrenadeSounds), 0.8, ATTN_NORM);
}

void CSpForce::ReloadSound()
{
	if (IsUsingSecondaryWeapon())
	{
		EMIT_SOUND(ENT(pev), CHAN_WEAPON, pReloadSounds[1], 1, ATTN_NORM);
	}
	else
	{
		EMIT_SOUND(ENT(pev), CHAN_WEAPON, pReloadSounds[0], 1, ATTN_NORM);
	}
}

void CSpForce::FireWeapon1(const Vector& shootOrigin, const Vector& shootDirection, const Vector& forward, const Vector& right, const Vector& up)
{
	Vector	vecShellVelocity = right * RANDOM_FLOAT(40, 90) + up * RANDOM_FLOAT(75, 200) + forward * RANDOM_FLOAT(-40, 40);
	EjectBrass(shootOrigin - shootDirection * 24, vecShellVelocity, pev->angles.y, m_iShell[SHELL_SLOT1], TE_BOUNCE_SHELL);
	FireBullets(1, shootOrigin, shootDirection, VECTOR_CONE_10DEGREES, 2048, BULLET_MONSTER_9MMM41A); // shoot +-5 degrees
}

void CSpForce::FireWeapon2(const Vector& shootOrigin, const Vector& shootDirection, const Vector& forward, const Vector& right, const Vector& up)
{
	Vector	vecShellVelocity = right * RANDOM_FLOAT(40, 90) + up * RANDOM_FLOAT(75, 200) + forward * RANDOM_FLOAT(-40, 40);
	EjectBrass(shootOrigin - shootDirection * 24, vecShellVelocity, pev->angles.y, m_iShell[SHELL_SLOT2], TE_BOUNCE_SHOTSHELL);
	FireBullets(1, shootOrigin, shootDirection, VECTOR_CONE_15DEGREES, 2048, BULLET_PLAYER_BERETTA, 0); // shoot +-7.5 degrees
}

void CSpForce::ChangeWeaponVisibility(BOOL visible)
{
	if (!visible)
		SetBodygroup(GUN_GROUP, GUN_NONE);
	else 
	{
		if (IsUsingSecondaryWeapon())
			SetBodygroup(GUN_GROUP, GUN_BERETTA);
		else 
			SetBodygroup(GUN_GROUP, GUN_9MMM41A);
	}
}

void CSpForce::GiveDefaultWeapons()
{
	pev->weapons = SPFORCE_9MMM41A | SPFORCE_HANDGRENADE;
}

CBaseEntity* CSpForce::DropWeapon1()
{
	Vector	vecGunPos;
	Vector	vecGunAngles;

	GetAttachment(0, vecGunPos, vecGunAngles);

	CBaseEntity* pGun = DropItem("weapon_9mmm41a", vecGunPos, vecGunAngles);
	return pGun;
}

CBaseEntity* CSpForce::DropWeapon2()
{
	Vector	vecGunPos;
	Vector	vecGunAngles;

	GetAttachment(0, vecGunPos, vecGunAngles);

	CBaseEntity* pGun = DropItem("weapon_beretta", vecGunPos, vecGunAngles);
	return pGun;
}

CBaseEntity* CSpForce::DropLauncherGrenade()
{
	return GetSharedUtils()->DropLauncherGrenade(this);
}
