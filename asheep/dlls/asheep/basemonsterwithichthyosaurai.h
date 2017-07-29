#ifndef BASEMONSTERWITHICHTHYOSAURAI_H__
#define BASEMONSTERWITHICHTHYOSAURAI_H__

class CBaseMonsterWithIchthyosaurAIConstants;
class CBaseMonsterWithIchthyosaurAIUtil;

class CBaseMonsterWithIchthyosaurAI : public CFlyingMonster
{
	typedef CFlyingMonster BaseClass;
public:
	void  Spawn(void);
	void  Precache(void);
	void  SetYawSpeed(void);
	virtual int   Classify(void);
	void  HandleAnimEvent(MonsterEvent_t *pEvent);
	CUSTOM_SCHEDULES;

	int	Save(CSave &save);
	int Restore(CRestore &restore);
	static TYPEDESCRIPTION m_SaveData[];

	Schedule_t *GetSchedule(void);
	Schedule_t *GetScheduleOfType(int Type);

	void Killed(entvars_t *pevAttacker, int iGib);
	void BecomeDead(void);

	void EXPORT CombatUse(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);
	void EXPORT BiteTouch(CBaseEntity *pOther);

	void  StartTask(Task_t *pTask);
	void  RunTask(Task_t *pTask);

	BOOL  CheckMeleeAttack1(float flDot, float flDist);
	BOOL  CheckRangeAttack1(float flDot, float flDist);

	float ChangeYaw(int speed);
	Activity GetStoppedActivity(void);

	void  Move(float flInterval);
	void  MoveExecute(CBaseEntity *pTargetEnt, const Vector &vecDir, float flInterval);
	void  MonsterThink(void);
	void  Stop(void);
	void  Swim(void);
	Vector DoProbe(const Vector &Probe);

	float VectorToPitch(const Vector &vec);
	float FlPitchDiff(void);
	float ChangePitch(int speed);

	Vector m_SaveVelocity;
	float m_idealDist;

	float m_flBlink;

	float m_flEnemyTouched;
	BOOL  m_bOnAttack;

	float m_flMaxSpeed;
	float m_flMinSpeed;
	float m_flMaxDist;

	CBeam *m_pBeam;

	float m_flNextAlert;

	virtual void IdleSound(void) = 0;
	virtual void AlertSound(void) = 0;
	virtual void AttackSound(void) = 0;
	virtual void BiteSound(void) = 0;
	virtual void DeathSound(void) = 0;
	virtual void PainSound(void) = 0;

protected:

	virtual void PrecacheModels();
	virtual void PrecacheSounds();
	virtual void SetModel() = 0;
	virtual void SetMonsterCollisionBounds() = 0;
	virtual void SetDefaultViewOffset() = 0;
	virtual void SetSkinAndBodyGroups();
	virtual void ResetEyeSkinAfterDeath();
	virtual int GetFirstTimeSpawnHealth() const = 0;

	virtual float GetDamageShake() const = 0;

	virtual BOOL CanBlink() const;
	virtual BOOL Blink();
	virtual void SetDeathSequence();

	virtual CBaseMonsterWithIchthyosaurAIConstants* GetSharedConstants() const;

private:
};


#include "basemonsterwithichthyosaurai_constants.h"

#endif // BASEMONSTERWITHICHTHYOSAURAI_H__