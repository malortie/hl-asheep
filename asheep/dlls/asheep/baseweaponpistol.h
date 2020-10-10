#ifndef BASEWEAPONPISTOL_H__
#define BASEWEAPONPISTOL_H__

class CBaseWeaponPistol : public CBasePlayerWeapon, public IPlayerWeapon
{
public:
	virtual void Spawn(void);
	void Precache(void);
	int iItemSlot(void) { return 2; }
	int AddToPlayer(CBasePlayer* pPlayer);

	void PrimaryAttack(void);
	void SecondaryAttack(void);
	void GlockFire(float flSpread, float flCycleTime, BOOL fUseAutoAim);
	BOOL Deploy(void);
	void Holster(int skiplocal = 0);
	void Reload(void);
	void WeaponIdle(void);

	virtual BOOL UseDecrement(void)
	{
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

private:
	int m_iShell;
public:

	virtual void PrecacheModels();
	virtual void PrecacheSounds();
	virtual void SetModel();

	virtual int GetBulletType() = 0;
	virtual void GiveFirstTimeSpawnDefaultAmmo() {}

	virtual CBasePlayerWeaponUtil* GetSharedUtils();

	virtual float GetBulletSpread(BOOL bSecondary) { return bSecondary ? 0.1 : 0.01; }
	virtual float GetPrimaryFireRate() { return 0.3; }
	virtual float GetSecondaryFireRate() { return 0.2; }

	virtual const unsigned short& GetPrimaryFireEvent() const= 0;
	virtual const unsigned short& GetSecondaryFireEvent() const = 0;

	virtual int GetReloadSequence() = 0;
	virtual int GetReloadEmptySequence() = 0;

	virtual float GetReloadSequenceDuration() = 0;
	virtual float GetReloadEmptySequenceDuration() = 0;

	virtual float PlayIdleAnimation() { return 0; }

	virtual float GetNextIdleTimeAfterShoot();
};

#endif // BASEWEAPONPISTOL_H__