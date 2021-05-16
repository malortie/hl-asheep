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
#include	"basemonster.h"
#include	"schedule.h"
#include	"effects.h"
#include	"weapons.h"
#include	"soundent.h"

#define clamp( val, min, max ) ( ((val) > (max)) ? (max) : ( ((val) < (min)) ? (min) : (val) ) )

#define PANTHER_MAX_BEAMS	8
#define	PANTHER_ARM_BEAMS	PANTHER_MAX_BEAMS
#define	PANTHER_EYE_BEAMS	6

#define PANTHER_SPINE_YAW_BONE			0
#define PANTHER_SPINE_YAW_MAX_VALUE		90


#define PANTHER_BEAM_LONGEST_TRACE_DISTANCE 1024

#define PANTHER_BEAM_SPRITE		"sprites/lgtning.spr"

//=========================================================
// Monster's Anim Events Go Here
//=========================================================
#define		PANTHER_AE_CLAW			( 1 )
#define		PANTHER_AE_CLAWRAKE		( 2 )
#define		PANTHER_AE_ZAP_POWERUP	( 3 )
#define		PANTHER_AE_ZAP_SHOOT	( 4 )
#define		PANTHER_AE_ZAP_DONE		( 5 )

class CPanther : public CBaseMonster
{
	typedef CBaseMonster BaseClass;
public:
	void Spawn(void);
	void Precache(void);
	void SetYawSpeed(void);
	int	 ISoundMask(void);
	int  Classify(void);
	int  IRelationship(CBaseEntity *pTarget);
	void HandleAnimEvent(MonsterEvent_t *pEvent);
	BOOL CheckRangeAttack1(float flDot, float flDist);
	void TraceAttack(entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType);
	int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType);

	void DeathSound(void);
	void PainSound(void);
	void AlertSound(void);
	void IdleSound(void);

	void Killed(entvars_t *pevAttacker, int iGib);


	void StartTask(Task_t *pTask);
	void RunTask(Task_t *pTask);
	Schedule_t *GetSchedule(void);
	Schedule_t *GetScheduleOfType(int Type);
	CUSTOM_SCHEDULES;

	int	Save(CSave &save);
	int Restore(CRestore &restore);
	static TYPEDESCRIPTION m_SaveData[];

	void ClearBeams();
	void ArmBeam(int side);
	void ZapBeam(int side);
	void BeamGlow(void);

	CBeam *m_pBeam[PANTHER_MAX_BEAMS];

	int m_iBeams;
	float m_flNextAttack;

	int	m_voicePitch;

	static const char *pAttackHitSounds[];
	static const char *pAttackMissSounds[];
	static const char *pPainSounds[];
	static const char *pDeathSounds[];
	static const char *pIdleSounds[];
	static const char *pAlertSounds[];
	static const char *pBeamElectroSounds[];
	static const char *pBeamPowerupSounds[];
	static const char *pBeamZapSounds[];

	void StopBeamSounds();

	CBeam * CreateArmBeam(int startAttachment, const Vector& endPosition);
	CBeam * CreateZapBeam(int startAttachment, const Vector& endPosition);
	void SpineControls(float angleY);
	float GetSpineTurnRate() { return 8; }

	float		m_spineY;
};

LINK_ENTITY_TO_CLASS(monster_alien_panther, CPanther);

TYPEDESCRIPTION	CPanther::m_SaveData[] =
{
	DEFINE_ARRAY(CPanther, m_pBeam, FIELD_CLASSPTR, PANTHER_MAX_BEAMS),
	DEFINE_FIELD(CPanther, m_iBeams, FIELD_INTEGER),
	DEFINE_FIELD(CPanther, m_flNextAttack, FIELD_TIME),

	DEFINE_FIELD(CPanther, m_voicePitch, FIELD_INTEGER),
	DEFINE_FIELD(CPanther, m_spineY, FIELD_FLOAT),
};

IMPLEMENT_SAVERESTORE(CPanther, CBaseMonster);

const char *CPanther::pAttackHitSounds[] =
{
	"panther/pclaw_strike1.wav",
	"panther/pclaw_strike2.wav",
	"panther/pclaw_strike3.wav",
};

const char *CPanther::pAttackMissSounds[] =
{
	"panther/pclaw_miss1.wav",
	"panther/pclaw_miss2.wav",
};

const char *CPanther::pPainSounds[] =
{
	"panther/p_pain1.wav",
	"panther/p_pain2.wav",
};

const char *CPanther::pDeathSounds[] =
{
	"panther/p_die1.wav",
	"panther/p_die2.wav",
};

const char *CPanther::pIdleSounds[] =
{
	"panther/p_idle1.wav",
	"panther/p_idle2.wav",
	"panther/p_idle3.wav",
};

const char *CPanther::pAlertSounds[] =
{
	"panther/p_alert1.wav",
	"panther/p_alert2.wav",
	"panther/p_alert3.wav",
};

const char *CPanther::pBeamElectroSounds[] =
{
	"panther/p_electro1.wav",
};

const char *CPanther::pBeamPowerupSounds[] =
{
	"panther/p_zap1.wav",
	"panther/p_zap2.wav",
};

const char *CPanther::pBeamZapSounds[] =
{
	"panther/p_shoot1.wav",
};

//=========================================================
// Classify - indicates this monster's place in the 
// relationship table.
//=========================================================
int	CPanther::Classify(void)
{
	return	CLASS_ALIEN_MILITARY;
}


int CPanther::IRelationship(CBaseEntity *pTarget)
{
	if ((pTarget->IsPlayer()))
		if ((pev->spawnflags & SF_MONSTER_WAIT_UNTIL_PROVOKED) && !(m_afMemory & bits_MEMORY_PROVOKED))
			return R_NO;
	return BaseClass::IRelationship(pTarget);
}

//=========================================================
// ALertSound - scream
//=========================================================
void CPanther::AlertSound(void)
{
	if (m_hEnemy != NULL)
	{
		EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, RANDOM_SOUND_ARRAY(pAlertSounds), 0.85, ATTN_NORM, 0, m_voicePitch);
	}
}

//=========================================================
// IdleSound
//=========================================================
void CPanther::IdleSound(void)
{
	if (RANDOM_LONG(0, 2) == 0)
	{
		EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, RANDOM_SOUND_ARRAY(pIdleSounds), 0.85, ATTN_NORM, 0, m_voicePitch);
	}
}

//=========================================================
// PainSound
//=========================================================
void CPanther::PainSound(void)
{
	if (RANDOM_LONG(0, 2) == 0)
	{
		EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, RANDOM_SOUND_ARRAY(pPainSounds), 1.0, ATTN_NORM, 0, m_voicePitch);
	}
}

//=========================================================
// DieSound
//=========================================================

void CPanther::DeathSound(void)
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, RANDOM_SOUND_ARRAY(pDeathSounds), 1.0, ATTN_NORM, 0, m_voicePitch);
}


//=========================================================
// ISoundMask - returns a bit mask indicating which types
// of sounds this monster regards. 
//=========================================================
int CPanther::ISoundMask(void)
{
	return	bits_SOUND_WORLD |
		bits_SOUND_COMBAT |
		bits_SOUND_DANGER |
		bits_SOUND_PLAYER;
}


void CPanther::Killed(entvars_t *pevAttacker, int iGib)
{
	ClearBeams();
	BaseClass::Killed(pevAttacker, iGib);
}

//=========================================================
// SetYawSpeed - allows each sequence to have a different
// turn rate associated with it.
//=========================================================
void CPanther::SetYawSpeed(void)
{
	int ys;

	switch (m_Activity)
	{
	case ACT_WALK:
		ys = 40;
		break;
	case ACT_RUN:
		ys = 60;
		break;
	case ACT_IDLE:
		ys = 40;
		break;
	default:
		ys = 50;
		break;
	}

	pev->yaw_speed = ys;
}

//=========================================================
// HandleAnimEvent - catches the monster-specific messages
// that occur when tagged animation frames are played.
//
// Returns number of events handled, 0 if none.
//=========================================================
void CPanther::HandleAnimEvent(MonsterEvent_t *pEvent)
{
	// ALERT( at_console, "event %d : %f\n", pEvent->event, pev->frame );
	switch (pEvent->event)
	{
	case PANTHER_AE_CLAW:
	{
		// SOUND HERE!
		CBaseEntity *pHurt = CheckTraceHullAttack(70, gSkillData.pantherDmgClaw, DMG_SLASH);
		if (pHurt)
		{
			if (pHurt->pev->flags & (FL_MONSTER | FL_CLIENT))
			{
				pHurt->pev->punchangle.z = -18;
				pHurt->pev->punchangle.x = 5;
			}
			// Play a random attack hit sound
			EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, pAttackHitSounds[RANDOM_LONG(0, ARRAYSIZE(pAttackHitSounds) - 1)], 1.0, ATTN_NORM, 0, m_voicePitch);
		}
		else
		{
			// Play a random attack miss sound
			EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, pAttackMissSounds[RANDOM_LONG(0, ARRAYSIZE(pAttackMissSounds) - 1)], 1.0, ATTN_NORM, 0, m_voicePitch);
		}
	}
	break;

	case PANTHER_AE_CLAWRAKE:
	{
		CBaseEntity *pHurt = CheckTraceHullAttack(70, gSkillData.pantherDmgClawrake, DMG_SLASH);
		if (pHurt)
		{
			if (pHurt->pev->flags & (FL_MONSTER | FL_CLIENT))
			{
				pHurt->pev->punchangle.z = -18;
				pHurt->pev->punchangle.x = 5;
			}
			EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, pAttackHitSounds[RANDOM_LONG(0, ARRAYSIZE(pAttackHitSounds) - 1)], 1.0, ATTN_NORM, 0, m_voicePitch);
		}
		else
		{
			EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, pAttackMissSounds[RANDOM_LONG(0, ARRAYSIZE(pAttackMissSounds) - 1)], 1.0, ATTN_NORM, 0, m_voicePitch);
		}
	}
	break;

	case PANTHER_AE_ZAP_POWERUP:
	{
		// speed up attack when on hard
		if (g_iSkillLevel == SKILL_HARD)
			pev->framerate = 1.5;

		UTIL_MakeAimVectors(pev->angles);

		if (m_iBeams == 0)
		{
			Vector vecSrc = pev->origin + gpGlobals->v_forward * 2;
			MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, vecSrc);
			WRITE_BYTE(TE_DLIGHT);
			WRITE_COORD(vecSrc.x);	// X
			WRITE_COORD(vecSrc.y);	// Y
			WRITE_COORD(vecSrc.z);	// Z
			WRITE_BYTE(12);		// radius * 0.1
			WRITE_BYTE(255);		// r
			WRITE_BYTE(180);		// g
			WRITE_BYTE(96);		// b
			WRITE_BYTE(20 / pev->framerate);		// time * 10
			WRITE_BYTE(0);		// decay * 0.1
			MESSAGE_END();

		}

		ArmBeam(-1);
		ArmBeam(1);
		BeamGlow();

		EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, RANDOM_SOUND_ARRAY(pBeamPowerupSounds), 1, ATTN_NORM, 0, 100 + m_iBeams * 10);
		pev->skin = m_iBeams / 2;
	}
	break;

	case PANTHER_AE_ZAP_SHOOT:
	{
		ClearBeams();

		ClearMultiDamage();

		UTIL_MakeAimVectors(pev->angles);

		ZapBeam(-1);
		ZapBeam(1);

		EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, RANDOM_SOUND_ARRAY(pBeamZapSounds), 1, ATTN_NORM, 0, RANDOM_LONG(130, 160));
		ApplyMultiDamage(pev, pev);

		m_flNextAttack = gpGlobals->time + RANDOM_FLOAT(0.5, 4.0);
	}
	break;

	case PANTHER_AE_ZAP_DONE:
	{
		ClearBeams();
	}
	break;

	default:
		BaseClass::HandleAnimEvent(pEvent);
		break;
	}
}

//=========================================================
// CheckRangeAttack1 - normal beam attack 
//=========================================================
BOOL CPanther::CheckRangeAttack1(float flDot, float flDist)
{
	if (m_flNextAttack > gpGlobals->time)
	{
		return FALSE;
	}

	return BaseClass::CheckRangeAttack1(flDot, flDist);
}

//=========================================================
// StartTask
//=========================================================
void CPanther::StartTask(Task_t *pTask)
{
	ClearBeams();

	switch (pTask->iTask)
	{
	case TASK_RANGE_ATTACK1:
		m_IdealActivity = ACT_RANGE_ATTACK1;
		break;
	default:
		BaseClass::StartTask(pTask);
		break;
	}
}

void CPanther::RunTask(Task_t *pTask)
{
	switch (pTask->iTask)
	{
	case TASK_RANGE_ATTACK1:
	{
		MakeIdealYaw(m_vecEnemyLKP);
		ChangeYaw(pev->yaw_speed);

		if (m_fSequenceFinished)
		{
			// Reset spine yaw controller.
			SpineControls(0);
			SetBoneController(0, 0);

			m_Activity = ACT_RESET;
			TaskComplete();
		}
		else
		{
			Vector angles = g_vecZero;

			CBaseEntity *pEnemy = m_hEnemy;
			if (pEnemy)
			{
				Vector org = pev->origin;
				org.z += 32;

				// From org, calculate the angle between our enemy
				// and our body direction.
				Vector dirToEnemy, bodyDirection, crossUp;
				float distanceBetweenMeAndEnemy;

				dirToEnemy = (pEnemy->BodyTarget(org) - org).Normalize();
				distanceBetweenMeAndEnemy = (pEnemy->BodyTarget(org) - org).Length2D();
				UTIL_MakeVectorsPrivate(Vector(0, pev->angles.y, 0), bodyDirection, NULL, NULL);

				float yawDot = DotProduct(dirToEnemy, bodyDirection);
				angles.y = std::acos(yawDot) * 180.0f / M_PI;
				angles.y = clamp(angles.y, -PANTHER_SPINE_YAW_MAX_VALUE, PANTHER_SPINE_YAW_MAX_VALUE);

				// On which side is our enemy?
				crossUp = CrossProduct(dirToEnemy, bodyDirection);
				if (crossUp.z > 0)
					angles.y = -angles.y;
			}

			//ALERT(at_console, "Angle: (%.2f) Bone controller 0: (%d).\n", angles.y, (int)angles.y);
			SpineControls(angles.y);
		}
	}
	break;

	default:
		BaseClass::RunTask(pTask);
		break;
	}
}


//=========================================================
// Spawn
//=========================================================
void CPanther::Spawn()
{
	Precache();

	SET_MODEL(ENT(pev), "models/panther.mdl");
	UTIL_SetSize(pev, Vector(-36, -36, 0), Vector(36, 36, 66));

	pev->solid = SOLID_SLIDEBOX;
	pev->movetype = MOVETYPE_STEP;
	m_bloodColor = BLOOD_COLOR_GREEN;
	pev->effects = 0;
	pev->health = gSkillData.pantherHealth;

	pev->view_ofs = Vector(72, 0, 64); // position of the eyes relative to monster's origin.
	m_flFieldOfView = VIEW_FIELD_WIDE; // NOTE: we need a wide field of view so npc will notice player and say hello
	m_MonsterState = MONSTERSTATE_NONE;
	m_afCapability = bits_CAP_HEAR | bits_CAP_DOORS_GROUP;

	m_voicePitch = 100;

	MonsterInit();
}

//=========================================================
// Precache - precaches all resources this monster needs
//=========================================================
void CPanther::Precache()
{
	PRECACHE_MODEL("models/panther.mdl");
	PRECACHE_MODEL(PANTHER_BEAM_SPRITE);
	PRECACHE_SOUND("panther/p_headbite.wav");

	PRECACHE_SOUND_ARRAY(pAttackHitSounds);
	PRECACHE_SOUND_ARRAY(pAttackMissSounds);
	PRECACHE_SOUND_ARRAY(pPainSounds);
	PRECACHE_SOUND_ARRAY(pDeathSounds);
	PRECACHE_SOUND_ARRAY(pIdleSounds);
	PRECACHE_SOUND_ARRAY(pAlertSounds);
	PRECACHE_SOUND_ARRAY(pBeamElectroSounds);
	PRECACHE_SOUND_ARRAY(pBeamPowerupSounds);
	PRECACHE_SOUND_ARRAY(pBeamZapSounds);
}


//=========================================================
// TakeDamage - get provoked when injured
//=========================================================

int CPanther::TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType)
{
	// don't slash one of your own
	if ((bitsDamageType & DMG_SLASH) && pevAttacker && IRelationship(Instance(pevAttacker)) < R_DL)
		return 0;

	m_afMemory |= bits_MEMORY_PROVOKED;
	return BaseClass::TakeDamage(pevInflictor, pevAttacker, flDamage, bitsDamageType);
}

void CPanther::TraceAttack(entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType)
{
	if (bitsDamageType & DMG_SHOCK)
		return;

	if (ptr->iHitgroup != HITGROUP_HEAD)
	{
		// absorb damage
		flDamage -= 20;
		if (flDamage <= 0)
		{
			UTIL_Ricochet(ptr->vecEndPos, 1.0);
			flDamage = 0.01;
		}
		return;
	}

	BaseClass::TraceAttack(pevAttacker, flDamage, vecDir, ptr, bitsDamageType);
}


//=========================================================
// AI Schedules Specific to this monster
//=========================================================

// primary range attack
Task_t	tlPantherAttack1[] =
{
	{ TASK_STOP_MOVING,			0 },
	{ TASK_FACE_IDEAL,			(float)0 },
	{ TASK_RANGE_ATTACK1,		(float)0 },
};

Schedule_t	slPantherAttack1[] =
{
	{
		tlPantherAttack1,
		ARRAYSIZE(tlPantherAttack1),
		bits_COND_CAN_MELEE_ATTACK1 |
		bits_COND_HEAR_SOUND |
	bits_COND_HEAVY_DAMAGE,

	bits_SOUND_DANGER,
	"Panther Range Attack1"
	},
};


DEFINE_CUSTOM_SCHEDULES(CPanther)
{
	slPantherAttack1,
};

IMPLEMENT_CUSTOM_SCHEDULES(CPanther, CBaseMonster);


//=========================================================
//=========================================================
Schedule_t *CPanther::GetSchedule(void)
{
	ClearBeams();

	if (HasConditions(bits_COND_HEAR_SOUND))
	{
		CSound *pSound;
		pSound = PBestSound();

		ASSERT(pSound != NULL);

		if (pSound && (pSound->m_iType & bits_SOUND_DANGER))
			return GetScheduleOfType(SCHED_TAKE_COVER_FROM_BEST_SOUND);
		if (pSound->m_iType & bits_SOUND_COMBAT)
			m_afMemory |= bits_MEMORY_PROVOKED;
	}

	switch (m_MonsterState)
	{
	case MONSTERSTATE_COMBAT:
		// dead enemy
		if (HasConditions(bits_COND_ENEMY_DEAD))
		{
			// call base class, all code to handle dead enemies is centralized there.
			return BaseClass::GetSchedule();
		}

		const int defaultFailSchedule = SCHED_COMBAT_FACE;

		if (!HasConditions(bits_COND_SEE_ENEMY) || HasConditions(bits_COND_ENEMY_OCCLUDED))
		{
			return GetScheduleOfType(SCHED_CHASE_ENEMY);
		}

		if (HasConditions(bits_COND_CAN_MELEE_ATTACK1))
		{
			m_failSchedule = defaultFailSchedule;
			return GetScheduleOfType(SCHED_MELEE_ATTACK1);
		}
		else if (HasConditions(bits_COND_CAN_RANGE_ATTACK1) && RANDOM_LONG(0, 2) == 1)
		{
			m_failSchedule = defaultFailSchedule;
			return GetScheduleOfType(SCHED_RANGE_ATTACK1);
		}
		else 
		{
			return GetScheduleOfType(SCHED_CHASE_ENEMY);
		}
		break;
	}
	return BaseClass::GetSchedule();
}


Schedule_t *CPanther::GetScheduleOfType(int Type)
{
	switch (Type)
	{
	case SCHED_FAIL:
		if (HasConditions(bits_COND_CAN_MELEE_ATTACK1))
		{
			return BaseClass::GetScheduleOfType(SCHED_MELEE_ATTACK1); ;
		}
		break;
	case SCHED_RANGE_ATTACK1:
		return slPantherAttack1;
	case SCHED_RANGE_ATTACK2:
		return slPantherAttack1;
	}
	return BaseClass::GetScheduleOfType(Type);
}


//=========================================================
// ArmBeam - small beam from arm to nearby geometry
//=========================================================

void CPanther::ArmBeam(int side)
{
	if (m_iBeams >= PANTHER_ARM_BEAMS)
		return;

	TraceResult tr;
	float flDist = 1.0;

	UTIL_MakeAimVectors(pev->angles);
	Vector vecSrc = pev->origin + gpGlobals->v_up * 60;

	for (int i = 0; i < 3; i++)
	{
		Vector vecAim = gpGlobals->v_right * side * RANDOM_FLOAT(0, 1) + gpGlobals->v_up * RANDOM_FLOAT(-1, 1);

		TraceResult tr1;
		UTIL_TraceLine(vecSrc, vecSrc + vecAim * PANTHER_BEAM_LONGEST_TRACE_DISTANCE, dont_ignore_monsters, ENT(pev), &tr1);
		if (flDist > tr1.flFraction)
		{
			tr = tr1;
			flDist = tr.flFraction;
		}
	}

	// Couldn't find anything close enough
	if (flDist == 1.0)
		return;

	DecalGunshot(&tr, BULLET_PLAYER_CROWBAR);

	m_pBeam[m_iBeams] = CreateArmBeam(1, // Start attachment.
		tr.vecEndPos); // end position.

	if (m_pBeam[m_iBeams] != NULL)
		m_iBeams++;

	//ALERT(at_console, "CPanther::ArmBeam : beam count: %d\n", m_iBeams);

	m_pBeam[m_iBeams] = CreateArmBeam(2, // Start attachment.
		tr.vecEndPos); // end position.

	if (m_pBeam[m_iBeams] != NULL)
		m_iBeams++;

	//ALERT(at_console, "CPanther::ArmBeam : beam count: %d\n", m_iBeams);
}


//=========================================================
// BeamGlow - brighten all beams
//=========================================================
void CPanther::BeamGlow()
{
	int b = m_iBeams * 32;
	if (b > 255)
		b = 255;

	for (int i = 0; i < m_iBeams; i++)
	{
		if (m_pBeam[i]->GetBrightness() != 255)
		{
			m_pBeam[i]->SetBrightness(b);
		}
	}
}

//=========================================================
// ZapBeam - heavy damage directly forward
//=========================================================
void CPanther::ZapBeam(int side)
{
	if (m_iBeams >= PANTHER_EYE_BEAMS)
		return;

	Vector vecSrc, vecAim;
	TraceResult tr;
	CBaseEntity *pEntity;

	vecSrc = pev->origin + gpGlobals->v_up * 36;
	vecAim = ShootAtEnemy(vecSrc);
	float deflection = 0.01;
	vecAim = vecAim + side * gpGlobals->v_right * RANDOM_FLOAT(0, deflection) + gpGlobals->v_up * RANDOM_FLOAT(-deflection, deflection);
	UTIL_TraceLine(vecSrc, vecSrc + vecAim * 1024, dont_ignore_monsters, ENT(pev), &tr);

	m_pBeam[m_iBeams] = CreateZapBeam(3, tr.vecEndPos);
	if (!m_pBeam[m_iBeams])
		return;
	m_iBeams++;

	//ALERT(at_console, "CPanther::ZapBeam : beam count: %d\n", m_iBeams);

	pEntity = CBaseEntity::Instance(tr.pHit);
	if (pEntity != NULL && pEntity->pev->takedamage)
	{
		pEntity->TraceAttack(pev, gSkillData.pantherDmgZap, vecAim, &tr, DMG_SHOCK);
	}
	UTIL_EmitAmbientSound(ENT(pev), tr.vecEndPos, RANDOM_SOUND_ARRAY(pBeamElectroSounds), 0.5, ATTN_NORM, 0, RANDOM_LONG(140, 160));
}


//=========================================================
// ClearBeams - remove all beams
//=========================================================
void CPanther::ClearBeams()
{
	for (int i = 0; i < PANTHER_MAX_BEAMS; i++)
	{
		if (m_pBeam[i])
		{
			UTIL_Remove(m_pBeam[i]);
			m_pBeam[i] = NULL;
		}
	}
	m_iBeams = 0;
	pev->skin = 0;

	StopBeamSounds();
}

void CPanther::StopBeamSounds()
{
	STOP_SOUND(ENT(pev), CHAN_WEAPON, "panther/p_zap1.wav");
	STOP_SOUND(ENT(pev), CHAN_WEAPON, "panther/p_zap2.wav");
}

CBeam * CPanther::CreateArmBeam(int startAttachment, const Vector& endPosition)
{
	CBeam* pBeam = CBeam::BeamCreate(PANTHER_BEAM_SPRITE, 30);
	if (pBeam != NULL)
	{
		pBeam->PointEntInit(endPosition, entindex());
		pBeam->SetEndAttachment(startAttachment);
		pBeam->SetColor(202, 64, 4);
		pBeam->SetBrightness(64);
		pBeam->SetNoise(80);
	}

	return pBeam;
}

CBeam * CPanther::CreateZapBeam(int startAttachment, const Vector& endPosition)
{
	CBeam* pBeam = CBeam::BeamCreate(PANTHER_BEAM_SPRITE, 50);
	if (pBeam != NULL)
	{
		pBeam->PointEntInit(endPosition, entindex());
		pBeam->SetEndAttachment(startAttachment);
		pBeam->SetColor(202, 64, 4);
		pBeam->SetBrightness(255);
		pBeam->SetNoise(5);
	}

	return pBeam;
}

void CPanther::SpineControls(float angleY)
{
	if (angleY < -180)
		angleY += 360;
	else if (angleY > 180)
		angleY -= 360;

	m_spineY = UTIL_ApproachAngle(angleY, m_spineY, GetSpineTurnRate());
	SetBoneController(PANTHER_SPINE_YAW_BONE, m_spineY);
}
