#ifndef SQUADMONSTERWWITHHGRUNTAI_UTIL_H__
#define SQUADMONSTERWWITHHGRUNTAI_UTIL_H__

class CBaseMonster;

class HumanGruntSounds
{
public:
	static const char *GetRandomHGruntMP5FireSound();
	static const char *GetRandomHGruntMP5GrenadeLaunchSound();
	static const char *GetRandomHGruntMP5ReloadSound();
	static const char *GetRandomHGruntShotgunFireSound();

	static const char *GetRandomPainSound();
	static const char *GetRandomDeathSound();

	static const char* pHGruntMP5FireSounds[];
	static const char* pHGruntMP5GrenadeLaunchSounds[];
	static const char* pHGruntMP5ReloadSounds[];
	static const char* pHGruntShotgunFireSounds[];

	static const char* pPainSounds[];
	static const char* pDeathSounds[];
};

class CSquadMonsterWithHGruntAIUtil
{
public:
	virtual void PrecacheSharedSounds();
	virtual void PrecacheHGruntWeaponSounds();

	virtual void PlayHGruntPainSound(CBaseEntity* outer);
	virtual void PlayHGruntDeathSound(CBaseEntity* outer);
	virtual void PlayHGruntMP5FireSound(CBaseEntity* outer);
	virtual void PlayHGruntShotgunFireSound(CBaseEntity* outer);
	virtual void PlayHGruntLaunchGrenadeSound(CBaseEntity* outer);
	virtual void PlayHGruntReloadSound(CBaseEntity* outer);
	virtual void PlayHGruntKickSound(CBaseEntity* outer);

	virtual void TraceAttackNormal(CSquadMonsterWithHGruntAI* outer, entvars_t *pevAttacker, Vector vecDir, TraceResult *ptr, int bitsDamageType, float& flDamage);

	virtual CBaseEntity* DropMP5(CBaseMonster* outer);
	virtual CBaseEntity* DropShotgun(CBaseMonster* outer);
	virtual CBaseEntity* DropLauncherGrenade(CBaseMonster* outer);
};

#endif // SQUADMONSTERWWITHHGRUNTAI_UTIL_H__
