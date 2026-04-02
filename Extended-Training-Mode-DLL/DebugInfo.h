#pragma once
#include <cstddef>
//#include <windows.h>
#include "..\Common\Common.h"
#include <d3d9.h>

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
	WORD customHitstop;
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
	WORD guardDamage;
	WORD circuitBreakTime;
	UNUSED(2);
} AttackData;
#pragma pack(pop)

#define CHECKOFFSET(v, n) static_assert(offsetof(AttackData, v) == n, "AttackData offset incorrect for " #v);

CHECKOFFSET(standingHitVector, 0x4);
CHECKOFFSET(crouchingHitVector, 0x8);
CHECKOFFSET(standingGuardVector, 0x14);
CHECKOFFSET(hitEffectID, 0x24);
CHECKOFFSET(customHitstop, 0x2C);
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
typedef struct SineData {
	union {
		BYTE sineFlags;
		struct {
			BYTE useY : 4;
			BYTE useX : 4;
		};
	};
	UNUSED(1);
	short xDistance;
	short yDistance;
	short xFrequency;
	short yFrequency;
} SineData;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct StateData { // i am sure of nothing in this struct.
	MovementData* movementData;
	short maxXSpeed;
	UNUSED(2);
	SineData* sineData;
	BYTE stance; // could be used for tas, for either tks or landing to do a move as soon as able
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
			BYTE standGuardpoint : 1;
			BYTE airGuardpoint : 1;
			BYTE crouchGuardpoint : 1;
			UNUSEDBITS(5);
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
typedef struct EF { // Effect
	int EFTP;
	int EFNO;
	int EFPR[12];
} EF;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct IF { // Condition
	int IFTP;
	int IFPR[9];
} IF;
#pragma pack(pop)


#pragma pack(push,1)
typedef struct CameraBoxData {
	int x1;
	int y1;
	int x2;
	int y2;
} CameraBoxData;
#pragma pack(pop)

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
	short goToData; // i cant use goto as a var name, im pretty confident this is signed, look at warc arc drive (pattern 224 state 21)
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
	AttackData* attackData;
	BYTE highestIFIndex;
	BYTE highestEFIndex;
	BYTE highestNonHitboxIndex;
	BYTE highestHitboxIndex;
	IF** IFs;
	EF** EFs;
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

static_assert(sizeof(AnimationData) == 0x54, "AnimationData MUST be 0x50 large!");
#undef CHECKOFFSET

template <typename T>
#pragma pack(push,1)
struct ArrayContainer {
	int isPointerArray;
	T* array;
	int elementSize;
	int count;
	int capacity;
};
#pragma pack(pop)

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
	ArrayContainer<AnimationData>* animationDataContainer;
	UNUSED(0x20);
} PatternData;
#pragma pack(pop)

#define CHECKOFFSET(v, n) static_assert(offsetof(PatternData, v) == n, "PatternData offset incorrect for " #v);

CHECKOFFSET(PSTS, 0x24);
CHECKOFFSET(PFLG, 0x2C);
CHECKOFFSET(animationDataContainer, 0x34);

static_assert(sizeof(PatternData) == 0x58, "PatternData must have size 0x58."); // i am pretty confident on this number, but it could be off.
#undef CHECKOFFSET

#pragma pack(push,1)
typedef struct CommandData {
	int ID;
	char input[20];
	int pattern;
	int specialFlag;
	int meterSpend;
	byte assistVar;
	byte specialVar;
	byte dashVar;
	UNUSED(1);
	uint8_t flagsets[4]; //flagset 1, flagset 2, unused, unused
} CommandData;
#pragma pack(pop)

#define CHECKOFFSET(v, n) static_assert(offsetof(CommandData, v) == n, "CommandData offset incorrect for " #v);

CHECKOFFSET(flagsets, 0x28);

static_assert(sizeof(CommandData) == 0x2C, "CommandData must have size 0x2C.");
#undef CHECKOFFSET

#pragma pack(push,1)
typedef struct CommandFileData {
	ArrayContainer<CommandData*>* cmdDataPtr;
	byte airJumpNum;
	UNUSED(0x4);
	byte Flags;
	UNUSED(0x1);
	byte KoRareVoice;
	UNUSED(0x4);
	int CharaGravity_AddY;
	int CharaGravity_MaxX;
	float guts[4];
	UNUSED(0x8);
	int ShieldCounter_Ground;
	int ShieldCounter_Air;
	int ShieldCounter_Crouch;
	UNUSED(0x8);
	int CancelSparkPat_Ground; //unused
	int CancelSparkPat_Air;
	int CancelHighJumpPat;
	short isGroundThrowDefined;
	short groundThrowPat;
	short groundThrowRange;
	short isAirThrowDefined;
	short airThrowPat;
	short airThrowRange;
	int KoAniEff;
	int ExComCheck_Num;
	void* ExComCheckPtr;
} CommandFileData;
#pragma pack(pop)

#define CHECKOFFSET(v, n) static_assert(offsetof(CommandFileData, v) == n, "CommandFileData offset incorrect for " #v);

CHECKOFFSET(airThrowPat, 0x58);

static_assert(sizeof(CommandFileData) == 0x68, "CommandFileData must have size 0x5A.");
#undef CHECKOFFSET

#pragma pack(push,1)
typedef struct HA6Data {
	DWORD isStaticSize;
	ArrayContainer<PatternData*>* patternContainer;
} HA6Data;
#pragma pack(pop)


#pragma pack(push,1)
typedef struct CharFileData { 
	// out of all the structs, i understand this one the least.
	// the important thing, is that it eventually leads to 0x31C patternDataPtr, and so in most cases, you can and should probs just use that
	// this however, can be used to see all paterns in a char. seems very nice to have
	HA6Data* DataFile;
	void* BmpcutFile;
	void* PAniFile;
} CharFileData;
#pragma pack(pop)

typedef struct EffectData;
typedef struct PlayerData;

#pragma pack(push,1)
typedef struct ActorData {
	BYTE index;
	BYTE charID;
	BYTE charIDCopy;
	BYTE doTrainingAction;
	BYTE source;
	BYTE someFlag;
	BYTE palette;
	UNUSED(1);
	WORD moon;
	UNUSED(2);
	DWORD pattern;
	DWORD state;
	DWORD nextSprite;
	DWORD framesInCurrentState;
	BYTE isFirstFramePlusOne;
	UNUSED(2);
	BYTE didPatternTransition;
	UNUSED(1);
	BYTE remainingLoops;
	WORD EFTP1flagset1;
	WORD EFTP1flagset2;
	WORD commandSpecialVar;
	UNUSED(0x24);
	DWORD randomAirTech;
	DWORD randomGroundTech;
	UNUSED(0xC);
	DWORD notInCombo;
	DWORD numFrameAndPatternTransitions;
	DWORD defensiveStateFlag;
	UNUSED(0x18);
	DWORD defensiveStateQueue;
	DWORD flags[6];
	UNUSED(0xC);
	DWORD exGuard;
	DWORD exGuardTimer;
	DWORD doClashExtraHitstop;
	DWORD health;
	DWORD redHealth;
	float guardGauge;
	float guardGaugeHeal;
	DWORD guardGaugeState;
	DWORD guardGaugeStop;
	WORD onBlockComboCount;
	WORD guardQualityStop;
	float quardQuality;
	WORD exGuardMeterPenaltyTimer;
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
	WORD meterMultTimer;
	WORD meterMultTimerTotal;
	WORD meterGainMultiplier; //out of 256. ex moves set to 110 for 43% meter gain
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
	int momentum;
	int momentumQueue;
	short momentumDecay;
	short momentumGrantingPattern;
	short useAddYMaxXParams;
	UNUSED(0x12);
	int thrownXOffset;
	int thrownYOffset;
	short hitType;
	short projectileHitType;
	short zPriority;
	short zPrioritySetter;
	UNUSED(0x4);
	int shieldHeldTime;
	BYTE shieldSuccessType;
	BYTE autoSuperJump;
	BYTE hitstop;
	BYTE hitstopAdvanceFrames;
	UNUSED(1);
	BYTE ungrantedHitstop;
	BYTE throwFlag;
	BYTE noInputFlag;
	BYTE tagFlag;
	UNUSED(1);
	BYTE remainingHits;
	BYTE inBlockstun;
	BYTE willBlock;
	BYTE proxyGuardTime;
	BYTE bounceCount;
	UNUSED(2);
	BYTE comboJumpCancel;
	BYTE burstLock;
	UNUSED(2);
	BYTE strikeInvuln;
	BYTE throwInvuln;
	UNUSED(1);
	BYTE preJumpThrowInvuln;
	UNUSED(1);
	WORD airTime;
	WORD timeThrown;
	WORD totalUntechTime;
	WORD untechTimeElapsed;
	UNUSED(2);
	BYTE canAirTech;
	BYTE canGroundTech;
	BYTE isGroundTech;
	UNUSED(1);
	WORD armorTimer;
	WORD reversedControlsTimer;
	UNUSED(8);
	BYTE receivedHitstop;
	BYTE isHitboxConnect;
	UNUSED(2);
	DWORD hitstunBlockstunTimeElapsed;
	int hitstunTimeRemaining; // -2 if airborne
	BYTE completedHitVectors;
	BYTE didHitVectorFaceLeft;
	BYTE receivedHitVector;
	UNUSED(0x2);
	BYTE someDeathFlagMaybe;
	UNUSED(1);
	BYTE jumpVariable;
	BYTE specialVariables[10];
	BYTE dashVariable;
	UNUSED(1);
	WORD extraVariables[10];
	WORD xVelStorage;
	WORD yVelStorage;
	WORD xAccStorage;
	WORD yAccStorage;
	UNUSED(0x6);
	BYTE afterImageBlendMode;
	BYTE numAfterImages;
	BYTE afterImageFramePosOffset;
	UNUSED(0x1);
	WORD reversePenalty;
	WORD reversePenaltyDecayTimer;
	WORD onHitComboCount;
	WORD unknownHitCounter;
	UNUSED(0x8);
	BYTE counterhitState;
	UNUSED(1);
	AttackData* receivingAttackDataPtrArr[8];
	CameraBoxData hitboxOverlapArr[8];
	ActorData* attackingSubObjPtrArr[8];
	ActorData* lastHitBySubObjPtr;
	ActorData* lastHitSubObjPtr;
	ActorData* isControllingSubObjPtr;
	BYTE isControllingActor;
	UNUSED(0x3);
	ActorData* OwnerSubObjPtr;
	ActorData* isControlledBySubObjPtr;
	UNUSED(0x8);
	int type2FlashSpawnedDuring;
	UNUSED(0x4);
	float gravity;
	WORD untechPenalty;
	BYTE correctedDirInput;
	BYTE rawDirInput;
	DWORD buttonInputs; // 0x000AA0CC  -  AA : binary combination of buttons held (0x01 = A, 0x02 = B ... 0x10 = E) ,  CC : binary combination of buttons just pressed
	DWORD buttonReleased; // binary combination of buttons just released (0x01 = A, 0x02 = B ... 0x10 = E)
	BYTE ownerIndex;
	BYTE queueDespawn;
	WORD numSpawnedEffects;
	int grabLocX;
	int grabLocY;
	int spriteRotation;
	float xScale;
	float yScale;
	WORD targetPattern;
	WORD targetPatternPriority;
	WORD targetState;
	WORD targetStatePriority;
	BYTE facingLeft;
	BYTE isOpponentToLeft;
	BYTE needToCrossupInputs;
	UNUSED(0x1);
	BYTE delayedStance;
	BYTE doLanding;
	BYTE justLanded;
	UNUSED(0x1);
	PatternData* patternDataPtr;
	AnimationData* animationDataPtr;
	AttackData* attackDataPtr;
	EffectData* selfPtr;
	ActorData* partnerPtr;
	CharFileData* charFileDataPtr;
	DWORD framesIntoCurrentPattern;
	UNUSED(4);
} ActorData;
#pragma pack(pop)

#define CHECKOFFSET(v, n) static_assert(offsetof(ActorData, v) == n, "ActorData offset incorrect for " #v);

CHECKOFFSET(randomAirTech, 0x4c);
CHECKOFFSET(defensiveStateQueue, 0x84);
CHECKOFFSET(xPos, 0x104);
CHECKOFFSET(momentum, 0x134);
CHECKOFFSET(thrownXOffset, 0x154);
CHECKOFFSET(airTime, 0x186);
CHECKOFFSET(extraVariables, 0x1c0);
CHECKOFFSET(remainingHits, 0x176);
CHECKOFFSET(unknownHitCounter, 0x1EC);
CHECKOFFSET(receivingAttackDataPtrArr, 0x1f8);
CHECKOFFSET(isControllingSubObjPtr, 0x2c0);
CHECKOFFSET(gravity, 0x2e0);
CHECKOFFSET(numSpawnedEffects, 0x2f2);
CHECKOFFSET(needToCrossupInputs, 0x312);

#undef CHECKOFFSET

static_assert(sizeof(ActorData) == 0x338, "ActorData MUST be 0x338 large!");

#pragma pack(push,1)
typedef struct EffectData {
	void describe(char* buffer, int bufLen);
	PatternData* getPatternDataPtr(int p);
	AnimationData* getAnimationDataPtr(int p, int s);
	// -----
	DWORD exists;
	ActorData subObj;
} EffectData;
#pragma pack(pop)

static_assert(sizeof(EffectData) == 0x33C, "EffectData MUST be 0x33C large!");

#pragma pack(push,1)
typedef struct PlayerData : public EffectData {
	CommandFileData* cmdFileDataPtr;
	UNUSED(0x8);
	int reduceFlag;
	int reduceCounter;
	UNUSED(0x4);
	int usedNormalsInString;
	UNUSED(0x80);
	int inputCmdID;
	UNUSED(0x4);
	int chainedCmdsCounter;
	UNUSED(0x4);
	WORD dirInputs[0x81];
	WORD aInputs[0x81];
	WORD bInputs[0x81];
	WORD cInputs[0x81];
	WORD dInputs[0x81];
	WORD eInputs[0x81];
	WORD fInputs[0x81];
	UNUSED(0x6);
} PlayerData;
#pragma pack(pop)

#define CHECKOFFSET(v, n) static_assert(offsetof(PlayerData, v) == n, "PlayerData offset incorrect for " #v);

CHECKOFFSET(exists, 0)
CHECKOFFSET(chainedCmdsCounter, 0x3e0);

static_assert(sizeof(PlayerData) == 0xAFC, "PlayerData MUST be 0xAFC large!");

#undef CHECKOFFSET

#pragma pack(push,1)
typedef struct AttackDisplayData {
	int comboInvalid;
	int comboTrue;
	int damageScaled;
	int damageUnscaled;
	int vsDamage;
	int meterGain;
} AttackDisplayData;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct ComboCalcData {
	byte index;
	short field_0x2;
	int numHits;
	int damage;
	short isInvalid;
	short field_0xe;
	short proration;
	short otgMeterMult;
	byte drawComboData;
	byte field_0x15;
	short timer1;
	short timer2;
	short field_0x1a;
	int someFlag;
	int timer3;
	int xPos;
	int yPos;
	byte alpha;
} ComboCalcData;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct PlayerAuxData {
	int activeCharacter;
	int assistChangeState;
	UNUSED(0x4);
	int hasPartner;
	UNUSED(0x10);
	int dispCorrectionValue;
	UNUSED(0x18);
	float someDamageMult;
	UNUSED(0x4);
	int comboCount;
	int comboCountCopy;
	UNUSED(0x4);
	int dispMaxCombo;
	int maxDamage;
	int maxCombo;
	UNUSED(0x8);
	int nextAttackDisplayIndex;
	int currentAttackDisplayIndex;
	UNUSED(0x4);
	AttackDisplayData attackDisplayData[2];
	byte comboCalcIndex;
	ComboCalcData comboCalcData[8];
	UNUSED(0x3);
	int dispCHTimer;
	int inactionableFrames;
} PlayerAuxData;
#pragma pack(pop)

#define CHECKOFFSET(v, n) static_assert(offsetof(PlayerAuxData, v) == n, "PlayerAuxData offset incorrect for " #v);

CHECKOFFSET(comboCalcData, 0xA1);
CHECKOFFSET(inactionableFrames, 0x208);

static_assert(sizeof(PlayerAuxData) == 0x20C, "PlayerAuxData must have size 0x5A.");
#undef CHECKOFFSET

#pragma pack(push,1)
typedef struct CSSData {
	int CharaNo;
	char Name[48];
	char File1[32];
	char File2[32];
	int CharaNum[2];
	int GiantFlag;
	int TagType;
	UNUSED(0xC);
} CSSData;
#pragma pack(pop)

#define CHECKOFFSET(v, n) static_assert(offsetof(CSSData, v) == n, "CSSData offset incorrect for " #v);

CHECKOFFSET(TagType, 0x80);

static_assert(sizeof(CSSData) == 0x90, "CSSData must have size 0x90.");
#undef CHECKOFFSET

#pragma pack(push,1)
typedef struct HitEffectData {

} HitEffectData; // remember when i reversed this whole thing and then proceded to accidentally wipe all my work?
#pragma pack(pop)


// below is the render stuff from 2v2, trust none of it

#define packedStruct typedef struct 

#pragma pack(push,1)
packedStruct RawMeltyVert{
	float x;
	float y;
	float z;
	float w;
	DWORD diffuse;
	DWORD specular;
	float u;
	float v;
} RawMeltyVert;
static_assert(sizeof(RawMeltyVert) == 0x20, "RawMeltyVert must be size 0x20");
#pragma pack(pop)

#pragma pack(push,1)
packedStruct RenderList {

	packedStruct LinkedListRenderList {

		packedStruct LinkedListRenderElement {

			packedStruct LinkedListRenderData {
			// straight up? not fuckin sure about the size of this
			// its either going to point to another 

			LinkedListRenderElement * nextElement; // loops back to where this thing would have been if,, yea, yk

			// not sure about this one at all
			union {
				DWORD flags; //
				struct {
					BYTE flag0; // most likely a D3DRESOURCETYPE. very weird that its only,,, one byte though,,, so it might be something else. check out 004c0243 for an explanation
					BYTE flag1;
					BYTE flag2;
					BYTE flag3;
				};
			};

			bool isTexData() {
				// is flag0 here correct?? in terms of byte order/endianness
				return flag0 == 0x01;
			}

			union {
				RawMeltyVert verts[4];
				struct {
					RawMeltyVert v0;
					RawMeltyVert v1;
					RawMeltyVert v2;
					RawMeltyVert v3;
				};
			};

			IDirect3DTexture9* tex; // at 0x88

			// why do i feel like these look similar to that weird memory area i used to,,, see if something was heat?
			// check out 0041650b
			BYTE unknown1; // 0x8C
			BYTE unknown2; // 0x8D
			WORD unknown3; // 0x8E
			WORD unknown4; // 0x90;
			WORD unknown5; // 0x92; // might just be padding

		} LinkedListRenderData;
		static_assert(sizeof(LinkedListRenderData) == 0x94 , "LinkedListRenderData size error"); // created at 00414EEA

		// dawg. i got NO FUCKING CLUE WHATS HAPPENING
		// basically, if this ptr + 4 is nonzero, then hit it with the switch at 004c03cb
		// and its an alloc size 94. if not, then just keep chugging down the list!

		// if this is a nextElement, [choice + 4] == 0
		// if this is an actual fucking thing with draw details, [choice + 4] != 0

		union {
			LinkedListRenderElement* nextElement;
			LinkedListRenderData* nextData; // this is for textures. but there is a different kind of struct for each directx type. look at 004c0243 for more info
		};

		//LinkedListAssetsElementChoice choice; 
		DWORD stupidFlags; // 00414e48 sets this to 0, 004C0566 skips drawing if its,, not 0?
		DWORD unknown2; // swaps between 0 and 1, not sure why. this is what doesThingsIfList+8=0 interacts with
		DWORD possibleCount; // 1600. why? im not sure

		bool isTexChoice() {
			if (nextData == NULL) {
				return false;
			}
			return nextData->isTexData();
		}

		} LinkedListRenderElement;
		static_assert(sizeof(LinkedListRenderElement) == 0x10, "LinkedListRenderElement != 0x10");

		// i could, maybe should, have this whole struct be a union of LinkedListAssets and LinkedListAssetsData
		// tbh i will
		// although, is this fucker always accessed as a linked list? or also as an array??
		// theres no fucking way that its,, i,,,
		LinkedListRenderElement elements[1600];
		} LinkedListRenderList;
		static_assert(sizeof(LinkedListRenderList) == 0x6400, "LinkedListRenderList != 0x6400");

		packedStruct LinkedListAllAssets {

			packedStruct LinkedListAssetsList {
			   LinkedListRenderList::LinkedListRenderElement * elements[8000];
			} LinkedListAssetsList;
			static_assert(sizeof(LinkedListAssetsList) == 0x7D00, "LinkedListAssetsList != 0x7D00"); // alloced at 00401BDF

			DWORD unknown1; // set to one
			LinkedListAssetsList* assetsList;
			DWORD numDraws; // set to,, 4? but i believe that, holy fuck. i knew from previous bs this was the number of gameplay drawss on screen. im in css now, but im drawing 4 char sprites!!
			DWORD unknown3; // 8000
			DWORD unknown4; // 8000. this is the length

		} LinkedListAllAssets;
		static_assert(sizeof(LinkedListAllAssets) == 0x14, "LinkedListAllAssets != 0x14"); // created at 0041513E

		/*

			going off of nothing but vibes here, but i think i had the two lists reversed
			which explains why i could never find the one at 5550b0 in the directx area, but could with 5550a0
			justifications:
				A0 is only 1600 long, sprite ids for chars go up to,,, what is it ~1000? 2 chars couldnt fit
				B0 is 8000. plenty of space

				B0 is offset by addr too, for example, sprite index probs!!!

				this is beautiful.
				i will now go sleep

			as for how the game draws 2 of the same thing, twice at the same time, check out 0041618

		*/

		LinkedListRenderList* linkedListRenderList; // malloced at 0041507D with size 0x6400
		DWORD linkedListAssetsCount; // 1600, the malloc was 0x6400, 0x6400 / 0x10 is 1600.

		LinkedListAllAssets* linkedListAllAssets;

		DWORD unknownCount1;
		DWORD unknownCount1_sub;

		DWORD unknownCount2;
		DWORD unknownCount2_sub;

} RenderList;
#pragma pack(push,1)

static_assert(sizeof(RenderList) == 7 * sizeof(DWORD), "RenderList != 0x1C (7 * 0x4)");

static RenderList* renderList = (RenderList*)0x005550A8;

// the following is horrible, but i want access to nested types. 

typedef RenderList::LinkedListRenderList                                                            LinkedListRenderList;
typedef RenderList::LinkedListRenderList::LinkedListRenderElement                                   LinkedListRenderElement;
typedef RenderList::LinkedListRenderList::LinkedListRenderElement::LinkedListRenderData             LinkedListRenderData;
//typedef RenderList::LinkedListRenderList::LinkedListRenderElement::LinkedListAssetsElementChoice    LinkedListAssetsElementChoice;

typedef RenderList::LinkedListAllAssets                                                             LinkedListAllAssets;
typedef RenderList::LinkedListAllAssets::LinkedListAssetsList									LinkedListAssetsList;



extern PlayerData* playerDataArr;
extern PlayerAuxData* playerAuxDataArr;
extern EffectData* effectDataArr;

extern bool shouldDisplayDebugInfo;
void displayDebugInfo();
void debugLinkedList();
bool isValidTexture(DWORD addr);
bool isValidResource(DWORD addr);
bool isValidSurface(DWORD addr);
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
const DWORD MBAA_FullCharacterReload = 0x00448fb0;
void FullCharacterReload();
const DWORD MBAA_UpdateCharPointers = 0x0045f650;
void UpdateCharPointers(ActorData* actorData);
DWORD getCancelStatus(int playerIndex, const char* move);
