/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#ifndef WEAPONS_H
#define WEAPONS_H

#include "effects.h"

class CBasePlayer;
extern int gmsgWeapPickup;

void DeactivateSatchels( CBasePlayer *pOwner );

// Contact Grenade / Timed grenade / Satchel Charge
class CGrenade : public CBaseMonster
{
public:
	void Spawn( void );

	typedef enum { SATCHEL_DETONATE = 0, SATCHEL_RELEASE } SATCHELCODE;

	static CGrenade *ShootTimed( entvars_t *pevOwner, Vector vecStart, Vector vecVelocity, float time );
	static CGrenade *ShootContact( entvars_t *pevOwner, Vector vecStart, Vector vecVelocity, float flGrenadeDamage );
	static CGrenade *ShootSatchelCharge( entvars_t *pevOwner, Vector vecStart, Vector vecVelocity );
	static void UseSatchelCharges( entvars_t *pevOwner, SATCHELCODE code );

	void Explode( Vector vecSrc, Vector vecAim );
	void Explode( TraceResult *pTrace, int bitsDamageType );
	void EXPORT Smoke( void );

	void EXPORT BounceTouch( CBaseEntity *pOther );
	void EXPORT SlideTouch( CBaseEntity *pOther );
	void EXPORT ExplodeTouch( CBaseEntity *pOther );
	void EXPORT DangerSoundThink( void );
	void EXPORT PreDetonate( void );
	void EXPORT Detonate( void );
	void EXPORT DetonateUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	void EXPORT TumbleThink( void );

	virtual void BounceSound( void );
	virtual int	BloodColor( void ) { return DONT_BLEED; }
	virtual void Killed( entvars_t *pevAttacker, int iGib );

	BOOL m_fRegisteredSound;// whether or not this grenade has issued its DANGER sound to the world sound list yet.
};


// constant items
#define ITEM_HEALTHKIT		1
#define ITEM_ANTIDOTE		2
#define ITEM_SECURITY		3
#define ITEM_BATTERY		4

#define WEAPON_NONE				0
#define WEAPON_CROWBAR			1
#define WEAPON_POOLSTICK		2
#define WEAPON_KMEDKIT			3
#define WEAPON_GLOCK			4
#define WEAPON_BARNEY9MMHG		5
#define WEAPON_BERETTA			6
#define WEAPON_PYTHON			7
#define WEAPON_MP5				8
#define WEAPON_BARNEY9MMAR		9
#define WEAPON_SHOTGUN			10
#define WEAPON_BARNEYSHOTGUN	11
#define WEAPON_CROSSBOW			12
#define WEAPON_9MMM41A			13
#define WEAPON_RPG				14
#define WEAPON_GAUSS			15
#define WEAPON_EGON				16
#define WEAPON_HORNETGUN		17
#define WEAPON_HANDGRENADE		18
#define WEAPON_BARNEYHANDGRENADE 19
#define WEAPON_SATCHEL			20
#define WEAPON_TRIPMINE			21
#define WEAPON_SNARK			22
#define WEAPON_TOAD				23
#define WEAPON_ALLWEAPONS		(~(1<<WEAPON_ARMOR))

#define WEAPON_ARMOR			30
#define WEAPON_SUIT				31	// ?????

#define MAX_WEAPONS			32


#define MAX_NORMAL_BATTERY	100


// weapon weight factors (for auto-switching)   (-1 = noswitch)
#define CROWBAR_WEIGHT		0
#define GLOCK_WEIGHT		10
#define PYTHON_WEIGHT		15
#define MP5_WEIGHT			15
#define SHOTGUN_WEIGHT		15
#define CROSSBOW_WEIGHT		10
#define RPG_WEIGHT			20
#define GAUSS_WEIGHT		20
#define EGON_WEIGHT			20
#define HORNETGUN_WEIGHT	10
#define HANDGRENADE_WEIGHT	5
#define SNARK_WEIGHT		5
#define SATCHEL_WEIGHT		-10
#define TRIPMINE_WEIGHT		-10
#define POOLSTICK_WEIGHT	CROWBAR_WEIGHT
#define KMEDKIT_WEIGHT		-10
#define BARNEY9MMHG_WEIGHT	GLOCK_WEIGHT
#define BERETTA_WEIGHT		15
#define BARNEY9MMAR_WEIGHT	MP5_WEIGHT
#define BARNEYSHOTGUN_WEIGHT SHOTGUN_WEIGHT
#define _9MMM41A_WEIGHT		15
#define BARNEYHANDGRENADE_WEIGHT HANDGRENADE_WEIGHT
#define TOAD_WEIGHT			SNARK_WEIGHT 


// weapon clip/carry ammo capacities
#define URANIUM_MAX_CARRY		100
#define	_9MM_MAX_CARRY			250
#define _357_MAX_CARRY			36
#define BUCKSHOT_MAX_CARRY		125
#define BOLT_MAX_CARRY			50
#define ROCKET_MAX_CARRY		5
#define HANDGRENADE_MAX_CARRY	10
#define SATCHEL_MAX_CARRY		5
#define TRIPMINE_MAX_CARRY		5
#define SNARK_MAX_CARRY			15
#define HORNET_MAX_CARRY		8
#define M203_GRENADE_MAX_CARRY	10
#define KMEDKIT_MAX_CARRY			3
#define BARNEY9MMHG_MAX_CARRY		_9MM_MAX_CARRY
#define BERETTA_MAX_CARRY			250
#define BARNEY9MMAR_MAX_CARRY		_9MM_MAX_CARRY
#define BARNEYSHOTGUN_MAX_CARRY		BUCKSHOT_MAX_CARRY
#define _9MMM41A_MAX_CARRY			250
#define _9MMM41A_GRENADE_MAX_CARRY	10
#define BARNEYHANDGRENADE_MAX_CARRY	HANDGRENADE_MAX_CARRY
#define TOAD_MAX_CARRY				12 

// the maximum amount of ammo each weapon's clip can hold
#define WEAPON_NOCLIP			-1

//#define CROWBAR_MAX_CLIP		WEAPON_NOCLIP
#define GLOCK_MAX_CLIP			17
#define PYTHON_MAX_CLIP			6
#define MP5_MAX_CLIP			50
#define MP5_DEFAULT_AMMO		25
#define SHOTGUN_MAX_CLIP		8
#define CROSSBOW_MAX_CLIP		5
#define RPG_MAX_CLIP			1
#define GAUSS_MAX_CLIP			WEAPON_NOCLIP
#define EGON_MAX_CLIP			WEAPON_NOCLIP
#define HORNETGUN_MAX_CLIP		WEAPON_NOCLIP
#define HANDGRENADE_MAX_CLIP	WEAPON_NOCLIP
#define SATCHEL_MAX_CLIP		WEAPON_NOCLIP
#define TRIPMINE_MAX_CLIP		WEAPON_NOCLIP
#define SNARK_MAX_CLIP			WEAPON_NOCLIP

#define KMEDKIT_MAX_CLIP		WEAPON_NOCLIP
#define BARNEY9MMHG_MAX_CLIP		GLOCK_MAX_CLIP
#define BERETTA_MAX_CLIP			15
#define BARNEY9MMAR_MAX_CLIP		MP5_MAX_CLIP
#define BARNEYSHOTGUN_MAX_CLIP		SHOTGUN_MAX_CLIP
#define _9MMM41A_MAX_CLIP			50
#define BARNEYHANDGRENADE_MAX_CLIP	HANDGRENADE_MAX_CLIP
#define TOAD_MAX_CLIP				WEAPON_NOCLIP

// the default amount of ammo that comes with each gun when it spawns
#define GLOCK_DEFAULT_GIVE			17
#define PYTHON_DEFAULT_GIVE			6
#define MP5_DEFAULT_GIVE			25
#define MP5_DEFAULT_AMMO			25
#define MP5_M203_DEFAULT_GIVE		0
#define SHOTGUN_DEFAULT_GIVE		12
#define CROSSBOW_DEFAULT_GIVE		5
#define RPG_DEFAULT_GIVE			1
#define GAUSS_DEFAULT_GIVE			20
#define EGON_DEFAULT_GIVE			20
#define HANDGRENADE_DEFAULT_GIVE	5
#define SATCHEL_DEFAULT_GIVE		1
#define TRIPMINE_DEFAULT_GIVE		1
#define SNARK_DEFAULT_GIVE			5
#define HIVEHAND_DEFAULT_GIVE		8
#define KMEDKIT_DEFAULT_GIVE			1
#define BARNEY9MMHG_DEFAULT_GIVE		GLOCK_DEFAULT_GIVE
#define BERETTA_DEFAULT_GIVE			15
#define BARNEY9MMAR_DEFAULT_GIVE		MP5_DEFAULT_GIVE
#define BARNEYSHOTGUN_DEFAULT_GIVE		SHOTGUN_DEFAULT_GIVE
#define _9MMM41A_DEFAULT_GIVE			25
#define BARNEYHANDGRENADE_DEFAULT_GIVE	HANDGRENADE_DEFAULT_GIVE
#define TOAD_DEFAULT_GIVE				1

// The amount of ammo given to a player by an ammo item.
#define AMMO_URANIUMBOX_GIVE	20
#define AMMO_GLOCKCLIP_GIVE		GLOCK_MAX_CLIP
#define AMMO_357BOX_GIVE		PYTHON_MAX_CLIP
#define AMMO_MP5CLIP_GIVE		MP5_MAX_CLIP
#define AMMO_CHAINBOX_GIVE		200
#define AMMO_M203BOX_GIVE		2
#define AMMO_BUCKSHOTBOX_GIVE	12
#define AMMO_CROSSBOWCLIP_GIVE	CROSSBOW_MAX_CLIP
#define AMMO_RPGCLIP_GIVE		RPG_MAX_CLIP
#define AMMO_URANIUMBOX_GIVE	20
#define AMMO_SNARKBOX_GIVE		5
#define AMMO_KMEDKIT_GIVE			1
#define AMMO_TOAD_GIVE				1

// bullet types
typedef	enum
{
	BULLET_NONE = 0,
	BULLET_PLAYER_9MM, // glock
	BULLET_PLAYER_MP5, // mp5
	BULLET_PLAYER_357, // python
	BULLET_PLAYER_BUCKSHOT, // shotgun
	BULLET_PLAYER_CROWBAR, // crowbar swipe
	BULLET_PLAYER_POOLSTICK,
	BULLET_PLAYER_BERETTA,
	BULLET_PLAYER_9MMM41A,

	BULLET_MONSTER_9MM,
	BULLET_MONSTER_MP5,
	BULLET_MONSTER_12MM,
	BULLET_MONSTER_9MMM41A,
	BULLET_MONSTER_BERETTA,
} Bullet;


#define ITEM_FLAG_SELECTONEMPTY		1
#define ITEM_FLAG_NOAUTORELOAD		2
#define ITEM_FLAG_NOAUTOSWITCHEMPTY	4
#define ITEM_FLAG_LIMITINWORLD		8
#define ITEM_FLAG_EXHAUSTIBLE		16 // A player can totally exhaust their ammo supply and lose this weapon

#define WEAPON_IS_ONTARGET 0x40

typedef struct
{
	int		iSlot;
	int		iPosition;
	const char	*pszAmmo1;	// ammo 1 type
	int		iMaxAmmo1;		// max ammo 1
	const char	*pszAmmo2;	// ammo 2 type
	int		iMaxAmmo2;		// max ammo 2
	const char	*pszName;
	int		iMaxClip;
	int		iId;
	int		iFlags;
	int		iWeight;// this value used to determine this weapon's importance in autoselection.
} ItemInfo;

typedef struct
{
	const char *pszName;
	int iId;
} AmmoInfo;

// Items that the player has in their inventory that they can use
class CBasePlayerItem : public CBaseAnimating
{
public:
	virtual void SetObjectCollisionBox( void );

	virtual int		Save( CSave &save );
	virtual int		Restore( CRestore &restore );
	
	static	TYPEDESCRIPTION m_SaveData[];

	virtual int AddToPlayer( CBasePlayer *pPlayer );	// return TRUE if the item you want the item added to the player inventory
	virtual int AddDuplicate( CBasePlayerItem *pItem ) { return FALSE; }	// return TRUE if you want your duplicate removed from world
	void EXPORT DestroyItem( void );
	void EXPORT DefaultTouch( CBaseEntity *pOther );	// default weapon touch
	void EXPORT FallThink ( void );// when an item is first spawned, this think is run to determine when the object has hit the ground.
	void EXPORT Materialize( void );// make a weapon visible and tangible
	void EXPORT AttemptToMaterialize( void );  // the weapon desires to become visible and tangible, if the game rules allow for it
	CBaseEntity* Respawn ( void );// copy a weapon
	void FallInit( void );
	void CheckRespawn( void );
	virtual int GetItemInfo(ItemInfo *p) { return 0; };	// returns 0 if struct not filled out
	virtual BOOL CanDeploy( void ) { return TRUE; };
	virtual BOOL Deploy( )								// returns is deploy was successful
		 { return TRUE; };

	virtual BOOL CanHolster( void ) { return TRUE; };// can this weapon be put away right now?
	virtual void Holster( int skiplocal = 0 );
	virtual void UpdateItemInfo( void ) { return; };

	virtual void ItemPreFrame( void )	{ return; }		// called each frame by the player PreThink
	virtual void ItemPostFrame( void ) { return; }		// called each frame by the player PostThink

	virtual void Drop( void );
	virtual void Kill( void );
	virtual void AttachToPlayer ( CBasePlayer *pPlayer );

	virtual int PrimaryAmmoIndex() { return -1; };
	virtual int SecondaryAmmoIndex() { return -1; };

	virtual int UpdateClientData( CBasePlayer *pPlayer ) { return 0; }

	virtual CBasePlayerItem *GetWeaponPtr( void ) { return NULL; };

	static ItemInfo ItemInfoArray[ MAX_WEAPONS ];
	static AmmoInfo AmmoInfoArray[ MAX_AMMO_SLOTS ];

	CBasePlayer	*m_pPlayer;
	CBasePlayerItem *m_pNext;
	int		m_iId;												// WEAPON_???

	virtual int iItemSlot( void ) { return 0; }			// return 0 to MAX_ITEMS_SLOTS, used in hud

	int			iItemPosition( void ) { return ItemInfoArray[ m_iId ].iPosition; }
	const char	*pszAmmo1( void )	{ return ItemInfoArray[ m_iId ].pszAmmo1; }
	int			iMaxAmmo1( void )	{ return ItemInfoArray[ m_iId ].iMaxAmmo1; }
	const char	*pszAmmo2( void )	{ return ItemInfoArray[ m_iId ].pszAmmo2; }
	int			iMaxAmmo2( void )	{ return ItemInfoArray[ m_iId ].iMaxAmmo2; }
	const char	*pszName( void )	{ return ItemInfoArray[ m_iId ].pszName; }
	int			iMaxClip( void )	{ return ItemInfoArray[ m_iId ].iMaxClip; }
	int			iWeight( void )		{ return ItemInfoArray[ m_iId ].iWeight; }
	int			iFlags( void )		{ return ItemInfoArray[ m_iId ].iFlags; }

	// int		m_iIdPrimary;										// Unique Id for primary ammo
	// int		m_iIdSecondary;										// Unique Id for secondary ammo
};


// inventory items that 
class CBasePlayerWeapon : public CBasePlayerItem
{
public:
	virtual int		Save( CSave &save );
	virtual int		Restore( CRestore &restore );
	
	static	TYPEDESCRIPTION m_SaveData[];

	// generic weapon versions of CBasePlayerItem calls
	virtual int AddToPlayer( CBasePlayer *pPlayer );
	virtual int AddDuplicate( CBasePlayerItem *pItem );

	virtual int ExtractAmmo( CBasePlayerWeapon *pWeapon ); //{ return TRUE; };			// Return TRUE if you can add ammo to yourself when picked up
	virtual int ExtractClipAmmo( CBasePlayerWeapon *pWeapon );// { return TRUE; };			// Return TRUE if you can add ammo to yourself when picked up

	virtual int AddWeapon( void ) { ExtractAmmo( this ); return TRUE; };	// Return TRUE if you want to add yourself to the player

	// generic "shared" ammo handlers
	BOOL AddPrimaryAmmo( int iCount, char *szName, int iMaxClip, int iMaxCarry );
	BOOL AddSecondaryAmmo( int iCount, char *szName, int iMaxCarry );

	virtual void UpdateItemInfo( void ) {};	// updates HUD state

	int m_iPlayEmptySound;
	int m_fFireOnEmpty;		// True when the gun is empty and the player is still holding down the
							// attack key(s)
	virtual BOOL PlayEmptySound( void );
	virtual void ResetEmptySound( void );

	virtual void SendWeaponAnim( int iAnim, int skiplocal = 1, int body = 0 );  // skiplocal is 1 if client is predicting weapon animations

	virtual BOOL CanDeploy( void );
	virtual BOOL IsUseable( void );
	BOOL DefaultDeploy( char *szViewModel, char *szWeaponModel, int iAnim, char *szAnimExt, int skiplocal = 0, int body = 0 );
	BOOL DefaultHolster( int iAnim, float flDelay, int skiplocal = 0, int body = 0 );
	int DefaultReload( int iClipSize, int iAnim, float fDelay, int body = 0 );

	virtual void ItemPostFrame( void );	// called each frame by the player PostThink
	// called by CBasePlayerWeapons ItemPostFrame()
	virtual void PrimaryAttack( void ) { return; }				// do "+ATTACK"
	virtual void SecondaryAttack( void ) { return; }			// do "+ATTACK2"
	virtual void Reload( void ) { return; }						// do "+RELOAD"
	virtual void WeaponIdle( void ) { return; }					// called when no buttons pressed
	virtual int UpdateClientData( CBasePlayer *pPlayer );		// sends hud info to client dll, if things have changed
	virtual void RetireWeapon( void );
	virtual BOOL ShouldWeaponIdle( void ) {return FALSE; };
	virtual void Holster( int skiplocal = 0 );
	virtual BOOL UseDecrement( void ) { return FALSE; };
	
	int	PrimaryAmmoIndex(); 
	int	SecondaryAmmoIndex(); 

	void PrintState( void );

	virtual CBasePlayerItem *GetWeaponPtr( void ) { return (CBasePlayerItem *)this; };
	float GetNextAttackDelay( float delay );

	float m_flPumpTime;
	int		m_fInSpecialReload;									// Are we in the middle of a reload for the shotguns
	float	m_flNextPrimaryAttack;								// soonest time ItemPostFrame will call PrimaryAttack
	float	m_flNextSecondaryAttack;							// soonest time ItemPostFrame will call SecondaryAttack
	float	m_flTimeWeaponIdle;									// soonest time ItemPostFrame will call WeaponIdle
	int		m_iPrimaryAmmoType;									// "primary" ammo index into players m_rgAmmo[]
	int		m_iSecondaryAmmoType;								// "secondary" ammo index into players m_rgAmmo[]
	int		m_iClip;											// number of shots left in the primary weapon clip, -1 it not used
	int		m_iClientClip;										// the last version of m_iClip sent to hud dll
	int		m_iClientWeaponState;								// the last version of the weapon state sent to hud dll (is current weapon, is on target)
	int		m_fInReload;										// Are we in the middle of a reload;

	int		m_iDefaultAmmo;// how much ammo you get when you pick up this weapon as placed by a level designer.
	
	// hle time creep vars
	float	m_flPrevPrimaryAttack;
	float	m_flLastFireTime;			

	BOOL UsesPrimaryAmmo();
	BOOL UsesSecondaryAmmo();
	BOOL HasPrimaryAmmo();
	BOOL HasSecondaryAmmo();
	BOOL UsesClipsForAmmo();
	int GetPrimaryAmmoCount();
	int GetSecondaryAmmoCount();
	BOOL IsAllowedToNotifyOutOfAmmo();
	void DoSuitSpeakOutOfAmmo();
};


class CBasePlayerAmmo : public CBaseEntity
{
public:
	virtual void Spawn( void );
	void EXPORT DefaultTouch( CBaseEntity *pOther ); // default weapon touch
	virtual BOOL AddAmmo( CBaseEntity *pOther ) { return TRUE; };

	CBaseEntity* Respawn( void );
	void EXPORT Materialize( void );
};


extern DLL_GLOBAL	short	g_sModelIndexLaser;// holds the index for the laser beam
extern DLL_GLOBAL	const char *g_pModelNameLaser;

extern DLL_GLOBAL	short	g_sModelIndexLaserDot;// holds the index for the laser beam dot
extern DLL_GLOBAL	short	g_sModelIndexFireball;// holds the index for the fireball
extern DLL_GLOBAL	short	g_sModelIndexSmoke;// holds the index for the smoke cloud
extern DLL_GLOBAL	short	g_sModelIndexWExplosion;// holds the index for the underwater explosion
extern DLL_GLOBAL	short	g_sModelIndexBubbles;// holds the index for the bubbles model
extern DLL_GLOBAL	short	g_sModelIndexBloodDrop;// holds the sprite index for blood drops
extern DLL_GLOBAL	short	g_sModelIndexBloodSpray;// holds the sprite index for blood spray (bigger)

extern void ClearMultiDamage(void);
extern void ApplyMultiDamage(entvars_t* pevInflictor, entvars_t* pevAttacker );
extern void AddMultiDamage( entvars_t *pevInflictor, CBaseEntity *pEntity, float flDamage, int bitsDamageType);

extern void DecalGunshot( TraceResult *pTrace, int iBulletType );
extern void SpawnBlood(Vector vecSpot, int bloodColor, float flDamage);
extern int DamageDecal( CBaseEntity *pEntity, int bitsDamageType );
extern void RadiusDamage( Vector vecSrc, entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, float flRadius, int iClassIgnore, int bitsDamageType );

typedef struct 
{
	CBaseEntity		*pEntity;
	float			amount;
	int				type;
} MULTIDAMAGE;

extern MULTIDAMAGE gMultiDamage;


#define LOUD_GUN_VOLUME			1000
#define NORMAL_GUN_VOLUME		600
#define QUIET_GUN_VOLUME		200

#define	BRIGHT_GUN_FLASH		512
#define NORMAL_GUN_FLASH		256
#define	DIM_GUN_FLASH			128

#define BIG_EXPLOSION_VOLUME	2048
#define NORMAL_EXPLOSION_VOLUME	1024
#define SMALL_EXPLOSION_VOLUME	512

#define	WEAPON_ACTIVITY_VOLUME	64

#define VECTOR_CONE_1DEGREES	Vector( 0.00873, 0.00873, 0.00873 )
#define VECTOR_CONE_2DEGREES	Vector( 0.01745, 0.01745, 0.01745 )
#define VECTOR_CONE_3DEGREES	Vector( 0.02618, 0.02618, 0.02618 )
#define VECTOR_CONE_4DEGREES	Vector( 0.03490, 0.03490, 0.03490 )
#define VECTOR_CONE_5DEGREES	Vector( 0.04362, 0.04362, 0.04362 )
#define VECTOR_CONE_6DEGREES	Vector( 0.05234, 0.05234, 0.05234 )
#define VECTOR_CONE_7DEGREES	Vector( 0.06105, 0.06105, 0.06105 )
#define VECTOR_CONE_8DEGREES	Vector( 0.06976, 0.06976, 0.06976 )
#define VECTOR_CONE_9DEGREES	Vector( 0.07846, 0.07846, 0.07846 )
#define VECTOR_CONE_10DEGREES	Vector( 0.08716, 0.08716, 0.08716 )
#define VECTOR_CONE_15DEGREES	Vector( 0.13053, 0.13053, 0.13053 )
#define VECTOR_CONE_20DEGREES	Vector( 0.17365, 0.17365, 0.17365 )

//=========================================================
// CWeaponBox - a single entity that can store weapons
// and ammo. 
//=========================================================
class CWeaponBox : public CBaseEntity
{
	void Precache( void );
	void Spawn( void );
	void Touch( CBaseEntity *pOther );
	void KeyValue( KeyValueData *pkvd );
	BOOL IsEmpty( void );
	int  GiveAmmo( int iCount, char *szName, int iMax, int *pIndex = NULL );
	void SetObjectCollisionBox( void );

public:
	void EXPORT Kill ( void );
	int		Save( CSave &save );
	int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];

	BOOL HasWeapon( CBasePlayerItem *pCheckItem );
	BOOL PackWeapon( CBasePlayerItem *pWeapon );
	BOOL PackAmmo( int iszName, int iCount );
	
	CBasePlayerItem	*m_rgpPlayerItems[MAX_ITEM_TYPES];// one slot for each 

	int m_rgiszAmmo[MAX_AMMO_SLOTS];// ammo names
	int	m_rgAmmo[MAX_AMMO_SLOTS];// ammo quantities

	int m_cAmmoTypes;// how many ammo types packed into this box (if packed by a level designer)
};

#ifdef CLIENT_DLL
bool bIsMultiplayer ( void );
void LoadVModel ( char *szViewModel, CBasePlayer *m_pPlayer );
#endif

#include "weaponinterface.h"
#include "weapon_util.h"
#include "baseweaponmelee.h"
#include "baseweaponpistol.h"
#include "baseweaponarwithgrenadelauncher.h"
#include "baseweaponshotgun.h"
#include "baseweapongrenade.h"
#include "baseweaponwiththrowablemonster.h"

class CGlock : public CBaseWeaponPistol
{
	typedef CBaseWeaponPistol BaseClass;
public:
	void Spawn(void);
	int iItemSlot(void) { return 2; }
	int GetItemInfo(ItemInfo *p);

	const char* GetThirdpersonModel();
	const char* GetViewModel();
	const char* GetWorldModel();
	int GetWeaponID();

	void PrecacheModels();
	void PrecacheEvents();

	int GetBulletType();
	void GiveFirstTimeSpawnDefaultAmmo();

	const unsigned short& GetPrimaryFireEvent() const;
	const unsigned short& GetSecondaryFireEvent() const;

	int GetDeploySequence();
	int GetReloadSequence();
	int GetReloadEmptySequence();
	int GetHolsterSequence();

	float GetReloadSequenceDuration();
	float GetReloadEmptySequenceDuration();
	float GetHolsterSequenceDuration();

	float PlayIdleAnimation();

private:

	unsigned short m_usFireGlock1;
	unsigned short m_usFireGlock2;
};

class CCrowbar : public CBaseWeaponMelee
{
	typedef CBaseWeaponMelee BaseClass;
public:
	int iItemSlot(void) { return 1; }
	int GetItemInfo(ItemInfo *p);

	int GetWeaponID();
	const char* GetThirdpersonModel();
	const char* GetViewModel();
	const char* GetWorldModel();
	float GetMeleeDamage();

	int GetBulletType();

	void PrecacheModels();
	void PrecacheSounds();
	void PrecacheEvents();

	const unsigned short& GetSwingEvent() const;

	int GetDeploySequence();
	int GetHolsterSequence();
	float GetHolsterSequenceDuration();

	void PlayMissAnimation();
	void PlayMissSound();

	void PlayHitAnimation();
	void PlayHitBodySound();
	void PlayHitWorldSound(float fvolbar);

private:
	unsigned short m_usCrowbar;

	static const char* pMissSounds[];
	static const char* pHitBodySounds[];
	static const char* pHitWorldSounds[];
};

class CPython : public CBasePlayerWeapon
{
public:
	void Spawn( void );
	void Precache( void );
	int iItemSlot( void ) { return 2; }
	int GetItemInfo(ItemInfo *p);
	int AddToPlayer( CBasePlayer *pPlayer );
	void PrimaryAttack( void );
	void SecondaryAttack( void );
	BOOL Deploy( void );
	void Holster( int skiplocal = 0 );
	void Reload( void );
	void WeaponIdle( void );

	BOOL m_fInZoom;// don't save this. 

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

private:
	unsigned short m_usFirePython;
};

class CMP5 : public CBaseWeaponARWithGrenadeLauncher
{
	typedef CBaseWeaponARWithGrenadeLauncher BaseClass;
public:
	void Spawn(void);
	int iItemSlot(void) { return 3; }
	int GetItemInfo(ItemInfo *p);

	const char* GetThirdpersonModel();
	const char* GetViewModel();
	const char* GetWorldModel();
	int GetWeaponID();

	void PrecacheModels();
	void PrecacheEvents();
	void GiveFirstTimeSpawnDefaultAmmo();

	const unsigned short& GetFireEvent() const;
	const unsigned short& GetGrenadeLaunchEvent() const;

	int GetBulletType();

	int GetDeploySequence();
	int GetHolsterSequence();
	int GetReloadSequence();

	float GetHolsterSequenceDuration();
	float GetReloadSequenceDuration();

	float PlayIdleAnimation();

private:
	unsigned short m_usMP5;
	unsigned short m_usMP52;
};

class CCrossbow : public CBasePlayerWeapon
{
public:
	void Spawn( void );
	void Precache( void );
	int iItemSlot( ) { return 3; }
	int GetItemInfo(ItemInfo *p);

	void FireBolt( void );
	void FireSniperBolt( void );
	void PrimaryAttack( void );
	void SecondaryAttack( void );
	int AddToPlayer( CBasePlayer *pPlayer );
	BOOL Deploy( );
	void Holster( int skiplocal = 0 );
	void Reload( void );
	void WeaponIdle( void );

	int m_fInZoom; // don't save this

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

private:
	unsigned short m_usCrossbow;
	unsigned short m_usCrossbow2;
};

// p->iSlot = 2;
// p->iPosition = 2;
class CShotgun : public CBaseWeaponShotgun
{
	typedef CBaseWeaponShotgun BaseClass;
public:
	int iItemSlot() { return 3; }
	int GetItemInfo(ItemInfo *p);

	int GetWeaponID();
	const char* GetThirdpersonModel();
	const char* GetViewModel();
	const char* GetWorldModel();

	void PrecacheModels();
	void GiveFirstTimeSpawnDefaultAmmo();

	int GetDeploySequence();
	int GetHolsterSequence();
	float GetHolsterSequenceDuration();

	float PlayIdleAnimation();
	float PlayReloadAnimation();
	float PlayStartReloadAnimation();
	float PlayPumpAnimation();

	void PlayReloadSound();
	void PlayPumpSound();

private:

	static const char* pReloadSounds[];
};

class CLaserSpot : public CBaseEntity
{
	void Spawn( void );
	void Precache( void );

	int	ObjectCaps( void ) { return FCAP_DONT_SAVE; }

public:
	void Suspend( float flSuspendTime );
	void EXPORT Revive( void );
	
	static CLaserSpot *CreateSpot( void );
};

class CRpg : public CBasePlayerWeapon
{
public:

#ifndef CLIENT_DLL
	int		Save( CSave &save );
	int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];
#endif

	void Spawn( void );
	void Precache( void );
	void Reload( void );
	int iItemSlot( void ) { return 4; }
	int GetItemInfo(ItemInfo *p);
	int AddToPlayer( CBasePlayer *pPlayer );

	BOOL Deploy( void );
	BOOL CanHolster( void );
	void Holster( int skiplocal = 0 );

	void PrimaryAttack( void );
	void SecondaryAttack( void );
	void WeaponIdle( void );

	void UpdateSpot( void );
	BOOL ShouldWeaponIdle( void ) { return TRUE; };

	CLaserSpot *m_pSpot;
	int m_fSpotActive;
	int m_cActiveRockets;// how many missiles in flight from this launcher right now?

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

private:
	unsigned short m_usRpg;

};

class CRpgRocket : public CGrenade
{
public:
	int		Save( CSave &save );
	int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];
	void Spawn( void );
	void Precache( void );
	void EXPORT FollowThink( void );
	void EXPORT IgniteThink( void );
	void EXPORT RocketTouch( CBaseEntity *pOther );
	static CRpgRocket *CreateRpgRocket( Vector vecOrigin, Vector vecAngles, CBaseEntity *pOwner, CRpg *pLauncher );

	int m_iTrail;
	float m_flIgniteTime;
	CRpg *m_pLauncher;// pointer back to the launcher that fired me. 
};

class CGauss : public CBasePlayerWeapon
{
public:

#ifndef CLIENT_DLL
	int		Save( CSave &save );
	int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];
#endif

	void Spawn( void );
	void Precache( void );
	int iItemSlot( void ) { return 4; }
	int GetItemInfo(ItemInfo *p);
	int AddToPlayer( CBasePlayer *pPlayer );

	BOOL Deploy( void );
	void Holster( int skiplocal = 0  );

	void PrimaryAttack( void );
	void SecondaryAttack( void );
	void WeaponIdle( void );
	
	void StartFire( void );
	void Fire( Vector vecOrigSrc, Vector vecDirShooting, float flDamage );
	float GetFullChargeTime( void );
	int m_iBalls;
	int m_iGlow;
	int m_iBeam;
	int m_iSoundState; // don't save this

	// was this weapon just fired primary or secondary?
	// we need to know so we can pick the right set of effects. 
	BOOL m_fPrimaryFire;

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

private:
	unsigned short m_usGaussFire;
	unsigned short m_usGaussSpin;
};

class CEgon : public CBasePlayerWeapon
{
public:
#ifndef CLIENT_DLL
	int		Save( CSave &save );
	int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];
#endif

	void Spawn( void );
	void Precache( void );
	int iItemSlot( void ) { return 4; }
	int GetItemInfo(ItemInfo *p);
	int AddToPlayer( CBasePlayer *pPlayer );

	BOOL Deploy( void );
	void Holster( int skiplocal = 0 );

	void UpdateEffect( const Vector &startPoint, const Vector &endPoint, float timeBlend );

	void CreateEffect ( void );
	void DestroyEffect ( void );

	void EndAttack( void );
	void Attack( void );
	void PrimaryAttack( void );
	void WeaponIdle( void );

	float m_flAmmoUseTime;// since we use < 1 point of ammo per update, we subtract ammo on a timer.

	float GetPulseInterval( void );
	float GetDischargeInterval( void );

	void Fire( const Vector &vecOrigSrc, const Vector &vecDir );

	BOOL HasAmmo( void );

	void UseAmmo( int count );
	
	enum EGON_FIREMODE { FIRE_NARROW, FIRE_WIDE};

	CBeam				*m_pBeam;
	CBeam				*m_pNoise;
	CSprite				*m_pSprite;

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

	unsigned short m_usEgonStop;

private:
	float				m_shootTime;
	EGON_FIREMODE		m_fireMode;
	float				m_shakeTime;
	BOOL				m_deployed;

	unsigned short m_usEgonFire;
};

class CHgun : public CBasePlayerWeapon
{
public:
	void Spawn( void );
	void Precache( void );
	int iItemSlot( void ) { return 4; }
	int GetItemInfo(ItemInfo *p);
	int AddToPlayer( CBasePlayer *pPlayer );

	void PrimaryAttack( void );
	void SecondaryAttack( void );
	BOOL Deploy( void );
	BOOL IsUseable( void );
	void Holster( int skiplocal = 0 );
	void Reload( void );
	void WeaponIdle( void );
	float m_flNextAnimTime;

	float m_flRechargeTime;
	
	int m_iFirePhase;// don't save me.

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}
private:
	unsigned short m_usHornetFire;
};


class CHandGrenade : public CBaseWeaponGrenade
{
public:

	int iItemSlot(void) { return 5; }
	int GetItemInfo(ItemInfo *p);

	const char* GetThirdpersonModel();
	const char* GetViewModel();
	const char* GetWorldModel();
	int GetWeaponID();

	void PrecacheModels();
	void GiveFirstTimeSpawnDefaultAmmo();

	int GetDeploySequence();
	int GetHolsterSequence();

	float GetHolsterSequenceDuration();

	float PlayPinPullAnimation();
	float PlayThrowAnimation(float tossVelocity);
	float PlayIdleAnimation();
};

class CSatchel : public CBasePlayerWeapon
{
public:

#ifndef CLIENT_DLL
	int		Save( CSave &save );
	int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];
#endif

	void Spawn( void );
	void Precache( void );
	int iItemSlot( void ) { return 5; }
	int GetItemInfo(ItemInfo *p);
	int AddToPlayer( CBasePlayer *pPlayer );
	void PrimaryAttack( void );
	void SecondaryAttack( void );
	int AddDuplicate( CBasePlayerItem *pOriginal );
	BOOL CanDeploy( void );
	BOOL Deploy( void );
	BOOL IsUseable( void );
	
	void Holster( int skiplocal = 0 );
	void WeaponIdle( void );
	void Throw( void );
	
	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}
};


class CTripmine : public CBasePlayerWeapon
{
public:
	void Spawn( void );
	void Precache( void );
	int iItemSlot( void ) { return 5; }
	int GetItemInfo(ItemInfo *p);
	void SetObjectCollisionBox( void )
	{
		//!!!BUGBUG - fix the model!
		pev->absmin = pev->origin + Vector(-16, -16, -5);
		pev->absmax = pev->origin + Vector(16, 16, 28); 
	}

	void PrimaryAttack( void );
	BOOL Deploy( void );
	void Holster( int skiplocal = 0 );
	void WeaponIdle( void );

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

private:
	unsigned short m_usTripFire;

};

class CSqueak : public CBaseWeaponWithThrowableMonster
{
	typedef CBaseWeaponWithThrowableMonster BaseClass;
public:
	int iItemSlot(void) { return 5; }
	int GetItemInfo(ItemInfo *p);

	const char* GetThirdpersonModel();
	const char* GetViewModel();
	const char* GetWorldModel();
	int GetWeaponID();

	void PrecacheModels();
	void PrecacheSounds();
	void PrecacheEvents();
	void GiveFirstTimeSpawnDefaultAmmo();

	const unsigned short& GetThrowEvent() const;
	const char* GetClassnameOfMonsterToThrow();

	int GetDeploySequence();
	int GetHolsterSequence();
	float GetHolsterSequenceDuration();

	float PlayIdleAnimation();
	void PlayDeploySound();
	void PlayHuntSound();

private:
	unsigned short m_usSnarkFire;

	static const char* pDeploySounds[];
	static const char* pHuntSounds[];
};

// p->iPosition = 1;
class CBarney9MMHandgun : public CBaseWeaponPistol
{
	typedef CBaseWeaponPistol BaseClass;
public:
	int iItemSlot(void) { return 2; }
	int GetItemInfo(ItemInfo *p);

	const char* GetThirdpersonModel();
	const char* GetViewModel();
	const char* GetWorldModel();
	int GetWeaponID();

	void PrecacheModels();
	void PrecacheEvents();

	int GetBulletType();
	void GiveFirstTimeSpawnDefaultAmmo();

	const unsigned short& GetPrimaryFireEvent() const;
	const unsigned short& GetSecondaryFireEvent() const;

	int GetDeploySequence();
	int GetReloadSequence();
	int GetReloadEmptySequence();
	int GetHolsterSequence();

	float GetReloadSequenceDuration();
	float GetReloadEmptySequenceDuration();
	float GetHolsterSequenceDuration();

	float PlayIdleAnimation();

private:

	unsigned short m_usFireHandgun1;
	unsigned short m_usFireHandgun2;
};

// p->iPosition = 1;
class CBarney9MMAR : public CBaseWeaponARWithGrenadeLauncher
{
	typedef CBaseWeaponARWithGrenadeLauncher BaseClass;
public:
	int iItemSlot(void) { return 3; }
	int GetItemInfo(ItemInfo *p);

	const char* GetThirdpersonModel();
	const char* GetViewModel();
	const char* GetWorldModel();
	int GetWeaponID();

	void PrecacheModels();
	void PrecacheEvents();
	void GiveFirstTimeSpawnDefaultAmmo();

	const unsigned short& GetFireEvent() const;
	const unsigned short& GetGrenadeLaunchEvent() const;

	int GetBulletType();

	int GetDeploySequence();
	int GetHolsterSequence();
	int GetReloadSequence();

	float GetHolsterSequenceDuration();
	float GetReloadSequenceDuration();

	float PlayIdleAnimation();

private:
	unsigned short m_usBarney9MMAR;
	unsigned short m_usBarney9MMAR2;
};

//p->iPosition = 3;
class CBarneyShotgun : public CBaseWeaponShotgun
{
	typedef CBaseWeaponShotgun BaseClass;
public:
	int iItemSlot() { return 3; }
	int GetItemInfo(ItemInfo *p);

	int GetWeaponID();
	const char* GetThirdpersonModel();
	const char* GetViewModel();
	const char* GetWorldModel();

	void PrecacheModels();
	void GiveFirstTimeSpawnDefaultAmmo();

	int GetDeploySequence();
	int GetHolsterSequence();
	float GetHolsterSequenceDuration();

	float PlayIdleAnimation();
	float PlayReloadAnimation();
	float PlayStartReloadAnimation();
	float PlayPumpAnimation();

	void PlayReloadSound();
	void PlayPumpSound();

private:

	static const char* pReloadSounds[];
};

//p->iPosition = 1;
class CBarneyHandGrenade : public CBaseWeaponGrenade
{
public:

	int GetItemInfo(ItemInfo *p);

	const char* GetThirdpersonModel();
	const char* GetViewModel();
	const char* GetWorldModel();
	int GetWeaponID();

	void PrecacheModels();
	void GiveFirstTimeSpawnDefaultAmmo();

	int GetDeploySequence();
	int GetHolsterSequence();

	float GetHolsterSequenceDuration();

	float PlayPinPullAnimation();
	float PlayThrowAnimation(float tossVelocity);
	float PlayIdleAnimation();
};

class CPoolstick : public CBaseWeaponMelee
{
	typedef CBaseWeaponMelee BaseClass;
public:
	int GetItemInfo(ItemInfo *p);
	int iItemSlot(void) { return 1; }

	int GetWeaponID();
	const char* GetThirdpersonModel();
	const char* GetViewModel();
	const char* GetWorldModel();
	float GetMeleeDamage();
	int GetBulletType();

	void PrecacheModels();
	void PrecacheSounds();
	void PrecacheEvents();

	const unsigned short& GetSwingEvent() const;

	int GetDeploySequence();
	int GetHolsterSequence();
	float GetHolsterSequenceDuration();

	void PlayMissAnimation();
	void PlayMissSound();

	void PlayHitAnimation();
	void PlayHitBodySound();
	void PlayHitWorldSound(float fvolbar);

private:
	unsigned short m_usPoolstick;

	static const char* pMissSounds[];
	static const char* pHitBodySounds[];
	static const char* pHitWorldSounds[];
};

//p->iSlot = 1;
//p->iPosition = 2;
//
//int iItemSlot(void) { return 2; }
//
class CBeretta : public CBaseWeaponPistol
{
	typedef CBaseWeaponPistol BaseClass;
public:
	int GetItemInfo(ItemInfo *p);
	int iItemSlot(void) { return 2; }

	const char* GetThirdpersonModel();
	const char* GetViewModel();
	const char* GetWorldModel();
	int GetWeaponID();

	void PrecacheModels();
	void PrecacheEvents();

	int GetBulletType();
	void GiveFirstTimeSpawnDefaultAmmo();

	const unsigned short& GetPrimaryFireEvent() const;
	const unsigned short& GetSecondaryFireEvent() const;

	int GetDeploySequence();
	int GetReloadSequence();
	int GetReloadEmptySequence();
	int GetHolsterSequence();

	float GetReloadSequenceDuration();
	float GetReloadEmptySequenceDuration();
	float GetHolsterSequenceDuration();

	float PlayIdleAnimation();

private:

	unsigned short m_usFireBeretta1;
	unsigned short m_usFireBeretta2;
};

class C9MMM41A : public CBaseWeaponARWithGrenadeLauncher
{
	typedef CBaseWeaponARWithGrenadeLauncher BaseClass;
public:
	int GetItemInfo(ItemInfo *p);
	int iItemSlot(void) { return 3; }

	const char* GetThirdpersonModel();
	const char* GetViewModel();
	const char* GetWorldModel();
	int GetWeaponID();

	void PrecacheModels();
	void PrecacheEvents();
	void GiveFirstTimeSpawnDefaultAmmo();

	const unsigned short& GetFireEvent() const;
	const unsigned short& GetGrenadeLaunchEvent() const;
	float GetGrenadeDamage();
	int GetBulletType();

	int GetDeploySequence();
	int GetHolsterSequence();
	int GetReloadSequence();

	float GetHolsterSequenceDuration();
	float GetReloadSequenceDuration();
	float PlayIdleAnimation();

private:
	unsigned short m_us9MMM41A;
	unsigned short m_us9MMM41A2;
};

class CKMedkitSentence;

class CKMedkit : public CBasePlayerWeapon
{
public:

#ifndef CLIENT_DLL
	int		Save(CSave &save);
	int		Restore(CRestore &restore);
	static	TYPEDESCRIPTION m_SaveData[];
#endif

	void Spawn(void);
	void Precache(void);
	int iItemSlot(void) { return 1; }
	int GetItemInfo(ItemInfo *p);

	void PrimaryAttack(void);
	void SecondaryAttack(void);
	BOOL Deploy(void);
	void Holster(int skiplocal = 0);
	void WeaponIdle(void);

	virtual BOOL UseDecrement(void)
	{
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

	virtual BOOL CanDeploy(void) { return TRUE; }
	virtual BOOL IsUseable(void) { return TRUE; }
	virtual BOOL ShouldWeaponIdle(void);

	float GetHealAmount() const;

	BOOL HasAmmo(void);
	void UseAmmo(int count);

	float GetHealSequenceDuration() const;
	float GetSayHealthSequenceDuration() const;

	float GetStateHealDelay();
	float GetStateVitalSignsDelay();

	BOOL IsCurrentlyUsingMedkit() const;
	BOOL IsAllowedToHealTarget(CBaseEntity* target);
	BOOL IsAllowedToSayTargetHealth(CBaseEntity* target);
	void StartPressButton(CBaseEntity* target, BOOL isSecondaryAttack);
	void PlayMissSound();
	void PlayHealSound();
	void PlayBellSound();
	void PlayVitalSignsSound(float* resultingSoundDuration);
	void PlaySoundPercent(float* resultingSoundDuration);
	void State_Heal();
	void State_VitalSigns();
	void State_HealthPercentage();
	void State_SoundPercent();
	void GetTextColorForHealth(const int health, byte& r, byte& g, byte& b);
	void ResetVariables();

	CBaseEntity* CheckForEntityInFrontOfPlayer(float maxDistance);

	void ChooseAndExecuteUseState(int useState);

protected:
	void DisplayTargetHealth();

	CKMedkitSentence* GetSentenceManager() const;

private:
	void SendMedkitAnim(int useType);
	void SetNextAttack(float nextAttackTime);

	void UpdateState();

	enum KMedkitUseState
	{
		STATE_RESETVARIABLES = -1,
		STATE_NONE = 0,
		STATE_HEAL,
		STATE_VITALSIGNS,
		STATE_HEALTH_PERCENTAGE,
		STATE_SOUND_PERCENT,
	};

	CBaseEntity* m_target;
	int m_targetHealth;
	int m_targetMaxHealth;
	int m_healthPercentState;

	unsigned short m_usMedkit;
};

class CToadWeaponMonster;

class CToadWeapon : public CBaseWeaponWithThrowableMonster
{
	typedef CBaseWeaponWithThrowableMonster BaseClass;
public:
	void Spawn(void);
	void Precache(void);

	int iItemSlot(void) { return 5; }
	int GetItemInfo(ItemInfo *p);

	const char* GetThirdpersonModel();
	const char* GetViewModel();
	const char* GetWorldModel();
	int GetWeaponID();

	void PrecacheModels();
	void PrecacheSounds();
	void PrecacheEvents();
	void GiveFirstTimeSpawnDefaultAmmo();

	const unsigned short& GetThrowEvent() const;
	const char* GetClassnameOfMonsterToThrow();

	int GetDeploySequence();
	int GetHolsterSequence();
	float GetHolsterSequenceDuration();

	float PlayIdleAnimation();
	void PlayDeploySound();
	void PlayHuntSound();

	BOOL m_doNotSpawnToad;

#if !defined ( CLIENT_DLL )
protected:
	virtual const char* GetClassnameOfMonsterToCreate();
	void SpawnMonsterAndRemoveWeapon();
#endif // !defined ( CLIENT_DLL )

private:
	unsigned short m_usToad;

	static const char* pDeploySounds[];
	static const char* pHuntSounds[];
};

#endif // WEAPONS_H
