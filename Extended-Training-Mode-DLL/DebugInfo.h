#pragma once
#include <cstddef>
//#include <windows.h>
#include "..\Common\Common.h"

#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define LINE_NAME CONCATENATE(LINE, __LINE__)

#define UNUSED(length) BYTE LINE_NAME[length]
#define UNUSEDBITS(length) DWORD LINE_NAME : length

#pragma pack(push,1)
typedef struct AttackData {
	union {
		struct {
			DWORD standBlockable : 1;
			DWORD airBlockable : 1;
			DWORD crouchBlockable : 1;
			UNUSEDBITS(5);
			DWORD missStanding : 1;
			DWORD missAirborne: 1;
			DWORD missCrouching : 1;
			DWORD missHitstun : 1; // includes blockstun
			DWORD missBlockstun : 1;
			DWORD missOTG : 1;
			DWORD hitOnlyHitstun : 1; // includes blockstun
			DWORD missPlayableCharacter : 1;
			UNUSEDBITS(16);
		};
		DWORD guardFlags;
	};
	short standingHitVector;
	short airborneHitVector;
	short crouchingHitVector;
	UNUSED(0xA);
	short standingGuardVector;
	short airGuardVector;
	short crouchingGuardVector;
	UNUSED(0xA);
	WORD hitEffectID;
	WORD soundEffectID;
	UNUSED(4);
	WORD custonHitstop;
	WORD untechTime;
	BYTE effect; // 3 is electric right?
	BYTE hitgrab;
	BYTE hitstop;
	BYTE proration;
	BYTE prorationType;
	UNUSED(1);
	BYTE blockstop;
	UNUSED(1);
	float extraGravity;
	union {
		struct {
			DWORD chipDamage : 1;
			DWORD noKO : 1;
			DWORD makeEnemyUnhittable : 1;
			DWORD notClashable : 1;
			DWORD autoSuperjumpCancel : 1;
			DWORD dontAddToComboCount : 1;
			DWORD screenShake : 1;
			DWORD notAirTechable : 1;

			DWORD notGroundTechable : 1;
			DWORD canHitAllies : 1;
			DWORD noSelfHitstop : 1;
			UNUSEDBITS(1);
			DWORD cantBeBursted : 1;
			DWORD cantBeShielded : 1;
			DWORD cantCrit : 1;
			UNUSEDBITS(1);

			DWORD useCustomBlockstop : 1;
			DWORD canOTGRelaunch : 1;
			DWORD cantCounterhit : 1;
			UNUSEDBITS(3);
			DWORD extraInternalHitscaling : 1;
			UNUSEDBITS(1);

		};
		DWORD hitFlags;
	};
	UNUSED(4);
	WORD damage;
	WORD meterGain;
	WORD vsDamage;
	WORD guardBreak;
	WORD circuitBreakTime;
	UNUSED(2);
};
#pragma pack(pop)

#define CHECKOFFSET(v, n) static_assert(offsetof(AttackData, v) == n, "AttackData offset incorrect for " #v);

CHECKOFFSET(standingHitVector, 0x4);
CHECKOFFSET(crouchingHitVector, 0x8);
CHECKOFFSET(standingGuardVector, 0x14);
CHECKOFFSET(hitEffectID, 0x24);
CHECKOFFSET(custonHitstop, 0x2C);
CHECKOFFSET(extraGravity, 0x38);
CHECKOFFSET(damage, 0x44);

static_assert(sizeof(AttackData) == 0x50, "AttackData size must be 0x50");
#undef CHECKOFFSET

#pragma pack(push,1)
typedef struct PatternData {

};
#pragma pack(pop)

#pragma pack(push,1)
typedef struct AnimationData {

};
#pragma pack(pop)

#pragma pack(push,1)
typedef struct FrameData {

};
#pragma pack(pop)

// im going off of base 00555130, sorta wish we could switch to 00555134 tho
#pragma pack(push,1)
typedef struct PlayerData {
	
	void describe(char* buffer, int bufLen);

	// -----

	DWORD exists;
	UNUSED(1);
	BYTE charID;
	BYTE charIDCopy;
	BYTE recording;
	UNUSED(2);
	BYTE palette;
	UNUSED(1);
	BYTE moon;
	UNUSED(3);
	DWORD pattern;
	DWORD state;
	DWORD nextSprite;
	DWORD framesInCurrentState;
	BYTE isFirstFramePlusOne;
	UNUSED(4);
	BYTE remainingLoops;
	UNUSED(0x3E);
	DWORD notInCombo;
	DWORD numFrameAndPatternTransitions;
	UNUSED(0x44);
	DWORD exGuard;
	UNUSED(0x8);
	DWORD health;
	DWORD redHealth;
	float guardGauge;
	float guardGaugeHeal;
	DWORD guardGaugeState;
	DWORD guardGaugeStop;
	UNUSED(2);
	BYTE guardQualityStop;
	UNUSED(1);
	float quardQuality;
	BYTE exMoveMeterPenaltyTiimer;
	UNUSED(3);
	DWORD magicCircuit;
	DWORD heatTimeLeft;
	WORD magicCircuitState;
	UNUSED(4);
	BYTE magicCircuitPause;
	UNUSED(1);
	DWORD heatTimeThisHeat;
	UNUSED(4);
	DWORD storedMeterUsage;
	DWORD storedMeterCircuitState;
	WORD circuitBreakExPenaltyTimer;
	WORD circuitBreakExPenaltyTimerLength;
	WORD ifExPenalty;
	UNUSED(2);
	int xPos;
	int yPos;
	UNUSED(4);
	int prevXPos;
	int prevYPos;
	int xVel;
	int yVel;
	short xAccel;
	short yAccel;
	short maxXSpeed;
	UNUSED(2);
	int xVelChange;
	int yVelChange;
	short xAccelChange;
	short yAccelChange;
	short momentum;
	UNUSED(0x32);
	WORD shieldHeldTime;
	UNUSED(2);
	WORD shieldType;
	BYTE selfHitstop;
	UNUSED(3);
	BYTE throwFlag;
	BYTE deathFlag;
	BYTE tagFlag;
	UNUSED(1);
	BYTE remainingHits;
	BYTE inBlockstun;
	BYTE willBlock;
	BYTE proxyGuardTime;
	BYTE bounceCount;
	UNUSED(6);
	BYTE strikeInvuln;
	BYTE throwInvuln;
	UNUSED(1);
	BYTE preJumpTimeRemaining;
	WORD airTime;
	WORD timeThrown;
	WORD totalUntechTime;
	WORD untechTimeElapsed;
	UNUSED(7);
	WORD armorTimer;
	WORD reversedControlsTimer;
	UNUSED(8);
	BYTE recievedHitstop;
	UNUSED(3);
	DWORD hitstunBlockstunTimeElapsed;
	int hitstunTimeRemaining; // -2 if airborn
	BYTE isKnockedDown;
	UNUSED(0x11);
	WORD airdashVariable;
	UNUSED(0x26);
	WORD reversePenalty;
	WORD reversePenaltyDecayTimer;
	UNUSED(0xC);
	BYTE counterhitState;
	UNUSED(1);
	AttackData* recievingAttackPtr;
	UNUSED(0xE4);
	float gravity;
	WORD extraUntechPenalty;
	BYTE correctedInput;
	BYTE rawInput;
	BYTE buttonJustPressed;
	BYTE buttonHeld;
	WORD macroPressed;
	WORD buttonReleased;
	UNUSED(0x1A);
	WORD inputEvent;
	WORD fiveThousandMinusInputPrio;
	UNUSED(0x4);
	BYTE facingLeft;
	BYTE isOpponentToLeft;
	UNUSED(5);
	BYTE delayedStandAirbornCrouchState;
	PatternData* patternDataPtr;
	AnimationData* animationDataPtr;
	AttackData* attackDataPtr;
	PlayerData* selfPtr;
	UNUSED(4); // i have never seen a more suspicious 4 bytes in my life
	FrameData* frameDataPtr;
	DWORD framesIntoCurrentPattern;

	UNUSED(0x7C4);
} PlayerData;
#pragma pack(pop)


#define CHECKOFFSET(v, n) static_assert(offsetof(PlayerData, v) == n, "PlayerData offset incorrect for " #v);

CHECKOFFSET(exists, 0)
CHECKOFFSET(charID, 5)
CHECKOFFSET(pattern, 0x10)
CHECKOFFSET(isFirstFramePlusOne, 0x20)
CHECKOFFSET(numFrameAndPatternTransitions, 0x68)
CHECKOFFSET(exGuard, 0xB0)
CHECKOFFSET(health, 0xBC)
CHECKOFFSET(guardGaugeStop, 0xD0)
CHECKOFFSET(guardQualityStop, 0xD6)
CHECKOFFSET(quardQuality, 0xD8)
CHECKOFFSET(exMoveMeterPenaltyTiimer, 0xDC)
CHECKOFFSET(magicCircuit, 0xE0)
CHECKOFFSET(magicCircuitState, 0xE8)
CHECKOFFSET(magicCircuitPause, 0xEE)
CHECKOFFSET(heatTimeThisHeat, 0xF0)
CHECKOFFSET(storedMeterUsage, 0xF8)
CHECKOFFSET(circuitBreakExPenaltyTimer, 0x100)
CHECKOFFSET(xPos, 0x108)
CHECKOFFSET(maxXSpeed, 0x128);
CHECKOFFSET(yVelChange, 0x130);
CHECKOFFSET(momentum, 0x138);
CHECKOFFSET(shieldHeldTime, 0x16C)
CHECKOFFSET(tagFlag, 0x178);
CHECKOFFSET(bounceCount, 0x17E);
CHECKOFFSET(strikeInvuln, 0x185);
CHECKOFFSET(armorTimer, 0x198);
CHECKOFFSET(recievedHitstop, 0x1A4);
CHECKOFFSET(isKnockedDown, 0x1B0);
CHECKOFFSET(airdashVariable, 0x1C2);
CHECKOFFSET(reversePenalty, 0x1EA);
CHECKOFFSET(counterhitState, 0x1FA);
CHECKOFFSET(recievingAttackPtr, 0x1FC);
CHECKOFFSET(gravity, 0x2E4);
CHECKOFFSET(buttonHeld, 0x2ED);
CHECKOFFSET(fiveThousandMinusInputPrio, 0x30E);
CHECKOFFSET(facingLeft, 0x314);
CHECKOFFSET(delayedStandAirbornCrouchState, 0x31B);
CHECKOFFSET(patternDataPtr, 0x31C);
CHECKOFFSET(selfPtr, 0x328);
CHECKOFFSET(frameDataPtr, 0x330);

static_assert(sizeof(PlayerData) == 0xAFC, "PlayerData MUST be 0xAFC large!");

extern PlayerData* playerDataArr[4];

#undef CHECKOFFSET

#pragma pack(push,1)
typedef struct EffectData {

} EffectData;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct HitEffectData {

} HitEffectData; // remember when i reversed this whole thing and then proceded to accidentally wipe all my work?
#pragma pack(pop)

extern bool shouldDisplayDebugInfo;
void displayDebugInfo();
