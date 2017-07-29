#ifndef BASEMONSTERWITHZOMBIEAI_UTIL_H__
#define BASEMONSTERWITHZOMBIEAI_UTIL_H__

class CBaseMonsterWithZombieAI;

class ZombieSounds
{
public:

	static const char *GetRandomAttackHitSound();
	static const char *GetRandomAttackMissSound();
	static const char *GetRandomAttackSound();
	static const char *GetRandomIdleSound();
	static const char *GetRandomAlertSound();
	static const char *GetRandomPainSound();

	static const char *pAttackHitSounds[];
	static const char *pAttackMissSounds[];
	static const char *pAttackSounds[];
	static const char *pIdleSounds[];
	static const char *pAlertSounds[];
	static const char *pPainSounds[];
};

class CBaseMonsterWithZombieAIUtil
{
public:
	virtual void PrecacheSharedSounds();

	virtual void PlayPainSound(CBaseMonsterWithZombieAI* outer);
	virtual void PlayAlertSound(CBaseMonsterWithZombieAI* outer);
	virtual void PlayIdleSound(CBaseMonsterWithZombieAI* outer);
	virtual void PlayAttackSound(CBaseMonsterWithZombieAI* outer);
	virtual void PlayAttackHitSound(CBaseMonsterWithZombieAI* outer);
	virtual void PlayAttackMissSound(CBaseMonsterWithZombieAI* outer);
};

#endif // BASEMONSTERWITHZOMBIEAI_UTIL_H__