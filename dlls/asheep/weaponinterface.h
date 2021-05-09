#ifndef WEAPONINTERFACE_H__
#define WEAPONINTERFACE_H__

class CBasePlayerWeaponUtil;

class IPlayerWeapon
{
public:
	virtual void PrecacheModels() = 0;
	virtual void PrecacheSounds() = 0;
	virtual void PrecacheEvents() = 0;
	virtual int GetWeaponID() = 0;

	virtual const char* GetThirdpersonModel() = 0;
	virtual const char* GetViewModel() = 0;
	virtual const char* GetWorldModel() = 0;

	virtual int GetDeploySequence() = 0;
	virtual int GetHolsterSequence() = 0;

	virtual float GetHolsterSequenceDuration() = 0;

	virtual CBasePlayerWeaponUtil* GetSharedUtils() = 0;
};

#endif // WEAPONINTERFACE_H__
