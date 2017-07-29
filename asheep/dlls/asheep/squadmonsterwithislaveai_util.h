#ifndef SQUADMONSTERWITHISLAVEAI_UTIL_H__
#define SQUADMONSTERWITHISLAVEAI_UTIL_H__

class CSquadMonsterWithISlaveAI;

class AlienSlaveSounds
{
public:
	static const char *GetRandomAttackHitSound();
	static const char *GetRandomAttackMissSound();
	static const char *GetRandomPainSound();
	static const char *GetRandomDeathSound();

	static const char *pAttackHitSounds[];
	static const char *pAttackMissSounds[];
	static const char *pPainSounds[];
	static const char *pDeathSounds[];
};

class CSquadMonsterWithISlaveAIUtil
{
public:
	virtual void PrecacheSharedSounds();

	virtual void PlayPainSound(CSquadMonsterWithISlaveAI* outer);
	virtual void PlayAlertSound(CSquadMonsterWithISlaveAI* outer);
	virtual void PlayIdleSound(CSquadMonsterWithISlaveAI* outer);
	virtual void PlayDeathSound(CSquadMonsterWithISlaveAI* outer);
	virtual void PlayAttackHitSound(CSquadMonsterWithISlaveAI* outer);
	virtual void PlayAttackMissSound(CSquadMonsterWithISlaveAI* outer);
};

#endif // SQUADMONSTERWITHISLAVEAI_UTIL_H__