#ifndef PLAYER_UTILS_H__
#define PLAYER_UTILS_H__

CBasePlayer* UTIL_GetLocalPlayer();
BOOL UTIL_IsPlayerEquippedWithArmor(CBasePlayer* player);
BOOL UTIL_IsPlayerEquippedWithSuit(CBasePlayer* player);

void PlayerMediator_AddArmorToPlayer(CBasePlayer* player);

enum PlayerBatteryDenyMessageType
{
	BATTERYDENY_BATTERY = 0,
	BATTERYDENY_SUITCHARGER,
};

void UTIL_DisplayBatteryDenyMessage(CBasePlayer* pPlayer, PlayerBatteryDenyMessageType messageType);

#endif // PLAYER_UTILS_H__
