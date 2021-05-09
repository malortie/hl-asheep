#ifndef DEAD_ACTOR_H__
#define DEAD_ACTOR_H__

class CBaseDeadActor : public CBaseMonster
{
public:
	void Spawn(void);
	void KeyValue(KeyValueData *pkvd);

	virtual int	Classify(void) = 0;

	int	m_iPose;// which sequence to display	-- temporary, don't need to save

protected:

	virtual void PrecacheModels() = 0;
	virtual void SetModel() = 0;
	virtual void SetBloodColor();
	virtual void SetSkinAndBodygroups();
	virtual const char* GetPoseByIndex( int index ) = 0;
	virtual int GetLastPoseIndex() = 0;
};

class CDeadActorWithBarneySequences : public CBaseDeadActor
{
public:
	virtual int	Classify(void);
protected:
	virtual const char* GetPoseByIndex(int index);
	virtual int GetLastPoseIndex();
};

class CDeadActorWithScientistSequences : public CBaseDeadActor
{
public:
	virtual int	Classify(void);
protected:
	virtual const char* GetPoseByIndex(int index);
	virtual int GetLastPoseIndex();
};

class CDeadActorWithHGruntSequences : public CBaseDeadActor
{
public:
	virtual int	Classify(void);
protected:
	virtual const char* GetPoseByIndex(int index);
	virtual int GetLastPoseIndex();
};

#endif // DEAD_ACTOR_H__
