#ifndef ASHEEP_MAP_FIXES_H__
#define ASHEEP_MAP_FIXES_H__

void MapFixes_ApplyAllPossibleFixes();
BOOL MapFixes_CheckIfOnlyKateShouldActivateTrigger(CBaseEntity* trigger);
BOOL MapFixes_CheckIfEntityIsOutroJeep(CBaseEntity* entity);
BOOL MapFixes_CheckIfShouldTeleportKateToHealingSequence(CBaseEntity* trigger);
void MapFixes_FixupTriggerForAsmap06KateHealingSequence(CBaseEntity* trigger);
#endif // ASHEEP_MAP_FIXES_H__