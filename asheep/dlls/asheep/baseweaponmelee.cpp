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

void FindHullIntersection(const Vector &vecSrc, TraceResult &tr, float *mins, float *maxs, edict_t *pEntity);

#define	DEFAULT_MELEE_BODYHIT_VOLUME 128
#define	DEFAULT_MELEE_WALLHIT_VOLUME 512

void CBaseWeaponMelee::Spawn()
{
	Precache();
	m_iId = GetWeaponID();
	SetModel();
	m_iClip = -1;

	FallInit();// get ready to fall down.
}


void CBaseWeaponMelee::Precache(void)
{
	PrecacheModels();
	PrecacheSounds();
	PrecacheEvents();
}

BOOL CBaseWeaponMelee::Deploy()
{
	return DefaultDeploy((char*)GetViewModel(), (char*)GetThirdpersonModel(), GetDeploySequence(), "crowbar");
}

void CBaseWeaponMelee::Holster(int skiplocal /* = 0 */)
{
#if defined ( ASHEEP_WEAPONHOLSTER )
	DefaultHolster(GetHolsterSequence(), GetHolsterSequenceDuration(), skiplocal, 0);
#else
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;
	SendWeaponAnim(GetHolsterSequence());
#endif // defined ( ASHEEP_WEAPONHOLSTER )
}

void CBaseWeaponMelee::PrimaryAttack()
{
	if (!Swing(1))
	{
		SetThink(&CBaseWeaponMelee::SwingAgain);
		pev->nextthink = gpGlobals->time + 0.1;
	}
}

void CBaseWeaponMelee::Smack()
{
	DecalGunshot(&m_trHit, GetBulletType());
}


void CBaseWeaponMelee::SwingAgain(void)
{
	Swing(0);
}

int CBaseWeaponMelee::Swing(int fFirst)
{
	int fDidHit = FALSE;

	TraceResult tr;

	UTIL_MakeVectors(m_pPlayer->pev->v_angle);
	Vector vecSrc = m_pPlayer->GetGunPosition();
	Vector vecEnd = vecSrc + gpGlobals->v_forward * GetMeleeRange();

	UTIL_TraceLine(vecSrc, vecEnd, dont_ignore_monsters, ENT(m_pPlayer->pev), &tr);

#ifndef CLIENT_DLL
	if (tr.flFraction >= 1.0)
	{
		UTIL_TraceHull(vecSrc, vecEnd, dont_ignore_monsters, head_hull, ENT(m_pPlayer->pev), &tr);
		if (tr.flFraction < 1.0)
		{
			// Calculate the point of intersection of the line (or hull) and the object we hit
			// This is and approximation of the "best" intersection
			CBaseEntity *pHit = CBaseEntity::Instance(tr.pHit);
			if (!pHit || pHit->IsBSPModel())
				FindHullIntersection(vecSrc, tr, VEC_DUCK_HULL_MIN, VEC_DUCK_HULL_MAX, m_pPlayer->edict());
			vecEnd = tr.vecEndPos;	// This is the point on the actual surface (the hull could have hit space)
		}
	}
#endif

#if defined ( ASHEEP_CLIENT_WEAPONS )
	PLAYBACK_EVENT_FULL(FEV_NOTHOST, m_pPlayer->edict(), GetSwingEvent(),
		0.0, (float *)&g_vecZero, (float *)&g_vecZero, 0, 0, 0,
		0.0, 0, 0.0);
#endif // defined ( ASHEEP_CLIENT_WEAPONS )

	if (tr.flFraction >= 1.0)
	{
		if (fFirst)
		{
#if !defined ( ASHEEP_CLIENT_WEAPONS )
			// miss
			PlayMissAnimation();
			m_flNextPrimaryAttack = gpGlobals->time + GetFireRate();
			// play wiff or swish sound
			PlayMissSound();

			// player "shoot" animation
			m_pPlayer->SetAnimation(PLAYER_ATTACK1);
#else
			// miss
			m_flNextPrimaryAttack = GetNextAttackDelay(GetFireRate());

			// player "shoot" animation
			m_pPlayer->SetAnimation(PLAYER_ATTACK1);
#endif // !defined ( ASHEEP_CLIENT_WEAPONS )
		}
	}
	else
	{
		PlayHitAnimation();

		// player "shoot" animation
		m_pPlayer->SetAnimation(PLAYER_ATTACK1);

#ifndef CLIENT_DLL

		// hit
		fDidHit = TRUE;
		CBaseEntity *pEntity = CBaseEntity::Instance(tr.pHit);

		ClearMultiDamage();

		if ((m_flNextPrimaryAttack + 1 < UTIL_WeaponTimeBase()) || g_pGameRules->IsMultiplayer())
		{
			// first swing does full damage
			pEntity->TraceAttack(m_pPlayer->pev, GetMeleeDamage(), gpGlobals->v_forward, &tr, DMG_CLUB);
		}
		else
		{
			// subsequent swings do half
			pEntity->TraceAttack(m_pPlayer->pev, GetMeleeDamage() / 2, gpGlobals->v_forward, &tr, DMG_CLUB);
		}
		ApplyMultiDamage(m_pPlayer->pev, m_pPlayer->pev);

		// play thwack, smack, or dong sound
		float flVol = 1.0;
		int fHitWorld = TRUE;

		if (pEntity)
		{
			if (pEntity->Classify() != CLASS_NONE && pEntity->Classify() != CLASS_MACHINE)
			{
				// play thwack or smack sound
				PlayHitBodySound();
				m_pPlayer->m_iWeaponVolume = DEFAULT_MELEE_BODYHIT_VOLUME;
				if (!pEntity->IsAlive())
					return TRUE;
				else
					flVol = 0.1;

				fHitWorld = FALSE;
			}
		}

		// play texture hit sound
		// UNDONE: Calculate the correct point of intersection when we hit with the hull instead of the line

		if (fHitWorld)
		{
			float fvolbar = TEXTURETYPE_PlaySound(&tr, vecSrc, vecSrc + (vecEnd - vecSrc) * 2, GetBulletType());

			if (g_pGameRules->IsMultiplayer())
			{
				// override the volume here, cause we don't play texture sounds in multiplayer, 
				// and fvolbar is going to be 0 from the above call.

				fvolbar = 1;
			}

			// also play crowbar strike
			PlayHitWorldSound(fvolbar);

			// delay the decal a bit
			m_trHit = tr;
		}

		m_pPlayer->m_iWeaponVolume = flVol * DEFAULT_MELEE_WALLHIT_VOLUME;
#endif
		m_flNextPrimaryAttack = GetNextAttackDelay(GetFireRate() / 2);

		SetThink(&CBaseWeaponMelee::Smack);
		pev->nextthink = UTIL_WeaponTimeBase() + 0.2;


	}
	return fDidHit;
}

void CBaseWeaponMelee::SetModel()
{
	SET_MODEL(ENT(pev), GetWorldModel());
}

CBasePlayerWeaponUtil* CBaseWeaponMelee::GetSharedUtils()
{
	static CBasePlayerWeaponUtil utilitySingleton;
	return &utilitySingleton;
}