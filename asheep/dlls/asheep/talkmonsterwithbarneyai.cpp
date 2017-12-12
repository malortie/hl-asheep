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
#include	"weapons.h"
#include	"soundent.h"
#include	"talkmonsterwithbarneyai.h"

#define DEFAULT_MELEE_DISTANCE 60

//=========================================================
// monster-specific schedule types
//=========================================================
enum
{
	SCHED_BARNEY_COVER_AND_RELOAD = LAST_COMMON_SCHEDULE + 1,
};

//=========================================================
// Monster's Anim Events Go Here
//=========================================================
// first flag is barney dying for scripted sequences?
#define		BARNEY_AE_DRAW		( 2 )
#define		BARNEY_AE_SHOOT		( 3 )
#define		BARNEY_AE_HOLSTER	( 4 )
#define		BARNEY_AE_RELOAD	( 5 )

#define	BARNEY_BODY_GUNHOLSTERED	0
#define	BARNEY_BODY_GUNDRAWN		1
#define BARNEY_BODY_GUNGONE			2

TYPEDESCRIPTION	CTalkMonsterWithBarneyAI::m_SaveData[] = 
{
	DEFINE_FIELD( CTalkMonsterWithBarneyAI, m_fGunDrawn, FIELD_BOOLEAN ),
	DEFINE_FIELD( CTalkMonsterWithBarneyAI, m_painTime, FIELD_TIME ),
	DEFINE_FIELD( CTalkMonsterWithBarneyAI, m_checkAttackTime, FIELD_TIME ),
	DEFINE_FIELD( CTalkMonsterWithBarneyAI, m_lastAttackCheck, FIELD_BOOLEAN ),
	DEFINE_FIELD( CTalkMonsterWithBarneyAI, m_flPlayerDamage, FIELD_FLOAT ),
	DEFINE_FIELD( CTalkMonsterWithBarneyAI, m_cClipSize, FIELD_INTEGER ),
};

IMPLEMENT_SAVERESTORE( CTalkMonsterWithBarneyAI, CTalkMonster );

//=========================================================
// AI Schedules Specific to this monster
//=========================================================
Task_t	tlBaFollow[] =
{
	{ TASK_MOVE_TO_TARGET_RANGE,(float)128		},	// Move within 128 of target ent (client)
	{ TASK_SET_SCHEDULE,		(float)SCHED_TARGET_FACE },
};

Schedule_t	slBaFollow[] =
{
	{
		tlBaFollow,
		ARRAYSIZE ( tlBaFollow ),
		bits_COND_NEW_ENEMY		|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_HEAR_SOUND |
		bits_COND_PROVOKED,
		bits_SOUND_DANGER,
		"Follow"
	},
};

//=========================================================
// BarneyDraw- much better looking draw schedule for when
// barney knows who he's gonna attack.
//=========================================================
Task_t	tlBarneyEnemyDraw[] =
{
	{ TASK_STOP_MOVING,					0				},
	{ TASK_FACE_ENEMY,					0				},
	{ TASK_PLAY_SEQUENCE_FACE_ENEMY,	(float) ACT_ARM },
};

Schedule_t slBarneyEnemyDraw[] = 
{
	{
		tlBarneyEnemyDraw,
		ARRAYSIZE ( tlBarneyEnemyDraw ),
		0,
		0,
		"Barney Enemy Draw"
	}
};

Task_t	tlBaFaceTarget[] =
{
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_FACE_TARGET,			(float)0		},
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_SET_SCHEDULE,		(float)SCHED_TARGET_CHASE },
};

Schedule_t	slBaFaceTarget[] =
{
	{
		tlBaFaceTarget,
		ARRAYSIZE ( tlBaFaceTarget ),
		bits_COND_CLIENT_PUSH	|
		bits_COND_NEW_ENEMY		|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_HEAR_SOUND |
		bits_COND_PROVOKED,
		bits_SOUND_DANGER,
		"FaceTarget"
	},
};


Task_t	tlIdleBaStand[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_WAIT,				(float)2		}, // repick IDLESTAND every two seconds.
	{ TASK_TLK_HEADRESET,		(float)0		}, // reset head position
};

Schedule_t	slIdleBaStand[] =
{
	{ 
		tlIdleBaStand,
		ARRAYSIZE ( tlIdleBaStand ), 
		bits_COND_NEW_ENEMY		|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_HEAR_SOUND	|
		bits_COND_SMELL			|
		bits_COND_PROVOKED,

		bits_SOUND_COMBAT		|// sound flags - change these, and you'll break the talking code.
		//bits_SOUND_PLAYER		|
		//bits_SOUND_WORLD		|
		
		bits_SOUND_DANGER		|
		bits_SOUND_MEAT			|// scents
		bits_SOUND_CARCASS		|
		bits_SOUND_GARBAGE,
		"IdleStand"
	},
};

//=========================================================
// Grunt reload schedule
//=========================================================
Task_t	tlBaHideReload[] =
{
	{ TASK_STOP_MOVING,				(float)0 },
	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_RELOAD },
	{ TASK_FIND_COVER_FROM_ENEMY,	(float)0 },
	{ TASK_RUN_PATH,				(float)0 },
	{ TASK_WAIT_FOR_MOVEMENT,		(float)0 },
	{ TASK_REMEMBER,				(float)bits_MEMORY_INCOVER },
	{ TASK_FACE_ENEMY,				(float)0 },
	{ TASK_PLAY_SEQUENCE,			(float)ACT_RELOAD },
};

Schedule_t slBaHideReload[] =
{
	{
		tlBaHideReload,
		ARRAYSIZE(tlBaHideReload),
		bits_COND_HEAVY_DAMAGE |
		bits_COND_HEAR_SOUND,

	bits_SOUND_DANGER,
	"BarneyHideReload"
	}
};

DEFINE_CUSTOM_SCHEDULES( CTalkMonsterWithBarneyAI )
{
	slBaFollow,
	slBarneyEnemyDraw,
	slBaFaceTarget,
	slIdleBaStand,
	slBaHideReload,
};

IMPLEMENT_CUSTOM_SCHEDULES( CTalkMonsterWithBarneyAI, CTalkMonster );

void CTalkMonsterWithBarneyAI :: StartTask( Task_t *pTask )
{
	switch (pTask->iTask)
	{
	case TASK_RELOAD:
		m_IdealActivity = ACT_RELOAD;
		break;
	default:
		CTalkMonster::StartTask(pTask);
		break;
	}
}

void CTalkMonsterWithBarneyAI :: RunTask( Task_t *pTask )
{
	switch ( pTask->iTask )
	{
	case TASK_RANGE_ATTACK1:
		if (m_hEnemy != NULL && (m_hEnemy->IsPlayer()))
		{
			pev->framerate = 1.5;
		}
		CTalkMonster::RunTask( pTask );
		break;
	default:
		CTalkMonster::RunTask( pTask );
		break;
	}
}

//=========================================================
// ISoundMask - returns a bit mask indicating which types
// of sounds this monster regards. 
//=========================================================
int CTalkMonsterWithBarneyAI :: ISoundMask ( void) 
{
	return	bits_SOUND_WORLD	|
			bits_SOUND_COMBAT	|
			bits_SOUND_CARCASS	|
			bits_SOUND_MEAT		|
			bits_SOUND_GARBAGE	|
			bits_SOUND_DANGER	|
			bits_SOUND_PLAYER;
}

//=========================================================
// Classify - indicates this monster's place in the 
// relationship table.
//=========================================================
int	CTalkMonsterWithBarneyAI :: Classify ( void )
{
	return	CLASS_PLAYER_ALLY;
}

//=========================================================
// ALertSound - barney says "Freeze!"
//=========================================================
void CTalkMonsterWithBarneyAI :: AlertSound( void )
{
	if ( m_hEnemy != NULL )
	{
		if ( FOkToSpeak() )
		{
			SpeakAttack();
		}
	}

}
//=========================================================
// SetYawSpeed - allows each sequence to have a different
// turn rate associated with it.
//=========================================================
void CTalkMonsterWithBarneyAI :: SetYawSpeed ( void )
{
	int ys;

	ys = 0;

	switch ( m_Activity )
	{
	case ACT_IDLE:		
		ys = 70;
		break;
	case ACT_WALK:
		ys = 70;
		break;
	case ACT_RUN:
		ys = 90;
		break;
	default:
		ys = 70;
		break;
	}

	pev->yaw_speed = ys;
}

//=========================================================
// CheckMeleeAttack1
//=========================================================
BOOL CTalkMonsterWithBarneyAI::CheckMeleeAttack1(float flDot, float flDist)
{
	if (HasConditions(bits_COND_SEE_ENEMY) && flDist <= GetMeleeRange() && flDot >= 0.6 && m_hEnemy != NULL)
	{
		return TRUE;
	}
	return FALSE;
}

//=========================================================
// CheckRangeAttack1
//=========================================================
BOOL CTalkMonsterWithBarneyAI :: CheckRangeAttack1 ( float flDot, float flDist )
{
	if ( flDist <= 1024 && flDot >= 0.5 )
	{
		if ( gpGlobals->time > m_checkAttackTime )
		{
			TraceResult tr;
			
			Vector shootOrigin = pev->origin + Vector( 0, 0, 55 );
			CBaseEntity *pEnemy = m_hEnemy;
			Vector shootTarget = ( (pEnemy->BodyTarget( shootOrigin ) - pEnemy->pev->origin) + m_vecEnemyLKP );
			UTIL_TraceLine( shootOrigin, shootTarget, dont_ignore_monsters, ENT(pev), &tr );
			m_checkAttackTime = gpGlobals->time + 1;
			if ( tr.flFraction == 1.0 || (tr.pHit != NULL && CBaseEntity::Instance(tr.pHit) == pEnemy) )
				m_lastAttackCheck = TRUE;
			else
				m_lastAttackCheck = FALSE;
			m_checkAttackTime = gpGlobals->time + 1.5;
		}
		return m_lastAttackCheck;
	}
	return FALSE;
}


//=========================================================
// BarneyFirePistol - shoots one round from the pistol at
// the enemy barney is facing.
//=========================================================
void CTalkMonsterWithBarneyAI :: BarneyFirePistol ( void )
{
	Vector vecShootOrigin;

	UTIL_MakeVectors(pev->angles);
	vecShootOrigin = pev->origin + Vector( 0, 0, 55 );
	Vector vecShootDir = ShootAtEnemy( vecShootOrigin );

	Vector angDir = UTIL_VecToAngles( vecShootDir );
	SetBlending( 0, angDir.x );
	pev->effects = EF_MUZZLEFLASH;

	FireBullets(1, vecShootOrigin, vecShootDir, VECTOR_CONE_2DEGREES, 1024, BULLET_MONSTER_9MM );
	
	GunFireSound();

	CSoundEnt::InsertSound ( bits_SOUND_COMBAT, pev->origin, 384, 0.3 );

	if (IsAllowedToReloadAndUseReloadSchedule())
		m_cAmmoLoaded--; // take away a bullet!
}
		
//=========================================================
// HandleAnimEvent - catches the monster-specific messages
// that occur when tagged animation frames are played.
//
// Returns number of events handled, 0 if none.
//=========================================================
void CTalkMonsterWithBarneyAI :: HandleAnimEvent( MonsterEvent_t *pEvent )
{
	switch( pEvent->event )
	{
	case BARNEY_AE_SHOOT:
		BarneyFirePistol();
		break;

	case BARNEY_AE_DRAW:
		// barney's bodygroup switches here so he can pull gun from holster
		pev->body = BARNEY_BODY_GUNDRAWN;
		m_fGunDrawn = TRUE;
		break;

	case BARNEY_AE_HOLSTER:
		// change bodygroup to replace gun in holster
		pev->body = BARNEY_BODY_GUNHOLSTERED;
		m_fGunDrawn = FALSE;
		break;

	case BARNEY_AE_RELOAD:
		Reload();
		break;

	default:
		CTalkMonster::HandleAnimEvent( pEvent );
	}
}

//=========================================================
// Spawn
//=========================================================
void CTalkMonsterWithBarneyAI :: Spawn()
{
	Precache( );

	SetModel();
	UTIL_SetSize(pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX);

	pev->solid			= SOLID_SLIDEBOX;
	pev->movetype		= MOVETYPE_STEP;
	m_bloodColor		= BLOOD_COLOR_RED;
	pev->health			= GetFirstTimeSpawnHealth();
	pev->view_ofs		= Vector ( 0, 0, 50 );// position of the eyes relative to monster's origin.
	m_flFieldOfView		= VIEW_FIELD_WIDE; // NOTE: we need a wide field of view so npc will notice player and say hello
	m_MonsterState		= MONSTERSTATE_NONE;

	pev->body			= 0; // gun in holster
	m_fGunDrawn			= FALSE;

	m_afCapability		= bits_CAP_HEAR | bits_CAP_TURN_HEAD | bits_CAP_DOORS_GROUP;

	SetSkinAndBodyGroups();

	SetDefaultVoicePitch();

	SetWeaponClipSize();
	m_cAmmoLoaded = m_cClipSize;

	MonsterInit();
	SetUse( &CTalkMonsterWithBarneyAI::FollowerUse );
}

//=========================================================
// Precache - precaches all resources this monster needs
//=========================================================
void CTalkMonsterWithBarneyAI :: Precache()
{
	PrecacheModels();
	PrecacheSounds();
	
	// every new barney must call this, otherwise
	// when a level is loaded, nobody will talk (time is reset to 0)
	TalkInit();
	CTalkMonster::Precache();
}	

// Init talk data
void CTalkMonsterWithBarneyAI :: TalkInit()
{
	CTalkMonster::TalkInit();

	// scientists speach group names (group names are in sentences.txt)
	InitSentenceGroup();
}


static BOOL IsFacing( entvars_t *pevTest, const Vector &reference )
{
	Vector vecDir = (reference - pevTest->origin);
	vecDir.z = 0;
	vecDir = vecDir.Normalize();
	Vector forward, angle;
	angle = pevTest->v_angle;
	angle.x = 0;
	UTIL_MakeVectorsPrivate( angle, forward, NULL, NULL );
	// He's facing me, he meant it
	if ( DotProduct( forward, vecDir ) > 0.96 )	// +/- 15 degrees or so
	{
		return TRUE;
	}
	return FALSE;
}


int CTalkMonsterWithBarneyAI :: TakeDamage( entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType)
{
	// make sure friends talk about it if player hurts talkmonsters...
	int ret = CTalkMonster::TakeDamage(pevInflictor, pevAttacker, flDamage, bitsDamageType);
	if ( !IsAlive() || pev->deadflag == DEAD_DYING )
		return ret;

	if ( m_MonsterState != MONSTERSTATE_PRONE && (pevAttacker->flags & FL_CLIENT) )
	{
		m_flPlayerDamage += flDamage;

		// This is a heurstic to determine if the player intended to harm me
		// If I have an enemy, we can't establish intent (may just be crossfire)
		if ( m_hEnemy == NULL )
		{
			// If the player was facing directly at me, or I'm already suspicious, get mad
			if ( (m_afMemory & bits_MEMORY_SUSPICIOUS) || IsFacing( pevAttacker, pev->origin ) )
			{
				// Alright, now I'm pissed!
				SpeakAlrightIAmPissed();

				Remember( bits_MEMORY_PROVOKED );
				StopFollowing( TRUE );
			}
			else
			{
				// Hey, be careful with that
				SpeakHeyBeCareful();
				Remember( bits_MEMORY_SUSPICIOUS );
			}
		}
		else if ( !(m_hEnemy->IsPlayer()) && pev->deadflag == DEAD_NO )
		{
			SpeakIHaveBeenShot();
		}
	}

	// When in a script, allow to check for AI condition,
	// so that security guards playing sequences can still
	// trigger events such as player_loadsaved.
	if (m_MonsterState == MONSTERSTATE_SCRIPT)
		FCheckAITrigger();

	return ret;
}

	
//=========================================================
// PainSound
//=========================================================
void CTalkMonsterWithBarneyAI :: PainSound ( void )
{
	if (gpGlobals->time < m_painTime)
		return;
	
	m_painTime = gpGlobals->time + RANDOM_FLOAT(0.5, 0.75);

	DoPainSound();
}

//=========================================================
// DeathSound 
//=========================================================
void CTalkMonsterWithBarneyAI :: DeathSound ( void )
{
	DoDeathSound();
}

void CTalkMonsterWithBarneyAI::TraceAttack(entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType)
{
	GetSharedUtils()->TraceAttackNormal(this, pevAttacker,
		vecDir, ptr, bitsDamageType, flDamage);

	CTalkMonster::TraceAttack(pevAttacker, flDamage, vecDir, ptr, bitsDamageType);
}

void CTalkMonsterWithBarneyAI::Killed( entvars_t *pevAttacker, int iGib )
{
	if ( pev->body < BARNEY_BODY_GUNGONE )
	{// drop the gun!

		pev->body = BARNEY_BODY_GUNGONE;

		CBaseEntity *pGun = DropWeapon();
	}

	SetUse( NULL );	
	CTalkMonster::Killed( pevAttacker, iGib );
}

//=========================================================
// AI Schedules Specific to this monster
//=========================================================

Schedule_t* CTalkMonsterWithBarneyAI :: GetScheduleOfType ( int Type )
{
	Schedule_t *psched;

	switch( Type )
	{
	case SCHED_ARM_WEAPON:
		if ( m_hEnemy != NULL )
		{
			// face enemy, then draw.
			return slBarneyEnemyDraw;
		}
		break;

	// Hook these to make a looping schedule
	case SCHED_TARGET_FACE:
		// call base class default so that barney will talk
		// when 'used' 
		psched = CTalkMonster::GetScheduleOfType(Type);

		if (psched == slIdleStand)
			return slBaFaceTarget;	// override this for different target face behavior
		else
			return psched;

	case SCHED_TARGET_CHASE:
		return slBaFollow;

	case SCHED_IDLE_STAND:
		// call base class default so that scientist will talk
		// when standing during idle
		psched = CTalkMonster::GetScheduleOfType(Type);

		if (psched == slIdleStand)
		{
			// just look straight ahead.
			return slIdleBaStand;
		}
		else
			return psched;	

	case SCHED_BARNEY_COVER_AND_RELOAD:
		return slBaHideReload;
	}

	return CTalkMonster::GetScheduleOfType( Type );
}

//=========================================================
// GetSchedule - Decides which type of schedule best suits
// the monster's current state and conditions. Then calls
// monster's member function to get a pointer to a schedule
// of the proper type.
//=========================================================
Schedule_t *CTalkMonsterWithBarneyAI :: GetSchedule ( void )
{
	if ( HasConditions( bits_COND_HEAR_SOUND ) )
	{
		CSound *pSound;
		pSound = PBestSound();

		ASSERT( pSound != NULL );
		if ( pSound && (pSound->m_iType & bits_SOUND_DANGER) )
			return GetScheduleOfType( SCHED_TAKE_COVER_FROM_BEST_SOUND );
	}
	if ( HasConditions( bits_COND_ENEMY_DEAD ) && FOkToSpeak() )
	{
		SpeakKilledOneEnemy();
	}

	switch( m_MonsterState )
	{
	case MONSTERSTATE_COMBAT:
		{
// dead enemy
			if ( HasConditions( bits_COND_ENEMY_DEAD ) )
			{
				// call base class, all code to handle dead enemies is centralized there.
				return CBaseMonster :: GetSchedule();
			}

			// always act surprized with a new enemy
			if ( HasConditions( bits_COND_NEW_ENEMY ) && HasConditions( bits_COND_LIGHT_DAMAGE) )
				return GetScheduleOfType( SCHED_SMALL_FLINCH );

			if ((CanKick() || CanPunch()) && (HasConditions(bits_COND_SEE_ENEMY) && !HasConditions(bits_COND_ENEMY_OCCLUDED)) &&
				HasConditions(bits_COND_CAN_MELEE_ATTACK1))
			{
				return GetScheduleOfType(SCHED_MELEE_ATTACK1);
			}
				
			// wait for one schedule to draw gun
			if (!m_fGunDrawn )
				return GetScheduleOfType( SCHED_ARM_WEAPON );

			if (IsAllowedToReloadAndUseReloadSchedule() && HasConditions( bits_COND_NO_AMMO_LOADED ))
				return GetScheduleOfType( SCHED_BARNEY_COVER_AND_RELOAD );

			if ( HasConditions( bits_COND_HEAVY_DAMAGE ) )
				return GetScheduleOfType( SCHED_TAKE_COVER_FROM_ENEMY );
		}
		break;

	case MONSTERSTATE_ALERT:	
	case MONSTERSTATE_IDLE:
		if ( HasConditions(bits_COND_LIGHT_DAMAGE | bits_COND_HEAVY_DAMAGE))
		{
			// flinch if hurt
			return GetScheduleOfType( SCHED_SMALL_FLINCH );
		}

		if (IsAllowedToReloadAndUseReloadSchedule() && HasConditions(bits_COND_NO_AMMO_LOADED))
			return GetScheduleOfType(SCHED_RELOAD);

		if ( m_hEnemy == NULL && IsFollowing() )
		{
			if ( !m_hTargetEnt->IsAlive() )
			{
				// UNDONE: Comment about the recently dead player here?
				StopFollowing( FALSE );
				break;
			}
			else
			{
				if ( HasConditions( bits_COND_CLIENT_PUSH ) )
				{
					return GetScheduleOfType( SCHED_MOVE_AWAY_FOLLOW );
				}
				return GetScheduleOfType( SCHED_TARGET_FACE );
			}
		}

		if ( HasConditions( bits_COND_CLIENT_PUSH ) )
		{
			return GetScheduleOfType( SCHED_MOVE_AWAY );
		}

		// try to say something about smells
		TrySmellTalk();
		break;
	}
	
	return CTalkMonster::GetSchedule();
}

MONSTERSTATE CTalkMonsterWithBarneyAI :: GetIdealState ( void )
{
	return CTalkMonster::GetIdealState();
}

void CTalkMonsterWithBarneyAI::DeclineFollowing( void )
{
	SpeakDeclineFollowing();
}

void CTalkMonsterWithBarneyAI::PrecacheSounds()
{
	GetSharedUtils()->PrecacheSharedSounds();

	PRECACHE_SOUND("weapons/reload3.wav");
}

void CTalkMonsterWithBarneyAI::ReloadSound()
{
	EMIT_SOUND(edict(), CHAN_ITEM, "weapons/reload3.wav", 0.8f, ATTN_NORM);
}

CBaseEntity* CTalkMonsterWithBarneyAI::DropWeapon(void)
{
	return GetSharedUtils()->DropHandGun(this);
}

float CTalkMonsterWithBarneyAI::GetMeleeRange()
{
	return DEFAULT_MELEE_DISTANCE;
}

BOOL CTalkMonsterWithBarneyAI::CanKick()
{
	return FALSE;
}

BOOL CTalkMonsterWithBarneyAI::CanPunch()
{
	return FALSE;
}

BOOL CTalkMonsterWithBarneyAI::IsWearingHEVSuit()
{
	return FALSE;
}

BOOL CTalkMonsterWithBarneyAI::IsWearingArmor()
{
	return TRUE;
}

BOOL CTalkMonsterWithBarneyAI::IsWearingHelmet()
{
	return TRUE;
}

void CTalkMonsterWithBarneyAI::CheckAmmo(void)
{
	if (m_cAmmoLoaded <= 0)
	{
		SetConditions(bits_COND_NO_AMMO_LOADED);
	}
}

BOOL CTalkMonsterWithBarneyAI::IsAllowedToReloadAndUseReloadSchedule()
{
	return TRUE;
}

void CTalkMonsterWithBarneyAI::Reload()
{
	ReloadSound();
	m_cAmmoLoaded = m_cClipSize;
	ClearConditions(bits_COND_NO_AMMO_LOADED);
}

void CTalkMonsterWithBarneyAI::SetWeaponClipSize()
{
	m_cClipSize = GLOCK_MAX_CLIP;
}

CTalkMonsterWithBarneyAIUtil* CTalkMonsterWithBarneyAI::GetSharedUtils() const
{
	static CTalkMonsterWithBarneyAIUtil utilitySingleton;
	return &utilitySingleton;
}

