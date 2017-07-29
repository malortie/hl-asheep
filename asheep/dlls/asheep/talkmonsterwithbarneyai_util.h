#ifndef TALKMONSTERWITHBARNEYAI_UTIL_H__
#define TALKMONSTERWITHBARNEYAI_UTIL_H__

class CTalkMonsterWithBarneyAI;

class BarneySounds
{
public:

	static const char* GetRandomAttackSound();
	static const char* GetRandomPainSound();
	static const char* GetRandomDeathSound();

	static const char* pBarneyAttackSounds[];
	static const char* pBarneyPainSounds[];
	static const char* pBarneyDeathSounds[];
};

class CTalkMonsterWithBarneyAIUtil
{
public:
	virtual void PrecacheSharedSounds();

	virtual void TraceAttackNormal(CTalkMonsterWithBarneyAI* outer, entvars_t *pevAttacker, Vector vecDir, TraceResult *ptr, int bitsDamageType, float& flDamage);

	virtual CBaseEntity* DropHandGun(CBaseMonster* outer);

protected:

	virtual BOOL ShouldReduceDamageForDamageType(int bitsDamageType) const;
	void AdjustBodyShotDamage(float& flDamage);
	void AdjustHeadShotDamage(float& flDamage);
};

#endif // TALKMONSTERWITHBARNEYAI_UTIL_H__