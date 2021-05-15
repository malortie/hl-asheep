/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "gamerules.h"

// special deathmatch shotgun spreads
#define VECTOR_CONE_DM_SHOTGUN	Vector( 0.08716, 0.04362, 0.00  )// 10 degrees by 5 degrees
#define VECTOR_CONE_DM_DOUBLESHOTGUN Vector( 0.17365, 0.04362, 0.00 ) // 20 degrees by 5 degrees

void CBaseWeaponShotgun::Spawn()
{
	Precache();
	m_iId = GetWeaponID();
	SetModel();

	GiveFirstTimeSpawnDefaultAmmo();
	m_iDefaultAmmo = SHOTGUN_DEFAULT_GIVE;

	FallInit();// get ready to fall
}


void CBaseWeaponShotgun::Precache(void)
{
	PrecacheModels();
	PrecacheSounds();
	PrecacheEvents();
}

int CBaseWeaponShotgun::AddToPlayer(CBasePlayer *pPlayer)
{
	if (CBasePlayerWeapon::AddToPlayer(pPlayer))
	{
		MESSAGE_BEGIN(MSG_ONE, gmsgWeapPickup, NULL, pPlayer->pev);
		WRITE_BYTE(m_iId);
		MESSAGE_END();
		return TRUE;
	}
	return FALSE;
}

BOOL CBaseWeaponShotgun::Deploy()
{
	return DefaultDeploy((char*)GetViewModel(), (char*)GetThirdpersonModel(), GetDeploySequence(), "shotgun");
}

void CBaseWeaponShotgun::Holster(int skiplocal /*= 0*/)
{
#if defined ( ASHEEP_WEAPONHOLSTER )
	DefaultHolster(GetHolsterSequence(), GetHolsterSequenceDuration(), skiplocal, 0);
#else
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;
	SendWeaponAnim(GetHolsterSequence());
#endif // defined ( ASHEEP_WEAPONHOLSTER )
}

void CBaseWeaponShotgun::PrimaryAttack()
{
	// don't fire underwater
	if (m_pPlayer->pev->waterlevel == 3)
	{
		PlayEmptySound();
		m_flNextPrimaryAttack = GetNextAttackDelay(0.15);
		return;
	}

	if (m_iClip <= 0)
	{
		Reload();
		if (m_iClip == 0)
			PlayEmptySound();
		return;
	}

	m_pPlayer->m_iWeaponVolume = LOUD_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = NORMAL_GUN_FLASH;

	m_iClip--;

	int flags;
#if defined( CLIENT_WEAPONS )
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	m_pPlayer->pev->effects = (int)(m_pPlayer->pev->effects) | EF_MUZZLEFLASH;

	Vector vecSrc = m_pPlayer->GetGunPosition();
	Vector vecAiming = m_pPlayer->GetAutoaimVector(AUTOAIM_5DEGREES);

	Vector vecDir;

#ifdef CLIENT_DLL
	if (bIsMultiplayer())
#else
	if (g_pGameRules->IsMultiplayer())
#endif
	{
		vecDir = m_pPlayer->FireBulletsPlayer(4, vecSrc, vecAiming, VECTOR_CONE_DM_SHOTGUN, 2048, BULLET_PLAYER_BUCKSHOT, 0, 0, m_pPlayer->pev, m_pPlayer->random_seed);
	}
	else
	{
		// regular old, untouched spread. 
		vecDir = m_pPlayer->FireBulletsPlayer(6, vecSrc, vecAiming, VECTOR_CONE_10DEGREES, 2048, BULLET_PLAYER_BUCKSHOT, 0, 0, m_pPlayer->pev, m_pPlayer->random_seed);
	}

	PLAYBACK_EVENT_FULL(flags, m_pPlayer->edict(), m_usSingleFire, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, vecDir.x, vecDir.y, 0, 0, 0, 0);

	if (!HasPrimaryAmmo() && IsAllowedToNotifyOutOfAmmo())
	{
		// HEV suit - indicate out of ammo condition
		DoSuitSpeakOutOfAmmo();
	}

	if (m_iClip != 0)
		m_flPumpTime = gpGlobals->time + GetPumpSoundDelayAfterSingleFire();

	m_flNextPrimaryAttack = GetNextAttackDelay(GetPrimaryFireRate());
	m_flNextSecondaryAttack = GetNextAttackDelay(GetPrimaryFireRate());
	if (m_iClip != 0)
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + GetIdleAnimationDelayAfterPrimaryAttackWithNoEmptyClip();
	else
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + GetIdleAnimationDelayAfterPrimaryAttackWithEmptyClip();
	m_fInSpecialReload = 0;
}

void CBaseWeaponShotgun::SecondaryAttack(void)
{
	// don't fire underwater
	if (m_pPlayer->pev->waterlevel == 3)
	{
		PlayEmptySound();
		m_flNextPrimaryAttack = GetNextAttackDelay(0.15);
		return;
	}

	if (m_iClip <= 1)
	{
		Reload();
		PlayEmptySound();
		return;
	}

	m_pPlayer->m_iWeaponVolume = LOUD_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = NORMAL_GUN_FLASH;

	m_iClip -= 2;


	int flags;
#if defined( CLIENT_WEAPONS )
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	m_pPlayer->pev->effects = (int)(m_pPlayer->pev->effects) | EF_MUZZLEFLASH;

	// player "shoot" animation
	m_pPlayer->SetAnimation(PLAYER_ATTACK1);

	Vector vecSrc = m_pPlayer->GetGunPosition();
	Vector vecAiming = m_pPlayer->GetAutoaimVector(AUTOAIM_5DEGREES);

	Vector vecDir;

#ifdef CLIENT_DLL
	if (bIsMultiplayer())
#else
	if (g_pGameRules->IsMultiplayer())
#endif
	{
		// tuned for deathmatch
		vecDir = m_pPlayer->FireBulletsPlayer(8, vecSrc, vecAiming, VECTOR_CONE_DM_DOUBLESHOTGUN, 2048, BULLET_PLAYER_BUCKSHOT, 0, 0, m_pPlayer->pev, m_pPlayer->random_seed);
	}
	else
	{
		// untouched default single player
		vecDir = m_pPlayer->FireBulletsPlayer(12, vecSrc, vecAiming, VECTOR_CONE_10DEGREES, 2048, BULLET_PLAYER_BUCKSHOT, 0, 0, m_pPlayer->pev, m_pPlayer->random_seed);
	}

	PLAYBACK_EVENT_FULL(flags, m_pPlayer->edict(), m_usDoubleFire, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, vecDir.x, vecDir.y, 0, 0, 0, 0);

	if (!HasPrimaryAmmo() && IsAllowedToNotifyOutOfAmmo())
	{
		// HEV suit - indicate out of ammo condition
		DoSuitSpeakOutOfAmmo();
	}

	if (m_iClip != 0)
		m_flPumpTime = gpGlobals->time + GetPumpSoundDelayAfterDoubleFire();

	m_flNextPrimaryAttack = GetNextAttackDelay(GetSecondaryFireRate());
	m_flNextSecondaryAttack = GetNextAttackDelay(GetSecondaryFireRate());
	if (m_iClip != 0)
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + GetIdleAnimationDelayAfterSecondaryAttackWithNoEmptyClip();
	else
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + GetIdleAnimationDelayAfterSecondaryAttackWithEmptyClip();

	m_fInSpecialReload = 0;

}


void CBaseWeaponShotgun::Reload(void)
{
	if (m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0 || m_iClip == iMaxClip())
		return;

	// don't reload until recoil is done
	if (m_flNextPrimaryAttack > UTIL_WeaponTimeBase())
		return;

	// check to see if we're ready to reload
	if (m_fInSpecialReload == 0)
	{
		float animDuration = PlayStartReloadAnimation();

		m_fInSpecialReload = 1;
		m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + animDuration;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + animDuration;
		m_flNextPrimaryAttack = GetNextAttackDelay(GetNextAttackDelayAfterStartReload());
		m_flNextSecondaryAttack = GetNextAttackDelay(GetNextAttackDelayAfterStartReload());
		return;
	}
	else if (m_fInSpecialReload == 1)
	{
		if (m_flTimeWeaponIdle > UTIL_WeaponTimeBase())
			return;
		// was waiting for gun to move to side
		m_fInSpecialReload = 2;

		PlayReloadSound();

		float animDuration = PlayReloadAnimation();

		m_flNextReload = UTIL_WeaponTimeBase() + animDuration;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + animDuration;
	}
	else
	{
		// Add them to the clip
		m_iClip += 1;
		m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] -= 1;
		m_fInSpecialReload = 1;
	}
}


void CBaseWeaponShotgun::WeaponIdle(void)
{
	ResetEmptySound();

	m_pPlayer->GetAutoaimVector(AUTOAIM_5DEGREES);

	float animDuration;

	if (m_flPumpTime && m_flPumpTime < gpGlobals->time)
	{
		// play pumping sound
		PlayPumpSound();
		m_flPumpTime = 0;
	}

	if (m_flTimeWeaponIdle <  UTIL_WeaponTimeBase())
	{
		if (m_iClip == 0 && m_fInSpecialReload == 0 && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType])
		{
			Reload();
		}
		else if (m_fInSpecialReload != 0)
		{
			if (m_iClip != iMaxClip() && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType])
			{
				Reload();
			}
			else
			{
				// reload debounce has timed out
				animDuration = PlayPumpAnimation();

				// play cocking sound
				PlayPumpSound();
				m_fInSpecialReload = 0;
				m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + animDuration;
			}
		}
		else
		{
			animDuration = PlayIdleAnimation();
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + animDuration;
		}
	}
}

void CBaseWeaponShotgun::PrecacheModels()
{
	m_iShell = PRECACHE_MODEL("models/shotgunshell.mdl");// shotgun shell
}

void CBaseWeaponShotgun::PrecacheSounds()
{
	GetSharedUtils()->PrecacheCommonShotgunSounds();
}

CBasePlayerWeaponUtil* CBaseWeaponShotgun::GetSharedUtils()
{
	static CBasePlayerWeaponUtil utilitySingleton;
	return &utilitySingleton;
}

void CBaseWeaponShotgun::PrecacheEvents()
{
	m_usSingleFire = PRECACHE_EVENT(1, "events/shotgun1.sc");
	m_usDoubleFire = PRECACHE_EVENT(1, "events/shotgun2.sc");
}

void CBaseWeaponShotgun::SetModel()
{
	SET_MODEL(ENT(pev), GetWorldModel());
}

const int& CBaseWeaponShotgun::GetShellIndex() const
{
	return m_iShell;
}

const unsigned short& CBaseWeaponShotgun::GetSingleFireEvent() const
{
	return m_usSingleFire;
}

const unsigned short& CBaseWeaponShotgun::GetDoubleFireEvent() const
{
	return m_usDoubleFire;
}
