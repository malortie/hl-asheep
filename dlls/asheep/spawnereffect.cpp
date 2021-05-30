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
#include "customentity.h"
#include "effects.h"
#include "decals.h"
#include "spawnereffect.h"


#define EFFECT_BEAM_SPRITE		"sprites/lgtning.spr"
#define EFFECT_SPRITE_EXPLOSION "sprites/Fexplo1.spr"
#define EFFECT_SPRITE_FLARE		"sprites/XFlare1.spr"
#define EFFECT_SOUND_BEAMSTART2	"debris/beamstart2.wav"
#define EFFECT_SOUND_BEAMSTART7	"debris/beamstart7.wav"

#define EFFECT_DURATION		1.0f
#define EFFECT_LIFETIME_PER_BEAM	0.5f
#define EFFECT_BEAM_WIDTH	18
#define EFFECT_BEAM_FRAMERATE	0.0f
#define EFFECT_BEAM_RADIUS	100
#define EFFECT_BEAM_NOISE	65
#define EFFECT_BEAM_STRIKE_FREQUENCY	-0.5f
#define EFFECT_BEAM_TEXTURESCROLL	35
#define EFFECT_LIGHT_LIFETIME	2.0f

class CSpawnerEffect : public CBaseEntity
{
public:

	void Precache();
	void Spawn();
	void KeyValue(KeyValueData* pkvd);
	virtual int	ObjectCaps(void) { return CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }
	void EXPORT EffectUse(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);
	void EXPORT EffectThink(void);

	virtual int		Save(CSave &save);
	virtual int		Restore(CRestore &restore);
	static	TYPEDESCRIPTION m_SaveData[];

protected:

	virtual void UpdateEffect();
	BOOL CheckIfIsTimeToStopEffect();

	void PlaySoundEffect1();
	void PlaySoundEffect2();
	void SpawnLight();
	void StartBeam();
	void ShowSprite1();
	void ShowSprite2();
	void UpdateBeam();

	virtual int* GetBeamColor();
	virtual int* GetLightColor();
	virtual int* GetSprite1Color();
	virtual int* GetSprite2Color();

private:

	static int m_beamColor[2][4];
	static int m_lightColor[2][3];
	static int m_sprite1Color[2][4];
	static int m_sprite2Color[2][4];

	void Beam_RandomPoint(Vector &vecSrc);
	void Beam_Zap(const Vector &vecSrc, const Vector &vecDest);

	int m_spawnType;
	int m_beamCount;
	float m_spriteScale;
	int m_spawnSoundRadius;
	float m_spawnVolume;
	int m_lightRadius;

	float m_startTime;
	float m_nextStageTime;
	float m_beamStrikeTime;
	int m_stage;
	int m_sprite1ModelIndex;
	int m_sprite2ModelIndex;
	short m_beamSpriteTexture;
};

LINK_ENTITY_TO_CLASS(env_spawnereffect, CSpawnerEffect);

TYPEDESCRIPTION	CSpawnerEffect::m_SaveData[] =
{
	DEFINE_FIELD(CSpawnerEffect, m_spawnType, FIELD_INTEGER),
	DEFINE_FIELD(CSpawnerEffect, m_beamCount, FIELD_INTEGER),
	DEFINE_FIELD(CSpawnerEffect, m_spriteScale, FIELD_FLOAT),
	DEFINE_FIELD(CSpawnerEffect, m_spawnSoundRadius, FIELD_INTEGER),
	DEFINE_FIELD(CSpawnerEffect, m_spawnVolume, FIELD_FLOAT),
	DEFINE_FIELD(CSpawnerEffect, m_lightRadius, FIELD_INTEGER),

	DEFINE_FIELD(CSpawnerEffect, m_startTime, FIELD_TIME),
	DEFINE_FIELD(CSpawnerEffect, m_stage, FIELD_INTEGER),
	DEFINE_FIELD(CSpawnerEffect, m_nextStageTime, FIELD_TIME),
	DEFINE_FIELD(CSpawnerEffect, m_beamStrikeTime, FIELD_TIME),
};
IMPLEMENT_SAVERESTORE(CSpawnerEffect, CBaseEntity);

int CSpawnerEffect::m_beamColor[2][4] =
{
	{ 197, 243, 169, 150 },	// Default greenish.
	{ 243, 169, 197, 150 }	// Reddish/pink.
};

int CSpawnerEffect::m_lightColor[2][3] = 
{
	{ 77, 210, 130 },	// Default greenish.
	{ 210, 77, 130 },	// Reddish/pink.
};

int CSpawnerEffect::m_sprite1Color[2][4] = 
{
	{ 77, 210, 130, 255 },	// Default greenish.
	{ 210, 77, 130, 255 },	// Reddish/pink.
};

int CSpawnerEffect::m_sprite2Color[2][4] =
{
	{ 184, 250, 214, 255 },	// Default greenish.
	{ 250, 184, 214, 255 },	// Reddish/pink.
};

void CSpawnerEffect::Precache()
{
	m_beamSpriteTexture = PRECACHE_MODEL(EFFECT_BEAM_SPRITE);
	m_sprite1ModelIndex = PRECACHE_MODEL(EFFECT_SPRITE_EXPLOSION);
	m_sprite2ModelIndex = PRECACHE_MODEL(EFFECT_SPRITE_FLARE);
	PRECACHE_SOUND(EFFECT_SOUND_BEAMSTART2);
	PRECACHE_SOUND(EFFECT_SOUND_BEAMSTART7);
}

void CSpawnerEffect::Spawn()
{
	Precache();

	pev->movetype = MOVETYPE_NONE;
	pev->solid = SOLID_NOT;

	SetThink(NULL);
	SetUse(&CSpawnerEffect::EffectUse);
}

void CSpawnerEffect::KeyValue(KeyValueData* pkvd)
{
	if (FStrEq(pkvd->szKeyName, "spawntype"))
	{
		m_spawnType = std::atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "beamcount"))
	{
		m_beamCount = std::atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "spritescale"))
	{
		m_spriteScale = std::atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "spawnsoundrad"))
	{
		m_spawnSoundRadius = std::atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "spawnvol"))
	{
		m_spawnVolume = std::atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "lightrad"))
	{
		m_lightRadius = std::atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else
		CBaseEntity::KeyValue(pkvd);
}

void CSpawnerEffect::EffectUse(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value)
{
	m_startTime = gpGlobals->time;

	SetThink(&CSpawnerEffect::EffectThink);
	pev->nextthink = gpGlobals->time;
}

void CSpawnerEffect::EffectThink(void)
{
	UpdateEffect();

	if (CheckIfIsTimeToStopEffect())
		SetThink(&CBaseEntity::SUB_Remove);

	pev->nextthink = gpGlobals->time;
}

void CSpawnerEffect::UpdateEffect()
{
	if (m_nextStageTime <= gpGlobals->time)
	{
		switch (m_stage)
		{
		case 0:
			PlaySoundEffect1();
			ShowSprite1();
			ShowSprite2();
			SpawnLight();
			StartBeam();
			m_stage++;
			m_nextStageTime = gpGlobals->time + 0.5;
			break;
		case 1:
			PlaySoundEffect2();
			m_stage++;
			m_nextStageTime = gpGlobals->time;
			break;
		case 2:
		default:
			break;
		}
	}

	UpdateBeam();
}

BOOL CSpawnerEffect::CheckIfIsTimeToStopEffect()
{
	return (gpGlobals->time - m_startTime) > EFFECT_DURATION;
}

void CSpawnerEffect::UpdateBeam()
{
	if (m_beamStrikeTime <= gpGlobals->time)
		m_beamStrikeTime = gpGlobals->time + EFFECT_LIFETIME_PER_BEAM + RANDOM_FLOAT(0, EFFECT_BEAM_STRIKE_FREQUENCY);

	Beam_RandomPoint(pev->origin);
}

void CSpawnerEffect::PlaySoundEffect1()
{
	EMIT_SOUND_DYN(
		ENT(pev),
		CHAN_AUTO,
		EFFECT_SOUND_BEAMSTART2,
		m_spawnVolume,
		ATTN_NORM, 
		0,
		100);
}

void CSpawnerEffect::PlaySoundEffect2()
{
	EMIT_SOUND_DYN(
		ENT(pev),
		CHAN_AUTO,
		EFFECT_SOUND_BEAMSTART7,
		m_spawnVolume,
		ATTN_NORM,
		0,
		100);
}

void CSpawnerEffect::SpawnLight()
{
	MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, pev->origin);
		WRITE_BYTE(TE_DLIGHT);
		WRITE_COORD(pev->origin.x);	// X
		WRITE_COORD(pev->origin.y);	// Y
		WRITE_COORD(pev->origin.z);	// Z
		WRITE_BYTE(m_lightRadius);	// radius * 0.1
		WRITE_BYTE(GetLightColor()[0]);	// r
		WRITE_BYTE(GetLightColor()[1]);	// g
		WRITE_BYTE(GetLightColor()[2]);	// b
		WRITE_BYTE(EFFECT_LIGHT_LIFETIME * 10);	// time * 10
		WRITE_BYTE(0);		// decay * 0.1
	MESSAGE_END();
}

void CSpawnerEffect::StartBeam()
{
	m_beamStrikeTime = gpGlobals->time;
}

void CSpawnerEffect::ShowSprite1()
{
	CSprite* pSprite = CSprite::SpriteCreate(EFFECT_SPRITE_EXPLOSION, pev->origin, TRUE);
	pSprite->AnimateAndDie(10);

	pSprite->SetTransparency(
		kRenderGlow,
		GetSprite1Color()[0],
		GetSprite1Color()[1], 
		GetSprite1Color()[2], 
		GetSprite1Color()[3], 
		kRenderFxGlowShell);

	pSprite->SetScale(m_spriteScale);
}

void CSpawnerEffect::ShowSprite2()
{
	CSprite* pSprite = CSprite::SpriteCreate(EFFECT_SPRITE_FLARE, pev->origin, TRUE);
	pSprite->AnimateAndDie(10);

	pSprite->SetTransparency(
		kRenderGlow, 
		GetSprite2Color()[0], 
		GetSprite2Color()[1], 
		GetSprite2Color()[2], 
		GetSprite2Color()[3], 
		kRenderFxNoDissipation);

	pSprite->SetScale(m_spriteScale);
}

void CSpawnerEffect::Beam_RandomPoint(Vector &vecSrc)
{
	int iLoops = 0;

	for (iLoops = 0; iLoops < 10; iLoops++)
	{
		Vector vecDir1 = Vector(RANDOM_FLOAT(-1.0, 1.0), RANDOM_FLOAT(-1.0, 1.0), RANDOM_FLOAT(-1.0, 1.0));
		vecDir1 = vecDir1.Normalize();
		TraceResult		tr1;
		UTIL_TraceLine(vecSrc, vecSrc + vecDir1 * EFFECT_BEAM_RADIUS, ignore_monsters, ENT(pev), &tr1);

		if ((tr1.vecEndPos - vecSrc).Length() < EFFECT_BEAM_RADIUS * 0.1)
			continue;

		if (tr1.flFraction == 1.0)
			continue;

		Beam_Zap(vecSrc, tr1.vecEndPos);
		break;
	}
}
void CSpawnerEffect::Beam_Zap(const Vector &vecSrc, const Vector &vecDest)
{
	MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
		WRITE_BYTE( TE_BEAMPOINTS);
		WRITE_COORD(vecSrc.x);
		WRITE_COORD(vecSrc.y);
		WRITE_COORD(vecSrc.z);
		WRITE_COORD(vecDest.x);
		WRITE_COORD(vecDest.y);
		WRITE_COORD(vecDest.z);
		WRITE_SHORT( m_beamSpriteTexture );
		WRITE_BYTE( 0 ); // framestart
		WRITE_BYTE( (int)EFFECT_BEAM_FRAMERATE ); // framerate
		WRITE_BYTE( (int)( EFFECT_LIFETIME_PER_BEAM *10.0) ); // life
		WRITE_BYTE( EFFECT_BEAM_WIDTH );  // width
		WRITE_BYTE( EFFECT_BEAM_NOISE );   // noise
		WRITE_BYTE( GetBeamColor()[0] );   // r, g, b
		WRITE_BYTE( GetBeamColor()[1] );   // r, g, b
		WRITE_BYTE( GetBeamColor()[2] );   // r, g, b
		WRITE_BYTE( GetBeamColor()[3] );	// brightness
		WRITE_BYTE( EFFECT_BEAM_TEXTURESCROLL );		// speed
	MESSAGE_END();
}

int* CSpawnerEffect::GetBeamColor()
{
	if (m_spawnType == SPAWNTYPE_GREEN)
		return &m_beamColor[0][0];
	else
		return &m_beamColor[1][0];
}

int* CSpawnerEffect::GetLightColor()
{
	if (m_spawnType == SPAWNTYPE_GREEN)
		return &m_lightColor[0][0];
	else
		return &m_lightColor[1][0];
}

int* CSpawnerEffect::GetSprite1Color()
{
	if (m_spawnType == SPAWNTYPE_GREEN)
		return &m_sprite1Color[0][0];
	else
		return &m_sprite1Color[1][0];
}

int* CSpawnerEffect::GetSprite2Color()
{
	if (m_spawnType == SPAWNTYPE_GREEN)
		return &m_sprite2Color[0][0];
	else
		return &m_sprite2Color[1][0];
}

static void SetEntityKeyValue(CBaseEntity* entity, const char* keyName, const char* valueAsString) 
{
	static KeyValueData kvData;
	kvData.szKeyName = (char*)keyName;
	kvData.szValue = (char*)valueAsString;
	entity->KeyValue(&kvData);
}

CBaseEntity* AlienSpawnEffect_CreateAndSpawn(
	CBaseEntity* activator,
	CBaseEntity* caller,
	const Vector& spawnPosition,
	CBaseEntity* owner,
	SpawnerEffectSpawnType spawnType)
{
	CSpawnerEffect* spawnerEffect = (CSpawnerEffect*)CBaseEntity::Create("env_spawnereffect", spawnPosition, g_vecZero, owner->edict());
	if (spawnerEffect != NULL)
	{
		if (spawnType == SPAWNTYPE_GREEN)
			SetEntityKeyValue(spawnerEffect, "spawntype", "0");
		else
			SetEntityKeyValue(spawnerEffect, "spawntype", "1");

		SetEntityKeyValue(spawnerEffect, "beamcount", "16");
		SetEntityKeyValue(spawnerEffect, "spritescale", "1");
		SetEntityKeyValue(spawnerEffect, "spawnsoundrad", "3");
		SetEntityKeyValue(spawnerEffect, "spawnvol", "0.8");
		SetEntityKeyValue(spawnerEffect, "lightrad", "24");

		spawnerEffect->Precache();
		spawnerEffect->Spawn();
		spawnerEffect->Use(activator, caller, USE_ON, 0);
	}

	return spawnerEffect;
}
