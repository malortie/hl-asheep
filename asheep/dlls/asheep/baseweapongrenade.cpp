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

void CBaseWeaponGrenade::Spawn()
{
	Precache();
	m_iId = GetWeaponID();
	SetModel();

#ifndef CLIENT_DLL
	pev->dmg = GetExplodeDamage();
#endif
	GiveFirstTimeSpawnDefaultAmmo();

	FallInit();// get ready to fall down.
}

void CBaseWeaponGrenade::Precache(void)
{
	PrecacheModels();
	PrecacheSounds();
	PrecacheEvents();
}

BOOL CBaseWeaponGrenade::Deploy()
{
	m_flReleaseThrow = -1;
	return DefaultDeploy((char*)GetViewModel(), (char*)GetThirdpersonModel(), GetDeploySequence(), "crowbar");
}

BOOL CBaseWeaponGrenade::CanHolster(void)
{
	// can only holster hand grenades when not primed!
	return (m_flStartThrow == 0);
}

void CBaseWeaponGrenade::Holster(int skiplocal /* = 0 */)
{
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;

	if (m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType])
	{
#if defined ( ASHEEP_WEAPONHOLSTER )
		DefaultHolster(GetHolsterSequence(), GetHolsterSequenceDuration(), skiplocal, 0);
#else
		SendWeaponAnim(GetHolsterSequence());
#endif // defined ( ASHEEP_WEAPONHOLSTER )
	}
	else
	{
		// no more grenades!
		m_pPlayer->pev->weapons &= ~(1 << GetWeaponID());
		SetThink(&CBaseWeaponGrenade::DestroyItem);
		pev->nextthink = gpGlobals->time + 0.1;
	}

	EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "common/null.wav", 1.0, ATTN_NORM);
}

void CBaseWeaponGrenade::PrimaryAttack()
{
	if (!m_flStartThrow && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] > 0)
	{
		m_flStartThrow = gpGlobals->time;
		m_flReleaseThrow = 0;

		float animDuration = PlayPinPullAnimation();
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + animDuration;
	}
}


void CBaseWeaponGrenade::WeaponIdle(void)
{
	if (m_flReleaseThrow == 0 && m_flStartThrow)
		m_flReleaseThrow = gpGlobals->time;

	if (m_flTimeWeaponIdle > UTIL_WeaponTimeBase())
		return;

	if (m_flStartThrow)
	{
		Vector angThrow = m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle;

		if (angThrow.x < 0)
			angThrow.x = -10 + angThrow.x * ((90 - 10) / 90.0);
		else
			angThrow.x = -10 + angThrow.x * ((90 + 10) / 90.0);

		float flVel = (90 - angThrow.x) * 4;
		if (flVel > 500)
			flVel = 500;

		UTIL_MakeVectors(angThrow);

		Vector vecSrc = m_pPlayer->pev->origin + m_pPlayer->pev->view_ofs + gpGlobals->v_forward * 16;

		Vector vecThrow = gpGlobals->v_forward * flVel + m_pPlayer->pev->velocity;

		// alway explode 3 seconds after the pin was pulled
		float time = m_flStartThrow - gpGlobals->time + GetDetonateDelay();
		if (time < 0)
			time = 0;

		CGrenade::ShootTimed(m_pPlayer->pev, vecSrc, vecThrow, time);

		float animDuration = PlayThrowAnimation(flVel);

		// player "shoot" animation
		m_pPlayer->SetAnimation(PLAYER_ATTACK1);

		m_flReleaseThrow = 0;
		m_flStartThrow = 0;
		m_flNextPrimaryAttack = GetNextAttackDelay(animDuration);
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + animDuration;

		m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]--;

		if (!m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType])
		{
			// just threw last grenade
			// set attack times in the future, and weapon idle in the future so we can see the whole throw
			// animation, weapon idle will automatically retire the weapon for us.
			m_flTimeWeaponIdle = m_flNextSecondaryAttack = m_flNextPrimaryAttack = GetNextAttackDelay(animDuration);// ensure that the animation can finish playing
		}
		return;
	}
	else if (m_flReleaseThrow > 0)
	{
		// we've finished the throw, restart.
		m_flStartThrow = 0;

		if (m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType])
		{
			SendWeaponAnim(GetDeploySequence());
		}
		else
		{
			RetireWeapon();
			return;
		}

		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat(m_pPlayer->random_seed, 10, 15);
		m_flReleaseThrow = -1;
		return;
	}

	if (m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType])
	{
		float animDuration = PlayIdleAnimation();
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + animDuration;
	}
}

void CBaseWeaponGrenade::SetModel()
{
	SET_MODEL(ENT(pev), GetWorldModel());
}

float CBaseWeaponGrenade::GetExplodeDamage()
{
	return gSkillData.plrDmgHandGrenade;
}

CBasePlayerWeaponUtil* CBaseWeaponGrenade::GetSharedUtils()
{
	static CBasePlayerWeaponUtil utilitySingleton;
	return &utilitySingleton;
}