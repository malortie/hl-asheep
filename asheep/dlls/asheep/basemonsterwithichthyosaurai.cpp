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

#define SEARCH_RETRY	16

#define MIN_BLINK_FREQUENCY		3
#define MAX_BLINK_FREQUENCY		4

extern CGraph WorldGraph;

//=========================================================
// Monster's Anim Events Go Here
//=========================================================

// UNDONE: Save/restore here
TYPEDESCRIPTION	CBaseMonsterWithIchthyosaurAI::m_SaveData[] =
{
	DEFINE_FIELD(CBaseMonsterWithIchthyosaurAI, m_SaveVelocity, FIELD_VECTOR),
	DEFINE_FIELD(CBaseMonsterWithIchthyosaurAI, m_idealDist, FIELD_FLOAT),
	DEFINE_FIELD(CBaseMonsterWithIchthyosaurAI, m_flBlink, FIELD_FLOAT),
	DEFINE_FIELD(CBaseMonsterWithIchthyosaurAI, m_flEnemyTouched, FIELD_FLOAT),
	DEFINE_FIELD(CBaseMonsterWithIchthyosaurAI, m_bOnAttack, FIELD_BOOLEAN),
	DEFINE_FIELD(CBaseMonsterWithIchthyosaurAI, m_flMaxSpeed, FIELD_FLOAT),
	DEFINE_FIELD(CBaseMonsterWithIchthyosaurAI, m_flMinSpeed, FIELD_FLOAT),
	DEFINE_FIELD(CBaseMonsterWithIchthyosaurAI, m_flMaxDist, FIELD_FLOAT),
	DEFINE_FIELD(CBaseMonsterWithIchthyosaurAI, m_flNextAlert, FIELD_TIME),
};

IMPLEMENT_SAVERESTORE(CBaseMonsterWithIchthyosaurAI, CFlyingMonster);

//=========================================================
// monster-specific tasks and states
//=========================================================
enum
{
	TASK_ICHTHYOSAUR_CIRCLE_ENEMY = LAST_COMMON_TASK + 1,
	TASK_ICHTHYOSAUR_SWIM,
	TASK_ICHTHYOSAUR_FLOAT,
};

//=========================================================
// AI Schedules Specific to this monster
//=========================================================

static Task_t	tlSwimAround[] =
{
	{ TASK_SET_ACTIVITY,			(float)ACT_WALK },
	{ TASK_ICHTHYOSAUR_SWIM,		0.0 },
};

static Schedule_t	slSwimAround[] =
{
	{
		tlSwimAround,
		ARRAYSIZE(tlSwimAround),
		bits_COND_LIGHT_DAMAGE |
		bits_COND_HEAVY_DAMAGE |
	bits_COND_SEE_ENEMY |
	bits_COND_NEW_ENEMY |
	bits_COND_HEAR_SOUND,
	bits_SOUND_PLAYER |
	bits_SOUND_COMBAT,
	"SwimAround"
	},
};

static Task_t	tlSwimAgitated[] =
{
	{ TASK_STOP_MOVING,				(float)0 },
	{ TASK_SET_ACTIVITY,			(float)ACT_RUN },
	{ TASK_WAIT,					(float)2.0 },
};

static Schedule_t	slSwimAgitated[] =
{
	{
		tlSwimAgitated,
		ARRAYSIZE(tlSwimAgitated),
		0,
		0,
		"SwimAgitated"
	},
};


static Task_t	tlCircleEnemy[] =
{
	{ TASK_SET_ACTIVITY,			(float)ACT_WALK },
	{ TASK_ICHTHYOSAUR_CIRCLE_ENEMY, 0.0 },
};

static Schedule_t	slCircleEnemy[] =
{
	{
		tlCircleEnemy,
		ARRAYSIZE(tlCircleEnemy),
		bits_COND_NEW_ENEMY |
		bits_COND_LIGHT_DAMAGE |
	bits_COND_HEAVY_DAMAGE |
	bits_COND_CAN_MELEE_ATTACK1 |
	bits_COND_CAN_RANGE_ATTACK1,
	0,
	"CircleEnemy"
	},
};


Task_t tlTwitchDie[] =
{
	{ TASK_STOP_MOVING,			0 },
	{ TASK_SOUND_DIE,			(float)0 },
	{ TASK_DIE,					(float)0 },
	{ TASK_ICHTHYOSAUR_FLOAT,	(float)0 },
};

Schedule_t slTwitchDie[] =
{
	{
		tlTwitchDie,
		ARRAYSIZE(tlTwitchDie),
		0,
		0,
		"Die"
	},
};


DEFINE_CUSTOM_SCHEDULES(CBaseMonsterWithIchthyosaurAI)
{
	slSwimAround,
	slSwimAgitated,
	slCircleEnemy,
	slTwitchDie,
};
IMPLEMENT_CUSTOM_SCHEDULES(CBaseMonsterWithIchthyosaurAI, CFlyingMonster);

//=========================================================
// Classify - indicates this monster's place in the 
// relationship table.
//=========================================================
int	CBaseMonsterWithIchthyosaurAI::Classify(void)
{
	return	CLASS_ALIEN_MONSTER;
}


//=========================================================
// CheckMeleeAttack1
//=========================================================
BOOL CBaseMonsterWithIchthyosaurAI::CheckMeleeAttack1(float flDot, float flDist)
{
	if (flDot >= 0.7 && m_flEnemyTouched > gpGlobals->time - 0.2)
	{
		return TRUE;
	}
	return FALSE;
}

void CBaseMonsterWithIchthyosaurAI::BiteTouch(CBaseEntity *pOther)
{
	// bite if we hit who we want to eat
	if (pOther == m_hEnemy)
	{
		m_flEnemyTouched = gpGlobals->time;
		m_bOnAttack = TRUE;
	}
}

void CBaseMonsterWithIchthyosaurAI::CombatUse(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value)
{
	if (!ShouldToggle(useType, m_bOnAttack))
		return;

	if (m_bOnAttack)
	{
		m_bOnAttack = 0;
	}
	else
	{
		m_bOnAttack = 1;
	}
}

//=========================================================
// CheckRangeAttack1  - swim in for a chomp
//
//=========================================================
BOOL CBaseMonsterWithIchthyosaurAI::CheckRangeAttack1(float flDot, float flDist)
{
	if (flDot > -0.7 && (m_bOnAttack || (flDist <= 192 && m_idealDist <= 192)))
	{
		return TRUE;
	}

	return FALSE;
}

//=========================================================
// SetYawSpeed - allows each sequence to have a different
// turn rate associated with it.
//=========================================================
void CBaseMonsterWithIchthyosaurAI::SetYawSpeed(void)
{
	pev->yaw_speed = 100;
}



//=========================================================
// Killed - overrides CFlyingMonster.
//
void CBaseMonsterWithIchthyosaurAI::Killed(entvars_t *pevAttacker, int iGib)
{
	CBaseMonster::Killed(pevAttacker, iGib);
	pev->velocity = Vector(0, 0, 0);
}

void CBaseMonsterWithIchthyosaurAI::BecomeDead(void)
{
	pev->takedamage = DAMAGE_YES;// don't let autoaim aim at corpses.

								 // give the corpse half of the monster's original maximum health. 
	pev->health = pev->max_health / 2;
	pev->max_health = 5; // max_health now becomes a counter for how many blood decals the corpse can place.
}

#define ICHTHYOSAUR_AE_SHAKE_RIGHT 1
#define ICHTHYOSAUR_AE_SHAKE_LEFT  2


//=========================================================
// HandleAnimEvent - catches the monster-specific messages
// that occur when tagged animation frames are played.
//=========================================================
void CBaseMonsterWithIchthyosaurAI::HandleAnimEvent(MonsterEvent_t *pEvent)
{
	int bDidAttack = FALSE;
	switch (pEvent->event)
	{
	case ICHTHYOSAUR_AE_SHAKE_RIGHT:
	case ICHTHYOSAUR_AE_SHAKE_LEFT:
	{
		if (m_hEnemy != NULL && FVisible(m_hEnemy))
		{
			CBaseEntity *pHurt = m_hEnemy;

			if (m_flEnemyTouched < gpGlobals->time - 0.2 && (m_hEnemy->BodyTarget(pev->origin) - pev->origin).Length() >(32 + 16 + 32))
				break;

			Vector vecShootDir = ShootAtEnemy(pev->origin);
			UTIL_MakeAimVectors(pev->angles);

			if (DotProduct(vecShootDir, gpGlobals->v_forward) > 0.707)
			{
				m_bOnAttack = TRUE;
				pHurt->pev->punchangle.z = -18;
				pHurt->pev->punchangle.x = 5;
				pHurt->pev->velocity = pHurt->pev->velocity - gpGlobals->v_right * 300;
				if (pHurt->IsPlayer())
				{
					pHurt->pev->angles.x += RANDOM_FLOAT(-35, 35);
					pHurt->pev->angles.y += RANDOM_FLOAT(-90, 90);
					pHurt->pev->angles.z = 0;
					pHurt->pev->fixangle = TRUE;
				}
				pHurt->TakeDamage(pev, pev, GetDamageShake(), DMG_SLASH);
			}
		}
		BiteSound();

		bDidAttack = TRUE;
	}
	break;
	default:
		BaseClass::HandleAnimEvent(pEvent);
		break;
	}

	if (bDidAttack)
	{
		Vector vecSrc = pev->origin + gpGlobals->v_forward * 32;
		UTIL_Bubbles(vecSrc - Vector(8, 8, 8), vecSrc + Vector(8, 8, 8), 16);
	}
}

//=========================================================
// Spawn
//=========================================================
void CBaseMonsterWithIchthyosaurAI::Spawn()
{
	Precache();

	SetModel();
	SetMonsterCollisionBounds();

	pev->solid = SOLID_BBOX;
	pev->movetype = MOVETYPE_FLY;
	m_bloodColor = BLOOD_COLOR_GREEN;
	pev->health = GetFirstTimeSpawnHealth();
	SetDefaultViewOffset();
	m_flFieldOfView = VIEW_FIELD_WIDE;
	m_MonsterState = MONSTERSTATE_NONE;
	SetBits(pev->flags, FL_SWIM);
	SetFlyingSpeed(GetSharedConstants()->GetFirstTimeSpawnDefaultSpeed());
	SetFlyingMomentum(GetSharedConstants()->GetFirstTimeSpawnMomentum());	// Set momentum constant

	m_afCapability = bits_CAP_RANGE_ATTACK1 | bits_CAP_SWIM;

	MonsterInit();

	SetTouch(&CBaseMonsterWithIchthyosaurAI::BiteTouch);
	SetUse(&CBaseMonsterWithIchthyosaurAI::CombatUse);

	m_idealDist = GetSharedConstants()->GetFirstTimeSpawnIdealDistanceAwayFromEnemy(); //384;
	m_flMinSpeed = GetSharedConstants()->GetFirstTimeSpawnMinimalSpeed(); //80;
	m_flMaxSpeed = GetSharedConstants()->GetFirstTimeSpawnMaximalSpeed(); //300;
	m_flMaxDist = GetSharedConstants()->GetFirstTimeSpawnMaximalDistanceAwayFromEnemy(); //384;

	Vector Forward;
	UTIL_MakeVectorsPrivate(pev->angles, Forward, 0, 0);
	pev->velocity = m_flightSpeed * Forward.Normalize();
	m_SaveVelocity = pev->velocity;
}

//=========================================================
// Precache - precaches all resources this monster needs
//=========================================================
void CBaseMonsterWithIchthyosaurAI::Precache()
{
	PrecacheModels();
	PrecacheSounds();
}

//=========================================================
// GetSchedule
//=========================================================
Schedule_t* CBaseMonsterWithIchthyosaurAI::GetSchedule()
{
	// ALERT( at_console, "GetSchedule( )\n" );
	switch (m_MonsterState)
	{
	case MONSTERSTATE_IDLE:
		m_flightSpeed = GetSharedConstants()->GetSpeedAfterEnteringIdleState();
		return GetScheduleOfType(SCHED_IDLE_WALK);

	case MONSTERSTATE_ALERT:
		m_flightSpeed = GetSharedConstants()->GetSpeedAfterEnteringAlertState();
		return GetScheduleOfType(SCHED_IDLE_WALK);

	case MONSTERSTATE_COMBAT:
		m_flMaxSpeed = GetSharedConstants()->GetMaximalSpeedAfterEnteringCombatState();
		// eat them
		if (HasConditions(bits_COND_CAN_MELEE_ATTACK1))
		{
			return GetScheduleOfType(SCHED_MELEE_ATTACK1);
		}
		// chase them down and eat them
		if (HasConditions(bits_COND_CAN_RANGE_ATTACK1))
		{
			return GetScheduleOfType(SCHED_CHASE_ENEMY);
		}
		if (HasConditions(bits_COND_HEAVY_DAMAGE))
		{
			m_bOnAttack = TRUE;
		}
		if (pev->health < pev->max_health - 20)
		{
			m_bOnAttack = TRUE;
		}

		return GetScheduleOfType(SCHED_STANDOFF);
	}

	return BaseClass::GetSchedule();
}


//=========================================================
//=========================================================
Schedule_t* CBaseMonsterWithIchthyosaurAI::GetScheduleOfType(int Type)
{
	// ALERT( at_console, "GetScheduleOfType( %d ) %d\n", Type, m_bOnAttack );
	switch (Type)
	{
	case SCHED_IDLE_WALK:
		return slSwimAround;
	case SCHED_STANDOFF:
		return slCircleEnemy;
	case SCHED_FAIL:
		return slSwimAgitated;
	case SCHED_DIE:
		return slTwitchDie;
	case SCHED_CHASE_ENEMY:
		AttackSound();
	}

	return CBaseMonster::GetScheduleOfType(Type);
}



//=========================================================
// Start task - selects the correct activity and performs
// any necessary calculations to start the next task on the
// schedule.
//=========================================================
void CBaseMonsterWithIchthyosaurAI::StartTask(Task_t *pTask)
{
	switch (pTask->iTask)
	{
	case TASK_ICHTHYOSAUR_CIRCLE_ENEMY:
		break;
	case TASK_ICHTHYOSAUR_SWIM:
		break;
	case TASK_SMALL_FLINCH:
		if (m_idealDist > 128)
		{
			m_flMaxDist = 512;
			m_idealDist = 512;
		}
		else
		{
			m_bOnAttack = TRUE;
		}
		BaseClass::StartTask(pTask);
		break;

	case TASK_ICHTHYOSAUR_FLOAT:
		ResetEyeSkinAfterDeath();
		SetDeathSequence();
		break;

	default:
		BaseClass::StartTask(pTask);
		break;
		}
	}

void CBaseMonsterWithIchthyosaurAI::RunTask(Task_t *pTask)
{
	switch (pTask->iTask)
	{
	case TASK_ICHTHYOSAUR_CIRCLE_ENEMY:
		if (m_hEnemy == NULL)
		{
			TaskComplete();
		}
		else if (FVisible(m_hEnemy))
		{
			Vector vecFrom = m_hEnemy->EyePosition();

			Vector vecDelta = (pev->origin - vecFrom).Normalize();
			Vector vecSwim = CrossProduct(vecDelta, Vector(0, 0, 1)).Normalize();

			if (DotProduct(vecSwim, m_SaveVelocity) < 0)
				vecSwim = vecSwim * -1.0;

			Vector vecPos = vecFrom + vecDelta * m_idealDist + vecSwim * GetSharedConstants()->GetCirclingOffset(); //32;

			// ALERT( at_console, "vecPos %.0f %.0f %.0f\n", vecPos.x, vecPos.y, vecPos.z );

			TraceResult tr;

			UTIL_TraceHull(vecFrom, vecPos, ignore_monsters, large_hull, m_hEnemy->edict(), &tr);

			if (tr.flFraction > 0.5)
				vecPos = tr.vecEndPos;

			m_SaveVelocity = m_SaveVelocity * 0.8 + 0.2 * (vecPos - pev->origin).Normalize() * m_flightSpeed;

			// ALERT( at_console, "m_SaveVelocity %.2f %.2f %.2f\n", m_SaveVelocity.x, m_SaveVelocity.y, m_SaveVelocity.z );

			if (HasConditions(bits_COND_ENEMY_FACING_ME) && m_hEnemy->FVisible(this))
			{
				m_flNextAlert -= 0.1;

				if (m_idealDist < m_flMaxDist)
				{
					m_idealDist += GetSharedConstants()->GetCirclingRadiusAccelerationWhenEnemyIsFacing(); //4;
				}
				if (m_flightSpeed > m_flMinSpeed)
				{
					m_flightSpeed -= GetSharedConstants()->GetCirclingSpeedDecelerationWhenEnemyIsFacing(); //2;
				}
				else if (m_flightSpeed < m_flMinSpeed)
				{
					m_flightSpeed += GetSharedConstants()->GetCirclingSpeedAccelerationWhenEnemyIsFacing(); // 2;
				}
				if (m_flMinSpeed < m_flMaxSpeed)
				{
					m_flMinSpeed += 0.5;
				}
			}
			else
			{
				m_flNextAlert += 0.1;

				if (m_idealDist > GetSharedConstants()->GetCirclingMinimalDistanceAwayFromEnemy())
				{
					m_idealDist -= GetSharedConstants()->GetCirclingRadiusDecelerationWhenEnemyIsOccluded();
				}
				if (m_flightSpeed < m_flMaxSpeed)
				{
					m_flightSpeed += GetSharedConstants()->GetCirclingSpeedAccelerationWhenEnemyIsOccluded();
				}
			}
			// ALERT( at_console, "%.0f\n", m_idealDist );
		}
		else
		{
			m_flNextAlert = gpGlobals->time + 0.2;
		}

		if (m_flNextAlert < gpGlobals->time)
		{
			// ALERT( at_console, "AlertSound()\n");
			AlertSound();
			m_flNextAlert = gpGlobals->time + RANDOM_FLOAT(3, 5);
		}

		break;
	case TASK_ICHTHYOSAUR_SWIM:
		if (m_fSequenceFinished)
		{
			TaskComplete();
		}
		break;
	case TASK_DIE:
		if (m_fSequenceFinished)
		{
			pev->deadflag = DEAD_DEAD;

			TaskComplete();
		}
		break;

	case TASK_ICHTHYOSAUR_FLOAT:
		pev->angles.x = UTIL_ApproachAngle(0, pev->angles.x, 20);
		pev->velocity = pev->velocity * 0.8;
		if (pev->waterlevel > 1 && pev->velocity.z < GetSharedConstants()->GetMaximalBuyoancyVerticalSpeedWhenDead())
		{
			pev->velocity.z += GetSharedConstants()->GetBuyoancyAccelerationWhenDead();
		}
		else
		{
			pev->velocity.z -= GetSharedConstants()->GetBuyoancyAccelerationWhenDead();
		}
		// ALERT( at_console, "%f\n", pev->velocity.z );
		break;

	default:
		BaseClass::RunTask(pTask);
		break;
	}
}



float CBaseMonsterWithIchthyosaurAI::VectorToPitch(const Vector &vec)
{
	float pitch;
	if (vec.z == 0 && vec.x == 0)
		pitch = 0;
	else
	{
		pitch = (int)(atan2(vec.z, sqrt(vec.x*vec.x + vec.y*vec.y)) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}
	return pitch;
}

//=========================================================
void CBaseMonsterWithIchthyosaurAI::Move(float flInterval)
{
	BaseClass::Move(flInterval);
}

float CBaseMonsterWithIchthyosaurAI::FlPitchDiff(void)
{
	float	flPitchDiff;
	float	flCurrentPitch;

	flCurrentPitch = UTIL_AngleMod(pev->angles.z);

	if (flCurrentPitch == pev->idealpitch)
	{
		return 0;
	}

	flPitchDiff = pev->idealpitch - flCurrentPitch;

	if (pev->idealpitch > flCurrentPitch)
	{
		if (flPitchDiff >= 180)
			flPitchDiff = flPitchDiff - 360;
	}
	else
	{
		if (flPitchDiff <= -180)
			flPitchDiff = flPitchDiff + 360;
	}
	return flPitchDiff;
}

float CBaseMonsterWithIchthyosaurAI::ChangePitch(int speed)
{
	if (pev->movetype == MOVETYPE_FLY)
	{
		float diff = FlPitchDiff();
		float target = 0;
		if (m_IdealActivity != GetStoppedActivity())
		{
			if (diff < -GetSharedConstants()->GetPitchDifferenceTolerance())
				target = GetSharedConstants()->GetHighestAllowedPitchDifference();
			else if (diff > GetSharedConstants()->GetPitchDifferenceTolerance())
				target = GetSharedConstants()->GetLowestAllowedPitchDifference();
		}
		pev->angles.x = UTIL_Approach(target, pev->angles.x, 220.0 * 0.1);
	}
	return 0;
}

float CBaseMonsterWithIchthyosaurAI::ChangeYaw(int speed)
{
	if (pev->movetype == MOVETYPE_FLY)
	{
		float diff = FlYawDiff();
		float target = 0;

		if (m_IdealActivity != GetStoppedActivity())
		{
			if (diff < -GetSharedConstants()->GetYawDifferenceTolerance())
				target = GetSharedConstants()->GetHighestAllowedYawDifference();
			else if (diff > GetSharedConstants()->GetYawDifferenceTolerance())
				target = GetSharedConstants()->GetLowestAllowedYawDifference();
		}
		pev->angles.z = UTIL_Approach(target, pev->angles.z, 220.0 * 0.1);
	}
	return BaseClass::ChangeYaw(speed);
}


Activity CBaseMonsterWithIchthyosaurAI::GetStoppedActivity(void)
{
	if (pev->movetype != MOVETYPE_FLY)		// UNDONE: Ground idle here, IDLE may be something else
		return GetSharedConstants()->GetIdleActivity();
	return GetSharedConstants()->GetWalkActivity();
}

void CBaseMonsterWithIchthyosaurAI::MoveExecute(CBaseEntity *pTargetEnt, const Vector &vecDir, float flInterval)
{
	m_SaveVelocity = vecDir * m_flightSpeed;
}


void CBaseMonsterWithIchthyosaurAI::MonsterThink(void)
{
	BaseClass::MonsterThink();

	if (pev->deadflag == DEAD_NO)
	{
		if (m_MonsterState != MONSTERSTATE_SCRIPT)
		{
			Swim();

			// blink the eye
			if (CanBlink() && m_flBlink < gpGlobals->time && Blink())
			{
				m_flBlink = gpGlobals->time + RANDOM_FLOAT(MIN_BLINK_FREQUENCY, MAX_BLINK_FREQUENCY);
			}
		}
	}
}

void CBaseMonsterWithIchthyosaurAI::Stop(void)
{
	if (!m_bOnAttack)
		m_flightSpeed = GetSharedConstants()->GetSpeedAfterStopping();
}

void CBaseMonsterWithIchthyosaurAI::Swim()
{
	int retValue = 0;

	Vector start = pev->origin;

	Vector Angles;
	Vector Forward, Right, Up;

	if (FBitSet(pev->flags, FL_ONGROUND))
	{
		pev->angles.x = 0;
		pev->angles.y += RANDOM_FLOAT(-45, 45);
		ClearBits(pev->flags, FL_ONGROUND);

		Angles = Vector(-pev->angles.x, pev->angles.y, pev->angles.z);
		UTIL_MakeVectorsPrivate(Angles, Forward, Right, Up);

		pev->velocity = Forward * GetSharedConstants()->GetDisplacementFactorWhenStrandedOnGround() +
			Up * GetSharedConstants()->GetDisplacementFactorWhenStrandedOnGround();

		return;
	}

	if (m_bOnAttack && m_flightSpeed < m_flMaxSpeed)
	{
		m_flightSpeed += GetSharedConstants()->GetEnemyChaseAcceleration();
	}
	if (m_flightSpeed < GetSharedConstants()->GetMinimalSpeedForRunActivity())
	{
		if (m_IdealActivity == GetSharedConstants()->GetRunActivity())
			SetActivity(GetSharedConstants()->GetWalkActivity());
		if (m_IdealActivity == GetSharedConstants()->GetWalkActivity())
			pev->framerate = m_flightSpeed / GetSharedConstants()->GetSequenceGroundSpeedForFramerateRatio();
		// ALERT( at_console, "walk %.2f\n", pev->framerate );
	}
	else
	{
		if (m_IdealActivity == GetSharedConstants()->GetWalkActivity())
			SetActivity(GetSharedConstants()->GetRunActivity());
		if (m_IdealActivity == GetSharedConstants()->GetRunActivity())
			pev->framerate = m_flightSpeed / GetSharedConstants()->GetSequenceGroundSpeedForFramerateRatio();
		// ALERT( at_console, "run  %.2f\n", pev->framerate );
	}

	/*
	if (!m_pBeam)
	{
	m_pBeam = CBeam::BeamCreate( "sprites/laserbeam.spr", 80 );
	m_pBeam->PointEntInit( pev->origin + m_SaveVelocity, entindex( ) );
	m_pBeam->SetEndAttachment( 1 );
	m_pBeam->SetColor( 255, 180, 96 );
	m_pBeam->SetBrightness( 192 );
	}
	*/

	//#define PROBE_LENGTH 150
	Angles = UTIL_VecToAngles(m_SaveVelocity);
	Angles.x = -Angles.x;
	UTIL_MakeVectorsPrivate(Angles, Forward, Right, Up);

	Vector f, u, l, r, d;
	f = DoProbe(start + GetSharedConstants()->GetProbeLength() * Forward);
	r = DoProbe(start + GetSharedConstants()->GetProbeLength() / 3 * Forward + Right);
	l = DoProbe(start + GetSharedConstants()->GetProbeLength() / 3 * Forward - Right);
	u = DoProbe(start + GetSharedConstants()->GetProbeLength() / 3 * Forward + Up);
	d = DoProbe(start + GetSharedConstants()->GetProbeLength() / 3 * Forward - Up);

	Vector SteeringVector = f + r + l + u + d;
	m_SaveVelocity = (m_SaveVelocity + SteeringVector / 2).Normalize();

	Angles = Vector(-pev->angles.x, pev->angles.y, pev->angles.z);
	UTIL_MakeVectorsPrivate(Angles, Forward, Right, Up);
	// ALERT( at_console, "%f : %f\n", Angles.x, Forward.z );

	float flDot = DotProduct(Forward, m_SaveVelocity);
	if (flDot > 0.5)
		pev->velocity = m_SaveVelocity = m_SaveVelocity * m_flightSpeed;
	else if (flDot > 0)
		pev->velocity = m_SaveVelocity = m_SaveVelocity * m_flightSpeed * (flDot + 0.5);
	else
		pev->velocity = m_SaveVelocity = m_SaveVelocity * 80;

	// ALERT( at_console, "%.0f %.0f\n", m_flightSpeed, pev->velocity.Length() );


	// ALERT( at_console, "Steer %f %f %f\n", SteeringVector.x, SteeringVector.y, SteeringVector.z );

	/*
	m_pBeam->SetStartPos( pev->origin + pev->velocity );
	m_pBeam->RelinkBeam( );
	*/

	// ALERT( at_console, "speed %f\n", m_flightSpeed );

	Angles = UTIL_VecToAngles(m_SaveVelocity);

	// Smooth Pitch
	//
	if (Angles.x > 180)
		Angles.x = Angles.x - 360;
	pev->angles.x = UTIL_Approach(Angles.x, pev->angles.x, 50 * 0.1);
	if (pev->angles.x < GetSharedConstants()->GetMinimalPitchAngle()) pev->angles.x = GetSharedConstants()->GetMinimalPitchAngle();
	if (pev->angles.x > GetSharedConstants()->GetMaximalPitchAngle()) pev->angles.x = GetSharedConstants()->GetMaximalPitchAngle();

	// Smooth Yaw and generate Roll
	//
	float turn = 360;
	// ALERT( at_console, "Y %.0f %.0f\n", Angles.y, pev->angles.y );

	if (fabs(Angles.y - pev->angles.y) < fabs(turn))
	{
		turn = Angles.y - pev->angles.y;
	}
	if (fabs(Angles.y - pev->angles.y + 360) < fabs(turn))
	{
		turn = Angles.y - pev->angles.y + 360;
	}
	if (fabs(Angles.y - pev->angles.y - 360) < fabs(turn))
	{
		turn = Angles.y - pev->angles.y - 360;
	}

	float speed = m_flightSpeed * 0.1;

	// ALERT( at_console, "speed %.0f %f\n", turn, speed );
	if (fabs(turn) > speed)
	{
		if (turn < 0.0)
		{
			turn = -speed;
		}
		else
		{
			turn = speed;
		}
	}
	pev->angles.y += turn;
	pev->angles.z -= turn;
	pev->angles.y = fmod((pev->angles.y + 360.0), 360.0);

	static float yaw_adj;

	yaw_adj = yaw_adj * 0.8 + turn;

	// ALERT( at_console, "yaw %f : %f\n", turn, yaw_adj );

	SetBoneController(0, -yaw_adj / 4.0);

	// Roll Smoothing
	//
	turn = 360;
	if (fabs(Angles.z - pev->angles.z) < fabs(turn))
	{
		turn = Angles.z - pev->angles.z;
	}
	if (fabs(Angles.z - pev->angles.z + 360) < fabs(turn))
	{
		turn = Angles.z - pev->angles.z + 360;
	}
	if (fabs(Angles.z - pev->angles.z - 360) < fabs(turn))
	{
		turn = Angles.z - pev->angles.z - 360;
	}
	speed = m_flightSpeed / 2 * 0.1;
	if (fabs(turn) < speed)
	{
		pev->angles.z += turn;
	}
	else
	{
		if (turn < 0.0)
		{
			pev->angles.z -= speed;
		}
		else
		{
			pev->angles.z += speed;
		}
	}
	if (pev->angles.z < GetSharedConstants()->GetMinimalRollAngle()) pev->angles.z = GetSharedConstants()->GetMinimalRollAngle(); //-20;
	if (pev->angles.z > GetSharedConstants()->GetMaximalRollAngle()) pev->angles.z = GetSharedConstants()->GetMaximalRollAngle(); //20;

	UTIL_MakeVectorsPrivate(Vector(-Angles.x, Angles.y, Angles.z), Forward, Right, Up);

	// UTIL_MoveToOrigin ( ENT(pev), pev->origin + Forward * speed, speed, MOVE_STRAFE );
}


Vector CBaseMonsterWithIchthyosaurAI::DoProbe(const Vector &Probe)
{
	Vector WallNormal = Vector(0, 0, -1); // WATER normal is Straight Down for fish.
	float frac;
	BOOL bBumpedSomething = ProbeZ(pev->origin, Probe, &frac);

	TraceResult tr;
	TRACE_MONSTER_HULL(edict(), pev->origin, Probe, dont_ignore_monsters, edict(), &tr);
	if (tr.fAllSolid || tr.flFraction < 0.99)
	{
		if (tr.flFraction < 0.0) tr.flFraction = 0.0;
		if (tr.flFraction > 1.0) tr.flFraction = 1.0;
		if (tr.flFraction < frac)
		{
			frac = tr.flFraction;
			bBumpedSomething = TRUE;
			WallNormal = tr.vecPlaneNormal;
		}
	}

	if (bBumpedSomething && (m_hEnemy == NULL || tr.pHit != m_hEnemy->edict()))
	{
		Vector ProbeDir = Probe - pev->origin;

		Vector NormalToProbeAndWallNormal = CrossProduct(ProbeDir, WallNormal);
		Vector SteeringVector = CrossProduct(NormalToProbeAndWallNormal, ProbeDir);

		float SteeringForce = m_flightSpeed * (1 - frac) * (DotProduct(WallNormal.Normalize(), m_SaveVelocity.Normalize()));
		if (SteeringForce < 0.0)
		{
			SteeringForce = -SteeringForce;
		}
		SteeringVector = SteeringForce * SteeringVector.Normalize();

		return SteeringVector;
	}
	return Vector(0, 0, 0);
}

void CBaseMonsterWithIchthyosaurAI::PrecacheModels()
{
}

void CBaseMonsterWithIchthyosaurAI::PrecacheSounds()
{
}

void CBaseMonsterWithIchthyosaurAI::SetSkinAndBodyGroups()
{
}

void CBaseMonsterWithIchthyosaurAI::ResetEyeSkinAfterDeath()
{
}

void CBaseMonsterWithIchthyosaurAI::SetDeathSequence()
{
}

BOOL CBaseMonsterWithIchthyosaurAI::CanBlink() const
{
	return FALSE;
}

BOOL CBaseMonsterWithIchthyosaurAI::Blink()
{
	return TRUE;
}

CBaseMonsterWithIchthyosaurAIConstants* CBaseMonsterWithIchthyosaurAI::GetSharedConstants() const
{
	static CBaseMonsterWithIchthyosaurAIConstants constantsSingleton;
	return &constantsSingleton;
}