#ifndef WEAPON_UTIL_H__
#define WEAPON_UTIL_H__

class CBasePlayerWeaponUtil
{
public:
	virtual void PrecacheCommonPistolSounds();
	virtual void PrecacheCommonMP5Sounds();
	virtual void PrecacheCommonShotgunSounds();
	virtual void PrecacheCommonAmmoPickupSounds();
	virtual void PrecacheCommonFireEmptySounds();
};

#endif // WEAPON_UTIL_H__