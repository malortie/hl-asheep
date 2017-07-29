#ifndef HEV_MEDIATOR_H_
#define HEV_MEDIATOR_H_

void HevMediator_CreateAndSetupSingleton();
BOOL HevMediator_PlayerAttemptToPickupSuitBattery(CBasePlayer* player, BOOL playEffects = TRUE);
BOOL HevMediator_PlayerAttemptToUseSuitCharger(CBasePlayer* player, BOOL playEffects = TRUE);

#endif // HEV_MEDIATOR_H_