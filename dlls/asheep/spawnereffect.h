#ifndef SPAWNER_EFFECT_H__
#define SPAWNER_EFFECT_H__

enum SpawnerEffectSpawnType
{
	SPAWNTYPE_GREEN = 0,
	SPAWNTYPE_RED,
};

CBaseEntity* AlienSpawnEffect_CreateAndSpawn(
	CBaseEntity* activator,
	CBaseEntity* caller,
	const Vector& spawnPosition,
	CBaseEntity* owner,
	SpawnerEffectSpawnType spawnType);

#endif // SPAWNER_EFFECT_H__
