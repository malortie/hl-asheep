#ifndef BASEWEAPONGRENADE_H__
#define BASEWEAPONGRENADE_H__

class CBaseWeaponGrenade : public CBasePlayerWeapon, public IPlayerWeapon
{
public:
	void Spawn(void);
	void Precache(void);

	void PrimaryAttack(void);
	BOOL Deploy(void);
	BOOL CanHolster(void);
	void Holster(int skiplocal = 0);
	void WeaponIdle(void);

	virtual BOOL UseDecrement(void)
	{
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

public:

	virtual void PrecacheSounds() {}
	virtual void PrecacheEvents() {}
	virtual void SetModel();
	virtual void GiveFirstTimeSpawnDefaultAmmo() {}

	virtual float GetExplodeDamage();
	virtual float GetDetonateDelay() { return 3.0f; }

	virtual float PlayPinPullAnimation() = 0;
	virtual float PlayThrowAnimation(float tossVelocity) = 0;
	virtual float PlayIdleAnimation() = 0;

	virtual CBasePlayerWeaponUtil* GetSharedUtils();
};

#endif // BASEWEAPONGRENADE_H__