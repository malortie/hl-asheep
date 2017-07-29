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
#include	"squadmonster.h"
#include	"schedule.h"
#include	"effects.h"
#include	"weapons.h"
#include	"soundent.h"
#include	"squadmonsterwithislaveai.h"

class CExpAlienSlave : public CSquadMonsterWithISlaveAI
{
	typedef CSquadMonsterWithISlaveAI BaseClass;
public:

protected:

	virtual void PrecacheModels();
	virtual void PrecacheSounds();
	virtual void SetModel();
	virtual int GetFirstTimeSpawnHealth() const;
	virtual void SetBloodColor();

	virtual void SetVoicePitch();
	virtual void BeamPowerupSound();
	virtual void BeamZapSound();
	virtual void BeamElectroSound(const Vector& soundOrigin);
	virtual void StopBeamSounds();

	virtual void SpawnDynamicLight(const Vector& spawnPosition);
	virtual CBeam* CreateArmBeam(int side, const Vector& endPostition);
	virtual CBeam* CreateZapBeam(int side, const Vector& endPostition);

	virtual float GetClawDamage() const;
	virtual float GetClawRakeDamage() const;
	virtual float GetZapBeamDamage() const;
};

LINK_ENTITY_TO_CLASS(monster_exp_alien_slave, CExpAlienSlave);

void CExpAlienSlave::PrecacheModels()
{
	PRECACHE_MODEL("models/sslave.mdl");
	PRECACHE_MODEL("sprites/lgtning.spr");
}

void CExpAlienSlave::PrecacheSounds()
{
	BaseClass::PrecacheSounds();

	PRECACHE_SOUND("debris/zap4.wav");
	PRECACHE_SOUND("weapons/electro4.wav");
	PRECACHE_SOUND("hassault/hw_shoot1.wav");
}

void CExpAlienSlave::SetModel()
{
	SET_MODEL(ENT(pev), "models/sslave.mdl");
}

int CExpAlienSlave::GetFirstTimeSpawnHealth() const
{
	return gSkillData.expSlaveHealth;
}

void CExpAlienSlave::SetBloodColor()
{
	m_bloodColor = BLOOD_COLOR_RED;
}

void CExpAlienSlave::SetVoicePitch()
{
	//m_voicePitch = RANDOM_LONG(85, 110);
	m_voicePitch = RANDOM_LONG(110, 120);
}

void CExpAlienSlave::BeamPowerupSound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, "debris/zap4.wav", 1, ATTN_NORM, 0, 100 + m_iBeams * 10);
}

void CExpAlienSlave::BeamZapSound()
{
	EMIT_SOUND_DYN(ENT(pev), CHAN_WEAPON, "hassault/hw_shoot1.wav", 1, ATTN_NORM, 0, RANDOM_LONG(130, 160));
}

void CExpAlienSlave::BeamElectroSound(const Vector& soundOrigin)
{
	UTIL_EmitAmbientSound(ENT(pev), soundOrigin, "weapons/electro4.wav", 0.5, ATTN_NORM, 0, RANDOM_LONG(140, 160));
}

void CExpAlienSlave::StopBeamSounds()
{
	STOP_SOUND(ENT(pev), CHAN_WEAPON, "debris/zap4.wav");
}

void CExpAlienSlave::SpawnDynamicLight(const Vector& spawnPosition)
{
	MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, spawnPosition);
	WRITE_BYTE(TE_DLIGHT);
	WRITE_COORD(spawnPosition.x);	// X
	WRITE_COORD(spawnPosition.y);	// Y
	WRITE_COORD(spawnPosition.z);	// Z
	WRITE_BYTE(12);		// radius * 0.1
	WRITE_BYTE(255);		// r
	WRITE_BYTE(180);		// g
	WRITE_BYTE(96);		// b
	WRITE_BYTE(20 / pev->framerate);		// time * 10
	WRITE_BYTE(0);		// decay * 0.1
	MESSAGE_END();
}

CBeam* CExpAlienSlave::CreateArmBeam(int side, const Vector& endPostition)
{
	CBeam* pBeam = CBeam::BeamCreate("sprites/lgtning.spr", 30);

	if (pBeam != NULL)
	{
		pBeam->PointEntInit(endPostition, entindex());
		pBeam->SetEndAttachment(side < 0 ? 2 : 1);
		pBeam->SetColor(128, 16, 96);
		pBeam->SetBrightness(64);
		pBeam->SetNoise(80);
	}

	return pBeam;
}

CBeam* CExpAlienSlave::CreateZapBeam(int side, const Vector& endPostition)
{
	CBeam* pBeam = CBeam::BeamCreate("sprites/lgtning.spr", 50);

	if (pBeam != NULL)
	{
		pBeam->PointEntInit(endPostition, entindex());
		pBeam->SetEndAttachment(side < 0 ? 2 : 1);
		pBeam->SetColor(255, 96, 180);
		pBeam->SetBrightness(255);
		pBeam->SetNoise(20);
	}

	return pBeam;
}

float CExpAlienSlave::GetClawDamage() const
{
	return gSkillData.expSlaveDmgClaw;
}

float CExpAlienSlave::GetClawRakeDamage() const
{
	return gSkillData.expSlaveDmgClawrake;
}

float CExpAlienSlave::GetZapBeamDamage() const
{
	return gSkillData.expSlaveDmgZap;
}