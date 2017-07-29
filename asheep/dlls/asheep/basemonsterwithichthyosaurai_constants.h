#ifndef BASEMONSTERWITHICHTHYOSAURAI_CONSTANTS__
#define BASEMONSTERWITHICHTHYOSAURAI_CONSTANTS_H__


class CBaseMonsterWithIchthyosaurAIConstants
{
public:
	virtual float GetFirstTimeSpawnDefaultSpeed();
	virtual float GetFirstTimeSpawnMinimalSpeed();
	virtual float GetFirstTimeSpawnMaximalSpeed();
	virtual float GetFirstTimeSpawnMomentum();
	virtual float GetFirstTimeSpawnIdealDistanceAwayFromEnemy();
	virtual float GetFirstTimeSpawnMaximalDistanceAwayFromEnemy();

	virtual float GetSpeedAfterStopping();
	virtual float GetSpeedAfterEnteringIdleState();
	virtual float GetSpeedAfterEnteringAlertState();
	virtual float GetMaximalSpeedAfterEnteringCombatState();

	virtual float GetMaximalBuyoancyVerticalSpeedWhenDead();
	virtual float GetBuyoancyAccelerationWhenDead();

	virtual float GetCirclingOffset();
	virtual float GetCirclingRadiusAccelerationWhenEnemyIsFacing();
	virtual float GetCirclingRadiusDecelerationWhenEnemyIsOccluded();
	virtual float GetCirclingSpeedAccelerationWhenEnemyIsFacing();
	virtual float GetCirclingSpeedDecelerationWhenEnemyIsFacing();
	virtual float GetCirclingSpeedAccelerationWhenEnemyIsOccluded();
	virtual float GetCirclingMinimalDistanceAwayFromEnemy();

	virtual float GetPitchDifferenceTolerance();
	virtual float GetLowestAllowedPitchDifference();
	virtual float GetHighestAllowedPitchDifference();
	virtual float GetYawDifferenceTolerance();
	virtual float GetLowestAllowedYawDifference();
	virtual float GetHighestAllowedYawDifference();

	virtual float GetMinimalPitchAngle();
	virtual float GetMaximalPitchAngle();
	virtual float GetMinimalRollAngle();
	virtual float GetMaximalRollAngle();

	virtual Activity GetIdleActivity();
	virtual Activity GetRunActivity();
	virtual Activity GetWalkActivity();
	virtual float GetMinimalSpeedForRunActivity();

	virtual float GetSequenceGroundSpeedForFramerateRatio();
	virtual float GetDisplacementFactorWhenStrandedOnGround();
	virtual float GetProbeLength();

	virtual float GetEnemyChaseAcceleration();
};


#endif // BASEMONSTERWITHICHTHYOSAURAI_CONSTANTS_H__