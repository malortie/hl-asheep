#ifndef TALKMONSTERWITHSCIENTISTAI_H__
#define TALKMONSTERWITHSCIENTISTAI_H__

#include "talkmonster.h"

enum TalkmonsterWithScientistAITasks
{
	TASK_SAY_FEAR = LAST_TALKMONSTER_TASK + 1,
	TASK_RUN_PATH_SCARED,
	TASK_SCREAM,
	TASK_RANDOM_SCREAM,
	TASK_MOVE_TO_TARGET_RANGE_SCARED,

	LAST_TALKMONSTER_WITH_AI_TASK, // Must be last.
};

class CTalkMonsterWithScientistAI : public CTalkMonster
{
	float m_painTime;
	float m_fearTime;

public:
	void Spawn(void);
	void Precache(void);

	void SetYawSpeed(void);
	int  Classify(void);
	virtual void HandleAnimEvent(MonsterEvent_t *pEvent);
	virtual void RunTask(Task_t *pTask);
	virtual void StartTask(Task_t *pTask);
	int	ObjectCaps(void) { return CTalkMonster::ObjectCaps() | FCAP_IMPULSE_USE; }
	int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType);
	virtual int FriendNumber(int arrayNumber);
	void SetActivity(Activity newActivity);
	Activity GetStoppedActivity(void);
	int ISoundMask(void);
	void DeclineFollowing(void);

	float	CoverRadius(void) { return 1200; }		// Need more room for cover because scientists want to get far away!
	BOOL	DisregardEnemy(CBaseEntity *pEnemy) { return !pEnemy->IsAlive() || (gpGlobals->time - m_fearTime) > 15; }

	BOOL	CanHeal(void);
	void	Heal(void);
	void	Scream(void);

	// Override these to set behavior
	Schedule_t *GetScheduleOfType(int Type);
	Schedule_t *GetSchedule(void);
	MONSTERSTATE GetIdealState(void);

	void DeathSound(void);
	void PainSound(void);

	void TalkInit(void);

	void			Killed(entvars_t *pevAttacker, int iGib);

	virtual int		Save(CSave &save);
	virtual int		Restore(CRestore &restore);
	static	TYPEDESCRIPTION m_SaveData[];

	CUSTOM_SCHEDULES;

	virtual Schedule_t *GetHealSchedule(void) { return NULL; }

protected:

	virtual void PrecacheModels() = 0;
	virtual void PrecacheSounds() = 0;
	virtual void SetModel() = 0;
	virtual void SetSkinAndBodygroups();
	virtual int GetFirstTimeSpawnHealth() const = 0;

	virtual void SetFriendsSpeechOrder() = 0;
	virtual void InitSentenceGroup() = 0;
	virtual void SetDefaultVoicePitch() = 0;

	virtual void DoPainSound();
	virtual void DoDeathSound();

	virtual void SpeakDeclineFollowing() = 0;
	virtual void SpeakScream() = 0;
	virtual void SpeakFearEnemy() = 0;
	virtual void SpeakFearPlayer() = 0;
};

#endif // TALKMONSTERWITHSCIENTISTAI_H__
