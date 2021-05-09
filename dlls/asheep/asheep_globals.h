#ifndef ASHEEP_GLOBALS_H__
#define ASHEEP_GLOBALS_H__

class AsheepGlobals
{
public:
	BOOL IsMapT0a0();
	BOOL IsMapT0a0a();
	BOOL IsMapT0a0b();
	BOOL IsMapT0a0b1();
	BOOL IsMapT0a0b2();
	BOOL IsMapT0a0c();
	BOOL IsMapT0a0d();
	BOOL IsMapPartOfTraining();
	BOOL IsMapAsmap00b();
	BOOL IsMapAsmap05();
	BOOL IsMapAsmap05b();
	BOOL IsMapAsmap06c();
	BOOL IsMapAsmap08b();
	BOOL IsMapAsmap09c();
	BOOL IsMapAsmap10b();
	BOOL IsMapAsmap11c();
	BOOL IsMapIntro();
	BOOL IsMapOutro();
	BOOL IsMapCredits();
};

AsheepGlobals* GetAsheepGlobalsSingleton();

#endif // ASHEEP_GLOBALS_H__
