/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"effects.h"
#include	"customentity.h"
#include	"basemonsterrepel.h"

void CBaseMonsterRepel::Spawn( void )
{
	Precache( );
	pev->solid = SOLID_NOT;

	SetUse( &CBaseMonsterRepel::RepelUse );
}

void CBaseMonsterRepel::Precache( void )
{
	m_iszClassNameOfMonsterToSpawn = MAKE_STRING(GetClassNameOfMonsterToSpawn());
	UTIL_PrecacheOther( STRING(m_iszClassNameOfMonsterToSpawn) );
	m_iSpriteTexture = PRECACHE_MODEL((char*)GetRopeSpriteName());
}

void CBaseMonsterRepel::RepelUse ( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	TraceResult tr;
	UTIL_TraceLine( pev->origin, pev->origin + Vector( 0, 0, -4096.0), dont_ignore_monsters, ENT(pev), &tr);

	CBaseEntity *pEntity = Create((char*)STRING(m_iszClassNameOfMonsterToSpawn), pev->origin, pev->angles );
	CBaseMonster *pRepel = pEntity->MyMonsterPointer( );
	pRepel->pev->movetype = MOVETYPE_FLY;
	pRepel->pev->velocity = Vector( 0, 0, GetFirstTimeSpawnRepellingSpeed() );
	pRepel->SetActivity( ACT_GLIDE );
	pRepel->m_vecLastPosition = tr.vecEndPos;

	CBeam *pBeam = CBeam::BeamCreate(GetRopeSpriteName(), 10 );
	pBeam->PointEntInit( pev->origin + Vector(0,0,112), pRepel->entindex() );
	pBeam->SetFlags( BEAM_FSOLID );
	pBeam->SetColor( 255, 255, 255 );
	pBeam->SetThink( &CBeam::SUB_Remove );
	pBeam->pev->nextthink = gpGlobals->time + -4096.0 * tr.flFraction / pRepel->pev->velocity.z + 0.5;

	UTIL_Remove( this );
}

const char* CBaseMonsterRepel::GetRopeSpriteName() const
{
	return "sprites/rope.spr";
}

float CBaseMonsterRepel::GetFirstTimeSpawnRepellingSpeed() const
{
	return RANDOM_FLOAT( -196, -128 );
}