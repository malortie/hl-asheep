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
#include "weapon_util.h"

void CBasePlayerWeaponUtil::PrecacheCommonPistolSounds()
{
	PRECACHE_SOUND("items/9mmclip1.wav");
	PRECACHE_SOUND("items/9mmclip2.wav");

	PRECACHE_SOUND("weapons/pl_gun1.wav");//silenced handgun
	PRECACHE_SOUND("weapons/pl_gun2.wav");//silenced handgun
	PRECACHE_SOUND("weapons/pl_gun3.wav");//handgun
}

void CBasePlayerWeaponUtil::PrecacheCommonMP5Sounds()
{
	PRECACHE_SOUND("items/clipinsert1.wav");
	PRECACHE_SOUND("items/cliprelease1.wav");

	PRECACHE_SOUND("weapons/hks1.wav");// H to the K
	PRECACHE_SOUND("weapons/hks2.wav");// H to the K
	PRECACHE_SOUND("weapons/hks3.wav");// H to the K

	PRECACHE_SOUND("weapons/glauncher.wav");
	PRECACHE_SOUND("weapons/glauncher2.wav");
}

void CBasePlayerWeaponUtil::PrecacheCommonShotgunSounds()
{
	PRECACHE_SOUND("weapons/dbarrel1.wav");//shotgun
	PRECACHE_SOUND("weapons/sbarrel1.wav");//shotgun

	PRECACHE_SOUND("weapons/reload1.wav");	// shotgun reload
	PRECACHE_SOUND("weapons/reload3.wav");	// shotgun reload

	PRECACHE_SOUND("weapons/scock1.wav");	// cock gun
}

void CBasePlayerWeaponUtil::PrecacheCommonAmmoPickupSounds()
{
	PRECACHE_SOUND("items/9mmclip1.wav");
}

void CBasePlayerWeaponUtil::PrecacheCommonFireEmptySounds()
{
	PRECACHE_SOUND("weapons/357_cock1.wav");
}
