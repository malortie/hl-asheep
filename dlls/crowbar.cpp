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

enum gauss_e {
	CROWBAR_IDLE = 0,
	CROWBAR_DRAW,
	CROWBAR_HOLSTER,
	CROWBAR_ATTACK1HIT,
	CROWBAR_ATTACK1MISS,
	CROWBAR_ATTACK2MISS,
	CROWBAR_ATTACK2HIT,
	CROWBAR_ATTACK3MISS,
	CROWBAR_ATTACK3HIT
};

LINK_ENTITY_TO_CLASS( weapon_crowbar, CCrowbar );

const char* CCrowbar::pMissSounds[] =
{
	"weapons/cbar_miss1.wav",
};

const char* CCrowbar::pHitBodySounds[] =
{
	"weapons/cbar_hitbod1.wav",
	"weapons/cbar_hitbod2.wav",
	"weapons/cbar_hitbod3.wav",
};

const char* CCrowbar::pHitWorldSounds[] =
{
	"weapons/cbar_hit1.wav",
	"weapons/cbar_hit2.wav",
};

int CCrowbar::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = NULL;
	p->iMaxAmmo1 = -1;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 0;
	p->iPosition = 0;
	p->iId = GetWeaponID();
	p->iWeight = CROWBAR_WEIGHT;
	return 1;
}

int CCrowbar::GetWeaponID()
{
	return WEAPON_CROWBAR;
}

const char* CCrowbar::GetThirdpersonModel()
{
	return "models/p_crowbar.mdl";
}

const char* CCrowbar::GetViewModel()
{
	return "models/v_crowbar.mdl";
}

const char* CCrowbar::GetWorldModel()
{
	return "models/w_crowbar.mdl";
}

float CCrowbar::GetMeleeDamage()
{
	return gSkillData.plrDmgCrowbar;
}

int CCrowbar::GetBulletType()
{
	return BULLET_PLAYER_CROWBAR;
}

const unsigned short& CCrowbar::GetSwingEvent() const
{
	return m_usCrowbar;
}

void CCrowbar::PrecacheModels()
{
	PRECACHE_MODEL("models/v_crowbar.mdl");
	PRECACHE_MODEL("models/w_crowbar.mdl");
	PRECACHE_MODEL("models/p_crowbar.mdl");
}

void CCrowbar::PrecacheSounds()
{
	PRECACHE_SOUND_ARRAY(pMissSounds);
	PRECACHE_SOUND_ARRAY(pHitBodySounds);
	PRECACHE_SOUND_ARRAY(pHitWorldSounds);
}

void CCrowbar::PrecacheEvents()
{
	m_usCrowbar = PRECACHE_EVENT(1, "events/crowbar.sc");
}

int CCrowbar::GetDeploySequence()
{
	return CROWBAR_DRAW;
}

int CCrowbar::GetHolsterSequence()
{
	return CROWBAR_HOLSTER;
}

float CCrowbar::GetHolsterSequenceDuration()
{
	return 13.0f / 24.0f;
}

void CCrowbar::PlayMissAnimation()
{
	switch ((m_iSwing++) % 3)
	{
	case 0:
		SendWeaponAnim(CROWBAR_ATTACK1MISS); break;
	case 1:
		SendWeaponAnim(CROWBAR_ATTACK2MISS); break;
	case 2:
		SendWeaponAnim(CROWBAR_ATTACK3MISS); break;
	}
}

void CCrowbar::PlayMissSound()
{
	EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/cbar_miss1.wav", 1, ATTN_NORM, 0, 94 + RANDOM_LONG(0, 0xF));
}

void CCrowbar::PlayHitAnimation()
{
	switch (((m_iSwing++) % 2) + 1)
	{
	case 0:
		SendWeaponAnim(CROWBAR_ATTACK1HIT); break;
	case 1:
		SendWeaponAnim(CROWBAR_ATTACK2HIT); break;
	case 2:
		SendWeaponAnim(CROWBAR_ATTACK3HIT); break;
	}
}

void CCrowbar::PlayHitBodySound()
{
	EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_ITEM, RANDOM_SOUND_ARRAY(pHitBodySounds), 1, ATTN_NORM);
}

void CCrowbar::PlayHitWorldSound(float fvolbar)
{
	EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_ITEM, RANDOM_SOUND_ARRAY(pHitWorldSounds), fvolbar, ATTN_NORM);
}

void FindHullIntersection( const Vector &vecSrc, TraceResult &tr, float *mins, float *maxs, edict_t *pEntity )
{
	int			i, j, k;
	float		distance;
	float		*minmaxs[2] = {mins, maxs};
	TraceResult tmpTrace;
	Vector		vecHullEnd = tr.vecEndPos;
	Vector		vecEnd;

	distance = 1e6f;

	vecHullEnd = vecSrc + ((vecHullEnd - vecSrc)*2);
	UTIL_TraceLine( vecSrc, vecHullEnd, dont_ignore_monsters, pEntity, &tmpTrace );
	if ( tmpTrace.flFraction < 1.0 )
	{
		tr = tmpTrace;
		return;
	}

	for ( i = 0; i < 2; i++ )
	{
		for ( j = 0; j < 2; j++ )
		{
			for ( k = 0; k < 2; k++ )
			{
				vecEnd.x = vecHullEnd.x + minmaxs[i][0];
				vecEnd.y = vecHullEnd.y + minmaxs[j][1];
				vecEnd.z = vecHullEnd.z + minmaxs[k][2];

				UTIL_TraceLine( vecSrc, vecEnd, dont_ignore_monsters, pEntity, &tmpTrace );
				if ( tmpTrace.flFraction < 1.0 )
				{
					float thisDistance = (tmpTrace.vecEndPos - vecSrc).Length();
					if ( thisDistance < distance )
					{
						tr = tmpTrace;
						distance = thisDistance;
					}
				}
			}
		}
	}
}
