#ifndef PLAYER_UTILS_H__
#define PLAYER_UTILS_H__

CBasePlayer* UTIL_GetLocalPlayer();
BOOL UTIL_IsPlayerEquippedWithArmor(CBasePlayer* player);
BOOL UTIL_IsPlayerEquippedWithSuit(CBasePlayer* player);

void PlayerMediator_AddArmorToPlayer(CBasePlayer* player);
void PlayerMediator_AddSuitToPlayer(CBasePlayer* player);

#endif // PLAYER_UTILS_H__