#ifndef BASEWEAPONWITHTHROWABLEMONSTER_H__
#define BASEWEAPONWITHTHROWABLEMONSTER_H__

class CBaseWeaponWithThrowableMonster : public CBasePlayerWeapon, public IPlayerWeapon
{
public:
	virtual void Spawn( void );
	virtual void Precache( void );
	int iItemSlot( void ) { return 5; }
	int GetItemInfo(ItemInfo *p);

	void PrimaryAttack( void );
	void SecondaryAttack( void );
	BOOL Deploy( void );
	void Holster( int skiplocal = 0 );
	void WeaponIdle( void );
	int m_fJustThrown;

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

public:

	virtual void SetModel();
	virtual void GiveFirstTimeSpawnDefaultAmmo() {}

	virtual float GetFireRate() { return 0.3f; }
	virtual float GetNextIdleAfterThrow() { return 1; }
	virtual float GetTossVelocity() { return 200; }

	virtual const unsigned short& GetThrowEvent() const = 0;
	virtual const char* GetClassnameOfMonsterToThrow() = 0;

	virtual float PlayIdleAnimation() { return 0.0f; }

	virtual void PlayDeploySound() {}
	virtual void PlayHuntSound() {}

	virtual CBasePlayerWeaponUtil* GetSharedUtils();
};

#endif // BASEWEAPONWITHTHROWABLEMONSTER_H__