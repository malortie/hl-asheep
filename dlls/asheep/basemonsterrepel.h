#ifndef BASEMONSTERREPEL_H__
#define BASEMONSTERREPEL_H__

class CBaseMonsterRepel : public CBaseEntity
{
public:
	void Spawn( void );
	void Precache( void );
	void EXPORT RepelUse ( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	int m_iSpriteTexture;	// Don't save, precache

	virtual const char* GetClassNameOfMonsterToSpawn() const = 0;
	virtual const char* GetRopeSpriteName() const;
	virtual float GetFirstTimeSpawnRepellingSpeed() const;
	
private:
	string_t m_iszClassNameOfMonsterToSpawn;
};

#endif // BASEMONSTERREPEL_H__
