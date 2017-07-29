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

void CBaseWeaponWithThrowableMonster::Spawn( )
{
	Precache( );
	m_iId = GetWeaponID();
	SetModel();

	FallInit();//get ready to fall down.
	GiveFirstTimeSpawnDefaultAmmo();
		
	pev->sequence = 1;
	pev->animtime = gpGlobals->time;
	pev->framerate = 1.0;
}


void CBaseWeaponWithThrowableMonster::Precache( void )
{
	PrecacheModels();
	PrecacheSounds();
	PrecacheEvents();

	UTIL_PrecacheOther( GetClassnameOfMonsterToThrow() );
}


int CBaseWeaponWithThrowableMonster::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "Snarks";
	p->iMaxAmmo1 = SNARK_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 4;
	p->iPosition = 3;
	p->iId = m_iId = GetWeaponID();
	p->iWeight = SNARK_WEIGHT;
	p->iFlags = ITEM_FLAG_LIMITINWORLD | ITEM_FLAG_EXHAUSTIBLE;

	return 1;
}

BOOL CBaseWeaponWithThrowableMonster::Deploy( )
{
	// play hunt sound
	PlayDeploySound();
	m_pPlayer->m_iWeaponVolume = QUIET_GUN_VOLUME;

	return DefaultDeploy((char*)GetViewModel(), (char*)GetThirdpersonModel(), GetDeploySequence(), "squeak" );
}


void CBaseWeaponWithThrowableMonster::Holster( int skiplocal /* = 0 */ )
{
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;
	
	if ( !m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] )
	{
		m_pPlayer->pev->weapons &= ~(1<<GetWeaponID());
		SetThink( &CBaseWeaponWithThrowableMonster::DestroyItem );
		pev->nextthink = gpGlobals->time + 0.1;
		return;
	}

	DefaultHolster(GetHolsterSequence(), GetHolsterSequenceDuration(), skiplocal, 0);
	EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "common/null.wav", 1.0, ATTN_NORM);
}


void CBaseWeaponWithThrowableMonster::PrimaryAttack()
{
	if ( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] )
	{
		UTIL_MakeVectors( m_pPlayer->pev->v_angle );
		TraceResult tr;
		Vector trace_origin;

		// HACK HACK:  Ugly hacks to handle change in origin based on new physics code for players
		// Move origin up if crouched and start trace a bit outside of body ( 20 units instead of 16 )
		trace_origin = m_pPlayer->pev->origin;
		if ( m_pPlayer->pev->flags & FL_DUCKING )
		{
			trace_origin = trace_origin - ( VEC_HULL_MIN - VEC_DUCK_HULL_MIN );
		}

		// find place to toss monster
		UTIL_TraceLine( trace_origin + gpGlobals->v_forward * 20, trace_origin + gpGlobals->v_forward * 64, dont_ignore_monsters, NULL, &tr );

	int flags;
#ifdef CLIENT_WEAPONS
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	    PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), GetThrowEvent(), 0.0, (float *)&g_vecZero, (float *)&g_vecZero, 0.0, 0.0, 0, 0, 0, 0 );

		if ( tr.fAllSolid == 0 && tr.fStartSolid == 0 && tr.flFraction > 0.25 )
		{
			// player "shoot" animation
			m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

#ifndef CLIENT_DLL
			CBaseEntity *pSqueak = CBaseEntity::Create((char*)GetClassnameOfMonsterToThrow(), tr.vecEndPos, m_pPlayer->pev->v_angle, m_pPlayer->edict() );
			pSqueak->pev->velocity = gpGlobals->v_forward * GetTossVelocity() + m_pPlayer->pev->velocity;
#endif

			// play hunt sound
			PlayHuntSound();

			m_pPlayer->m_iWeaponVolume = QUIET_GUN_VOLUME;

			m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]--;

			m_fJustThrown = 1;

			m_flNextPrimaryAttack = GetNextAttackDelay(GetFireRate());
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + GetNextIdleAfterThrow();
		}
	}
}


void CBaseWeaponWithThrowableMonster::SecondaryAttack( void )
{

}


void CBaseWeaponWithThrowableMonster::WeaponIdle( void )
{
	if ( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	if (m_fJustThrown)
	{
		m_fJustThrown = 0;

		if ( !m_pPlayer->m_rgAmmo[PrimaryAmmoIndex()] )
		{
			RetireWeapon();
			return;
		}

		SendWeaponAnim( GetDeploySequence() );
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
		return;
	}

	float animDuration = PlayIdleAnimation();
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + animDuration;

}

void CBaseWeaponWithThrowableMonster::SetModel()
{
	SET_MODEL(ENT(pev), GetWorldModel());
}

CBasePlayerWeaponUtil* CBaseWeaponWithThrowableMonster::GetSharedUtils()
{
	static CBasePlayerWeaponUtil utilitySingleton;
	return &utilitySingleton;
}