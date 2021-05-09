#ifndef ASHEEP_MESSAGES_H__
#define ASHEEP_MESSAGES_H__

void Message_CreateAndSetupSingleton();
void Message_ShowMessage(const char* message);
void Message_RemoveTrainingMapMessages();
BOOL Message_MessagesAllowedInCurrentMap();

#endif // ASHEEP_MESSAGES_H__
