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
#include "player.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "soundent.h"
#include "decals.h"
#include "gamerules.h"
#if defined ( CLIENT_DLL )
#include "com_weapons.h"
#endif

#define WEAPON_CLASSNAME_BARNEY_GLOCK			"weapon_barney9mmhg"
#define WEAPON_CLASSNAME_BARNEY_MP5				"weapon_barney9mmar"
#define WEAPON_CLASSNAME_BARNEY_SHOTGUN			"weapon_barneyshotgun"
#define WEAPON_CLASSNAME_BARNEY_HANDGRENADE		"weapon_barneyhandgrenade"

BOOL CBasePlayerWeapon::IsAllowedToNotifyOutOfAmmo()
{
#if defined ( CLIENT_DLL )
	return FALSE;
#else
	if (FClassnameIs(pev, WEAPON_CLASSNAME_BARNEY_GLOCK) || 
		FClassnameIs(pev, WEAPON_CLASSNAME_BARNEY_MP5) ||
		FClassnameIs(pev, WEAPON_CLASSNAME_BARNEY_SHOTGUN) ||
		FClassnameIs(pev, WEAPON_CLASSNAME_BARNEY_HANDGRENADE))
	{
		return FALSE;
	}

	return TRUE;
#endif // defined ( CLIENT_DLL )
}

BOOL CBasePlayerWeapon::UsesPrimaryAmmo()
{
	if (m_iPrimaryAmmoType != WEAPON_NOCLIP && pszAmmo1() != NULL && *pszAmmo1() != '\0' && iMaxAmmo1() != WEAPON_NOCLIP)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CBasePlayerWeapon::UsesSecondaryAmmo()
{
	if (m_iSecondaryAmmoType != WEAPON_NOCLIP && pszAmmo2() != NULL && *pszAmmo2() != '\0' && iMaxAmmo2() != WEAPON_NOCLIP)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CBasePlayerWeapon::HasPrimaryAmmo()
{
	// If I use a clip, and have some ammo in it, then I have ammo
	if (UsesClipsForAmmo() && m_iClip > 0)
	{
		return TRUE;
	}

	// Otherwise, I have ammo if I have some in my ammo counts
	if (GetPrimaryAmmoCount() > 0)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CBasePlayerWeapon::HasSecondaryAmmo()
{
	// I have ammo if I have some in my ammo counts
	if (GetSecondaryAmmoCount() > 0)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CBasePlayerWeapon::UsesClipsForAmmo()
{
	return iMaxClip() != WEAPON_NOCLIP;
}

int CBasePlayerWeapon::GetPrimaryAmmoCount()
{
	return m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] + m_iClip;
}

int CBasePlayerWeapon::GetSecondaryAmmoCount()
{
	return m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType];
}

void CBasePlayerWeapon::DoSuitSpeakOutOfAmmo()
{
	m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
}


#if defined ( ASHEEP_WEAPONHOLSTER )

BOOL CBasePlayerWeapon::DefaultHolster(int iAnim, float flDelay, int skiplocal /*= 0*/, int body /*= 0*/)
{
#if defined ( CLIENT_DLL )
	// CLIENT

	SendWeaponAnim(iAnim, skiplocal, body);

	m_pPlayer->m_flNextAttack = flDelay;
	m_pPlayer->m_flTimeWeaponIdle = flDelay + 2.0f;

#else
	// SERVER
	SendWeaponAnim(iAnim, skiplocal, body);

	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + flDelay;
	m_pPlayer->m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + flDelay + 2.0f;
	m_flLastFireTime = 0.0;

#endif // defined ( CLIENT_DLL )
	return TRUE;
}

void CBasePlayer::UpdateWeaponSwitching()
{
	switch (m_iSwitchWeaponState)
	{
	case SwitchWeaponState::STATE_DEPLOY:
	{
		switch (m_iSwitchWeaponMethod)
		{
		case SWITCH_SELECTWEAPON:
		{
			m_pLastItem = m_pActiveItem;
			m_pActiveItem = m_pWeaponToSwitchTo;

			if (m_pActiveItem)
			{
				m_pActiveItem->Deploy();
				m_pActiveItem->UpdateItemInfo();
			}
		}
		break;

		case SWITCH_SELECTNEXTWEAPON:
		{
			m_pActiveItem = m_pWeaponToSwitchTo;

			if (m_pActiveItem)
			{
				m_pActiveItem->Deploy();
				m_pActiveItem->UpdateItemInfo();
			}
		}
		break;

		case SWITCH_SELECTLASTWEAPON:
		{
			CBasePlayerItem *pTemp = m_pActiveItem;
			m_pActiveItem = m_pWeaponToSwitchTo;
			m_pLastItem = pTemp;
			m_pActiveItem->Deploy();
			m_pActiveItem->UpdateItemInfo();
		}
		break;

		case SWITCH_EXPLICIT:
		{
			m_pActiveItem = m_pWeaponToSwitchTo;
			m_pActiveItem->Deploy();
		}
		break;

		default:
			break;
		}

		m_flNextAttack = UTIL_WeaponTimeBase() + 0.5f;
		m_iSwitchWeaponState = SwitchWeaponState::STATE_RESETVARIABLES;
	}
	break;

	case SwitchWeaponState::STATE_RESETVARIABLES:
	default:
		ResetSwitchWeaponVariables();
		break;
	}
}

void CBasePlayer::ResetSwitchWeaponVariables()
{
	m_pWeaponToSwitchTo = NULL;
	m_iSwitchWeaponMethod = 0;
	m_iSwitchWeaponState = SwitchWeaponState::STATE_NONE;
}

void CBasePlayer::BeginSwitchWeapon(CBasePlayerItem* weaponToSwitchTo, int switchWeaponMethod)
{
	m_pWeaponToSwitchTo = weaponToSwitchTo;
	m_iSwitchWeaponMethod = switchWeaponMethod;
	m_iSwitchWeaponState = SwitchWeaponState::STATE_DEPLOY;
}

#endif // defined ( ASHEEP_WEAPONHOLSTER )
