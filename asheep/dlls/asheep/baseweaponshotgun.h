#ifndef BASEWEAPONSHOTGUN_H__
#define BASEWEAPONSHOTGUN_H__

class CBaseWeaponShotgun : public CBasePlayerWeapon, public IPlayerWeapon
{
public:

#ifndef CLIENT_DLL
	int		Save(CSave &save);
	int		Restore(CRestore &restore);
	static	TYPEDESCRIPTION m_SaveData[];
#endif

	void Spawn(void);
	void Precache(void);
	int iItemSlot() { return 3; }
	int AddToPlayer(CBasePlayer *pPlayer);
	BOOL Deploy();
	void Holster(int skiplocal = 0);
	void PrimaryAttack(void);
	void SecondaryAttack(void);
	void Reload(void);
	void WeaponIdle(void);
	int m_fInReload;
	float m_flNextReload;

	virtual BOOL UseDecrement(void)
	{
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

public:

	virtual void PrecacheModels();
	virtual void PrecacheSounds();
	virtual void PrecacheEvents();
	virtual void SetModel();
	virtual void GiveFirstTimeSpawnDefaultAmmo() {}

	virtual CBasePlayerWeaponUtil* GetSharedUtils();

	virtual const int& GetShellIndex() const;
	virtual const unsigned short& GetSingleFireEvent() const;
	virtual const unsigned short& GetDoubleFireEvent() const;

	virtual float GetPrimaryFireRate() { return 0.75; }
	virtual float GetSecondaryFireRate() { return 1.5; }

	virtual float GetNextAttackDelayAfterStartReload() { return 1.0; }
	virtual float GetIdleAnimationDelayAfterPrimaryAttackWithNoEmptyClip() { return 5.0f; }
	virtual float GetIdleAnimationDelayAfterPrimaryAttackWithEmptyClip() { return 0.75f; }
	virtual float GetIdleAnimationDelayAfterSecondaryAttackWithNoEmptyClip() { return 6.0f; }
	virtual float GetIdleAnimationDelayAfterSecondaryAttackWithEmptyClip() { return 1.5f; }

	virtual float GetPumpSoundDelayAfterSingleFire() { return 0.5f; }
	virtual float GetPumpSoundDelayAfterDoubleFire() { return 0.95f; }

	virtual float PlayIdleAnimation() { return 0.0f; }
	virtual float PlayReloadAnimation() { return 0.0f; }
	virtual float PlayStartReloadAnimation() { return 0.0f; }
	virtual float PlayPumpAnimation() { return 0.0f; }

	virtual void PlayReloadSound() {}
	virtual void PlayPumpSound() {}
private:
	int m_iShell;

	unsigned short m_usDoubleFire;
	unsigned short m_usSingleFire;
};

#endif // BASEWEAPONSHOTGUN_H__