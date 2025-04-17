#pragma once
#include <cstddef>
//#include <windows.h>
#include "..\Common\Common.h"

#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define LINE_NAME CONCATENATE(LINE, __LINE__)

#define UNUSED(length) BYTE LINE_NAME[length]
#define UNUSEDBITS(length) BYTE LINE_NAME : length

#pragma pack(push,1)
typedef struct AttackData {
	union {
		DWORD guardFlags;
		struct {
			BYTE standBlockable : 1;
			BYTE airBlockable : 1;
			BYTE crouchBlockable : 1;
			UNUSEDBITS(5);
			BYTE missStanding : 1;
			BYTE missAirborne: 1;
			BYTE missCrouching : 1;
			BYTE missHitstun : 1; // includes blockstun
			BYTE missBlockstun : 1;
			BYTE missOTG : 1;
			BYTE hitOnlyHitstun : 1; // includes blockstun
			BYTE missPlayableCharacter : 1;
			//UNUSEDBITS(16);
			UNUSED(2);
		};
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
		DWORD hitFlags;
		struct {
			BYTE chipDamage : 1;
			BYTE noKO : 1;
			BYTE makeEnemyUnhittable : 1;
			BYTE notClashable : 1;
			BYTE autoSuperjumpCancel : 1;
			BYTE dontAddToComboCount : 1;
			BYTE screenShake : 1;
			BYTE notAirTechable : 1;

			BYTE notGroundTechable : 1;
			BYTE canHitAllies : 1;
			BYTE noSelfHitstop : 1;
			UNUSEDBITS(1);
			BYTE cantBeBursted : 1;
			BYTE cantBeShielded : 1;
			BYTE cantCrit : 1;
			UNUSEDBITS(1);

			BYTE useCustomBlockstop : 1;
			BYTE canOTGRelaunch : 1;
			BYTE cantCounterhit : 1;
			UNUSEDBITS(3);
			BYTE extraInternalHitscaling : 1;
			UNUSEDBITS(1);

		};
	};
	UNUSED(4);
	WORD damage;
	WORD meterGain;
	WORD vsDamage;
	WORD guardBreak;
	WORD circuitBreakTime;
	UNUSED(2);
} AttackData;
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
typedef struct MovementData {
	union {
		BYTE movementFlags;
		struct {
			BYTE setY : 1;
			BYTE addY : 1;
			UNUSEDBITS(2);
			BYTE setX : 1;
			BYTE addX : 1;
			UNUSEDBITS(2);
		};
	};
	UNUSED(1);
	short xVel;
	short yVel;
	short xAccel;
	short yAccel;
} MovementData;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct StateData { // i am sure of nothing in this struct.
	MovementData* movementData;
	UNUSED(8);
	BYTE stance;
	BYTE invincibility;
	BYTE cancelNormal;
	BYTE cancelSpecial;
	BYTE hitCount;
	BYTE canMove;
	BYTE counterHit;
	UNUSED(1);
	union {
		DWORD flagset1;
		struct {
			BYTE giveMomentum : 1;
			BYTE clearMomentum : 1;
			BYTE dontTransitionToWalking : 1;
			UNUSEDBITS(1);
			BYTE canGroundTech : 1;
			UNUSEDBITS(3);
			BYTE blockHighsAndMids : 1;
			BYTE blockAirBlockable : 1;
			UNUSEDBITS(6);
			UNUSEDBITS(8);
			UNUSEDBITS(8);
		};
	};
	union {
		DWORD flagset2;
		struct {
			BYTE canAlwaysExCancel : 1;
			BYTE canOnlyJumpCancel : 1;
			UNUSEDBITS(6);
			UNUSEDBITS(8);
			UNUSEDBITS(8);
			UNUSEDBITS(7);
			BYTE cantBlock : 1;
		};
	};
} StateData;
#pragma pack(pop)

#define CHECKOFFSET(v, n) static_assert(offsetof(StateData, v) == n, "StateData offset incorrect for " #v);

CHECKOFFSET(stance, 0xC);
CHECKOFFSET(counterHit, 0x12);
CHECKOFFSET(flagset2, 0x18);

static_assert(sizeof(StateData) == 0x1C, "StateData MUST be 0x1C large!");

#undef CHECKOFFSET

#pragma pack(push,1)
typedef struct IF { // Condition
	int IFTP;
	int IFPR1;
	int IFPR2;
	int IFPR3;
	int IFPR4;
	int IFPR5;
	int IFPR6;
	int IFPR7;
	int IFPR8;
	int IFPR9;
} IF;
#pragma pack(pop)

#define CHECKOFFSET(v, n) static_assert(offsetof(IF, v) == n, "IF offset incorrect for " #v);

CHECKOFFSET(IFPR1, 0x4);

static_assert(sizeof(IF) == 0x28, "IF MUST be 0x28 large!");

#undef CHECKOFFSET

#pragma pack(push,1)
typedef struct IFData { // List of Conditions
	IF* IFs[4];
} IFData;
#pragma pack(pop)

#define CHECKOFFSET(v, n) static_assert(offsetof(IFData, v) == n, "IFData offset incorrect for " #v);

CHECKOFFSET(IFs, 0x0);

static_assert(sizeof(IFData) == 0x10, "IFData MUST be 0x10 large!");

#undef CHECKOFFSET

#pragma pack(push,1)
typedef struct RawBoxData {
	short x1;
	short y1;
	short x2;
	short y2;
} RawBoxData;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct NonHitboxData { // this needs verification on if it actually works
	union {
		RawBoxData* boxes[25];
		struct {
			RawBoxData* collisionBox;
			RawBoxData* hurtboxes[8];
			RawBoxData* shieldBoxes[2];
			RawBoxData* clashBox;
			RawBoxData* reflectBox;
			RawBoxData* specialBoxes[12];
		};
	};
} NonHitboxData;
#pragma pack(pop)

static_assert(sizeof(NonHitboxData) == 0x64, "NonHitboxData must be size 0x64");

#pragma pack(push,1)
typedef struct HitboxData {
	RawBoxData* boxes[8]; // this needs verification on if it actually works
} HitboxData;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct AnimationData {
	WORD usePat;
	WORD currentSprite;
	WORD xOffset;
	UNUSED(2);
	WORD yOffset;
	UNUSED(2);
	BYTE stateDuration;
	UNUSED(1);
	BYTE animationType;
	union {
		BYTE animationFlags;	
		struct {
			BYTE landToPattern : 1;
			BYTE checkLoopCounter : 1;
			BYTE gotoRelativeOffset: 1;
			BYTE relativeEndOfLoop : 1;
			UNUSEDBITS(4);
		};
	};
	WORD gotoAnim; // i cant use goto as a var name, not sure what this is going to tho! ask gonp!
	WORD landingFrame;
	WORD loopNTimes;
	WORD endOfLoop;
	BYTE r;
	BYTE g;
	BYTE b;
	BYTE blendMode;
	BYTE a;
	BYTE interp;
	UNUSED(2);
	float xRot;
	float yRot;
	float zRot;
	BYTE rotKeepsEFScale;
	UNUSED(1);
	WORD zPrio;
	float xScale;
	float yScale;
	StateData* stateData;
	UNUSED(4);
	BYTE highestIFIndex;
	BYTE highestEFIndex;
	BYTE highestNonHitboxIndex;
	BYTE highestHitboxIndex;
	IFData* IFDataPtr;
	DWORD EFDataPtr;
	NonHitboxData* nonHitboxData;
	HitboxData* hitboxData;
} AnimationData;
#pragma pack(pop)

#define CHECKOFFSET(v, n) static_assert(offsetof(AnimationData, v) == n, "AnimationData offset incorrect for " #v);

CHECKOFFSET(r, 0x18);
CHECKOFFSET(xScale, 0x30);
CHECKOFFSET(stateData, 0x38);
CHECKOFFSET(highestNonHitboxIndex, 0x42);
CHECKOFFSET(nonHitboxData, 0x4C);

static_assert(sizeof(AnimationData) == 0x54, "AnimationDataMUST be 0x50 large!");
#undef CHECKOFFSET

typedef struct AnimationDataArr {
	UNUSED(4);
	AnimationData* animationDataArr;
} AnimationDataArr;

#pragma pack(push,1)
typedef struct PatternData {
	// this is the pattern name. i dont know what encoding its in though. size is 0x24/36 bytes. also, 36 is a weird number of chars for a string. 32 would make more sense
	//wchar_t patternName[18];
	// the char encoding seems to be Shift JIS 
	char patternName[32];
	UNUSED(4);
	DWORD PSTS;
	DWORD level;
	DWORD PFLG;
	UNUSED(4);
	AnimationDataArr* ptrToAnimationDataArr;
	UNUSED(0x20);
} PatternData;
#pragma pack(pop)

#define CHECKOFFSET(v, n) static_assert(offsetof(PatternData, v) == n, "PatternData offset incorrect for " #v);

CHECKOFFSET(PSTS, 0x24);
CHECKOFFSET(PFLG, 0x2C);
CHECKOFFSET(ptrToAnimationDataArr, 0x34);

static_assert(sizeof(PatternData) == 0x58, "PatternData must have size 0x58."); // i am pretty confident on this number, but it could be off.
#undef CHECKOFFSET

#pragma pack(push,1)
typedef struct Command {
	int commandFilePriority;
	char input[20];
	int pattern;
	int specialFlag;
	int meterSpend;
	uint8_t vars[4]; //assist, special, dash, unused
	uint8_t flagsets[4]; //flagset 1, flagset 2, unused, unused
} Command;
#pragma pack(pop)

#define CHECKOFFSET(v, n) static_assert(offsetof(Command, v) == n, "Command offset incorrect for " #v);

CHECKOFFSET(flagsets, 0x28);

static_assert(sizeof(Command) == 0x2C, "Command must have size 0x2C.");
#undef CHECKOFFSET

#pragma pack(push,1)
typedef struct CommandPtrArray {
	Command* commands[1000];
} CommandPtrArray;
#pragma pack(pop)

static_assert(sizeof(CommandPtrArray) == 4000, "CommandPtrArray must have size 4000.");

#pragma pack(push,1)
typedef struct CommandFileData {
	UNUSED(0x4);
	CommandPtrArray* cmdPtrArray;
	UNUSED(0x4);
	int maxFilePriority;
	int maxFilePriorityCopy;
} CommandFileData;
#pragma pack(pop)

#define CHECKOFFSET(v, n) static_assert(offsetof(CommandFileData, v) == n, "CommandFileData offset incorrect for " #v);

CHECKOFFSET(maxFilePriorityCopy, 0x10);

static_assert(sizeof(CommandFileData) == 0x14, "CommandFileData must have size 0x14.");
#undef CHECKOFFSET

#pragma pack(push,1)
typedef struct CommandData {
	CommandFileData* cmdFileDataPtr;
	UNUSED(0x14);
	float guts[4];
	UNUSED(0x2A);
	uint16_t groundThrowPat;
	UNUSED(0x4);
	uint16_t airThrowPat;
} CommandData;
#pragma pack(pop)

#define CHECKOFFSET(v, n) static_assert(offsetof(CommandData, v) == n, "CommandData offset incorrect for " #v);

CHECKOFFSET(airThrowPat, 0x58);

static_assert(sizeof(CommandData) == 0x5A, "CommandData must have size 0x5A.");
#undef CHECKOFFSET

#pragma pack(push,1)
typedef struct SubHA6Data2 {
	DWORD isPointerList;
	PatternData* (*ptrToPatternDataArr)[1000];
	DWORD elementSize;
	DWORD count;
	DWORD capacity;
} SubHA6Data2;
#pragma pack(pop)

static_assert(sizeof(SubHA6Data2) == 0x14, "SubHA6Data2 must have a size of 0x14");

#pragma pack(push,1)
typedef struct SubHA6Data1 {
	DWORD isStaticSize;
	SubHA6Data2* subData2;
} SubHA6Data1;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct HA6Data { 
	// out of all the structs, i understand this one the least.
	// the important thing, is that it eventually leads to 0x31C patternDataPtr, and so in most cases, you can and should probs just use that
	// this however, can be used to see all paterns in a char. seems very nice to have
	SubHA6Data1* subData1;
} HA6Data;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct EffectData {
	
	void describe(char* buffer, int bufLen);
	inline PatternData* getPatternDataPtr(int p);
	inline AnimationData* getAnimationDataPtr(int p, int s);
	// -----

	DWORD exists;
	BYTE index;
	BYTE charID;
	BYTE charIDCopy;
	BYTE doTrainingAction;
	UNUSED(1);
	BYTE someFlag;
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
	WORD exMoveMeterPenaltyTiimer;
	UNUSED(2);
	DWORD magicCircuit;
	DWORD heatTimeLeft;
	BYTE magicCircuitState;
	UNUSED(3);
	WORD maxHeatTime;
	WORD magicCircuitPause;
	DWORD heatTimeCounter;
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
	BYTE hitstop;
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
	UNUSED(1);
	WORD airTime;
	WORD timeThrown;
	WORD totalUntechTime;
	WORD untechTimeElapsed;
	UNUSED(6);
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
	UNUSED(0x2);
	BYTE ownerIndex;
	UNUSED(0x17);
	WORD inputEvent;
	WORD inputPriority;
	UNUSED(0x4);
	BYTE facingLeft;
	BYTE isOpponentToLeft;
	UNUSED(5);
	BYTE delayedStandAirbornCrouchState;
	PatternData* patternDataPtr;
	AnimationData* animationDataPtr;
	AttackData* attackDataPtr;
	EffectData* selfPtr;
	UNUSED(4); // i have never seen a more suspicious 4 bytes in my life
	HA6Data* ha6DataPtr;
	DWORD framesIntoCurrentPattern;
	UNUSED(4);
} EffectData;
#pragma pack(pop)

#define CHECKOFFSET(v, n) static_assert(offsetof(EffectData, v) == n, "EffectData offset incorrect for " #v);

CHECKOFFSET(ha6DataPtr, 0x330);

#undef CHECKOFFSET

static_assert(sizeof(EffectData) == 0x33C, "EffectData MUST be 0x33C large!");

// im going off of base 00555130, sorta wish we could switch to 00555134 tho
#pragma pack(push,1)
typedef struct PlayerData : public EffectData {
	CommandData* cmdDataPtr;
	UNUSED(0x7BC);
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
CHECKOFFSET(heatTimeCounter, 0xF0)
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
CHECKOFFSET(timeThrown, 0x18C);
CHECKOFFSET(totalUntechTime, 0x18E);
CHECKOFFSET(armorTimer, 0x198);
CHECKOFFSET(recievedHitstop, 0x1A4);
CHECKOFFSET(isKnockedDown, 0x1B0);
CHECKOFFSET(airdashVariable, 0x1C2);
CHECKOFFSET(reversePenalty, 0x1EA);
CHECKOFFSET(counterhitState, 0x1FA);
CHECKOFFSET(recievingAttackPtr, 0x1FC);
CHECKOFFSET(gravity, 0x2E4);
CHECKOFFSET(buttonHeld, 0x2ED);
CHECKOFFSET(inputPriority, 0x30E);
CHECKOFFSET(facingLeft, 0x314);
CHECKOFFSET(delayedStandAirbornCrouchState, 0x31B);
CHECKOFFSET(patternDataPtr, 0x31C);
CHECKOFFSET(selfPtr, 0x328);
CHECKOFFSET(ha6DataPtr, 0x330);

static_assert(sizeof(PlayerData) == 0xAFC, "PlayerData MUST be 0xAFC large!");

extern PlayerData* playerDataArr;
extern EffectData* effectDataArr;

#undef CHECKOFFSET

#pragma pack(push,1)
typedef struct PlayerAuxData {
	int activeCharacter;
	UNUSED(0x1C);
	int proration;
	UNUSED(0x20);
	int comboCount;
	int comboCount2;
	UNUSED(0x58);
	int calc_comboCount;
	UNUSED(0x8);
	int calc_proration;
	UNUSED(0x154);
	int inactionableFrames;
} PlayerAuxData;
#pragma pack(pop)

#define CHECKOFFSET(v, n) static_assert(offsetof(PlayerAuxData, v) == n, "PlayerAuxData offset incorrect for " #v);

CHECKOFFSET(calc_comboCount, 0xA4);
CHECKOFFSET(inactionableFrames, 0x208);

static_assert(sizeof(PlayerAuxData) == 0x20C, "PlayerAuxData must have size 0x5A.");
#undef CHECKOFFSET

#pragma pack(push,1)
typedef struct HitEffectData {

} HitEffectData; // remember when i reversed this whole thing and then proceded to accidentally wipe all my work?
#pragma pack(pop)

extern bool shouldDisplayDebugInfo;
void displayDebugInfo();
void debugLinkedList();
bool isValidTexture(DWORD addr);
struct IDirect3DBaseTexture9;
extern bool shouldDisplayLinkedListInfo;
void saveTexture(IDirect3DBaseTexture9* pTex, int i=-1);
extern bool shouldDebugImportantDraw;
void debugImportantDraw();
int getComboCount();
extern bool useWind;
extern int xWindVel;
extern int changeWindDir;
void setWind();