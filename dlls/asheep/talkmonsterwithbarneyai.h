#ifndef TALKMONSTERWITHBARNEYAI_H__
#define TALKMONSTERWITHBARNEYAI_H__

#include "talkmonster.h"

class CTalkMonsterWithBarneyAIUtil;

class CTalkMonsterWithBarneyAI : public CTalkMonster
{
public:
	void Spawn(void);
	void Precache(void);
	void SetYawSpeed(void);
	int  ISoundMask(void);
	void BarneyFirePistol(void);
	void AlertSound(void);
	virtual int  Classify(void);
	void HandleAnimEvent(MonsterEvent_t *pEvent);

	void RunTask(Task_t *pTask);
	void StartTask(Task_t *pTask);
	virtual int	ObjectCaps(void) { return CTalkMonster::ObjectCaps() | FCAP_IMPULSE_USE; }
	int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType);
	BOOL CheckMeleeAttack1(float flDot, float flDist);
	BOOL CheckRangeAttack1(float flDot, float flDist);

	void DeclineFollowing(void);

	// Override these to set behavior
	Schedule_t *GetScheduleOfType(int Type);
	Schedule_t *GetSchedule(void);
	MONSTERSTATE GetIdealState(void);

	void DeathSound(void);
	void PainSound(void);

	void TalkInit(void);

	void TraceAttack(entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType);
	void Killed(entvars_t *pevAttacker, int iGib);

	virtual int		Save(CSave &save);
	virtual int		Restore(CRestore &restore);
	static	TYPEDESCRIPTION m_SaveData[];

	BOOL	m_fGunDrawn;
	float	m_painTime;
	float	m_checkAttackTime;
	BOOL	m_lastAttackCheck;

	// UNDONE: What is this for?  It isn't used?
	float	m_flPlayerDamage;// how much pain has the player inflicted on me?

	CUSTOM_SCHEDULES;

	virtual float GetMeleeRange();
	virtual BOOL CanKick();
	virtual BOOL CanPunch();

	virtual BOOL IsWearingHEVSuit();
	virtual BOOL IsWearingArmor();
	virtual BOOL IsWearingHelmet();

	virtual CTalkMonsterWithBarneyAIUtil* GetSharedUtils() const;

	void CheckAmmo(void);

protected:

	virtual void PrecacheModels() = 0;
	virtual void PrecacheSounds();
	virtual void SetModel() = 0;
	virtual void SetSkinAndBodyGroups() = 0;
	virtual int GetFirstTimeSpawnHealth() const = 0;

	virtual void InitSentenceGroup() = 0;
	virtual void SetDefaultVoicePitch() = 0;
	virtual void SpeakAttack() = 0;
	virtual void SpeakMadAtYou() = 0;
	virtual void SpeakIHaveBeenShot() = 0;
	virtual void SpeakHeyBeCareful() = 0;
	virtual void SpeakAlrightIAmPissed() = 0;
	virtual void SpeakKilledOneEnemy() = 0;
	virtual void SpeakDeclineFollowing() = 0;

	virtual void DoDeathSound() = 0;
	virtual void DoPainSound() = 0;
	virtual void GunFireSound() = 0;
	virtual void ReloadSound();

	virtual CBaseEntity* DropWeapon(void);

	virtual BOOL IsAllowedToReloadAndUseReloadSchedule();
	virtual void Reload();
	virtual void SetWeaponClipSize();

	int m_cClipSize;
};

#include "talkmonsterwithbarneyai_util.h"

#endif // TALKMONSTERWITHBARNEYAI_H__
