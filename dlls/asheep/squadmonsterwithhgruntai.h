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

#ifndef SQUADMONSTERWITHHGRUNTAI_H__
#define SQUADMONSTERWITHHGRUNTAI_H__

#include "squadmonster.h"

class CSquadMonsterWithHGruntAIUtil;

enum ShellSlot
{
	SHELL_SLOT1 = 0,
	SHELL_SLOT2,
	NUMBER_OF_SHELL_SLOTS,
};

class CSquadMonsterWithHGruntAI : public CSquadMonster
{
	static int m_fGruntQuestion; // true if an idle grunt asked a question. Cleared when someone answers.
public:
	void Spawn( void );
	void Precache( void );
	void SetYawSpeed ( void );
	int  Classify ( void );
	int ISoundMask ( void );
	void HandleAnimEvent( MonsterEvent_t *pEvent );
	BOOL FCanCheckAttacks ( void );
	BOOL CheckMeleeAttack1 ( float flDot, float flDist );
	BOOL CheckRangeAttack1 ( float flDot, float flDist );
	BOOL CheckRangeAttack2 ( float flDot, float flDist );
	void CheckAmmo ( void );
	void SetActivity ( Activity NewActivity );
	void StartTask ( Task_t *pTask );
	void RunTask ( Task_t *pTask );
	void DeathSound(void);
	void PainSound(void);
	void IdleSound(void);
	Vector GetGunPosition( void );
	void Weapon1 ( void );
	void Weapon2 ( void );
	void PrescheduleThink ( void );
	void GibMonster( void );
	void SpeakSentence( void );

	int	Save( CSave &save ); 
	int Restore( CRestore &restore );
	
	CBaseEntity	*Kick( void );
	Schedule_t	*GetSchedule( void );
	Schedule_t  *GetScheduleOfType ( int Type );
	void TraceAttack( entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType);
	virtual int TakeDamage( entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType );

	int IRelationship ( CBaseEntity *pTarget );

	BOOL FOkToSpeak( void );
	void JustSpoke( void );

	CUSTOM_SCHEDULES;
	static TYPEDESCRIPTION m_SaveData[];

	// checking the feasibility of a grenade toss is kind of costly, so we do it every couple of seconds,
	// not every server frame.
	float m_flNextGrenadeCheck;
	float m_flNextPainTime;
	float m_flLastEnemySightTime;

	Vector	m_vecTossVelocity;

	BOOL	m_fThrowGrenade;
	BOOL	m_fStanding;
	BOOL	m_fFirstEncounter;// only put on the handsign show in the squad's first encounter.
	int		m_cClipSize;

	int m_voicePitch;
	float m_voiceAttenuation;
	float m_voiceVolume;

	int		m_iShell[2];

	int		m_iSentence;

	static const char *pGruntSentences[];

	virtual int		GetVoicePitch() const;
	virtual float	GetVoiceAttenuation() const;
	virtual float	GetVoiceVolume() const;

	virtual BOOL	IsWearingHelmet() = 0;
	virtual BOOL	HasWeapons(int weaponFlags) const;
	virtual BOOL	CanDropWeapons() { return TRUE; }

	virtual BOOL	IsUsingSecondaryWeapon() const = 0;
	virtual BOOL	IsUsingHandGrenade() const = 0;
	virtual BOOL	IsUsingGrenadeLauncher() const = 0;

	virtual Vector	GetTossGrenadeOffset() const;

	virtual CSquadMonsterWithHGruntAIUtil* GetSharedUtils() const;

	virtual int		GetKickDamage() const = 0;
	virtual float	GetLaunchedGrenadeDamage() const;

protected:

	virtual void TossGrenade(
		const Vector& tossSourcePosition,
		const Vector& tossVelocity,
		const Vector& forward,
		const Vector& right,
		const Vector& up,
		float delay);

	virtual void LaunchGrenade(
		const Vector& launchPosition,
		const Vector& launchVelocity);

	virtual void ShootEffects();

	virtual void FireWeapon1(
		const Vector& shootOrigin,
		const Vector& shootDirection,
		const Vector& forward,
		const Vector& right,
		const Vector& up) = 0;

	virtual void FireWeapon2(
		const Vector& shootOrigin,
		const Vector& shootDirection,
		const Vector& forward,
		const Vector& right,
		const Vector& up) = 0;
		
	virtual void ChangeWeaponVisibility(BOOL visible) = 0;
	virtual void GiveDefaultWeapons() = 0;

	virtual CBaseEntity* DropWeapon1();
	virtual CBaseEntity* DropWeapon2();
	virtual CBaseEntity* DropLauncherGrenade();

	virtual void PrecacheModels() = 0;
	virtual int PrecacheShellModel(ShellSlot shellSlot);
	virtual void PrecacheSounds();
	virtual void SetModel() = 0;
	virtual void SetSkinAndBodyGroups() = 0;
	virtual int GetFirstTimeSpawnHealth() const = 0;
	virtual int GetWeapon1ClipSize() const = 0;
	virtual int GetWeapon2ClipSize() const = 0;

	virtual void SetDefaultVoicePitch();
	virtual void SetDefaultVoiceAttenuation();
	virtual void SetDefaultVoiceVolume();
	virtual void SpeakQuestion(int questionToAskID) = 0;
	virtual void SpeakAnswer(int questionToAnswerID) = 0;
	virtual void SpeakCaughtEnemy() = 0;
	virtual void SpeakCaughtMonster() = 0;
	virtual void SpeakWatchOutGrenade() = 0;
	virtual void SpeakReadyToThrowGrenade() = 0;
	virtual void SpeakTaunt() = 0;

	virtual void DoDeathSound();
	virtual void DoPainSound();
	virtual void Weapon1FireSound();
	virtual void Weapon2FireSound();
	virtual void LaunchGrenadeSound();
	virtual void ReloadSound();
};

#include "squadmonsterwithhgruntai_util.h"

#endif // SQUADMONSTERWITHHGRUNTAI_H__
