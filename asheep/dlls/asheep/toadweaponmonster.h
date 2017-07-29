#ifndef TOADWEAPONMONSTER_H__
#define TOADWEAPONMONSTER_H__

class CToadWeaponMonster : public CBaseMonster
{
	typedef CBaseMonster BaseClass;
public:
	void Spawn(void);
	void Precache(void);
	void SetYawSpeed(void);
	void EXPORT MonsterThink(void);
	void Move(float flInterval);
	void PickNewDest(int iCondition);
	void EXPORT Touch(CBaseEntity *pOther);

	virtual int Save(CSave& save);
	virtual int Restore(CRestore& restore);
	static TYPEDESCRIPTION m_SaveData[];

	int		Classify(void) { return CLASS_INSECT; }
	void	Look(int iDistance);
	int		ISoundMask(void);

	int		m_iMode;
	// -----------------------------

private:
	static const char* pHuntSounds[];

	EHANDLE m_scaryEntity;
	float m_flNextSoundTime;

public:
	void Advance();
	void Stop(void);
protected:

	void PlayIdleAnimation();
	void PlayWalkAnimation();
	void PlayHuntSound();
	void UpdateNextSound(BOOL didCroakSound);
	BOOL IsPlayingWalkSequence();
	void UpdateAnimation();
	Vector ComputeRunawayPosition(const Vector& enemyPosition, float stepSize);
	Vector ComputeRandomPosition(float stepSize);
	virtual const char* GetClassnameOfWeaponToGiveToToucher(CBaseEntity* toucher);

};

#endif // TOADWEAPONMONSTER_H__