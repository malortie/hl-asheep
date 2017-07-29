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
#include "asheep_globals.h"

#define IsCurrentMap(_str) FStrEq(STRING(gpGlobals->mapname), _str)

BOOL AsheepGlobals::IsMapT0a0()
{
	return IsCurrentMap("t0a0");
}

BOOL AsheepGlobals::IsMapT0a0a()
{
	return IsCurrentMap("t0a0a");
}

BOOL AsheepGlobals::IsMapT0a0b()
{
	return IsCurrentMap("t0a0b");
}

BOOL AsheepGlobals::IsMapT0a0b1()
{
	return IsCurrentMap("t0a0b1");
}

BOOL AsheepGlobals::IsMapT0a0b2()
{
	return IsCurrentMap("t0a0b2");
}

BOOL AsheepGlobals::IsMapT0a0c()
{
	return IsCurrentMap("t0a0c");
}

BOOL AsheepGlobals::IsMapT0a0d()
{
	return IsCurrentMap("t0a0d");
}

BOOL AsheepGlobals::IsMapPartOfTraining()
{
	return IsMapT0a0() ||
		IsMapT0a0a() ||
		IsMapT0a0b() ||
		IsMapT0a0b1() ||
		IsMapT0a0b2() ||
		IsMapT0a0c() ||
		IsMapT0a0d();
}

BOOL AsheepGlobals::IsMapAsmap00b()
{
	return IsCurrentMap("asmap00b");
}

BOOL AsheepGlobals::IsMapAsmap05()
{
	return IsCurrentMap("asmap05");
}

BOOL AsheepGlobals::IsMapAsmap05b()
{
	return IsCurrentMap("asmap05b");
}

BOOL AsheepGlobals::IsMapAsmap06c()
{
	return IsCurrentMap("asmap06c");
}

BOOL AsheepGlobals::IsMapAsmap08b()
{
	return IsCurrentMap("asmap08b");
}

BOOL AsheepGlobals::IsMapAsmap09c()
{
	return IsCurrentMap("asmap09c");
}

BOOL AsheepGlobals::IsMapAsmap10b()
{
	return IsCurrentMap("asmap10b");
}

BOOL AsheepGlobals::IsMapAsmap11c()
{
	return IsCurrentMap("asmap11c");
}

BOOL AsheepGlobals::IsMapIntro()
{
	return IsCurrentMap("asmapin");
}

BOOL AsheepGlobals::IsMapOutro()
{
	return IsMapAsmap11c();
}

BOOL AsheepGlobals::IsMapCredits()
{
	return IsCurrentMap("asmapend");
}

AsheepGlobals* GetAsheepGlobalsSingleton()
{
	static AsheepGlobals asheepGlobals;
	return &asheepGlobals;
}