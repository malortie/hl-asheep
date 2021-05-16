#ifndef BASEWEAPONARWITHGRENADELAUNCHER_H__
#define BASEWEAPONARWITHGRENADELAUNCHER_H__

class CBaseWeaponARWithGrenadeLauncher : public CBasePlayerWeapon, public IPlayerWeapon
{
public:
	virtual void Spawn( void );
	void Precache( void );
	int iItemSlot( void ) { return 3; }
	int AddToPlayer( CBasePlayer *pPlayer );

	void PrimaryAttack( void );
	void SecondaryAttack( void );
	int SecondaryAmmoIndex( void );
	BOOL Deploy( void );
	void Holster( int skiplocal = 0 );
	void Reload( void );
	void WeaponIdle( void );
	float m_flNextAnimTime;
	int m_iShell;

	virtual BOOL UseDecrement( void )
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
	virtual void SetModel();
	virtual void GiveFirstTimeSpawnDefaultAmmo() {}


	virtual CBasePlayerWeaponUtil* GetSharedUtils();

	virtual const unsigned short& GetFireEvent() const = 0;
	virtual const unsigned short& GetGrenadeLaunchEvent() const = 0;

	virtual float GetPrimaryFireRate();
	virtual float GetSecondaryFireRate();
	virtual float GetGrenadeTossSpeed();
	virtual float GetGrenadeDamage();
	virtual int GetBulletType() = 0;

	virtual const char* GetShellBrassModel();

	virtual int GetReloadSequence() = 0;
	virtual float GetReloadSequenceDuration() = 0;

	virtual float PlayIdleAnimation() { return 0.0f; }
};

#endif // BASEWEAPONARWITHGRENADELAUNCHER_H__
