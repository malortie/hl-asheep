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

int CBaseWeaponPistol::AddToPlayer(CBasePlayer* pPlayer)
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

void CBaseWeaponPistol::Spawn()
{
	Precache();
	m_iId = GetWeaponID();

	SetModel();
	GiveFirstTimeSpawnDefaultAmmo();

	FallInit();// get ready to fall down.
}

void CBaseWeaponPistol::Precache(void)
{
	PrecacheModels();
	PrecacheSounds();
	PrecacheEvents();
}

BOOL CBaseWeaponPistol::Deploy()
{
	// pev->body = 1;
	return DefaultDeploy((char*)GetViewModel(), (char*)GetThirdpersonModel(), GetDeploySequence(), "onehanded", /*UseDecrement() ? 1 : 0*/ 0);
}

void CBaseWeaponPistol::Holster(int skiplocal /* = 0 */)
{
#if defined ( ASHEEP_WEAPONHOLSTER )
	DefaultHolster(GetHolsterSequence(), GetHolsterSequenceDuration(), skiplocal, 0);
#else
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 1.0; // 16.0 / 16
	SendWeaponAnim(GetHolsterSequence());
#endif // defined ( ASHEEP_WEAPONHOLSTER )
}

void CBaseWeaponPistol::SecondaryAttack(void)
{
	GlockFire(GetBulletSpread(TRUE), GetSecondaryFireRate(), FALSE);
}

void CBaseWeaponPistol::PrimaryAttack(void)
{
	GlockFire(GetBulletSpread(FALSE), GetPrimaryFireRate(), TRUE);
}

void CBaseWeaponPistol::GlockFire(float flSpread, float flCycleTime, BOOL fUseAutoAim)
{
	if (m_iClip <= 0)
	{
		if (m_fFireOnEmpty)
		{
			PlayEmptySound();
			m_flNextPrimaryAttack = GetNextAttackDelay(0.2);
		}

		return;
	}

	m_iClip--;

	m_pPlayer->pev->effects = (int)(m_pPlayer->pev->effects) | EF_MUZZLEFLASH;

	int flags;

#if defined( CLIENT_WEAPONS )
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	// player "shoot" animation
	m_pPlayer->SetAnimation(PLAYER_ATTACK1);

	// silenced
	if (pev->body == 1)
	{
		m_pPlayer->m_iWeaponVolume = QUIET_GUN_VOLUME;
		m_pPlayer->m_iWeaponFlash = DIM_GUN_FLASH;
	}
	else
	{
		// non-silenced
		m_pPlayer->m_iWeaponVolume = NORMAL_GUN_VOLUME;
		m_pPlayer->m_iWeaponFlash = NORMAL_GUN_FLASH;
	}

	Vector vecSrc = m_pPlayer->GetGunPosition();
	Vector vecAiming;

	if (fUseAutoAim)
	{
		vecAiming = m_pPlayer->GetAutoaimVector(AUTOAIM_10DEGREES);
	}
	else
	{
		vecAiming = gpGlobals->v_forward;
	}

	Vector vecDir;
	vecDir = m_pPlayer->FireBulletsPlayer(1, vecSrc, vecAiming, Vector(flSpread, flSpread, flSpread), 8192, GetBulletType(), 0, 0, m_pPlayer->pev, m_pPlayer->random_seed);

	PLAYBACK_EVENT_FULL(flags, m_pPlayer->edict(), fUseAutoAim ? GetPrimaryFireEvent() : GetSecondaryFireEvent(), 0.0, (float *)&g_vecZero, (float *)&g_vecZero, vecDir.x, vecDir.y, 0, 0, (m_iClip == 0) ? 1 : 0, 0);

	m_flNextPrimaryAttack = m_flNextSecondaryAttack = GetNextAttackDelay(flCycleTime);

	if (!HasPrimaryAmmo() && IsAllowedToNotifyOutOfAmmo())
	{
		// HEV suit - indicate out of ammo condition
		DoSuitSpeakOutOfAmmo();
	}

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + GetNextIdleTimeAfterShoot();
}


void CBaseWeaponPistol::Reload(void)
{
	if (m_pPlayer->ammo_9mm <= 0)
		return;

	int iResult;

	if (m_iClip == 0)
		iResult = DefaultReload(iMaxClip(), GetReloadEmptySequence(), GetReloadEmptySequenceDuration());
	else
		iResult = DefaultReload(iMaxClip(), GetReloadSequence(), GetReloadSequenceDuration());

	if (iResult)
	{
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat(m_pPlayer->random_seed, 10, 15);
	}
}

void CBaseWeaponPistol::WeaponIdle(void)
{
	ResetEmptySound();

	m_pPlayer->GetAutoaimVector(AUTOAIM_10DEGREES);

	if (m_flTimeWeaponIdle > UTIL_WeaponTimeBase())
		return;

	// only idle if the slid isn't back
	if (m_iClip != 0)
	{
		float animDuration = PlayIdleAnimation();
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + animDuration;
	}
}

void CBaseWeaponPistol::PrecacheModels()
{
	m_iShell = PRECACHE_MODEL("models/shell.mdl");// brass shell
}

void CBaseWeaponPistol::PrecacheSounds()
{
	GetSharedUtils()->PrecacheCommonShotgunSounds();
}

void CBaseWeaponPistol::SetModel()
{
	SET_MODEL(ENT(pev), GetWorldModel());
}

float CBaseWeaponPistol::GetNextIdleTimeAfterShoot()
{
	return UTIL_SharedRandomFloat(m_pPlayer->random_seed, 10, 15);
}

CBasePlayerWeaponUtil* CBaseWeaponPistol::GetSharedUtils()
{
	static CBasePlayerWeaponUtil utilitySingleton;
	return &utilitySingleton;
}
