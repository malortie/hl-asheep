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
#include	"monsters.h"
#include	"schedule.h"
#include    "flyingmonster.h"
#include	"nodes.h"
#include	"soundent.h"
#include	"animation.h"
#include	"effects.h"
#include	"weapons.h"
#include	"basemonsterwithichthyosaurai.h"

float CBaseMonsterWithIchthyosaurAIConstants::GetFirstTimeSpawnDefaultSpeed()
{
	return 150;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetFirstTimeSpawnMinimalSpeed()
{
	return 80;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetFirstTimeSpawnMaximalSpeed()
{
	return 300;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetFirstTimeSpawnMomentum()
{
	return 2.5f;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetFirstTimeSpawnIdealDistanceAwayFromEnemy()
{
	return 384;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetFirstTimeSpawnMaximalDistanceAwayFromEnemy()
{
	return 384;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetSpeedAfterStopping()
{
	return 80;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetSpeedAfterEnteringIdleState()
{
	return 80;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetSpeedAfterEnteringAlertState()
{
	return 150;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetMaximalSpeedAfterEnteringCombatState()
{
	return 400;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetMaximalBuyoancyVerticalSpeedWhenDead()
{
	return 64;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetBuyoancyAccelerationWhenDead()
{
	return 8;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetCirclingOffset()
{
	return 32;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetCirclingRadiusAccelerationWhenEnemyIsFacing()
{
	return 4;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetCirclingRadiusDecelerationWhenEnemyIsOccluded()
{
	return 4;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetCirclingSpeedAccelerationWhenEnemyIsFacing()
{
	return 2;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetCirclingSpeedDecelerationWhenEnemyIsFacing()
{
	return 2;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetCirclingSpeedAccelerationWhenEnemyIsOccluded()
{
	return 4;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetCirclingMinimalDistanceAwayFromEnemy()
{
	return 128;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetPitchDifferenceTolerance()
{
	return 20;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetLowestAllowedPitchDifference()
{
	return -45;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetHighestAllowedPitchDifference()
{
	return 45;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetYawDifferenceTolerance()
{
	return 20;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetLowestAllowedYawDifference()
{
	return -20;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetHighestAllowedYawDifference()
{
	return 20;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetMinimalPitchAngle()
{
	return -80;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetMaximalPitchAngle()
{
	return 80;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetMinimalRollAngle()
{
	return -20;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetMaximalRollAngle()
{
	return 20;
}

Activity CBaseMonsterWithIchthyosaurAIConstants::GetIdleActivity()
{
	return ACT_IDLE;
}

Activity CBaseMonsterWithIchthyosaurAIConstants::GetRunActivity()
{
	return ACT_RUN;
}

Activity CBaseMonsterWithIchthyosaurAIConstants::GetWalkActivity()
{
	return ACT_WALK;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetMinimalSpeedForRunActivity()
{
	return 180;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetSequenceGroundSpeedForFramerateRatio()
{
	return 150;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetDisplacementFactorWhenStrandedOnGround()
{
	return 200;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetProbeLength()
{
	return 150;
}

float CBaseMonsterWithIchthyosaurAIConstants::GetEnemyChaseAcceleration()
{
	return 40;
}
