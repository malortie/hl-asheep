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
#include "soundent.h"
#include "gamerules.h"

//=========================================================
//=========================================================
int CBaseWeaponARWithGrenadeLauncher::SecondaryAmmoIndex( void )
{
	return m_iSecondaryAmmoType;
}

void CBaseWeaponARWithGrenadeLauncher::Spawn( )
{
	Precache( );
	SetModel();
	m_iId = GetWeaponID();
	GiveFirstTimeSpawnDefaultAmmo();

	FallInit();// get ready to fall down.
}


void CBaseWeaponARWithGrenadeLauncher::Precache( void )
{
	PrecacheModels();
	PrecacheSounds();
	PrecacheEvents();
}

int CBaseWeaponARWithGrenadeLauncher::AddToPlayer( CBasePlayer *pPlayer )
{
	if ( CBasePlayerWeapon::AddToPlayer( pPlayer ) )
	{
		MESSAGE_BEGIN( MSG_ONE, gmsgWeapPickup, NULL, pPlayer->pev );
			WRITE_BYTE( m_iId );
		MESSAGE_END();
		return TRUE;
	}
	return FALSE;
}

BOOL CBaseWeaponARWithGrenadeLauncher::Deploy(void)
{
	return DefaultDeploy((char*)GetViewModel(), (char*)GetThirdpersonModel(), GetDeploySequence(), "mp5");
}

void CBaseWeaponARWithGrenadeLauncher::Holster(int skiplocal /* = 0 */)
{
	DefaultHolster(GetHolsterSequence(), GetHolsterSequenceDuration(), skiplocal, 0);
}

void CBaseWeaponARWithGrenadeLauncher::PrimaryAttack()
{
	// don't fire underwater
	if (m_pPlayer->pev->waterlevel == 3)
	{
		PlayEmptySound( );
		m_flNextPrimaryAttack = GetNextAttackDelay(0.15);
		return;
	}

	if (m_iClip <= 0)
	{
		PlayEmptySound();
		m_flNextPrimaryAttack = GetNextAttackDelay(0.15);
		return;
	}

	m_pPlayer->m_iWeaponVolume = NORMAL_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = NORMAL_GUN_FLASH;

	m_iClip--;


	m_pPlayer->pev->effects = (int)(m_pPlayer->pev->effects) | EF_MUZZLEFLASH;

	// player "shoot" animation
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	Vector vecSrc	 = m_pPlayer->GetGunPosition( );
	Vector vecAiming = m_pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES );
	Vector vecDir;

#ifdef CLIENT_DLL
	if ( !bIsMultiplayer() )
#else
	if ( !g_pGameRules->IsMultiplayer() )
#endif
	{
		// optimized multiplayer. Widened to make it easier to hit a moving player
		vecDir = m_pPlayer->FireBulletsPlayer( 1, vecSrc, vecAiming, VECTOR_CONE_6DEGREES, 8192, GetBulletType(), 2, 0, m_pPlayer->pev, m_pPlayer->random_seed );
	}
	else
	{
		// single player spread
		vecDir = m_pPlayer->FireBulletsPlayer( 1, vecSrc, vecAiming, VECTOR_CONE_3DEGREES, 8192, GetBulletType(), 2, 0, m_pPlayer->pev, m_pPlayer->random_seed );
	}

  int flags;
#if defined( CLIENT_WEAPONS )
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), GetFireEvent(), 0.0, (float *)&g_vecZero, (float *)&g_vecZero, vecDir.x, vecDir.y, 0, 0, 0, 0 );

	if (!HasPrimaryAmmo() && IsAllowedToNotifyOutOfAmmo())
	{
		// HEV suit - indicate out of ammo condition
		DoSuitSpeakOutOfAmmo();
	}

	m_flNextPrimaryAttack = GetNextAttackDelay(GetPrimaryFireRate());

	if ( m_flNextPrimaryAttack < UTIL_WeaponTimeBase() )
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + GetPrimaryFireRate();

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}



void CBaseWeaponARWithGrenadeLauncher::SecondaryAttack( void )
{
	// don't fire underwater
	if (m_pPlayer->pev->waterlevel == 3)
	{
		PlayEmptySound( );
		m_flNextPrimaryAttack = 0.15;
		return;
	}

	if (m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType] == 0)
	{
		PlayEmptySound( );
		return;
	}

	m_pPlayer->m_iWeaponVolume = NORMAL_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = BRIGHT_GUN_FLASH;

	m_pPlayer->m_iExtraSoundTypes = bits_SOUND_DANGER;
	m_pPlayer->m_flStopExtraSoundTime = UTIL_WeaponTimeBase() + 0.2;
			
	m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType]--;

	// player "shoot" animation
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

 	UTIL_MakeVectors( m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle );

	// we don't add in player velocity anymore.
	CGrenade::ShootContact( m_pPlayer->pev, 
							m_pPlayer->pev->origin + m_pPlayer->pev->view_ofs + gpGlobals->v_forward * 16, 
							gpGlobals->v_forward * GetGrenadeTossSpeed() );

	int flags;
#if defined( CLIENT_WEAPONS )
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	PLAYBACK_EVENT( flags, m_pPlayer->edict(), GetGrenadeLaunchEvent());
	
	m_flNextPrimaryAttack = GetNextAttackDelay(GetSecondaryFireRate());
	m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + GetSecondaryFireRate();
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 5;// idle pretty soon after shooting.

	if (!HasSecondaryAmmo() && IsAllowedToNotifyOutOfAmmo())
	{
		// HEV suit - indicate out of ammo condition
		DoSuitSpeakOutOfAmmo();
	}
}

void CBaseWeaponARWithGrenadeLauncher::Reload( void )
{
	if ( m_pPlayer->ammo_9mm <= 0 )
		return;

	DefaultReload( iMaxClip(), GetReloadSequence(), GetReloadSequenceDuration() );
}


void CBaseWeaponARWithGrenadeLauncher::WeaponIdle( void )
{
	ResetEmptySound( );

	m_pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES );

	if ( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	float animDuration = PlayIdleAnimation();
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + animDuration; // how long till we do this again.
}

void CBaseWeaponARWithGrenadeLauncher::PrecacheModels()
{
	m_iShell = PRECACHE_MODEL((char*)GetShellBrassModel());// brass shellTE_MODEL
}

void CBaseWeaponARWithGrenadeLauncher::PrecacheSounds()
{
	GetSharedUtils()->PrecacheCommonMP5Sounds();
}

void CBaseWeaponARWithGrenadeLauncher::SetModel()
{
	SET_MODEL(ENT(pev), GetWorldModel());
}

float CBaseWeaponARWithGrenadeLauncher::GetPrimaryFireRate()
{
	return 0.1;
}

float CBaseWeaponARWithGrenadeLauncher::GetSecondaryFireRate()
{
	return 1;
}

float CBaseWeaponARWithGrenadeLauncher::GetGrenadeTossSpeed()
{
	return 800;	
}

const char* CBaseWeaponARWithGrenadeLauncher::GetShellBrassModel()
{
	return "models/shell.mdl";
}

CBasePlayerWeaponUtil* CBaseWeaponARWithGrenadeLauncher::GetSharedUtils()
{
	static CBasePlayerWeaponUtil utilitySingleton;
	return &utilitySingleton;
}
