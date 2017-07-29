#ifndef BASEMONSTERWITHSNARKAI_H__
#define BASEMONSTERWITHSNARKAI_H__

#include "weapons.h"

class CBaseMonsterWithSnarkAI : public CGrenade
{
public:
	void Spawn(void);
	void Precache(void);
	int  Classify(void);
	void EXPORT SuperBounceTouch(CBaseEntity *pOther);
	void EXPORT HuntThink(void);
	int  BloodColor(void) { return BLOOD_COLOR_YELLOW; }
	void Killed(entvars_t *pevAttacker, int iGib);
	void GibMonster(void);

	virtual int		Save(CSave &save);
	virtual int		Restore(CRestore &restore);

	static	TYPEDESCRIPTION m_SaveData[];

	static float m_flNextBounceSoundTime;

	// CBaseEntity *m_pTarget;
	float m_flDie;
	Vector m_vecTarget;
	float m_flNextHunt;
	float m_flNextHit;
	Vector m_posPrev;
	EHANDLE m_hOwner;
	int  m_iMyClass;
	int m_minVoicePitch;
	int m_maxVoicePitch;

protected:

	virtual float GetBlastDamage() = 0;
	virtual float GetBiteDamage() = 0;
	virtual float GetDetonateDelay();
	virtual float GetVoicePitch();

	virtual void PrecacheModels();
	virtual void PrecacheSounds();
	virtual void SetModel() = 0;
	virtual void SetVoicePitch();
	virtual float GetFirstTimeSpawnHealth() = 0;

	virtual void DeathSound() = 0;
	virtual void BlastSound() = 0;
	virtual void HuntSound() = 0;
	virtual void BiteSound() = 0;
	virtual void GibSound();

	virtual int GetMoveSequence() = 0;
};

#endif // BASEMONSTERWITHSNARKAI_H__