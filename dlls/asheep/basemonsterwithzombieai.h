#ifndef BASEMONSTERWITHZOMBIEAI_H__
#define BASEMONSTERWITHZOMBIEAI_H__

class CBaseMonsterWithZombieAIUtil;

class CBaseMonsterWithZombieAI : public CBaseMonster
{
public:
	void Spawn(void);
	void Precache(void);
	void SetYawSpeed(void);
	int  Classify(void);
	void HandleAnimEvent(MonsterEvent_t *pEvent);
	int IgnoreConditions(void);

	float m_flNextFlinch;

	virtual void PainSound(void);
	virtual void AlertSound(void);
	virtual void IdleSound(void);
	virtual void AttackSound(void);
	virtual void AttackHitSound();
	virtual void AttackMissSound();

	// No range attacks
	BOOL CheckRangeAttack1(float flDot, float flDist) { return FALSE; }
	BOOL CheckRangeAttack2(float flDot, float flDist) { return FALSE; }
	int TakeDamage(entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType);

	CBaseMonsterWithZombieAIUtil* GetSharedUtils() const;

protected:

	virtual void PrecacheModels() = 0;
	virtual void PrecacheSounds();
	virtual void SetModel() = 0;
	virtual void SetSkinAndBodygroups();
	virtual int GetFirstTimeSpawnHealth() const = 0;

	virtual int GetOneSlashAttackDamage() const = 0;
	virtual int GetBothSlashAttackDamage() const = 0;
};

#include "basemonsterwithzombieai_util.h"

#endif // BASEMONSTERWITHZOMBIEAI_H__
