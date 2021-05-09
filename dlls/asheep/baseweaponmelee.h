#ifndef BASEWEAPONMELEE_H__
#define BASEWEAPONMELEE_H__

class CBaseWeaponMelee : public CBasePlayerWeapon, public IPlayerWeapon
{
public:
	void Spawn(void);
	void Precache(void);
	int iItemSlot(void) { return 1; }
	void EXPORT SwingAgain(void);
	void EXPORT Smack(void);
	BOOL Deploy(void);
	void Holster(int skiplocal = 0);
	void PrimaryAttack(void);
	int Swing(int fFirst);
	int m_iSwing;
	TraceResult m_trHit;

	virtual BOOL UseDecrement(void)
	{
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

public:

	virtual float GetMeleeDamage() = 0;
	virtual float GetMeleeRange() { return 32; }
	virtual float GetFireRate() { return 0.5f; }
	virtual int GetBulletType() = 0;

	virtual void PrecacheSounds() {}
	virtual void SetModel();

	virtual CBasePlayerWeaponUtil* GetSharedUtils();

	virtual const unsigned short& GetSwingEvent() const = 0;

	virtual int GetDeploySequence() = 0;
	virtual int GetHolsterSequence() = 0;

	virtual float GetHolsterSequenceDuration() = 0;

	virtual void PlayMissAnimation() {}
	virtual void PlayMissSound() {}

	virtual void PlayHitAnimation() {}
	virtual void PlayHitBodySound() {}
	virtual void PlayHitWorldSound(float fvolbar) {}
};

#endif // BASEWEAPONMELEE_H__
