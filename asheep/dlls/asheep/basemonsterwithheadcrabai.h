#ifndef BASEMONSTERWITHHEADCRABAI_H__
#define BASEMONSTERWITHHEADCRABAI_H__

class CBaseMonsterWithHeadCrabAIUtil;

class CBaseMonsterWithHeadCrabAI : public CBaseMonster
{
	typedef CBaseMonster BaseClass;
public:
	void Spawn(void);
	void Precache(void);
	void RunTask(Task_t *pTask);
	void StartTask(Task_t *pTask);
	void SetYawSpeed(void);
	void EXPORT LeapTouch(CBaseEntity *pOther);
	Vector Center(void);
	Vector BodyTarget(const Vector &posSrc);
	void PrescheduleThink(void);
	void HandleAnimEvent(MonsterEvent_t *pEvent);
	BOOL CheckRangeAttack1(float flDot, float flDist);
	BOOL CheckRangeAttack2(float flDot, float flDist);
	int TakeDamage(entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType);

	virtual int GetVoicePitch(void) { return 100; }
	virtual float GetSoundVolue(void) { return 1.0; }
	Schedule_t* GetScheduleOfType(int Type);

	CUSTOM_SCHEDULES;

	virtual int  Classify(void) = 0;
	virtual float GetDamageAmount(void) = 0;

	virtual void PainSound(void) = 0;
	virtual void DeathSound(void) = 0;
	virtual void IdleSound(void) = 0;
	virtual void AlertSound(void) = 0;
	virtual void AttackSound(void) = 0;
	virtual void BiteSound() = 0;

protected:

	virtual void PrecacheModels() = 0;
	virtual void PrecacheSounds() = 0;
	virtual void SetModel() = 0;
	virtual void SetMonsterCollisionBounds() = 0;
	virtual void SetSkinAndBodygroups();
	virtual int GetFirstTimeSpawnHealth() const = 0;
	virtual void SetBloodColor() = 0;
};

#endif // BASEMONSTERWITHHEADCRABAI_H__