#include "FrameBar.h"

int nLastFrameCount = 0; //Counts slower during slowdown
int nLastTrueFrameCount = 0; //Counts all frames during slowdown

int nBarCounter = 0;
int nBarScrolling = 0;
int nBarIntervalCounter = 0;
int nBarIntervalMax = 0;

bool bIsBarReset = false;
bool bDoBarReset = false;
bool bUpdateBar = false;
bool bDoAdvantage = false;

bool bDisplayFreeze = false; //Whether to show global ex flashes and frames where both chars are in hitstop
bool bDisplayInputs = false;
int displayNumbers = true;

int nPlayerAdvantage;
int nSharedHitstop;

float fFrameBarX = 20.0f;
float fFrameBarY = 410.0f;
float fFrameBarW = 600; // total inside width of framebar
float fFrameBarH = 26; // total inside height of framebar

int nFrameBarY = 410;
int nFrameBarDisplayRange = 75;

char pcTextToDisplay[256];
char pcTextPattern[256];
int nDrawTextTimer = 0;
int nClearSaveTimer = 0;

FrameBarPlayerData FB_P1{ 0, (PlayerData*)(adMBAABase + adP1Base), adMBAABase + adP1Inaction };
FrameBarPlayerData FB_P2{ 1, (PlayerData*)(adMBAABase + adP2Base), adMBAABase + adP2Inaction };
FrameBarPlayerData FB_P3{ 2, (PlayerData*)(adMBAABase + adP3Base), adMBAABase + adP1Inaction };
FrameBarPlayerData FB_P4{ 3, (PlayerData*)(adMBAABase + adP4Base), adMBAABase + adP2Inaction };

FrameBarPlayerData* FB_PlayerArray[4] = { &FB_P1, &FB_P2, &FB_P3, &FB_P4 };

FrameBarPlayerData* FB_Main1 = &FB_P1;
FrameBarPlayerData* FB_Main2 = &FB_P2;
FrameBarPlayerData* FB_Assist1 = &FB_P3;
FrameBarPlayerData* FB_Assist2 = &FB_P4;

const DWORD METER_COLOR_MAP[3] = { 0xFFC80000, 0xFFC8C800, 0xFF00C800 };
const DWORD HEAT_COLOR = 0xFF5A5AE6;
const DWORD HEATFONT_COLOR = 0xFFFA5300;
const DWORD MAX_COLOR = 0xFFFAA000;
const DWORD MAXFONT_COLOR = 0xFF2796FD;
const DWORD BLOODHEAT_COLOR = 0xFFB4B4B4;
const DWORD BLOODHEATFONT_COLOR = 0xFF8C0000;
const DWORD UNLIMITED_COLOR = 0xFF3296FF;
const DWORD UNLIMITEDFONT_COLOR = 0xFFFEABFF;
const DWORD CIRCUITBREAK_COLOR = 0xFFBE64C8;
const DWORD CIRCUITBREAKFONT_COLOR = 0xFFB06ED7;

const DWORD FB_INACTIONABLE = 0xFF41C800;
const DWORD FB_JUMP = 0xFFF1E084;
const DWORD FB_HITSTUN = 0xFF8C8C8C;
const DWORD FB_BLOCKSTUN = 0xFFB4B4B4;
const DWORD FB_ACTIVE = 0xFFFF0000;
const DWORD FB_ACTIONABLE = 0xFF202020;
const DWORD FB_ADVANTAGE = 0xFF101010;
const DWORD FB_NEUTRAL = 0xFF205A00;
const DWORD FB_FREEZE = 0xFF3C3C3C;
const DWORD FB_FREEZE_ACTIVE = 0xFFFF8080;
const DWORD FB_THROWN = 0xFF6E6E6E;
const DWORD FB_HITSTOP = 0xFF3C5080;
const DWORD FB_SHIELD = 0xFF91C2FF;
const DWORD FB_THROW_ACTIVE = 0xFFC00080;
const DWORD FB_CLASH = 0xFFE1B800;
const DWORD FB_INVULN = 0xFFFFFFFF;
const DWORD FB_ASSIST_ACTIVE = 0xFFFF8000;
const DWORD FB_COUNTER = 0xFFC485EA;

FrameBar::FrameBar(float x_, float y_, float w_, float h_, int numCells_)
{
	x = x_;
	y = y_;
	w = w_;
	h = h_;
	numCells = numCells_;

	dragInfo.dragPointX = &x;
	dragInfo.dragPointY = &y;
	dragManager.add(&dragInfo);
}

void FrameBar::draw()
{
	int nBarDrawCounter = 0;

	short sAdjustedScroll = min(min(nBarCounter - numCells, BAR_MEMORY_SIZE - numCells), -nTRUE_SCROLL_DISPLAY);

	int nForStart = (nBarCounter % BAR_MEMORY_SIZE) - numCells - sAdjustedScroll;
	int nForEnd = (nBarCounter % BAR_MEMORY_SIZE) - sAdjustedScroll;
	if (nBarCounter <= numCells)
	{
		nForStart = 0;
		nForEnd = nBarCounter;
	}

	dragInfo.topLeftX = x - (w / 2) - 2;
	dragInfo.topLeftY = y - (h / 2);
	dragInfo.bottomRightX = x + (w / 2) + 1;
	dragInfo.bottomRightY = y + (h / 2) + 1;

	float l = x - (w / 2) + 0.5f;
	float t = y - (h / 2);

	RectDraw(l - 2, t, w + 3, h + 1, 0x99000000); //Background

	RectDraw(l - 3, t - 2, w + 5, 2, 0xFFFFFFFF);
	RectDraw(l - 4, t - 1, 2, h + 3, 0xFFFFFFFF);
	RectDraw(l - 3, t + h + 1, w + 5, 2, 0xFFFFFFFF);
	RectDraw(l + w + 1, t - 1, 2, h + 3, 0xFFFFFFFF);

	int j = 0;
	for (int i = nForStart; i < nForEnd; i++)
	{
		j = i < 0 ? i + BAR_MEMORY_SIZE : i;

		float cellX = l + w / (float)numCells * (float)nBarDrawCounter;
		float cellW = w / (float)numCells;
		FB_Main1->cells[j].draw(cellX, t + h * 0.06, cellW, h * 0.38);
		FB_Main2->cells[j].draw(cellX, t + h * 0.56, cellW, h * 0.38);

		nBarDrawCounter++;
	}
	static char buffer[256];
	snprintf(buffer, 256, "Startup %3iF / Total %3iF / Advantage %3iF", FB_Main1->nFirstActive % 1000, FB_Main1->nInactionableMemory % 1000, nPlayerAdvantage % 1000);
	TextDraw(l, t - 12, 10, 0xFFFFFFFF, buffer);

	snprintf(buffer, 256, "Startup %3iF / Total %3iF / Advantage %3iF", FB_Main2->nFirstActive % 1000, FB_Main2->nInactionableMemory % 1000, -nPlayerAdvantage % 1000);
	TextDraw(l, t + h + 3, 10, 0xFFFFFFFF, buffer);
}

FrameBar frameBar(320.0f, 410.0f, 600.0f, 26.0f, 75);

void FrameBarCell::draw(float x, float y, float w, float h)
{
	float adjW = w * 0.90f;
	if (mainColor != 0x0) RectDraw(x, y, adjW, h, mainColor);
	if (subColor != 0x0) RectDraw(x + (adjW / 2), y, adjW / 2, h, subColor);
	if (subActiveColor != 0x0) RectDraw(x, y + (0.9 * h), adjW, 0.2 * h, subActiveColor);
	if (airborneColor != 0x0) RectDraw(x, y - (0.1 * h), adjW, 0.2 * h, airborneColor);

	static char buffer[8];

	if (number >= 0 && displayNumbers)
	{
		float size = min(w / 0.8f, h);
		int nLength = floor(log10(number));
		snprintf(buffer, 8, "%i", number);
		TextDraw(x - (size * 0.72 * nLength + 1), y, size, 0xFFFFFFFF, buffer);
	}
}

void FrameBarCell::reset()
{
	mainColor = 0x0;
	subColor = 0x0;
	subActiveColor = 0x0;
	airborneColor = 0x0;
	number = -1;
	numFlag = 0;
}

void UpdatePlayers() //Called after bar handling
{
	for (int i = 0; i < 4; i++) {
		FrameBarPlayerData& P = *FB_PlayerArray[i];
		P.nLastInactionableFrames = *(int*)(P.adInaction);
		P.nLastFrameCount = P.PlayerData->subObj.heatTimeCounter;
		P.bLastOnRight = P.PlayerData->subObj.isOpponentToLeft;
		P.dwLastActivePointer = (DWORD)P.PlayerData->subObj.attackDataPtr;
		P.cLastHitstop = P.PlayerData->subObj.hitstop;
		if (P.PlayerData->subObj.targetPattern != 0xFFFF) {
			P.bAlreadyGotFirstActive = false;
			P.nFirstActiveCounter = 0;
		}
	}
}

void CheckProjectiles()
{
	for (int i = 0; i < 4; i++) {
		FB_PlayerArray[i]->bLastProjectileActive = FB_PlayerArray[i]->bProjectileActive;
		FB_PlayerArray[i]->bProjectileActive = false;
	}
	char cBlankEffectCount = 0;
	EffectData* curFX = (EffectData*)(adMBAABase + adEffectBase);
	for (int i = 0; i < 200; i++) //Check Projectiles for active
	{
		if (cBlankEffectCount > 16) break;
		cBlankEffectCount++;
		if (curFX->exists)
		{
			cBlankEffectCount = 0;
			if (curFX->subObj.attackDataPtr != 0 &&
				curFX->subObj.someFlag == 0xFF)
			{
				FrameBarPlayerData& P = *FB_PlayerArray[curFX->subObj.index];
				P.bProjectileActive = true;
			}
		}
		curFX = (EffectData*)(adMBAABase + adEffectBase + dwEffectStructSize * i);
	}
}

void CalculateAdvantage(FrameBarPlayerData& P1_, FrameBarPlayerData& P2_)
{
	if (*(int*)(P1_.adInaction) == 0 && *(int*)(P2_.adInaction) == 0)
	{
		bDoAdvantage = false;
	}
	else if (*(int*)(P1_.adInaction) != 0 && *(int*)(P2_.adInaction) != 0)
	{
		bDoAdvantage = true;
		P1_.nAdvantageCounter = 0;
		P2_.nAdvantageCounter = 0;
	}

	if (bDoAdvantage && *(int*)(adMBAABase + adFrameCount) != nLastFrameCount &&
		*(int*)(adMBAABase + adP1Freeze) == 0 &&
		*(int*)(adMBAABase + adP2Freeze) == 0 &&
		*(char*)(adMBAABase + adGlobalFreeze) == 0)
	{
		if (*(int*)(P1_.adInaction) == 0 && *(int*)(P2_.adInaction) != 0)
		{
			P1_.nAdvantageCounter += *(int*)(adMBAABase + adFrameCount) - nLastFrameCount;
		}
		else if (*(int*)(P2_.adInaction) == 0 && *(int*)(P1_.adInaction) != 0)
		{
			P2_.nAdvantageCounter += *(int*)(adMBAABase + adFrameCount) - nLastFrameCount;
		}
	}
}

void ResetBars()
{
	bIsBarReset = true;
	nBarCounter = 0;
	nBarIntervalCounter = 0;
	nBarScrolling = 0;
	bDoBarReset = false;
	nBarIntervalMax = frameBar.numCells;
	for (int i = 0; i < 4; i++)
	{
		FrameBarPlayerData& P = *FB_PlayerArray[i];
		for (int j = 0; j < BAR_MEMORY_SIZE; j++)
		{
			P.cells[j].reset();
		}
	}
	nTRUE_SCROLL_DISPLAY = 0;
	nSCROLL_DISPLAY = 2;
	*(short*)(adMBAABase + adXS_frameScroll) = -nTRUE_SCROLL_DISPLAY;
}

void UpdateBars(FrameBarPlayerData& P, FrameBarPlayerData& Assist)
{
	DWORD dwMainColor = 0x0;
	DWORD dwSubColor = 0x0;
	DWORD dwSubActiveColor = 0x0;
	DWORD dwAirborneColor = 0x0;

	int nNumber = -1;
	int nNumFlag = 0; //0 = default, go away on next info; 1 = persist always; 2 = persist and delete prior 2s; 3 = get deleted and pass it on if followed by 2

	//Bar 1 - General action information
	if (*(int*)(P.adInaction) != 0) //Doing something with limited actionability
	{
		dwMainColor = FB_INACTIONABLE;
		nNumber = *(int*)P.adInaction;
		if (P.PlayerData->subObj.pattern >= 35 && P.PlayerData->subObj.pattern <= 37) //Jump Startup
		{
			dwMainColor = FB_JUMP;
		}
		else if (P.PlayerData->subObj.hitstunTimeRemaining != 0 && !P.PlayerData->subObj.inBlockstun) //Hitstun
		{
			dwMainColor = FB_HITSTUN;
			if (P.PlayerData->subObj.animationDataPtr->stateData->stance == 1) //Airborne
			{
				if (P.PlayerData->subObj.untechTimeElapsed < P.PlayerData->subObj.totalUntechTime) //Still has untech remaining
				{
					nNumber = P.PlayerData->subObj.totalUntechTime - P.PlayerData->subObj.untechTimeElapsed;
				}
			}
			else //Grounded
			{
				if (P.PlayerData->subObj.hitstunTimeRemaining > 2) //Still has hitstun remaining
				{
					nNumber = P.PlayerData->subObj.hitstunTimeRemaining - 1;
				}
			}
		}
		else if (P.PlayerData->subObj.inBlockstun) //Blockstun
		{
			dwMainColor = FB_BLOCKSTUN;
			if (P.PlayerData->subObj.hitstunTimeRemaining > 2 && P.PlayerData->subObj.animationDataPtr->stateData->stance != 1)
			{
				nNumber = P.PlayerData->subObj.hitstunTimeRemaining - 1;
			}
		}
		else if (P.PlayerData->subObj.attackDataPtr) //Attacking
		{
			dwMainColor = FB_ACTIVE;
			nNumber = P.nActiveCounter;
			nNumFlag = 2;
			if (P.dwLastActivePointer == 0 && !P.bAlreadyGotFirstActive)
			{
				P.nFirstActive = P.nFirstActiveCounter;
				P.bAlreadyGotFirstActive = true;
			}
		}
	}
	else //Fully actionable
	{
		dwMainColor = FB_ACTIONABLE;

		if (P.nLastInactionableFrames != 0) //Neutral frame
		{
			dwMainColor = FB_NEUTRAL;
		}
		else if (bDoAdvantage) //Has advantage
		{
			dwMainColor = FB_ADVANTAGE;
		}
	}

	bool bIsShield = false;
	bool bIsThrow = false;
	bool bIsBoxCheck = false;
	if (P.PlayerData->subObj.animationDataPtr->highestIFIndex > 0)
	{
		AnimationData* animData = P.PlayerData->subObj.animationDataPtr;
		for (int i = 0; i < animData->highestIFIndex; i++) {
			if (animData->IFs[i] != 0) {
				if (animData->IFs[i]->IFTP == 52) {
					bIsThrow = true;
				}
				else if (animData->IFs[i]->IFTP == 51) {
					bIsShield = true;
				}
				else if (animData->IFs[i]->IFTP == 14) {
					bIsBoxCheck = true;
				}
			}
		}
	}

	if (P.PlayerData->subObj.throwFlag) //Being thrown
	{
		dwMainColor = FB_THROWN;
	}
	else if (P.PlayerData->subObj.hitstop != 0) //in hitstop
	{
		dwMainColor = FB_HITSTOP;
		if (!P.PlayerData->subObj.notInCombo && P.cLastHitstop == 0)
		{
			nNumFlag = 1;
		}
		else if (P.PlayerData->subObj.attackDataPtr != 0)
		{
			nNumFlag = 3;
			nNumber = -1;
		}
	}
	else if (bIsShield)
	{
		dwMainColor = FB_SHIELD;
	}
	else if (bIsThrow)
	{
		if (!P.bAlreadyGotFirstActive)
		{
			P.nFirstActive = P.nFirstActiveCounter;
			P.bAlreadyGotFirstActive = true;
		}
		dwMainColor = FB_THROW_ACTIVE;
	}

	if (*(char*)(adMBAABase + adGlobalFreeze) != 0 || //Screen is frozen
		((*(int*)(adMBAABase + adP1Freeze) != 0 ||
			*(int*)(adMBAABase + adP2Freeze) != 0 ||
			*(int*)(adMBAABase + adP3Freeze) != 0 ||
			*(int*)(adMBAABase + adP4Freeze) != 0) && *(byte*)(adMBAABase + adJustDidPlayerFreeze) == 0))
	{
		dwMainColor = FB_FREEZE;
		if (dwMainColor == FB_ACTIVE) //Attacking
		{
			dwMainColor = FB_FREEZE_ACTIVE;
		}
	}

	if (P.PlayerData->subObj.animationDataPtr->highestNonHitboxIndex == 12) //Clash
	{
		dwSubColor = FB_CLASH;
	}
	else if (P.PlayerData->subObj.animationDataPtr->highestNonHitboxIndex <= 1 || //Various forms of invuln
		P.PlayerData->subObj.strikeInvuln != 0 ||
		P.PlayerData->subObj.animationDataPtr->stateData->invincibility == 3)
	{
		dwSubColor = FB_INVULN;
	}
	else if (P.PlayerData->subObj.animationDataPtr->highestNonHitboxIndex == 10 &&
		bIsBoxCheck) //Special Box 1 and IFTP 14
	{
		dwSubColor = FB_COUNTER;
	}

	if (P.PlayerData->subObj.animationDataPtr->stateData->stance == 1) //Airborne
	{
		dwAirborneColor = FB_JUMP;
	}

	if (P.bProjectileActive || Assist.bProjectileActive)
	{
		dwSubActiveColor = FB_ACTIVE;
		if (!P.bLastProjectileActive && !P.bAlreadyGotFirstActive)
		{
			P.nFirstActive = P.nFirstActiveCounter;
			P.bAlreadyGotFirstActive = true;
			P.cells[(nBarCounter - 1) % BAR_MEMORY_SIZE].numFlag = 1;
		}
	}

	if (Assist.PlayerData->subObj.attackDataPtr != 0) //Check Assist for active
	{
		dwSubActiveColor = FB_ASSIST_ACTIVE;
		if (Assist.dwLastActivePointer == 0 && !P.bAlreadyGotFirstActive)
		{
			P.nFirstActive = P.nFirstActiveCounter;
			P.bAlreadyGotFirstActive = true;
			P.cells[(nBarCounter - 1) % BAR_MEMORY_SIZE].numFlag = 1;
		}
	}

	P.cells[nBarCounter % BAR_MEMORY_SIZE].mainColor = dwMainColor;
	P.cells[nBarCounter % BAR_MEMORY_SIZE].subColor = dwSubColor;
	P.cells[nBarCounter % BAR_MEMORY_SIZE].subActiveColor = dwSubActiveColor;
	P.cells[nBarCounter % BAR_MEMORY_SIZE].airborneColor = dwAirborneColor;

	P.cells[nBarCounter % BAR_MEMORY_SIZE].number = nNumber;
	P.cells[nBarCounter % BAR_MEMORY_SIZE].numFlag = nNumFlag;

	auto posMod = [](int x, int mod) {
		return (x % mod + mod) % mod;
		};

	int previousIndex = posMod(nBarCounter - 1, BAR_MEMORY_SIZE);

	if (nNumFlag != 2 &&
		nNumber >= 0 &&
		P.cells[previousIndex].numFlag == 0) //clear last frame if current frame has info and last frame numFlag = 0
	{
		P.cells[previousIndex].number = -1;
	}
	else if (nNumFlag == 2 &&
		nNumber >= 0 &&
		P.cells[previousIndex].numFlag == 2) //clear last frame if current and last frame numFlag = 2
	{
		P.cells[previousIndex].number = -1;
	}
	else if (nNumFlag == 2 &&
		nNumber >= 0 &&
		P.cells[previousIndex].numFlag == 3) //continuous clear if numFlag = 2 and previous = 3
	{
		int i = 1;
		while (P.cells[posMod(nBarCounter - i, BAR_MEMORY_SIZE)].numFlag == 3)
		{
			P.cells[posMod(nBarCounter - ++i, BAR_MEMORY_SIZE)].number = -1;
		}
	}
}

void IncrementActive(FrameBarPlayerData& P)
{
	if (P.PlayerData->subObj.attackDataPtr && P.PlayerData->subObj.hitstop == 0 && P.PlayerData->subObj.heatTimeCounter != P.nLastFrameCount)
	{
		P.nActiveCounter += *(int*)(adMBAABase + adFrameCount) - nLastFrameCount;
	}
	else if (P.PlayerData->subObj.attackDataPtr == 0)
	{
		P.nActiveCounter = 0;
	}
}

void IncrementFirstActive(FrameBarPlayerData& P1_, FrameBarPlayerData& P2_)
{
	if (P1_.PlayerData->subObj.hitstop == 0 &&
		P1_.PlayerData->subObj.heatTimeCounter != P1_.nLastFrameCount &&
		P2_.PlayerData->subObj.heatTimeCounter != P2_.nLastFrameCount)
	{
		P1_.nFirstActiveCounter += *(int*)(adMBAABase + adFrameCount) - nLastFrameCount;
	}
}

void HandleInactive(FrameBarPlayerData& P)
{
	if (*(int*)(P.adInaction) != 0)
	{
		P.nInactionableMemory = *(int*)(P.adInaction);
	}
}

void BarHandling(FrameBarPlayerData& P1_, FrameBarPlayerData& P2_, FrameBarPlayerData& P1Assist, FrameBarPlayerData& P2Assist)
{
	CalculateAdvantage(P1_, P2_);

	bool DoBarUpdate = (
		*(int*)(P1_.adInaction) != 0 ||
		*(int*)(P2_.adInaction) != 0 ||
		P1_.PlayerData->subObj.animationDataPtr->stateData->flagset2 != 0 ||
		P2_.PlayerData->subObj.animationDataPtr->stateData->flagset2 != 0 ||
		P1_.bProjectileActive != 0 ||
		P2_.bProjectileActive != 0 ||
		P1Assist.PlayerData->subObj.attackDataPtr != 0 ||
		P2Assist.PlayerData->subObj.attackDataPtr != 0 ||
		P1Assist.bProjectileActive != 0 ||
		P2Assist.bProjectileActive != 0
		); //True if either char is inactionable, can't block, has an active projectile, has an active assist, or has an active assist projectile

	if (DoBarUpdate)
	{
		bIsBarReset = false;
		bUpdateBar = true;
		nBarIntervalCounter = 0;
	}
	else
	{
		bUpdateBar = false;
		nBarIntervalCounter++;
	}

	if (nBarCounter >= frameBar.numCells)
	{
		nBarIntervalMax = BAR_INTERVAL;
	}

	if (nBarIntervalCounter > nBarIntervalMax)
	{
		bDoBarReset = true;
	}

	if (bDoBarReset && bUpdateBar)
	{
		ResetBars();
	}

	if (nBarIntervalCounter < nBarIntervalMax && !bIsBarReset)
	{
		bUpdateBar = true;
	}

	if (bUpdateBar)
	{
		if (P1_.PlayerData->subObj.hitstop != 0 &&
			P2_.PlayerData->subObj.hitstop != 0) //Player hitstop values count down but we need it to count up
		{
			nSharedHitstop++;
		}
		else
		{
			nSharedHitstop = 0;
		}

		bool bIsFreeze = (
			*(char*)(adMBAABase + adGlobalFreeze) != 0 ||
			nSharedHitstop > 1
			);

		IncrementFirstActive(P1_, P2_);
		IncrementFirstActive(P2_, P1_);

		if (bDisplayFreeze || !bIsFreeze)
		{
			IncrementActive(P1_);
			IncrementActive(P2_);
			HandleInactive(P1_);
			HandleInactive(P2_);
			UpdateBars(P1_, P1Assist);
			UpdateBars(P2_, P2Assist);
			if (P1Assist.PlayerData->exists)
			{
				UpdateBars(P1Assist, P1_);
			}
			if (P2Assist.PlayerData->exists)
			{
				UpdateBars(P2Assist, P2_);
			}
			nBarCounter += *(int*)(adMBAABase + adTrueFrameCount) - nLastTrueFrameCount;
			if (nBarCounter < 0) {
				nBarCounter = 0;
			}
		}
	}
}

void UpdateFrameBar()
{
	bDisplayFreeze = nSHOW_HITSTOP_AND_FREEZE != 0;
	bDisplayInputs = nSHOW_INPUTS != 0; //currently unused, may add input display to framebar at some point
	nBarScrolling = nTRUE_SCROLL_DISPLAY;

	FB_Main1 = &FB_P1;
	FB_Main2 = &FB_P2;
	FB_Assist1 = &FB_P3;
	FB_Assist2 = &FB_P4;
	if (FB_P1.PlayerData->subObj.tagFlag)
	{
		FB_Main1 = &FB_P3;
		FB_Assist1 = &FB_P1;
	}
	if (FB_P2.PlayerData->subObj.tagFlag)
	{
		FB_Main2 = &FB_P4;
		FB_Assist2 = &FB_P2;
	}

	CheckProjectiles();

	if (*(int*)(adMBAABase + adTrueFrameCount) != nLastTrueFrameCount)
	{
		BarHandling(*FB_Main1, *FB_Main2, *FB_Assist1, *FB_Assist2);
	}

	if (*(int*)(adMBAABase + adTrueFrameCount) == 1)
	{
		ResetBars();
	}

	nPlayerAdvantage = (FB_Main1->nAdvantageCounter - FB_Main2->nAdvantageCounter) % 100;

	UpdatePlayers();

	nLastFrameCount = *(int*)(adMBAABase + adFrameCount);
	nLastTrueFrameCount = *(int*)(adMBAABase + adTrueFrameCount);

	int nBarDrawCounter = 0;

	short sAdjustedScroll = min(min(nBarCounter - frameBar.numCells, BAR_MEMORY_SIZE - frameBar.numCells), -nTRUE_SCROLL_DISPLAY);

	int nForStart = (nBarCounter % BAR_MEMORY_SIZE) - frameBar.numCells - sAdjustedScroll;
	int nForEnd = (nBarCounter % BAR_MEMORY_SIZE) - sAdjustedScroll;
	if (nBarCounter <= frameBar.numCells)
	{
		nForStart = 0;
		nForEnd = nBarCounter;
	}

}

void drawFrameBar(bool doDraw)
{
	frameBar.dragInfo.enable = doDraw;
	if (frameBar.dragInfo.enable)
	{
		frameBar.draw();
		SetRegistryValue(sFRAME_BAR_X, frameBar.x);
		SetRegistryValue(sFRAME_BAR_Y, frameBar.y);
		SetRegistryValue(sFRAME_BAR_W, frameBar.w);
		SetRegistryValue(sFRAME_BAR_H, frameBar.h);
		SetRegistryValue(sFRAME_BAR_NUMCELLS, frameBar.numCells);
	}

}

void drawColorGuide()
{
	RectDraw(0, 8, 640, 50, 0xFF000000);

	RectDraw(10, 10, 7, 10, FB_INACTIONABLE);
	TextDraw(17, 10, 10, 0xFFFFFFFF, "INACTIONABLE");
	RectDraw(120, 10, 7, 10, FB_JUMP);
	TextDraw(127, 10, 10, 0xFFFFFFFF, "JUMP STARTUP");
	RectDraw(230, 10, 7, 10, FB_HITSTUN);
	TextDraw(237, 10, 10, 0xFFFFFFFF, "HITSTUN");
	RectDraw(300, 10, 7, 10, FB_BLOCKSTUN);
	TextDraw(307, 10, 10, 0xFFFFFFFF, "BLOCKSTUN");
	RectDraw(390, 10, 7, 10, FB_ACTIVE);
	TextDraw(397, 10, 10, 0xFFFFFFFF, "ACTIVE FRAMES");
	RectDraw(500, 10, 7, 10, FB_ACTIONABLE);
	TextDraw(507, 10, 10, 0xFFFFFFFF, "FULLY ACTIONABLE");
	RectDraw(10, 22, 7, 10, FB_ADVANTAGE);
	TextDraw(17, 22, 10, 0xFFFFFFFF, "FRAME ADVANTAGE");
	RectDraw(140, 22, 7, 10, FB_NEUTRAL);
	TextDraw(147, 22, 10, 0xFFFFFFFF, "NEUTRAL FRAME");
	RectDraw(260, 22, 7, 10, FB_FREEZE);
	TextDraw(267, 22, 10, 0xFFFFFFFF, "SCREEN FREEZE");
	RectDraw(370, 22, 7, 10, FB_FREEZE_ACTIVE);
	TextDraw(377, 22, 10, 0xFFFFFFFF, "ACTIVE DURING FREEZE");
	RectDraw(530, 22, 7, 10, FB_THROWN);
	TextDraw(537, 22, 10, 0xFFFFFFFF, "BEING THROWN");
	RectDraw(10, 34, 7, 10, FB_HITSTOP);
	TextDraw(17, 34, 10, 0xFFFFFFFF, "HITSTOP");
	RectDraw(80, 34, 7, 10, FB_SHIELD);
	TextDraw(87, 34, 10, 0xFFFFFFFF, "SHIELD");
	RectDraw(140, 34, 7, 10, FB_THROW_ACTIVE);
	TextDraw(147, 34, 10, 0xFFFFFFFF, "THROW ACTIVE FRAME");

	RectDraw(10, 45, 7, 10, FB_INACTIONABLE);
	RectDraw(14, 45, 3, 10, FB_CLASH);
	TextDraw(17, 45, 10, 0xFFFFFFFF, "CLASH");
	RectDraw(70, 45, 7, 10, FB_INACTIONABLE);
	RectDraw(74, 45, 3, 10, FB_INVULN);
	TextDraw(77, 45, 10, 0xFFFFFFFF, "INVULN");
	RectDraw(130, 45, 7, 10, FB_INACTIONABLE);
	RectDraw(134, 45, 3, 10, FB_COUNTER);
	TextDraw(137, 45, 10, 0xFFFFFFFF, "COUNTER");
	RectDraw(200, 45, 7, 10, FB_INACTIONABLE);
	RectDraw(200, 44, 7, 2, FB_JUMP);
	TextDraw(207, 45, 10, 0xFFFFFFFF, "AIRBORNE");
	RectDraw(280, 45, 7, 10, FB_INACTIONABLE);
	RectDraw(280, 54, 7, 2, FB_ACTIVE);
	TextDraw(287, 45, 10, 0xFFFFFFFF, "ACTIVE PROJECTILE");
	RectDraw(450, 45, 7, 10, FB_INACTIONABLE);
	RectDraw(450, 54, 7, 2, FB_ASSIST_ACTIVE);
	TextDraw(457, 45, 10, 0xFFFFFFFF, "ACTIVE ASSIST");
}

void drawSimpleMeter()
{
	float fScroll = *(float*)(adMBAABase + adTrainingResetScroll);
	int nResetOffset = 0;
	if (fScroll > 0)
		nResetOffset = 320.0f * fScroll;

	int Y = 19;

	DWORD P1Base = adP1SubBase + (*(uint8_t*)(adMBAABase + adP1SubBase + adTagFlag) * dwPlayerStructSize * 2);
	int nP1Meter = *(int*)(adMBAABase + P1Base + adMagicCircuit);
	int nP1MeterTime = *(int*)(adMBAABase + P1Base + adMagicCircuitTime);
	uint8_t nP1MeterMode = *(uint8_t*)(adMBAABase + P1Base + adMagicCircuitMode);
	short sP1CircuitBreakTimer = *(short*)(adMBAABase + P1Base + adCircuitBreakTimer);
	short sP1CircuitBreakTotal = *(short*)(adMBAABase + P1Base + adCircuitBreakTotal);
	short sP1CircuitBreakFlag = *(short*)(adMBAABase + P1Base + adBreakOrPenalty);
	uint8_t nP1Moon = *(uint8_t*)(adMBAABase + dwP1CharMoon);

	DWORD P2Base = adP2SubBase + (*(uint8_t*)(adMBAABase + adP2SubBase + adTagFlag) * dwPlayerStructSize * 2);
	int nP2Meter = *(int*)(adMBAABase + P2Base + adMagicCircuit);
	int nP2MeterTime = *(int*)(adMBAABase + P2Base + adMagicCircuitTime);
	uint8_t nP2MeterMode = *(uint8_t*)(adMBAABase + P2Base + adMagicCircuitMode);
	short sP2CircuitBreakTimer = *(short*)(adMBAABase + P2Base + adCircuitBreakTimer);
	short sP2CircuitBreakTotal = *(short*)(adMBAABase + P2Base + adCircuitBreakTotal);
	short sP2CircuitBreakFlag = *(short*)(adMBAABase + P2Base + adBreakOrPenalty);
	uint8_t nP2Moon = *(uint8_t*)(adMBAABase + dwP2CharMoon);

	static char buffer[8];

	RectDraw(60 - nResetOffset, Y, 214, 12, 0x99000000); //BG
	switch (nP1MeterMode)
	{
	case 0: //Normal, out of 30000
	{
		float fMeterScale = nP1Moon == 2 ? 94.3396 : 141.5094;
		DWORD dwMeterColor = METER_COLOR_MAP[nP1Meter / 10000];
		if (nP1Moon == 2 && nP1Meter >= 15000) dwMeterColor = METER_COLOR_MAP[2];
		RectDraw(61 - nResetOffset, Y + 1, nP1Meter / fMeterScale, 10, dwMeterColor);
		snprintf(buffer, 8, "%3.2f", nP1Meter / 100.0);
		TextDraw(61 - nResetOffset, Y + 1, 10, 0xFFFFFFFF, buffer);
		break;
	}
	case 1: //HEAT, out of 550
	{
		RectDraw(61 - nResetOffset, Y + 1, nP1MeterTime / 2.5943, 10, HEAT_COLOR);
		snprintf(buffer, 8, "%3i", nP1MeterTime);
		TextDraw(61 - nResetOffset, Y + 1, 10, 0xFFFFFFFF, buffer);
		TextDraw(61 + 212 - 4 * 7.7777 - nResetOffset, Y + 1, 10, HEATFONT_COLOR, "HEAT");
		break;
	}
	case 2: //MAX out of 600
	{
		RectDraw(61 - nResetOffset, Y + 1, nP1MeterTime / 2.8301, 10, MAX_COLOR);
		snprintf(buffer, 8, "%3i", nP1MeterTime);
		TextDraw(61 - nResetOffset, Y + 1, 10, 0xFFFFFFFF, buffer);
		TextDraw(61 + 212 - 3 * 7.7777 - nResetOffset, Y + 1, 10, MAXFONT_COLOR, "MAX");
		break;
	}
	case 3: //BLOOD HEAT out of 502
	{
		RectDraw(61 - nResetOffset, Y + 1, nP1MeterTime / 2.3679, 10, BLOODHEAT_COLOR);
		snprintf(buffer, 8, "%3i", nP1MeterTime);
		TextDraw(61 - nResetOffset, Y + 1, 10, 0xFFFFFFFF, buffer);
		TextDraw(61 + 212 - 10 * 7.7777 - nResetOffset, Y + 1, 10, BLOODHEATFONT_COLOR, "BLOOD HEAT");
		break;
	}
	case 5: //UNLIMITED
	{
		RectDraw(61 - nResetOffset, Y + 1, 212, 10, UNLIMITED_COLOR);
		TextDraw(61 - nResetOffset, Y + 1, 10, UNLIMITEDFONT_COLOR, "UNLIMITED");
		break;
	}
	}

	RectDraw(580 - 214 + nResetOffset, Y, 214, 12, 0x99000000); //BG
	switch (nP2MeterMode)
	{
	case 0: //NORMAL
	{
		float fMeterScale = nP2Moon == 2 ? 94.3396 : 141.5094;
		DWORD dwMeterColor = METER_COLOR_MAP[nP2Meter / 10000];
		if (nP2Moon == 2 && nP2Meter >= 15000) dwMeterColor = METER_COLOR_MAP[2];
		RectDraw(579 - nP2Meter / fMeterScale + nResetOffset, Y + 1, nP2Meter / fMeterScale, 10, dwMeterColor);
		snprintf(buffer, 8, "%6.2f", nP2Meter / 100.0);
		TextDraw(579 - 6 * 7.7777 + nResetOffset, Y + 1, 10, 0xFFFFFFFF, buffer);
		break;
	}
	case 1: //HEAT
	{
		RectDraw(579 - nP2MeterTime / 2.5943 + nResetOffset, Y + 1, nP2MeterTime / 2.5943, 10, HEAT_COLOR);
		snprintf(buffer, 8, "%3i", nP2MeterTime);
		TextDraw(579 - 3 * 7.7777 + nResetOffset, Y + 1, 10, 0xFFFFFFFF, buffer);
		TextDraw(579 - 212 + nResetOffset, Y + 1, 10, HEATFONT_COLOR, "HEAT");
		break;
	}
	case 2: //MAX
	{
		RectDraw(579 - nP2MeterTime / 2.8301 + nResetOffset, Y + 1, nP2MeterTime / 2.8301, 10, MAX_COLOR);
		snprintf(buffer, 8, "%3i", nP2MeterTime);
		TextDraw(579 - 3 * 7.7777 + nResetOffset, Y + 1, 10, 0xFFFFFFFF, buffer);
		TextDraw(579 - 212 + nResetOffset, Y + 1, 10, MAXFONT_COLOR, "MAX");
		break;
	}
	case 3: //BLOOD HEAT
	{
		RectDraw(579 - nP2MeterTime / 2.3679 + nResetOffset, Y + 1, nP2MeterTime / 2.3679, 10, BLOODHEAT_COLOR);
		snprintf(buffer, 8, "%3i", nP2MeterTime);
		TextDraw(579 - 3 * 7.7777 + nResetOffset, Y + 1, 10, 0xFFFFFFFF, buffer);
		TextDraw(579 - 212 + nResetOffset, Y + 1, 10, BLOODHEATFONT_COLOR, "BLOOD HEAT");
		break;
	}
	case 5: //UNLIMITED
	{
		RectDraw(579 - 212 + nResetOffset, Y + 1, 212, 10, UNLIMITED_COLOR);
		TextDraw(579 - 9 * 7.7777 + nResetOffset, Y + 1, 10, UNLIMITEDFONT_COLOR, "UNLIMITED");
		break;
	}
	}

	DWORD dwBorderColor = (sP1CircuitBreakTimer && !sP1CircuitBreakFlag) ? CIRCUITBREAK_COLOR : 0xFFFFFFFF;
	RectDraw(59 - nResetOffset, Y - 2, 216, 2, dwBorderColor);
	RectDraw(58 - nResetOffset, Y - 1, 2, 14, dwBorderColor);
	RectDraw(59 - nResetOffset, Y + 12, 216, 2, dwBorderColor);
	RectDraw(274 - nResetOffset, Y - 1, 2, 14, dwBorderColor);
	if (sP1CircuitBreakTimer && !sP1CircuitBreakFlag)
	{
		snprintf(buffer, 8, "%3i", sP1CircuitBreakTimer);
		RectDraw(61 - nResetOffset, Y + 1, sP1CircuitBreakTimer / (sP1CircuitBreakTotal / 212.0), 10, CIRCUITBREAK_COLOR & 0x60FFFFFF);
		TextDraw(61 + 106 - 1.5 * 7.7777 - nResetOffset, Y + 1, 10, CIRCUITBREAKFONT_COLOR, buffer);
	}

	dwBorderColor = (sP2CircuitBreakTimer && !sP2CircuitBreakFlag) ? CIRCUITBREAK_COLOR : 0xFFFFFFFF;
	RectDraw(59 + 306 + nResetOffset, Y - 2, 216, 2, dwBorderColor);
	RectDraw(58 + 306 + nResetOffset, Y - 1, 2, 14, dwBorderColor);
	RectDraw(59 + 306 + nResetOffset, Y + 12, 216, 2, dwBorderColor);
	RectDraw(274 + 306 + nResetOffset, Y - 1, 2, 14, dwBorderColor);
	if (sP2CircuitBreakTimer && !sP2CircuitBreakFlag)
	{
		snprintf(buffer, 8, "%3i", sP2CircuitBreakTimer);
		RectDraw(579 - sP2CircuitBreakTimer / (sP2CircuitBreakTotal / 212.0) + nResetOffset, Y + 1, sP2CircuitBreakTimer / (sP2CircuitBreakTotal / 212.0), 10, CIRCUITBREAK_COLOR & 0x60FFFFFF);
		TextDraw(579 - 106 - 1.5 * 7.7777 + nResetOffset, Y + 1, 10, CIRCUITBREAKFONT_COLOR, buffer);
	}
}