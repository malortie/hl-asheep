#ifndef SQUADMONSTERWITHISLAVEAI_H__
#define SQUADMONSTERWITHISLAVEAI_H__

class CSquadMonsterWithISlaveAIUtil;

#define ISLAVE_MAX_BEAMS 8

class CSquadMonsterWithISlaveAI : public CSquadMonster
{
public:
	void Spawn(void);
	void Precache(void);
	void SetYawSpeed(void);
	int	 ISoundMask(void);
	int  Classify(void);
	int  IRelationship(CBaseEntity *pTarget);
	void HandleAnimEvent(MonsterEvent_t *pEvent);
	BOOL CheckRangeAttack1(float flDot, float flDist);
	BOOL CheckRangeAttack2(float flDot, float flDist);
	void CallForHelp(char *szClassname, float flDist, EHANDLE hEnemy, Vector &vecLocation);
	void TraceAttack(entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType);
	int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType);

	void DeathSound(void);
	void PainSound(void);
	void AlertSound(void);
	void IdleSound(void);

	void Killed(entvars_t *pevAttacker, int iGib);

	void StartTask(Task_t *pTask);
	Schedule_t *GetSchedule(void);
	Schedule_t *GetScheduleOfType(int Type);
	CUSTOM_SCHEDULES;

	int	Save(CSave &save);
	int Restore(CRestore &restore);
	static TYPEDESCRIPTION m_SaveData[];

	void ClearBeams();
	void ArmBeam(int side);
	void WackBeam(int side, CBaseEntity *pEntity);
	void ZapBeam(int side);
	void BeamGlow(void);

	int m_iBravery;

	CBeam *m_pBeam[ISLAVE_MAX_BEAMS];

	int m_iBeams;
	float m_flNextAttack;

	int	m_voicePitch;

	EHANDLE m_hDead;

	virtual int GetVoicePitch();

	virtual CSquadMonsterWithISlaveAIUtil* GetSharedUtils() const;
	
protected:

	virtual void PrecacheModels() = 0;
	virtual void PrecacheSounds();
	virtual void SetModel() = 0;
	virtual void SetSkinAndBodygroups();
	virtual int GetFirstTimeSpawnHealth() const = 0;
	virtual void SetBloodColor();

	virtual void SetVoicePitch() = 0;

	virtual void DoDeathSound();
	virtual void DoPainSound();
	virtual void DoAlertSound();
	virtual void DoIdleSound();
	virtual void AttackHitSound();
	virtual void AttackMissSound();
	virtual void BeamPowerupSound() = 0;
	virtual void BeamZapSound() = 0;
	virtual void BeamElectroSound(const Vector& soundOrigin) = 0;
	virtual void StopBeamSounds() = 0;

	virtual void SpawnDynamicLight(const Vector& spawnPosition) = 0;
	virtual CBeam* CreateArmBeam(int side, const Vector& endPostition) = 0;
	virtual CBeam* CreateZapBeam(int side, const Vector& endPostition) = 0;

	virtual float GetClawDamage() const = 0;
	virtual float GetClawRakeDamage() const = 0;
	virtual float GetZapBeamDamage() const = 0;
};

#include "squadmonsterwithislaveai_util.h"

#endif // SQUADMONSTERWITHISLAVEAI_H__
