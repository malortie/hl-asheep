#ifndef COMBAT_TACTICS_H__
#define COMBAT_TACTICS_H__

class CBaseCombatTactics
{
public:
	virtual float GetKickDamage() const = 0;
	virtual float GetKickImpulseForce() const;

	virtual float GetPunchDamage() const = 0;
	virtual float GetPunchImpulseForce() const;

	virtual void Punch(CBaseMonster* const outer);
	virtual void PunchHitSound(CBaseMonster* const outer) {}
	virtual void PunchMissSound(CBaseMonster* const outer) {}

	virtual void Kick(CBaseMonster* const outer);
	virtual void KickHitSound(CBaseMonster* const outer) {}
	virtual void KickMissSound(CBaseMonster* const outer) {}

	virtual CBaseEntity* CheckTraceAttackHull(CBaseMonster* const outer, float distance, float damage, Vector punchAngles, Vector impulse);
	virtual float GetDefaultDistanceForTraceHullAttack();
	virtual Vector GetVictimPunchAngles();
};

#endif // COMBAT_TACTICS_H__
