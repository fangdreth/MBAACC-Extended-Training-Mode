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

Player P1{ 0, (PlayerData*)(adMBAABase + adP1Base), adMBAABase + adP1Inaction };
Player P2{ 1, (PlayerData*)(adMBAABase + adP2Base), adMBAABase + adP2Inaction };
Player P3{ 2, (PlayerData*)(adMBAABase + adP3Base), adMBAABase + adP1Inaction };
Player P4{ 3, (PlayerData*)(adMBAABase + adP4Base), adMBAABase + adP2Inaction };

Player* paPlayerArray[4] = { &P1, &P2, &P3, &P4 };

Player* Main1 = &P1;
Player* Main2 = &P2;
Player* Assist1 = &P3;
Player* Assist2 = &P4;

//Point frameBarDragPoint(20.0f, 410.0f);
//DragInfo frameBarDrag = DragInfo(&frameBarDragPoint.x, &frameBarDragPoint.y);
//DragInfo frameBarDragInfo(&fFrameBarX, &fFrameBarY);

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

		float cellX = l + w / numCells * nBarDrawCounter;
		float cellW = w / numCells;
		Main1->cells[j].draw(cellX, t + h * 0.06, cellW, h * 0.38);
		Main2->cells[j].draw(cellX, t + h * 0.56, cellW, h * 0.38);

		nBarDrawCounter++;
	}
	static char buffer[256];
	snprintf(buffer, 256, "Startup %3iF / Total %3iF / Advantage %3iF", Main1->nFirstActive % 1000, Main1->nInactionableMemory % 1000, nPlayerAdvantage % 1000);
	TextDraw(l, t - 12, 10, 0xFFFFFFFF, buffer);

	snprintf(buffer, 256, "Startup %3iF / Total %3iF / Advantage %3iF", Main2->nFirstActive % 1000, Main2->nInactionableMemory % 1000, -nPlayerAdvantage % 1000);
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

	if (number >= 0)
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
		Player& P = *paPlayerArray[i];
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
	P1.bLastProjectileActive = P1.bProjectileActive;
	P1.bProjectileActive = false;
	P2.bLastProjectileActive = P2.bProjectileActive;
	P2.bProjectileActive = false;
	P3.bLastProjectileActive = P3.bProjectileActive;
	P3.bProjectileActive = false;
	P4.bLastProjectileActive = P4.bProjectileActive;
	P4.bProjectileActive = false;
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
				Player& P = *paPlayerArray[curFX->subObj.index];
				P.bProjectileActive = true;
			}
		}
		curFX = (EffectData*)(adMBAABase + adEffectBase + dwEffectStructSize * i);
	}
}

void CalculateAdvantage(Player& P1, Player& P2)
{
	if (*(int*)(P1.adInaction) == 0 && *(int*)(P2.adInaction) == 0)
	{
		bDoAdvantage = false;
	}
	else if (*(int*)(P1.adInaction) != 0 && *(int*)(P2.adInaction) != 0)
	{
		bDoAdvantage = true;
		P1.nAdvantageCounter = 0;
		P2.nAdvantageCounter = 0;
	}

	if (bDoAdvantage && *(int*)(adMBAABase + adFrameCount) != nLastFrameCount &&
		*(int*)(adMBAABase + adP1Freeze) == 0 &&
		*(int*)(adMBAABase + adP2Freeze) == 0 &&
		*(char*)(adMBAABase + adGlobalFreeze) == 0)
	{
		if (*(int*)(P1.adInaction) == 0 && *(int*)(P2.adInaction) != 0)
		{
			P1.nAdvantageCounter += *(int*)(adMBAABase + adFrameCount) - nLastFrameCount;
		}
		else if (*(int*)(P2.adInaction) == 0 && *(int*)(P1.adInaction) != 0)
		{
			P2.nAdvantageCounter += *(int*)(adMBAABase + adFrameCount) - nLastFrameCount;
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
		Player& P = *paPlayerArray[i];
		for (int i = 0; i < BAR_MEMORY_SIZE; i++)
		{
			P.cells[i].reset();
		}
	}
	nTRUE_SCROLL_DISPLAY = 0;
	nSCROLL_DISPLAY = 2;
	*(short*)(adMBAABase + adXS_frameScroll) = -nTRUE_SCROLL_DISPLAY;
}

void UpdateBars(Player& P, Player& Assist)
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
			}
		}
	}

	if (*(char*)(adMBAABase + adGlobalFreeze) != 0 || //Screen is frozen
		*(int*)(adMBAABase + adP1Freeze) != 0 ||
		*(int*)(adMBAABase + adP2Freeze) != 0)
	{
		dwMainColor = FB_FREEZE;
		if (P.PlayerData->subObj.attackDataPtr != 0) //Attacking
		{
			dwMainColor = FB_FREEZE_ACTIVE;
		}
	}
	else if (P.PlayerData->subObj.throwFlag) //Being thrown
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
	else if (P.PlayerData->subObj.animationDataPtr->highestNonHitboxIndex == 10) //Special Box 1
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

void IncrementActive(Player& P)
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

void IncrementFirstActive(Player& P1, Player& P2)
{
	if (P1.PlayerData->subObj.hitstop == 0 &&
		P1.PlayerData->subObj.heatTimeCounter != P1.nLastFrameCount &&
		P2.PlayerData->subObj.heatTimeCounter != P2.nLastFrameCount)
	{
		P1.nFirstActiveCounter += *(int*)(adMBAABase + adFrameCount) - nLastFrameCount;
	}
}

void HandleInactive(Player& P)
{
	if (*(int*)(P.adInaction) != 0)
	{
		P.nInactionableMemory = *(int*)(P.adInaction);
	}
}

void BarHandling(Player& P1, Player& P2, Player& P1Assist, Player& P2Assist)
{
	CalculateAdvantage(P1, P2);

	bool DoBarUpdate = (
		*(int*)(P1.adInaction) != 0 ||
		*(int*)(P2.adInaction) != 0 ||
		P1.PlayerData->subObj.animationDataPtr->stateData->flagset2 != 0 ||
		P2.PlayerData->subObj.animationDataPtr->stateData->flagset2 != 0 ||
		P1.bProjectileActive != 0 ||
		P2.bProjectileActive != 0 ||
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
		if (P1.PlayerData->subObj.hitstop != 0 &&
			P2.PlayerData->subObj.hitstop != 0) //Player hitstop values count down but we need it to count up
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

		IncrementFirstActive(P1, P2);
		IncrementFirstActive(P2, P1);

		if (bDisplayFreeze || !bIsFreeze)
		{
			IncrementActive(P1);
			IncrementActive(P2);
			HandleInactive(P1);
			HandleInactive(P2);
			UpdateBars(P1, P1Assist);
			UpdateBars(P2, P2Assist);
			if (P1Assist.PlayerData->exists)
			{
				UpdateBars(P1Assist, P1);
			}
			if (P2Assist.PlayerData->exists)
			{
				UpdateBars(P2Assist, P2);
			}
			nBarCounter += *(int*)(adMBAABase + adTrueFrameCount) - nLastTrueFrameCount;
			if (nBarCounter < 0) {
				nBarCounter = 0;
			}
		}
	}
}

void UpdateFrameBar(Player& P1, Player& P2, Player& P3, Player& P4)
{
	bDisplayFreeze = nSHOW_HITSTOP_AND_FREEZE;
	bDisplayInputs = nSHOW_INPUTS; //currently unused, may add input display to framebar at some point
	nBarScrolling = nTRUE_SCROLL_DISPLAY;

	Main1 = &P1;
	Main2 = &P2;
	Assist1 = &P3;
	Assist2 = &P4;
	if (P1.PlayerData->subObj.tagFlag)
	{
		Main1 = &P3;
		Assist1 = &P1;
	}
	if (P2.PlayerData->subObj.tagFlag)
	{
		Main2 = &P4;
		Assist2 = &P2;
	}

	CheckProjectiles();

	if (*(int*)(adMBAABase + adTrueFrameCount) != nLastTrueFrameCount)
	{
		BarHandling(*Main1, *Main2, *Assist1, *Assist2);
	}

	if (*(int*)(adMBAABase + adTrueFrameCount) == 1)
	{
		ResetBars();
	}

	nPlayerAdvantage = (Main1->nAdvantageCounter - Main2->nAdvantageCounter) % 100;

	UpdatePlayers();

	nLastFrameCount = *(int*)(adMBAABase + adFrameCount);
	nLastTrueFrameCount = *(int*)(adMBAABase + adTrueFrameCount);

}

void DoFrameBar(bool doDraw)
{

	UpdateFrameBar(P1, P2, P3, P4);

	int nBarDrawCounter = 0;

	short sAdjustedScroll = min(min(nBarCounter - frameBar.numCells, BAR_MEMORY_SIZE - frameBar.numCells), -nTRUE_SCROLL_DISPLAY);

	int nForStart = (nBarCounter % BAR_MEMORY_SIZE) - frameBar.numCells - sAdjustedScroll;
	int nForEnd = (nBarCounter % BAR_MEMORY_SIZE) - sAdjustedScroll;
	if (nBarCounter <= frameBar.numCells)
	{
		nForStart = 0;
		nForEnd = nBarCounter;
	}

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
