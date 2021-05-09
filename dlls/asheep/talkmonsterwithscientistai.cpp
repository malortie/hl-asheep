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
//=========================================================
// monster template
//=========================================================
// UNDONE: Holster weapon?

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"talkmonster.h"
#include	"schedule.h"
#include	"defaultai.h"
#include	"scripted.h"
#include	"animation.h"
#include	"soundent.h"
#include	"talkmonsterwithscientistai.h"


enum
{
	SCHED_HIDE = LAST_TALKMONSTER_SCHEDULE + 1,
	SCHED_FEAR,
	SCHED_PANIC,
	SCHED_STARTLE,
	SCHED_TARGET_CHASE_SCARED,
	SCHED_TARGET_FACE_SCARED,
};

//=========================================================
// Monster's Anim Events Go Here
//=========================================================
#define		SCIENTIST_AE_HEAL		( 1 )
#define		SCIENTIST_AE_NEEDLEON	( 2 )
#define		SCIENTIST_AE_NEEDLEOFF	( 3 )

TYPEDESCRIPTION	CTalkMonsterWithScientistAI::m_SaveData[] =
{
	DEFINE_FIELD(CTalkMonsterWithScientistAI, m_painTime, FIELD_TIME),
	DEFINE_FIELD(CTalkMonsterWithScientistAI, m_fearTime, FIELD_TIME),
};

IMPLEMENT_SAVERESTORE(CTalkMonsterWithScientistAI, CTalkMonster);

//=========================================================
// AI Schedules Specific to this monster
//=========================================================
Task_t	tlFollow[] =
{
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_CANT_FOLLOW },	// If you fail, bail out of follow
	{ TASK_MOVE_TO_TARGET_RANGE,(float)128 },	// Move within 128 of target ent (client)
												//	{ TASK_SET_SCHEDULE,		(float)SCHED_TARGET_FACE },
};

Schedule_t	slFollow[] =
{
	{
		tlFollow,
		ARRAYSIZE(tlFollow),
		bits_COND_NEW_ENEMY |
		bits_COND_LIGHT_DAMAGE |
	bits_COND_HEAVY_DAMAGE |
	bits_COND_HEAR_SOUND,
	bits_SOUND_COMBAT |
	bits_SOUND_DANGER,
	"Follow"
	},
};

Task_t	tlFollowScared[] =
{
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_TARGET_CHASE },// If you fail, follow normally
	{ TASK_MOVE_TO_TARGET_RANGE_SCARED,(float)128 },	// Move within 128 of target ent (client)
														//	{ TASK_SET_SCHEDULE,		(float)SCHED_TARGET_FACE_SCARED },
};

Schedule_t	slFollowScared[] =
{
	{
		tlFollowScared,
		ARRAYSIZE(tlFollowScared),
		bits_COND_NEW_ENEMY |
		bits_COND_HEAR_SOUND |
	bits_COND_LIGHT_DAMAGE |
	bits_COND_HEAVY_DAMAGE,
	bits_SOUND_DANGER,
	"FollowScared"
	},
};

Task_t	tlFaceTargetScared[] =
{
	{ TASK_FACE_TARGET,			(float)0 },
	{ TASK_SET_ACTIVITY,		(float)ACT_CROUCHIDLE },
	{ TASK_SET_SCHEDULE,		(float)SCHED_TARGET_CHASE_SCARED },
};

Schedule_t	slFaceTargetScared[] =
{
	{
		tlFaceTargetScared,
		ARRAYSIZE(tlFaceTargetScared),
		bits_COND_HEAR_SOUND |
		bits_COND_NEW_ENEMY,
	bits_SOUND_DANGER,
	"FaceTargetScared"
	},
};

Task_t	tlStopFollowing[] =
{
	{ TASK_CANT_FOLLOW,		(float)0 },
};

Schedule_t	slStopFollowing[] =
{
	{
		tlStopFollowing,
		ARRAYSIZE(tlStopFollowing),
		0,
		0,
		"StopFollowing"
	},
};

Task_t	tlFaceTarget[] =
{
	{ TASK_STOP_MOVING,			(float)0 },
	{ TASK_FACE_TARGET,			(float)0 },
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_SET_SCHEDULE,		(float)SCHED_TARGET_CHASE },
};

Schedule_t	slFaceTarget[] =
{
	{
		tlFaceTarget,
		ARRAYSIZE(tlFaceTarget),
		bits_COND_CLIENT_PUSH |
		bits_COND_NEW_ENEMY |
	bits_COND_HEAR_SOUND,
	bits_SOUND_COMBAT |
	bits_SOUND_DANGER,
	"FaceTarget"
	},
};


Task_t	tlSciPanic[] =
{
	{ TASK_STOP_MOVING,			(float)0 },
	{ TASK_FACE_ENEMY,			(float)0 },
	{ TASK_SCREAM,				(float)0 },
	{ TASK_PLAY_SEQUENCE_FACE_ENEMY,		(float)ACT_EXCITED },	// This is really fear-stricken excitement
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
};

Schedule_t	slSciPanic[] =
{
	{
		tlSciPanic,
		ARRAYSIZE(tlSciPanic),
		0,
		0,
		"SciPanic"
	},
};


Task_t	tlIdleSciStand[] =
{
	{ TASK_STOP_MOVING,			0 },
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_WAIT,				(float)2 }, // repick IDLESTAND every two seconds.
	{ TASK_TLK_HEADRESET,		(float)0 }, // reset head position
};

Schedule_t	slIdleSciStand[] =
{
	{
		tlIdleSciStand,
		ARRAYSIZE(tlIdleSciStand),
		bits_COND_NEW_ENEMY |
		bits_COND_LIGHT_DAMAGE |
	bits_COND_HEAVY_DAMAGE |
	bits_COND_HEAR_SOUND |
	bits_COND_SMELL |
	bits_COND_CLIENT_PUSH |
	bits_COND_PROVOKED,

	bits_SOUND_COMBAT |// sound flags
					   //bits_SOUND_PLAYER		|
					   //bits_SOUND_WORLD		|
	bits_SOUND_DANGER |
	bits_SOUND_MEAT |// scents
	bits_SOUND_CARCASS |
	bits_SOUND_GARBAGE,
	"IdleSciStand"

	},
};


Task_t	tlScientistCover[] =
{
	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_PANIC },		// If you fail, just panic!
	{ TASK_STOP_MOVING,				(float)0 },
	{ TASK_FIND_COVER_FROM_ENEMY,	(float)0 },
	{ TASK_RUN_PATH_SCARED,			(float)0 },
	{ TASK_TURN_LEFT,				(float)179 },
	{ TASK_SET_SCHEDULE,			(float)SCHED_HIDE },
};

Schedule_t	slScientistCover[] =
{
	{
		tlScientistCover,
		ARRAYSIZE(tlScientistCover),
		bits_COND_NEW_ENEMY,
		0,
		"ScientistCover"
	},
};



Task_t	tlScientistHide[] =
{
	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_PANIC },		// If you fail, just panic!
	{ TASK_STOP_MOVING,				(float)0 },
	{ TASK_PLAY_SEQUENCE,			(float)ACT_CROUCH },
	{ TASK_SET_ACTIVITY,			(float)ACT_CROUCHIDLE },	// FIXME: This looks lame
	{ TASK_WAIT_RANDOM,				(float)10.0 },
};

Schedule_t	slScientistHide[] =
{
	{
		tlScientistHide,
		ARRAYSIZE(tlScientistHide),
		bits_COND_NEW_ENEMY |
		bits_COND_HEAR_SOUND |
	bits_COND_SEE_ENEMY |
	bits_COND_SEE_HATE |
	bits_COND_SEE_FEAR |
	bits_COND_SEE_DISLIKE,
	bits_SOUND_DANGER,
	"ScientistHide"
	},
};


Task_t	tlScientistStartle[] =
{
	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_PANIC },		// If you fail, just panic!
	{ TASK_RANDOM_SCREAM,			(float)0.3 },				// Scream 30% of the time
	{ TASK_STOP_MOVING,				(float)0 },
	{ TASK_PLAY_SEQUENCE_FACE_ENEMY,			(float)ACT_CROUCH },
	{ TASK_RANDOM_SCREAM,			(float)0.1 },				// Scream again 10% of the time
	{ TASK_PLAY_SEQUENCE_FACE_ENEMY,			(float)ACT_CROUCHIDLE },
	{ TASK_WAIT_RANDOM,				(float)1.0 },
};

Schedule_t	slScientistStartle[] =
{
	{
		tlScientistStartle,
		ARRAYSIZE(tlScientistStartle),
		bits_COND_NEW_ENEMY |
		bits_COND_SEE_ENEMY |
	bits_COND_SEE_HATE |
	bits_COND_SEE_FEAR |
	bits_COND_SEE_DISLIKE,
	0,
	"ScientistStartle"
	},
};



Task_t	tlFear[] =
{
	{ TASK_STOP_MOVING,				(float)0 },
	{ TASK_FACE_ENEMY,				(float)0 },
	{ TASK_SAY_FEAR,				(float)0 },
	//	{ TASK_PLAY_SEQUENCE,			(float)ACT_FEAR_DISPLAY		},
};

Schedule_t	slFear[] =
{
	{
		tlFear,
		ARRAYSIZE(tlFear),
		bits_COND_NEW_ENEMY,
		0,
		"Fear"
	},
};


DEFINE_CUSTOM_SCHEDULES(CTalkMonsterWithScientistAI)
{
	slFollow,
	slFaceTarget,
	slIdleSciStand,
	slFear,
	slScientistCover,
	slScientistHide,
	slScientistStartle,
	slStopFollowing,
	slSciPanic,
	slFollowScared,
	slFaceTargetScared,
};


IMPLEMENT_CUSTOM_SCHEDULES(CTalkMonsterWithScientistAI, CTalkMonster);


void CTalkMonsterWithScientistAI::DeclineFollowing(void)
{
	Talk(10);
	m_hTalkTarget = m_hEnemy;
	SpeakDeclineFollowing();
}

void CTalkMonsterWithScientistAI::Scream(void)
{
	if (FOkToSpeak())
	{
		Talk(10);
		m_hTalkTarget = m_hEnemy;
		SpeakScream();
	}
}


Activity CTalkMonsterWithScientistAI::GetStoppedActivity(void)
{
	if (m_hEnemy != NULL)
		return ACT_EXCITED;
	return CTalkMonster::GetStoppedActivity();
}


void CTalkMonsterWithScientistAI::StartTask(Task_t *pTask)
{
	switch (pTask->iTask)
	{
	case TASK_SCREAM:
		Scream();
		TaskComplete();
		break;

	case TASK_RANDOM_SCREAM:
		if (RANDOM_FLOAT(0, 1) < pTask->flData)
			Scream();
		TaskComplete();
		break;

	case TASK_SAY_FEAR:
		if (FOkToSpeak())
		{
			Talk(2);
			m_hTalkTarget = m_hEnemy;
			if (m_hEnemy->IsPlayer())
				SpeakFearPlayer();
			else
				SpeakFearEnemy();
		}
		TaskComplete();
		break;

	case TASK_RUN_PATH_SCARED:
		m_movementActivity = ACT_RUN_SCARED;
		break;

	case TASK_MOVE_TO_TARGET_RANGE_SCARED:
	{
		if ((m_hTargetEnt->pev->origin - pev->origin).Length() < 1)
			TaskComplete();
		else
		{
			m_vecMoveGoal = m_hTargetEnt->pev->origin;
			if (!MoveToTarget(ACT_WALK_SCARED, 0.5))
				TaskFail();
		}
	}
	break;

	default:
		CTalkMonster::StartTask(pTask);
		break;
	}
}

void CTalkMonsterWithScientistAI::RunTask(Task_t *pTask)
{
	switch (pTask->iTask)
	{
	case TASK_RUN_PATH_SCARED:
		if (MovementIsComplete())
			TaskComplete();
		if (RANDOM_LONG(0, 31) < 8)
			Scream();
		break;

	case TASK_MOVE_TO_TARGET_RANGE_SCARED:
	{
		if (RANDOM_LONG(0, 63)< 8)
			Scream();

		if (m_hEnemy == NULL)
		{
			TaskFail();
		}
		else
		{
			float distance;

			distance = (m_vecMoveGoal - pev->origin).Length2D();
			// Re-evaluate when you think your finished, or the target has moved too far
			if ((distance < pTask->flData) || (m_vecMoveGoal - m_hTargetEnt->pev->origin).Length() > pTask->flData * 0.5)
			{
				m_vecMoveGoal = m_hTargetEnt->pev->origin;
				distance = (m_vecMoveGoal - pev->origin).Length2D();
				FRefreshRoute();
			}

			// Set the appropriate activity based on an overlapping range
			// overlap the range to prevent oscillation
			if (distance < pTask->flData)
			{
				TaskComplete();
				RouteClear();		// Stop moving
			}
			else if (distance < 190 && m_movementActivity != ACT_WALK_SCARED)
				m_movementActivity = ACT_WALK_SCARED;
			else if (distance >= 270 && m_movementActivity != ACT_RUN_SCARED)
				m_movementActivity = ACT_RUN_SCARED;
		}
	}
	break;

	default:
		CTalkMonster::RunTask(pTask);
		break;
	}
}

//=========================================================
// Classify - indicates this monster's place in the 
// relationship table.
//=========================================================
int	CTalkMonsterWithScientistAI::Classify(void)
{
	return	CLASS_HUMAN_PASSIVE;
}


//=========================================================
// SetYawSpeed - allows each sequence to have a different
// turn rate associated with it.
//=========================================================
void CTalkMonsterWithScientistAI::SetYawSpeed(void)
{
	int ys;

	ys = 90;

	switch (m_Activity)
	{
	case ACT_IDLE:
		ys = 120;
		break;
	case ACT_WALK:
		ys = 180;
		break;
	case ACT_RUN:
		ys = 150;
		break;
	case ACT_TURN_LEFT:
	case ACT_TURN_RIGHT:
		ys = 120;
		break;
	}

	pev->yaw_speed = ys;
}

//=========================================================
// HandleAnimEvent - catches the monster-specific messages
// that occur when tagged animation frames are played.
//=========================================================
void CTalkMonsterWithScientistAI::HandleAnimEvent(MonsterEvent_t *pEvent)
{
	switch (pEvent->event)
	{
	case 0:
	default:
		CTalkMonster::HandleAnimEvent(pEvent);
	}
}

//=========================================================
// Spawn
//=========================================================
void CTalkMonsterWithScientistAI::Spawn(void)
{
	Precache();

	SetModel();
	UTIL_SetSize(pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX);

	pev->solid = SOLID_SLIDEBOX;
	pev->movetype = MOVETYPE_STEP;
	m_bloodColor = BLOOD_COLOR_RED;
	pev->health = GetFirstTimeSpawnHealth();
	pev->view_ofs = Vector(0, 0, 50);// position of the eyes relative to monster's origin.
	m_flFieldOfView = VIEW_FIELD_WIDE; // NOTE: we need a wide field of view so scientists will notice player and say hello
	m_MonsterState = MONSTERSTATE_NONE;

	//	m_flDistTooFar		= 256.0;

	m_afCapability = bits_CAP_HEAR | bits_CAP_TURN_HEAD | bits_CAP_OPEN_DOORS | bits_CAP_AUTO_DOORS | bits_CAP_USE;

	SetSkinAndBodygroups();

	MonsterInit();
	SetUse(&CTalkMonsterWithScientistAI::FollowerUse);
}

//=========================================================
// Precache - precaches all resources this monster needs
//=========================================================
void CTalkMonsterWithScientistAI::Precache(void)
{
	PrecacheModels();
	PrecacheSounds();

	// every new scientist must call this, otherwise
	// when a level is loaded, nobody will talk (time is reset to 0)
	TalkInit();

	CTalkMonster::Precache();
}

// Init talk data
void CTalkMonsterWithScientistAI::TalkInit()
{
	CTalkMonster::TalkInit();
	
	SetFriendsSpeechOrder();

	InitSentenceGroup();

	SetDefaultVoicePitch();
}

int CTalkMonsterWithScientistAI::TakeDamage(entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType)
{

	if (pevInflictor && pevInflictor->flags & FL_CLIENT)
	{
		Remember(bits_MEMORY_PROVOKED);
		StopFollowing(TRUE);
	}

	// make sure friends talk about it if player hurts scientist...
	return CTalkMonster::TakeDamage(pevInflictor, pevAttacker, flDamage, bitsDamageType);
}


//=========================================================
// ISoundMask - returns a bit mask indicating which types
// of sounds this monster regards. In the base class implementation,
// monsters care about all sounds, but no scents.
//=========================================================
int CTalkMonsterWithScientistAI::ISoundMask(void)
{
	return	bits_SOUND_WORLD |
		bits_SOUND_COMBAT |
		bits_SOUND_DANGER |
		bits_SOUND_PLAYER;
}

//=========================================================
// PainSound
//=========================================================
void CTalkMonsterWithScientistAI::PainSound(void)
{
	if (gpGlobals->time < m_painTime)
		return;

	m_painTime = gpGlobals->time + RANDOM_FLOAT(0.5, 0.75);

	DoPainSound();
}

//=========================================================
// DeathSound 
//=========================================================
void CTalkMonsterWithScientistAI::DeathSound(void)
{
	DoDeathSound();
}


void CTalkMonsterWithScientistAI::Killed(entvars_t *pevAttacker, int iGib)
{
	SetUse(NULL);
	CTalkMonster::Killed(pevAttacker, iGib);
}


void CTalkMonsterWithScientistAI::SetActivity(Activity newActivity)
{
	int	iSequence;

	iSequence = LookupActivity(newActivity);

	// Set to the desired anim, or default anim if the desired is not present
	if (iSequence == ACTIVITY_NOT_AVAILABLE)
		newActivity = ACT_IDLE;
	CTalkMonster::SetActivity(newActivity);
}


Schedule_t* CTalkMonsterWithScientistAI::GetScheduleOfType(int Type)
{
	Schedule_t *psched;

	switch (Type)
	{
		// Hook these to make a looping schedule
	case SCHED_TARGET_FACE:
		// call base class default so that scientist will talk
		// when 'used' 
		psched = CTalkMonster::GetScheduleOfType(Type);

		if (psched == slIdleStand)
			return slFaceTarget;	// override this for different target face behavior
		else
			return psched;

	case SCHED_TARGET_CHASE:
		return slFollow;

	case SCHED_CANT_FOLLOW:
		return slStopFollowing;

	case SCHED_PANIC:
		return slSciPanic;

	case SCHED_TARGET_CHASE_SCARED:
		return slFollowScared;

	case SCHED_TARGET_FACE_SCARED:
		return slFaceTargetScared;

	case SCHED_IDLE_STAND:
		// call base class default so that scientist will talk
		// when standing during idle
		psched = CTalkMonster::GetScheduleOfType(Type);

		if (psched == slIdleStand)
			return slIdleSciStand;
		else
			return psched;

	case SCHED_HIDE:
		return slScientistHide;

	case SCHED_STARTLE:
		return slScientistStartle;

	case SCHED_FEAR:
		return slFear;
	}

	return CTalkMonster::GetScheduleOfType(Type);
}

Schedule_t *CTalkMonsterWithScientistAI::GetSchedule(void)
{
	// so we don't keep calling through the EHANDLE stuff
	CBaseEntity *pEnemy = m_hEnemy;

	if (HasConditions(bits_COND_HEAR_SOUND))
	{
		CSound *pSound;
		pSound = PBestSound();

		ASSERT(pSound != NULL);
		if (pSound && (pSound->m_iType & bits_SOUND_DANGER))
			return GetScheduleOfType(SCHED_TAKE_COVER_FROM_BEST_SOUND);
	}

	switch (m_MonsterState)
	{
	case MONSTERSTATE_ALERT:
	case MONSTERSTATE_IDLE:
		if (pEnemy)
		{
			if (HasConditions(bits_COND_SEE_ENEMY))
				m_fearTime = gpGlobals->time;
			else if (DisregardEnemy(pEnemy))		// After 15 seconds of being hidden, return to alert
			{
				m_hEnemy = NULL;
				pEnemy = NULL;
			}
		}

		if (HasConditions(bits_COND_LIGHT_DAMAGE | bits_COND_HEAVY_DAMAGE))
		{
			// flinch if hurt
			return GetScheduleOfType(SCHED_SMALL_FLINCH);
		}

		// Cower when you hear something scary
		if (HasConditions(bits_COND_HEAR_SOUND))
		{
			CSound *pSound;
			pSound = PBestSound();

			ASSERT(pSound != NULL);
			if (pSound)
			{
				if (pSound->m_iType & (bits_SOUND_DANGER | bits_SOUND_COMBAT))
				{
					if (gpGlobals->time - m_fearTime > 3)	// Only cower every 3 seconds or so
					{
						m_fearTime = gpGlobals->time;		// Update last fear
						return GetScheduleOfType(SCHED_STARTLE);	// This will just duck for a second
					}
				}
			}
		}

		// Behavior for following the player
		if (IsFollowing())
		{
			if (!m_hTargetEnt->IsAlive())
			{
				// UNDONE: Comment about the recently dead player here?
				StopFollowing(FALSE);
				break;
			}

			int relationship = R_NO;

			// Nothing scary, just me and the player
			if (pEnemy != NULL)
				relationship = IRelationship(pEnemy);

			// UNDONE: Model fear properly, fix R_FR and add multiple levels of fear
			if (relationship != R_DL && relationship != R_HT)
			{
				// If I'm already close enough to my target
				if (TargetDistance() <= 128)
				{
					Schedule_t* healSchedule = GetHealSchedule(); 	// Heal opportunistically
					if (healSchedule != NULL)
						return healSchedule;
					if (HasConditions(bits_COND_CLIENT_PUSH))	// Player wants me to move
						return GetScheduleOfType(SCHED_MOVE_AWAY_FOLLOW);
				}
				return GetScheduleOfType(SCHED_TARGET_FACE);	// Just face and follow.
			}
			else	// UNDONE: When afraid, scientist won't move out of your way.  Keep This?  If not, write move away scared
			{
				if (HasConditions(bits_COND_NEW_ENEMY)) // I just saw something new and scary, react
					return GetScheduleOfType(SCHED_FEAR);					// React to something scary
				return GetScheduleOfType(SCHED_TARGET_FACE_SCARED);	// face and follow, but I'm scared!
			}
		}

		if (HasConditions(bits_COND_CLIENT_PUSH))	// Player wants me to move
			return GetScheduleOfType(SCHED_MOVE_AWAY);

		// try to say something about smells
		TrySmellTalk();
		break;
	case MONSTERSTATE_COMBAT:
		if (HasConditions(bits_COND_NEW_ENEMY))
			return slFear;					// Point and scream!
		if (HasConditions(bits_COND_SEE_ENEMY))
			return slScientistCover;		// Take Cover

		if (HasConditions(bits_COND_HEAR_SOUND))
			return slTakeCoverFromBestSound;	// Cower and panic from the scary sound!

		return slScientistCover;			// Run & Cower
		break;
	}

	return CTalkMonster::GetSchedule();
}

MONSTERSTATE CTalkMonsterWithScientistAI::GetIdealState(void)
{
	switch (m_MonsterState)
	{
	case MONSTERSTATE_ALERT:
	case MONSTERSTATE_IDLE:
		if (HasConditions(bits_COND_NEW_ENEMY))
		{
			if (IsFollowing())
			{
				int relationship = IRelationship(m_hEnemy);
				if (relationship != R_FR || relationship != R_HT && !HasConditions(bits_COND_LIGHT_DAMAGE | bits_COND_HEAVY_DAMAGE))
				{
					// Don't go to combat if you're following the player
					m_IdealMonsterState = MONSTERSTATE_ALERT;
					return m_IdealMonsterState;
				}
				StopFollowing(TRUE);
			}
		}
		else if (HasConditions(bits_COND_LIGHT_DAMAGE | bits_COND_HEAVY_DAMAGE))
		{
			// Stop following if you take damage
			if (IsFollowing())
				StopFollowing(TRUE);
		}
		break;

	case MONSTERSTATE_COMBAT:
	{
		CBaseEntity *pEnemy = m_hEnemy;
		if (pEnemy != NULL)
		{
			if (DisregardEnemy(pEnemy))		// After 15 seconds of being hidden, return to alert
			{
				// Strip enemy when going to alert
				m_IdealMonsterState = MONSTERSTATE_ALERT;
				m_hEnemy = NULL;
				return m_IdealMonsterState;
			}
			// Follow if only scared a little
			if (m_hTargetEnt != NULL)
			{
				m_IdealMonsterState = MONSTERSTATE_ALERT;
				return m_IdealMonsterState;
			}

			if (HasConditions(bits_COND_SEE_ENEMY))
			{
				m_fearTime = gpGlobals->time;
				m_IdealMonsterState = MONSTERSTATE_COMBAT;
				return m_IdealMonsterState;
			}

		}
	}
	break;
	}

	return CTalkMonster::GetIdealState();
}

int CTalkMonsterWithScientistAI::FriendNumber(int arrayNumber)
{
	static int array[3] = { 1, 2, 0 };
	if (arrayNumber < 3)
		return array[arrayNumber];
	return arrayNumber;
}

void CTalkMonsterWithScientistAI::SetSkinAndBodygroups()
{
	pev->skin = 0;
	pev->body = 0;
}

void CTalkMonsterWithScientistAI::DoPainSound()
{
	switch (RANDOM_LONG(0, 4))
	{
	case 0: EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "scientist/sci_pain1.wav", 1, ATTN_NORM, 0, GetVoicePitch()); break;
	case 1: EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "scientist/sci_pain2.wav", 1, ATTN_NORM, 0, GetVoicePitch()); break;
	case 2: EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "scientist/sci_pain3.wav", 1, ATTN_NORM, 0, GetVoicePitch()); break;
	case 3: EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "scientist/sci_pain4.wav", 1, ATTN_NORM, 0, GetVoicePitch()); break;
	case 4: EMIT_SOUND_DYN(ENT(pev), CHAN_VOICE, "scientist/sci_pain5.wav", 1, ATTN_NORM, 0, GetVoicePitch()); break;
	}
}

void CTalkMonsterWithScientistAI::DoDeathSound()
{
	PainSound();
}
