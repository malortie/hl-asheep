#ifndef COMBAT_TACTICS_H__
#define COMBAT_TACTICS_H__

class CBaseCombatTactics
{
	CBaseMonster* _outer;
public:
	CBaseCombatTactics(CBaseMonster* outer)
	{
		SetOuter(outer);
	}

	CBaseMonster* GetOuter();
	void SetOuter(CBaseMonster* outer);

	virtual float GetKickDamage() const = 0;
	virtual float GetKickImpulseForce() const;

	virtual float GetPunchDamage() const = 0;
	virtual float GetPunchImpulseForce() const;

	virtual void Punch();
	virtual void PunchHitSound() {}
	virtual void PunchMissSound() {}

	virtual void Kick();
	virtual void KickHitSound() {}
	virtual void KickMissSound() {}

	virtual CBaseEntity* CheckTraceAttackHull(float distance, float damage, Vector punchAngles, Vector impulse);
	virtual float GetDefaultDistanceForTraceHullAttack();
	virtual Vector GetVictimPunchAngles();
};

#endif // COMBAT_TACTICS_H__
