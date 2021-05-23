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

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "gamerules.h"

// Given a monster with 38 of health.
// When transiting from 30 to 8, cut
// a small portion of the overall (30)
// sound duration, to give a blending
// effect.
#define PERCENTAGE_SENTENCE_SHIFT 0.1

class CKMedkitSentence
{
public:
	const char* m_KMedkitSentences[31];

	virtual void Init();
	virtual float GetSentenceDuration(char* sentence);
	BOOL ShouldPercentageSentenceBePlayedInTwoParts(int percent);

	virtual const char* GetSentenceSayHealth() const;
	virtual const char* GetSentenceVitalSigns() const;
	virtual const char* GetSentencePercent() const;
	virtual const char* GetSentenceOne() const;
	virtual const char* GetSentenceTwo() const;
	virtual const char* GetSentenceThree() const;
	virtual const char* GetSentenceFour() const;
	virtual const char* GetSentenceFive() const;
	virtual const char* GetSentenceSix() const;
	virtual const char* GetSentenceSeven() const;
	virtual const char* GetSentenceEight() const;
	virtual const char* GetSentenceNine() const;
	virtual const char* GetSentenceTen() const;
	virtual const char* GetSentenceEleven() const;
	virtual const char* GetSentenceTwelve() const;
	virtual const char* GetSentenceThirteen() const;
	virtual const char* GetSentenceFourteen() const;
	virtual const char* GetSentenceFifteen() const;
	virtual const char* GetSentenceSixteen() const;
	virtual const char* GetSentenceSeventeen() const;
	virtual const char* GetSentenceEighteen() const;
	virtual const char* GetSentenceNineteen() const;
	virtual const char* GetSentenceTwenty() const;
	virtual const char* GetSentenceThirty() const;
	virtual const char* GetSentenceFourty() const;
	virtual const char* GetSentenceFifty() const;
	virtual const char* GetSentenceSixty() const;
	virtual const char* GetSentenceSeventy() const;
	virtual const char* GetSentenceEighty() const;
	virtual const char* GetSentenceNinety() const;
	virtual const char* GetSentenceOneHundred() const;

	virtual float GetTwoPartTensBlendingDelay() const;

	void PlaySentence(CBasePlayer* player, const char* sentence, float* resultSoundDuration);
	void PlaySentenceFromPercentage(CBasePlayer* player, int health,
		float* resultSoundDuration, BOOL playTensOnPercentageWithRemainder = TRUE);
};

const char* CKMedkitSentence::GetSentenceSayHealth() const { return "KA_HEALTHST1"; }
const char* CKMedkitSentence::GetSentenceVitalSigns() const { return "KA_HEALTHST2"; }
const char* CKMedkitSentence::GetSentencePercent() const { return "KA_HEALTHED"; }
const char* CKMedkitSentence::GetSentenceOne() const { return "KA_HEALTH1"; }
const char* CKMedkitSentence::GetSentenceTwo() const { return "KA_HEALTH2"; }
const char* CKMedkitSentence::GetSentenceThree() const { return "KA_HEALTH3"; }
const char* CKMedkitSentence::GetSentenceFour() const { return "KA_HEALTH4"; }
const char* CKMedkitSentence::GetSentenceFive() const { return "KA_HEALTH5"; }
const char* CKMedkitSentence::GetSentenceSix() const { return "KA_HEALTH6"; }
const char* CKMedkitSentence::GetSentenceSeven() const { return "KA_HEALTH7"; }
const char* CKMedkitSentence::GetSentenceEight() const { return "KA_HEALTH8"; }
const char* CKMedkitSentence::GetSentenceNine() const { return "KA_HEALTH9"; }
const char* CKMedkitSentence::GetSentenceTen() const { return "KA_HEALTH10"; }
const char* CKMedkitSentence::GetSentenceEleven() const { return "KA_HEALTH11"; }
const char* CKMedkitSentence::GetSentenceTwelve() const { return "KA_HEALTH12"; }
const char* CKMedkitSentence::GetSentenceThirteen() const { return "KA_HEALTH13"; }
const char* CKMedkitSentence::GetSentenceFourteen() const { return "KA_HEALTH14"; }
const char* CKMedkitSentence::GetSentenceFifteen() const { return "KA_HEALTH15"; }
const char* CKMedkitSentence::GetSentenceSixteen() const { return "KA_HEALTH16"; }
const char* CKMedkitSentence::GetSentenceSeventeen() const { return "KA_HEALTH17"; }
const char* CKMedkitSentence::GetSentenceEighteen() const { return "KA_HEALTH18"; }
const char* CKMedkitSentence::GetSentenceNineteen() const { return "KA_HEALTH19"; }
const char* CKMedkitSentence::GetSentenceTwenty() const { return "KA_HEALTH20"; }
const char* CKMedkitSentence::GetSentenceThirty() const { return "KA_HEALTH30"; }
const char* CKMedkitSentence::GetSentenceFourty() const { return "KA_HEALTH40"; }
const char* CKMedkitSentence::GetSentenceFifty() const { return "KA_HEALTH50"; }
const char* CKMedkitSentence::GetSentenceSixty() const { return "KA_HEALTH60"; }
const char* CKMedkitSentence::GetSentenceSeventy() const { return "KA_HEALTH70"; }
const char* CKMedkitSentence::GetSentenceEighty() const { return "KA_HEALTH80"; }
const char* CKMedkitSentence::GetSentenceNinety() const { return "KA_HEALTH90"; }
const char* CKMedkitSentence::GetSentenceOneHundred() const { return "KA_HEALTH00"; }

float CKMedkitSentence::GetTwoPartTensBlendingDelay() const
{
	return PERCENTAGE_SENTENCE_SHIFT;
}

void CKMedkitSentence::Init()
{
	m_KMedkitSentences[0] = GetSentenceSayHealth();
	m_KMedkitSentences[1] = GetSentenceVitalSigns();
	m_KMedkitSentences[2] = GetSentencePercent();
	m_KMedkitSentences[3] = GetSentenceOne();
	m_KMedkitSentences[4] = GetSentenceTwo();
	m_KMedkitSentences[5] = GetSentenceThree();
	m_KMedkitSentences[6] = GetSentenceFour();
	m_KMedkitSentences[7] = GetSentenceFive();
	m_KMedkitSentences[8] = GetSentenceSix();
	m_KMedkitSentences[9] = GetSentenceSeven();
	m_KMedkitSentences[10] = GetSentenceEight();
	m_KMedkitSentences[11] = GetSentenceNine();
	m_KMedkitSentences[12] = GetSentenceTen();
	m_KMedkitSentences[13] = GetSentenceEleven();
	m_KMedkitSentences[14] = GetSentenceTwelve();
	m_KMedkitSentences[15] = GetSentenceThirteen();
	m_KMedkitSentences[16] = GetSentenceFourteen();
	m_KMedkitSentences[17] = GetSentenceFifteen();
	m_KMedkitSentences[18] = GetSentenceSixteen();
	m_KMedkitSentences[19] = GetSentenceSeventeen();
	m_KMedkitSentences[20] = GetSentenceEighteen();
	m_KMedkitSentences[21] = GetSentenceNineteen();
	m_KMedkitSentences[22] = GetSentenceTwenty();
	m_KMedkitSentences[23] = GetSentenceThirty();
	m_KMedkitSentences[24] = GetSentenceFourty();
	m_KMedkitSentences[25] = GetSentenceFifty();
	m_KMedkitSentences[26] = GetSentenceSixty();
	m_KMedkitSentences[27] = GetSentenceSeventy();
	m_KMedkitSentences[28] = GetSentenceEighty();
	m_KMedkitSentences[29] = GetSentenceNinety();
	m_KMedkitSentences[30] = GetSentenceOneHundred();
}

float CKMedkitSentence::GetSentenceDuration(char* sentence) 
{
	if (!*sentence || *sentence == '\0')
		return 0;

	if (*sentence == '!')
		sentence++;

	float length = 0;

	if (!std::strcmp(sentence, GetSentenceSayHealth()))
		length = 1.618f;
	else if (!std::strcmp(sentence, GetSentenceVitalSigns()))
		length = 2.274f;
	else if (!std::strcmp(sentence, GetSentencePercent()))
		length = 0.762f;
	else if (!std::strcmp(sentence, GetSentenceOne()))
		length = 0.703f;
	else if (!std::strcmp(sentence, GetSentenceTwo()))
		length = 0.674f;
	else if (!std::strcmp(sentence, GetSentenceThree()))
		length = 0.776f;
	else if (!std::strcmp(sentence, GetSentenceFour()))
		length = 0.776f;
	else if (!std::strcmp(sentence, GetSentenceFive()))
		length = 0.879f;
	else if (!std::strcmp(sentence, GetSentenceSix()))
		length = 0.832f;
	else if (!std::strcmp(sentence, GetSentenceSeven()))
		length = 0.774f;
	else if (!std::strcmp(sentence, GetSentenceEight()))
		length = 0.58f;
	else if (!std::strcmp(sentence, GetSentenceNine()))
		length = 0.832f;
	else if (!std::strcmp(sentence, GetSentenceTen()))
		length = 0.582f;
	else if (!std::strcmp(sentence, GetSentenceEleven()))
		length = 0.744f;
	else if (!std::strcmp(sentence, GetSentenceTwelve()))
		length = 0.8f;
	else if (!std::strcmp(sentence, GetSentenceThirteen()))
		length = 0.996f;
	else if (!std::strcmp(sentence, GetSentenceFourteen()))
		length = 0.940f;
	else if (!std::strcmp(sentence, GetSentenceFifteen()))
		length = 0.982f;
	else if (!std::strcmp(sentence, GetSentenceSixteen()))
		length = 1.038f;
	else if (!std::strcmp(sentence, GetSentenceSeventeen()))
		length = 1.059f;
	else if (!std::strcmp(sentence, GetSentenceEighteen()))
		length = 0.809f;
	else if (!std::strcmp(sentence, GetSentenceNineteen()))
		length = 0.906f;
	else if (!std::strcmp(sentence, GetSentenceTwenty()))
		length = 0.822f;
	else if (!std::strcmp(sentence, GetSentenceThirty()))
		length = 0.867f;
	else if (!std::strcmp(sentence, GetSentenceFourty()))
		length = 0.903f;
	else if (!std::strcmp(sentence, GetSentenceFifty()))
		length = 0.998f;
	else if (!std::strcmp(sentence, GetSentenceSixty()))
		length = 0.991f;
	else if (!std::strcmp(sentence, GetSentenceSeventy()))
		length = 0.956f;
	else if (!std::strcmp(sentence, GetSentenceEighty()))
		length = 0.708f;
	else if (!std::strcmp(sentence, GetSentenceNinety()))
		length = 0.956f;
	else if (!std::strcmp(sentence, GetSentenceOneHundred()))
		length = 0.987f;
	else
		length = 0;

	return length;
}

BOOL CKMedkitSentence::ShouldPercentageSentenceBePlayedInTwoParts(int percent)
{
	if (percent > 19 && (percent % 10) != 0)
		return TRUE;

	return FALSE;
}

void CKMedkitSentence::PlaySentence(CBasePlayer* player, const char* sentence, float* resultSoundDuration)
{
#if !defined ( CLIENT_DLL )
	char szSentence[128] = {};
	std::strcpy(szSentence, "!");
	std::strncat(szSentence, sentence, ARRAYSIZE(szSentence) - std::strlen(szSentence));

	EMIT_SOUND_DYN(player->edict(), CHAN_VOICE, szSentence, 0.8, ATTN_NORM, 0, 100);

	if (resultSoundDuration != NULL)
		*resultSoundDuration = GetSentenceDuration((char*)sentence);

#endif // !defined ( CLIENT_DLL )
}

void CKMedkitSentence::PlaySentenceFromPercentage(
	CBasePlayer* player, 
	int percent, 
	float* resultSoundDuration,
	BOOL playTensOnPercentageWithRemainder)
{
#if !defined ( CLIENT_DLL )
	char szPercent[64] = {};

	if (percent == 100)
	{
		std::strcpy(szPercent, "!");
		std::strncat(szPercent, GetSentenceOneHundred(), ARRAYSIZE(szPercent) - std::strlen(szPercent));
	}
	else
	{
		// Only if number is greater than 19 and has remainder.
		if (ShouldPercentageSentenceBePlayedInTwoParts(percent))
		{
			if (playTensOnPercentageWithRemainder == TRUE)
				percent -= percent % 10; // Get Tens digit + digit 0 in number. .i.e: 35 -> 30, 49 -> 40
			else
				percent %= 10; // Get Ones digit + digit 0 in number.
		}

		std::snprintf(szPercent, ARRAYSIZE(szPercent), "!KA_HEALTH%d", percent);
	}

	EMIT_SOUND_DYN(player->edict(), CHAN_AUTO, szPercent, 0.8, ATTN_NORM, 0, 100);

	if (resultSoundDuration != NULL)
		*resultSoundDuration = GetSentenceDuration(szPercent);

#endif // !defined ( CLIENT_DLL )
}

#define WEAPON_KMEDKIT_VIEWMODEL	"models/v_kmedkit.mdl"
#define WEAPON_KMEDKIT_WORLDMODEL	"models/w_kmedkit.mdl"
#define WEAPON_KMEDKIT_THIRDPERSONMODEL		"models/p_kmedkit.mdl"

#define KATE_CLASSNAME	"monster_kate"
#define MAX_HEAL_RANGE	80
#define CRITICAL_HEALTH_LEVEL	25

#define LONGUSE_SEQUENCE_DURATION	(72.0 / 30.0)
#define SHORTUSE_SEQUENCE_DURATION	(36.0 / 30.0)

#define STATE_HEAL_DELAY				(21.0 / 30.0)
#define STATE_SAYHEALTH_DELAY			1.0f
#define STATE_VITALSIGNS_DELAY			0.5f

enum kmedkit_e {
	KMEDKIT_IDLE1 = 0,
	KMEDKIT_IDLE2,
	KMEDKIT_LONGIDLE,
	KMEDKIT_LONGUSE,
	KMEDKIT_SHORTUSE,
	KMEDKIT_HOLSTER,
	KMEDKIT_DEPLOY,
};

enum KMedkitUseType
{
	KMEDKIT_USE_NONE = 0,
	KMEDKIT_USE_HEAL,
	KMEDKIT_USE_SAYHEALTH,
};

enum KMedkitSound
{
	KMEDKIT_SOUND_NONE = 0,
	KMEDKIT_SOUND_MISS,
};

LINK_ENTITY_TO_CLASS(weapon_kmedkit, CKMedkit);

void CKMedkit::Spawn()
{
	Precache();
	m_iId = WEAPON_KMEDKIT;
	SET_MODEL(ENT(pev), WEAPON_KMEDKIT_WORLDMODEL);

	FallInit();//get ready to fall down.

	m_iDefaultAmmo = KMEDKIT_DEFAULT_GIVE;

	GetSentenceManager()->Init();

	ResetVariables();
}


void CKMedkit::Precache(void)
{
	PRECACHE_MODEL(WEAPON_KMEDKIT_VIEWMODEL);
	PRECACHE_MODEL(WEAPON_KMEDKIT_WORLDMODEL);
	PRECACHE_MODEL(WEAPON_KMEDKIT_THIRDPERSONMODEL);
	PRECACHE_SOUND("weapons/kmedkit_heal.wav");
	PRECACHE_SOUND("weapons/kmedkit_miss.wav");
	PRECACHE_SOUND("weapons/kmedkit_sayhealth.wav");
	PRECACHE_SOUND("fvox/fuzz.wav");
	PRECACHE_SOUND("fvox/health_kate.wav");

	m_usMedkit = PRECACHE_EVENT(1, "events/kmedkit.sc");
}


int CKMedkit::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "Medkit";
	p->iMaxAmmo1 = KMEDKIT_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 0;
	p->iPosition = 2;
	p->iId = m_iId = WEAPON_KMEDKIT;
	p->iWeight = KMEDKIT_WEIGHT;
	p->iFlags = ITEM_FLAG_SELECTONEMPTY;

	return 1;
}

BOOL CKMedkit::Deploy()
{
	return DefaultDeploy(WEAPON_KMEDKIT_VIEWMODEL, WEAPON_KMEDKIT_THIRDPERSONMODEL, KMEDKIT_DEPLOY, "trip");
}

void CKMedkit::PrimaryAttack(void)
{
	if (IsCurrentlyUsingMedkit())
		return;

	if (!HasAmmo())
	{
		PlayMissSound();
		SetNextAttack(0.5f);
		return;
	}

	CBaseEntity* targetToHeal = CheckForEntityInFrontOfPlayer(MAX_HEAL_RANGE);
	if (IsAllowedToHealTarget(targetToHeal))
	{
		//ALERT(at_console, "Begin heal!");
		StartPressButton(targetToHeal, FALSE);
		SetNextAttack(GetHealSequenceDuration());
	}
	else
	{
		PlayMissSound();
		SetNextAttack(0.5f);
	}

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat(m_pPlayer->random_seed, 10, 15);
}

void CKMedkit::SecondaryAttack(void)
{
	if (IsCurrentlyUsingMedkit())
		return;

	CBaseEntity* targetToSayHealth = CheckForEntityInFrontOfPlayer(MAX_HEAL_RANGE);
	if (IsAllowedToSayTargetHealth(targetToSayHealth))
	{
		StartPressButton(targetToSayHealth, TRUE);
		SetNextAttack(GetSayHealthSequenceDuration());
	}
	else
	{
		PlayMissSound();
		SetNextAttack(0.5f);
	}

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat(m_pPlayer->random_seed, 10, 15);
}

void CKMedkit::Holster(int skiplocal /*= 0*/)
{
	ResetVariables();
	DefaultHolster(KMEDKIT_HOLSTER, 16.0f / 30.0f, skiplocal, 0);
}

void CKMedkit::WeaponIdle(void)
{
	ResetEmptySound();

	UpdateState();

	if (m_flTimeWeaponIdle > UTIL_WeaponTimeBase())
		return;

	int iAnim;
	float flRand = UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0, 1);
	if (flRand <= 0.5)
	{
		iAnim = KMEDKIT_IDLE1;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 35.0 / 30.0;
	}
	else if (flRand <= 0.75)
	{
		iAnim = KMEDKIT_IDLE2;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 36.0 / 30.0;
	}
	else
	{
		iAnim = KMEDKIT_LONGIDLE;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 72.0 / 30.0;
	}

	SendWeaponAnim(iAnim, UseDecrement());
}

BOOL CKMedkit::ShouldWeaponIdle(void) 
{ 
	return TRUE; 
}

float CKMedkit::GetHealAmount() const
{
	return gSkillData.kmedkitHeal;
}

float CKMedkit::GetStateHealDelay()
{
	return STATE_HEAL_DELAY;
}

float CKMedkit::GetStateVitalSignsDelay()
{
	return	STATE_VITALSIGNS_DELAY;
}

float CKMedkit::GetHealSequenceDuration() const
{
	return SHORTUSE_SEQUENCE_DURATION;
}

float CKMedkit::GetSayHealthSequenceDuration() const
{
	return LONGUSE_SEQUENCE_DURATION;
}

BOOL CKMedkit::HasAmmo(void)
{
	if (m_pPlayer->ammo_medshots <= 0)
		return FALSE;

	return TRUE;
}

void CKMedkit::UseAmmo(int count)
{
	if (m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] >= count)
		m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] -= count;
	else
		m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] = 0;
}

BOOL CKMedkit::IsCurrentlyUsingMedkit() const
{
#if defined ( CLIENT_DLL )
	return FALSE;
#else
	if (m_useState != KMedkitUseState::STATE_NONE)
		return TRUE;

	return FALSE;
#endif // defined ( CLIENT_DLL )
}

BOOL CKMedkit::IsAllowedToHealTarget(CBaseEntity* target)
{
	if (target == NULL)
		return FALSE;

	if (target->pev->health == target->pev->max_health)
		return FALSE;

	if (target->MyMonsterPointer() == NULL)
		return FALSE;

	if (!FClassnameIs(ENT(target->pev), KATE_CLASSNAME))
		return FALSE;

	return TRUE;
}

BOOL CKMedkit::IsAllowedToSayTargetHealth(CBaseEntity* target)
{
	if (target == NULL)
		return FALSE;

	if (target->MyMonsterPointer() == NULL)
		return FALSE;

	if (!FClassnameIs(ENT(target->pev), KATE_CLASSNAME))
		return FALSE;

	return TRUE;
}

void CKMedkit::StartPressButton(CBaseEntity* target, BOOL isSecondaryAttack)
{
	m_target = target;
	m_targetHealth = target->pev->health;
	m_targetMaxHealth = target->pev->max_health;

	if (!isSecondaryAttack)
	{
		SendMedkitAnim(KMEDKIT_USE_HEAL);

		PlayHealSound();

		m_useState = KMedkitUseState::STATE_HEAL;
		m_delay = gpGlobals->time + GetHealSequenceDuration() + GetStateHealDelay();
	}
	else 
	{
		SendMedkitAnim(KMEDKIT_USE_SAYHEALTH);

		PlayBellSound();

		m_useState = KMedkitUseState::STATE_VITALSIGNS;
		m_delay = gpGlobals->time + GetSayHealthSequenceDuration() + GetStateVitalSignsDelay();
	}
}

void CKMedkit::State_Heal()
{
	if (m_target == NULL)
		return;

	int givenHealthAmount = m_target->TakeHealth(GetHealAmount(), DMG_GENERIC);

	if (givenHealthAmount > 0)
		UseAmmo(1);

	m_targetHealth = m_target->pev->health;
	m_targetMaxHealth = m_target->pev->max_health;

	m_useState = KMedkitUseState::STATE_VITALSIGNS;
	m_delay = gpGlobals->time + GetStateVitalSignsDelay();
}

void CKMedkit::State_VitalSigns()
{
	if (m_target == NULL)
		return;

	float soundDuration;
	PlayVitalSignsSound(&soundDuration);

	// Reduce the delay until ones sound plays to give a
	// blending effect.
	soundDuration = std::max(0.0f, soundDuration - 0.1f);

	m_useState = KMedkitUseState::STATE_HEALTH_PERCENTAGE;
	m_delay = gpGlobals->time + soundDuration;
}

void CKMedkit::State_HealthPercentage()
{
	DisplayTargetHealth();

	int health = m_targetHealth;
	if (health <= 0)
		return;

	int percent = (int)((float)(health * 100.0) * (1.0 / m_targetMaxHealth));

	float soundDuration;

	if (m_healthPercentState == 0)
	{
		// Check if health should be done in two parts.
		if (GetSentenceManager()->ShouldPercentageSentenceBePlayedInTwoParts(percent))
		{
			GetSentenceManager()->PlaySentenceFromPercentage(m_pPlayer, percent, &soundDuration);

			// Reduce the delay until ones sound plays to give a
			// blending effect.
			soundDuration -= GetSentenceManager()->GetTwoPartTensBlendingDelay();

			m_useState = KMedkitUseState::STATE_HEALTH_PERCENTAGE;
			m_delay = gpGlobals->time + soundDuration;

			m_healthPercentState++;
		}
		else // Skip directly to "percent" sound.
		{
			// Play percent tens sound.
			GetSentenceManager()->PlaySentenceFromPercentage(m_pPlayer, percent, &soundDuration);

			m_useState = KMedkitUseState::STATE_SOUND_PERCENT;
			m_delay = gpGlobals->time + soundDuration;
		}
	}
	else if (m_healthPercentState == 1)
	{
		GetSentenceManager()->PlaySentenceFromPercentage(m_pPlayer, percent, &soundDuration, FALSE);

		m_healthPercentState = 0;

		m_useState = KMedkitUseState::STATE_SOUND_PERCENT;
		m_delay = gpGlobals->time + soundDuration;
	}
}

void CKMedkit::State_SoundPercent()
{
	PlaySoundPercent(NULL);
}

void CKMedkit::DisplayTargetHealth()
{
#if !defined ( CLIENT_DLL )
	static char formattedTargetHealth[64];
	static hudtextparms_t params;

	std::memset(formattedTargetHealth, 0, ARRAYSIZE(formattedTargetHealth));
	std::snprintf(formattedTargetHealth, ARRAYSIZE(formattedTargetHealth), "Kate's Health: %d", m_targetHealth);

	GetTextColorForHealth(m_targetHealth, params.r1, params.g1, params.b1);
	params.a1 = 255;
	params.r2 = 255;
	params.g2 = 255;
	params.b2 = 255;
	params.a2 = 0;
	params.channel = 1;
	params.effect = 0;
	params.fadeinTime = 1;
	params.fadeoutTime = 1;
	params.holdTime = 5;
	params.fxTime = 0;
	params.x = 0.03f;
	params.y = 0.9f;

	UTIL_HudMessageAll(params, formattedTargetHealth);
#endif // !defined ( CLIENT_DLL )
}

void CKMedkit::GetTextColorForHealth(const int health, byte& r, byte& g, byte& b)
{
	if (health > CRITICAL_HEALTH_LEVEL)
	{
		// Normal HUD color.
		r = 94;
		g = 69;
		b = 228;
	}
	else 
	{
		// Redish HUD color.
		r = 250;
		g = 0;
		b = 0;
	}
}

void CKMedkit::ResetVariables()
{
	m_target = NULL;
	m_targetHealth = 0;
	m_targetMaxHealth = 0;
	m_delay = 0;
	m_useState = KMedkitUseState::STATE_NONE;
	m_healthPercentState = 0;
}

void CKMedkit::UpdateState()
{
	if (m_useState == KMedkitUseState::STATE_NONE)
		return;

	if (m_delay > gpGlobals->time)
		return;

	int currentUseState = m_useState;
	m_useState = KMedkitUseState::STATE_RESETVARIABLES;

	ChooseAndExecuteUseState(currentUseState);

	if (m_useState == KMedkitUseState::STATE_RESETVARIABLES)
		ResetVariables();
}

void CKMedkit::ChooseAndExecuteUseState(int useState)
{
	switch (useState)
	{
	case KMedkitUseState::STATE_HEAL:
		State_Heal();
		break;
	case KMedkitUseState::STATE_VITALSIGNS:
		State_VitalSigns();
		break;
	case KMedkitUseState::STATE_HEALTH_PERCENTAGE:
		State_HealthPercentage();
		break;
	case KMedkitUseState::STATE_SOUND_PERCENT:
		State_SoundPercent();
		break;
	default:
		break;
	}
}

#if !defined ( CLIENT_DLL )
void FindHullIntersection(const Vector &vecSrc, TraceResult &tr, float *mins, float *maxs, edict_t *pEntity);
#endif

CBaseEntity* CKMedkit::CheckForEntityInFrontOfPlayer(float maxDistance)
{
#if defined ( CLIENT_DLL )
	return NULL;
#else
	CBaseEntity* resultEntity = NULL;

	Vector source, destination;

	UTIL_MakeVectors(m_pPlayer->pev->v_angle);

	source = m_pPlayer->GetGunPosition();
	destination = source + gpGlobals->v_forward * maxDistance;

	TraceResult tr;
	UTIL_TraceLine(source, destination, dont_ignore_monsters, m_pPlayer->edict(), &tr);

	// Copy & pasted from crowbar.cpp
	if (tr.flFraction >= 1.0)
	{
		UTIL_TraceHull(source, destination, dont_ignore_monsters, human_hull, m_pPlayer->edict(), &tr);
		if (tr.flFraction < 1.0)
		{
			CBaseEntity *pHit = CBaseEntity::Instance(tr.pHit);
			if (!pHit || pHit->IsBSPModel())
				FindHullIntersection(source, tr, VEC_DUCK_HULL_MIN, VEC_DUCK_HULL_MAX, m_pPlayer->edict());
		}
	}

	if (tr.pHit != NULL)
	{
		resultEntity = CBaseEntity::Instance(tr.pHit);
	}

	return resultEntity;

#endif // defined ( CLIENT_DLL )
}

void CKMedkit::SendMedkitAnim(int useType)
{
	int flags;
#if defined( CLIENT_WEAPONS )
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	PLAYBACK_EVENT_FULL(0, m_pPlayer->edict(), m_usMedkit, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, 0, 0, useType, 0, 0, 0);
}

void CKMedkit::PlayHealSound()
{
#if !defined ( CLIENT_DLL )
	EMIT_SOUND_DYN(m_pPlayer->edict(), CHAN_AUTO, "weapons/kmedkit_heal.wav", 0.8, ATTN_NORM, 0, 100);
#endif // !defined ( CLIENT_DLL )
}

void CKMedkit::PlayMissSound()
{
	PLAYBACK_EVENT_FULL(0, m_pPlayer->edict(), m_usMedkit, 0.0, (float *)&m_pPlayer->pev->origin, (float *)&g_vecZero, 0, 0, 0, KMEDKIT_SOUND_MISS, 0, 0);
}

void CKMedkit::PlayBellSound()
{
#if !defined ( CLIENT_DLL )
	GetSentenceManager()->PlaySentence(m_pPlayer, GetSentenceManager()->GetSentenceSayHealth(), NULL);
#endif // !defined ( CLIENT_DLL )
}

void CKMedkit::PlayVitalSignsSound(float* resultingSoundDuration)
{
#if !defined ( CLIENT_DLL )
	GetSentenceManager()->PlaySentence(m_pPlayer, GetSentenceManager()->GetSentenceVitalSigns(), resultingSoundDuration);
#endif // !defined ( CLIENT_DLL )
}

void CKMedkit::PlaySoundPercent(float* resultingSoundDuration)
{
#if !defined ( CLIENT_DLL )
	GetSentenceManager()->PlaySentence(m_pPlayer, GetSentenceManager()->GetSentencePercent(), resultingSoundDuration);
#endif // !defined ( CLIENT_DLL )
}

void CKMedkit::SetNextAttack(float nextPrimaryAttackTime)
{
	m_flNextPrimaryAttack = GetNextAttackDelay(nextPrimaryAttackTime);
	m_flNextSecondaryAttack = GetNextAttackDelay(nextPrimaryAttackTime);
}

CKMedkitSentence* CKMedkit::GetSentenceManager() const
{
	static CKMedkitSentence sentenceSingleton;
	return &sentenceSingleton;
}
