
#include "DebugInfo.h"
#include "DirectX.h"
#include <string>
#include "dllmain.h"

extern bool hasTextureAddr(DWORD test);

// for reasons unknown to all above and below, this shit wont work with bools
int verboseShowPlayers = 0; // show player stuff
int verboseShowEffects = 0; // show effect stuff
int verboseShowUnknown = 0; // show styuff if i dont know what it is

int verboseShowPatternState = 0;

void EffectData::describe(char* buffer, int bufLen) {

	DWORD offset = ((DWORD)&exists);
	int index = -1;
	const char* entityString = "P";
	if (offset >= 0x0067BDE8) {
		index = (offset - 0x0067BDE8) / 0x33C;
		entityString = "E";
	} else {
		index = (offset - 0x00555130) / 0xAFC;
	}

	int bufferOffset = 0;
	//bufferOffset = snprintf(buffer, bufLen, "%s%d P%d S%d\n(%d,%d)\nUNTCH%d\n", entityString, index, subObj.pattern, subObj.state, subObj.xPos, subObj.yPos, subObj.totalUntechTime);

	bufferOffset += snprintf(buffer + bufferOffset, bufLen - bufferOffset, "%s%d\n", entityString, index);

	if (verboseShowPatternState) {
		bufferOffset += snprintf(buffer + bufferOffset, bufLen - bufferOffset, "P%d S%d\n", subObj.pattern, subObj.state);
	}

	if (subObj.attackDataPtr != NULL) {
		bufferOffset += snprintf(buffer + bufferOffset, bufLen - bufferOffset, "DMG%d\nPROR%d", subObj.attackDataPtr->damage, subObj.attackDataPtr->proration);
	}
	 
}

PatternData* EffectData::getPatternDataPtr(int p) {
	// doing this in a more normal way could never get me the results i wanted
	__try {
		HA6Data* ha6 = playerDataArr[0].subObj.someDataPtr->ha6DataPtr;
		if (!ha6) return 0;
		ArrayContainer<PatternData*>* patCont = ha6->patternContainer;
		if (!patCont || patCont->count < p) return 0;
		PatternData* pat = (patCont->array)[p];
		if (!pat) return 0;
		return pat;
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		return NULL;
	}
}

AnimationData* EffectData::getAnimationDataPtr(int p, int s) { 
	__try {
		PatternData* pattern = getPatternDataPtr(p);
		//DWORD temp = (DWORD)pattern->ptrToAnimationDataArr->animationDataArr;
		//return (AnimationData*)(temp + (0x54 * s));
		return &(pattern->animationDataContainer->array[s]);
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		return NULL;
	}
}

// -----

PlayerData* playerDataArr = (PlayerData*)(0x00555130);
EffectData* effectDataArr = (EffectData*)(0x0067BDE8);

bool shouldDisplayDebugInfo = false;
void displayDebugInfo() {

	if (!shouldDisplayDebugInfo) {
		return;
	}

	const int bufferLen = 2048;
	char buffer[bufferLen];

	int xIndex = 100;
	int xDif = 75;

	for (int i = 0; i < 4; i++) {
		if (playerDataArr[i].exists) {
			playerDataArr[i].describe(buffer, bufferLen);

			TextDraw(xIndex, 0, 6, 0xFFFFFFFF, buffer);
			xIndex += xDif;
		}
	}

	for (int i = 0; i < 50; i++) {
		if (effectDataArr[i].exists) {
			effectDataArr[i].describe(buffer, bufferLen);

			TextDraw(xIndex, 0, 6, 0xFFFFFFFF, buffer);
			xIndex += xDif;
		}
	}

	//DWORD testPatternDataPtr = (DWORD)playerDataArr[0].getPatternDataPtr(playerDataArr[0].pattern);
	//TextDraw(100, 200, 8, 0xFF00FF00, "pattern: %08X %08X", (DWORD)playerDataArr[0].patternDataPtr, testPatternDataPtr);

	//DWORD testAnimationDataPtr = (DWORD)playerDataArr[0].getAnimationDataPtr(playerDataArr[0].pattern, playerDataArr[0].state);
	//TextDraw(100, 216, 8, 0xFF00FF00, "state:   %08X %08X", (DWORD)playerDataArr[0].animationDataPtr, testAnimationDataPtr);


	static KeyState cKey('C');
	if (cKey.keyDown()) {
		char buffer[128];
		int bufLoc = 0;
		for (int i = 0; i < 32; i++) {
			bufLoc += snprintf(buffer + bufLoc, 128 - bufLoc, "%02X\n", (BYTE)playerDataArr[0].subObj.patternDataPtr->patternName[i]);

			if (i > 0 && (BYTE)playerDataArr[0].subObj.patternDataPtr->patternName[i] == 0 && (BYTE)playerDataArr[0].subObj.patternDataPtr->patternName[i - 1] == 0) {
				break;
			}
		}

		writeClipboard(std::string(buffer));
	}

}

#pragma pack(push,1)
typedef struct LinkedListTest {
	LinkedListTest* nextLink;
	DWORD unknown1;
	DWORD unknown2;
	DWORD unknown3;
} LinkedListTest;
#pragma pack(pop)

static_assert(sizeof(LinkedListTest) == 0x10, "LinkedListTest should have size 0x10");

#pragma pack(push,1)
typedef struct UnknownMallocData { 
	union {
		struct { // i would really like to find the sprite index in here!
			UNUSED(4);
			BYTE type; // 004c0231
			UNUSED(3);
			UNUSED(0x80);
			IDirect3DTexture9* texture; // huge. actually so huge
			UNUSED(8);
		};
		DWORD allData[0x94 / 4];
	};
};
#pragma pack(pop)

#define CHECKOFFSET(v, n) static_assert(offsetof(UnknownMallocData, v) == n, "UnknownMallocData offset incorrect for " #v);

CHECKOFFSET(type, 4);
CHECKOFFSET(texture, 0x88);

static_assert(sizeof(UnknownMallocData) == 0x94, "UnknownMallocData should have size 0x94");

#undef CHECKOFFSET

bool describePointer(char* buffer, int bufferLen, int* bufferOffset, const char* name, DWORD ptr, int depth = 0) {
	if (!isAddrValid(ptr)) {
		return false;
	}

	if (*bufferOffset >= bufferLen) {
		return false;
	}

	DWORD val = *(DWORD*)(ptr);
	int res = snprintf(buffer + *bufferOffset, bufferLen - *bufferOffset, "%s%d: %08X -> %08X ", name, depth, ptr, val);

	if (res < 0) {
		return false;
	}

	*bufferOffset += res;

	describePointer(buffer, bufferLen, bufferOffset, name, val, depth + 1);
	describePointer(buffer, bufferLen, bufferOffset, name, val+4, depth + 1);

	return true;
}

void displayLinkedList(int y, LinkedListTest* l, int depth = 0) {

	if (y > 420) {
		return;
	}

	if (l == NULL) {
		TextDraw(250, y, 4, 0xFF00FF00, "%d NULL", depth);
		return;
	}
	
	/*
	
	const int bufferLen = 1024;
	static char buffer[bufferLen];
	int bufferOffset = 0;

	bool res1 = describePointer(buffer, bufferLen, &bufferOffset, "UNK1", (DWORD)l->unknown1);
	bool res2 = describePointer(buffer, bufferLen, &bufferOffset, "UNK2", (DWORD)l->unknown2);
	bool res3 = describePointer(buffer, bufferLen, &bufferOffset, "UNK3", (DWORD)l->unknown3);

	if (res1 || res2 || res3) {

		TextDraw(250, y, 4, 0xFF00FF00, "%d %08X", depth, (DWORD)l->nextLink);
		buffer[bufferLen - 1] = '\0';
		TextDraw(250 + 45, y, 4, 0xFFFF00FF, buffer);
		y += 4;
	}

	displayLinkedList(y, l->nextLink, depth + 1);

	*/

	


};

bool isValidTexture(DWORD addr) {
	if (addr == 0) {
		return false;
	}
	__try {
		IDirect3DTexture9* tex = (IDirect3DTexture9*)addr;
		D3DRESOURCETYPE type = tex->GetType();
		return type != 0; // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dresourcetype. should i maybe convert this to... check for texture specifically?
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		return false;
	}
}

bool isValidSurface(DWORD addr) {
	if (addr == 0) {
		return false;
	}
	__try {
		IDirect3DSurface9* surf = (IDirect3DSurface9*)addr;
		D3DRESOURCETYPE type = surf->GetType();
		return type != 0;
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		return false;
	}
}

bool isValidResource(DWORD addr) {
	if (addr == 0) {
		return false;
	}
	__try {
		IDirect3DResource9* tex = (IDirect3DResource9*)addr;
		D3DRESOURCETYPE type = tex->GetType();
		return type != 0;
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		return false;
	}
}

bool shouldDisplayLinkedListInfo = false;
void debugLinkedList() {

	if (!showDebugMenu) {
		return;
	}

	if (!shouldDisplayLinkedListInfo) {
		return;
	}

	DWORD list = *(DWORD*)0x005550a8;
	DWORD listLength = *(DWORD*)0x005550ac;

	// eax of 004c0220 is multed by 0x10 and indexed into the base list
	// 004162c8 has a 0x94 malloc
	// that func returns a pointer to that malloc, which is most likely the stuff which stores the draw data? it is in ESI when passed to 004c0220
	// if i had a brain, i wouldnot include the object addr in the rendering pipeline(i really wish/hope they did) 
	// sprite index though? totally in there somewhere
	// ok a thought occurs. 
	// im saving this after the renderer pipeline is setup, but before its actually rendered.
	// and tbh, im getting complete shit out of this.
	// is it possible that textures are changed/altered during the actual rendering? (hell thats likely)
	// new idea: call this func AFTER rendering has occured. maybe. make sure the linked list isnt reset at the end of callsImportantDraw1
	// i could also increase the 0x94 malloc to 0x98 and put the address in those extrabytes? but i dont think that,,

	float font = 8;
	DWORD color = 0xFFFF00FF;

	const int buflen = 1024;
	char buffer[buflen];
	int bufOffset = 0;

	static int startIndex = 0;
	static int xOffset = 0;

	if (iDown) {
		startIndex -= 40;
	} else if (kDown) {
		startIndex += 40;
	}

	if (jDown) {
		xOffset -= 1;
	} else if (lDown) {
		xOffset += 1;
	}

	static int writeToDisk = 0;
	static bool toggle = 0;
	bool trigWrite = false;
	
	/*
	//if (((lDown && !toggle) || (jDown && toggle)) && writeToDisk == 0) {
	if(writeToDisk == 0 && mDown) {
		toggle = !toggle;
		writeToDisk += 100;
		trigWrite = true;
	}

	if (writeToDisk > 0) {
		writeToDisk--;
	} 
	*/
	

	startIndex = SAFEMOD(startIndex, 1600);
	xOffset = SAFEMOD(xOffset, 0x94 / 4);

	//TextDraw(0, 0, 16, 0xFF00FF00, "%d %02X", startIndex, xOffset);
	
	static std::set<DWORD> textureAddrs;
	bufOffset += snprintf(buffer, buflen, "%4d %2d %7d ", startIndex, xOffset, textureAddrs.size());
	textureAddrs.clear();

	for (int dataIndex = xOffset; dataIndex < 0x94 / 4; dataIndex++) {
		bufOffset += snprintf(buffer + bufOffset, buflen - bufOffset, "%08X ", dataIndex * 4);
	}

	TextDraw(0, 100 - font, font, 0xFF00FF00, buffer);

	int y = 100;
	for (int i = startIndex; i < listLength; i++) {

		/*if (y > 460) {
			break;
		}*/

		//DWORD elementAddr = (list + (i * 0x10));
		//DWORD EDI = elementAddr;
		
		// this element is now in edi, check 004c0273 for what happens to it.
		// edi is the pointer to the list element
		// *esi = *edi

		
		DWORD* in_EAX = (DWORD*)0x005550a8;
		int* piVar1 = (int*)in_EAX[2];
		//DWORD uVar2 = *(DWORD*)0x005550c0; // this is a indexer, incs on each func call. its max is the 1600 length. 

		DWORD uVar2 = i;
		DWORD* res = *(DWORD**)(piVar1[1] + uVar2 * 4);
		
		UnknownMallocData* data = (UnknownMallocData*)res;

		BYTE type = data->type;
		// this was actually filtering out things???
		//if (!type) {
		//	continue;
		//}

		bufOffset = 0;

		//bufOffset += snprintf(buffer, buflen, "%4d ", i);

		float x = 11.5f * font;
		for (int dataIndex = xOffset; dataIndex < 0x94 / 4; dataIndex++) {
			//bufOffset += snprintf(buffer + bufOffset, buflen - bufOffset, "%08X ", data->allData[dataIndex]);
			DWORD val = data->allData[dataIndex];
			color = 0xFFFF00FF;
			if (dataIndex * 4 == 0x88 && isValidTexture(val)) {
			//if (isValidResource(val)) {
				
				if (trigWrite && textureAddrs.find(val) == textureAddrs.end()) {
					//saveTexture((IDirect3DBaseTexture9*)val, i);
				}
				textureAddrs.insert(val);
				color = 0xFF00FF00;
			}

			// warc standing is in the range 1385 - 1408

			DWORD testVal = val;

			if (testVal >= 1385 * 4 && testVal <= 1408 * 4) {
				color = 0xFF0000FF;
			}
			
			if (x > 700) {
				continue;
			}

			TextDraw(x, y, font, color, "%08X", val);
			x += 6.5f * font;
			//snprintf(buffer, buflen, "%08X", data->enabled);

			
		}

		color = 0xFFFF00FF;

		int ESIOffset = -1;
		const char* typeString = "???";
		switch (type) {
		case 1:
		case 3:
			typeString = "Surf/Text";
			ESIOffset = 0x22 * 4;
			break;
		case 2:
			typeString = "Volume";
			break;
		case 4:
			typeString = "VolText";
			break;
		case 5:
		case 7:
			typeString = "cubeTx/ind";
			break;
		case 6:
			typeString = "vertexBuf";
			break;
		default:
			break;
		}

		/*
		if (ESIOffset == -1) {
			strcpy_s(buffer, "how");
		} else {
			DWORD* idek = (DWORD*)data->allData[ESIOffset / 4];
			
			snprintf(buffer, buflen, "%08X", (DWORD)idek);

			if (hasTextureAddr((DWORD)idek)) {
				color = 0xFFFF0000;
			}

			/*if (idek == NULL) {
				strcpy_s(buffer, "NULL");
			} else {
				bufOffset = 0;
				for (int dataIndex = 0; dataIndex < 0x10 / 4; dataIndex++) {
					bufOffset += snprintf(buffer + bufOffset, buflen - bufOffset, "%08X ", idek[dataIndex]);
					//snprintf(buffer, buflen, "%08X", data->enabled);
				}
			}
			
		}
		*/
		
		//int font = 5;
		if (y > 460) {
			continue;
		}


		//TextDraw(250 + (55 * (i / (480 / font))), (i * font) % 480, font, 0xFFFF00FF, "%08X", res);
		buffer[0] = '\0';
		TextDraw(0, y, font, color, "%4d %10s %s", i, typeString, buffer);

		y += font;
		
	}


	//TextDraw(250, 0, 4, 0xFF00FF00, "%d", listLength);

}

void saveTexture(IDirect3DBaseTexture9* pTex, int i){ // does this inc the refcounter??

	if (pTex == NULL) {
		log("saveTexture tex was NULL! ret");
		return;
	}

	volatile DWORD tempLevelCount = 0;  // i need to prevent this from being optimized out, possibly
	__try {
		tempLevelCount = pTex->GetLevelCount();
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		log("saveTexture tex was invalid!");
		return;
	}

	static unsigned imageCounter = 0;

	const char* writePath = "./temp/";

	static char fileName[256];
	if (i == -1) {
		snprintf(fileName, 256, "%s%7d.png", writePath, imageCounter);
	} else {
		snprintf(fileName, 256, "%s%7d.png", writePath, i);
	}
	
	D3DXSaveTextureToFileA(fileName, D3DXIFF_PNG, pTex, NULL);

	imageCounter++;
}

#pragma pack(push,1)
typedef struct UnknownDrawData { // ghidra only lets you rerun autocreatestructure if you right click the variable NAME not type. omfg
	// this struct and its size are mostly guesses from ghidra, assume everything is incorrect
	// highkey, what do i need? i need a method of getting WHATS being drawn from this struct. nothing else. that saves me that 
	// map, its lookups, and is everything.
	union {
		struct {
			DWORD unknown1;
			DWORD unknown2;
			UNUSED(12);
			DWORD* unknown3;	
			UNUSED(60);
			struct UnknownHasTexture {
				UNUSED(12);
				IDirect3DTexture9* tex; // this is the [0x54] + 0xC i was getting addresses from 
			} *unknownHasTexture; // i could also name this UnknownHasTexture. 
			//UnknownHasTexture* unknownHasTexture; 
			DWORD* unknown4;
			DWORD unknown5;
			DWORD unknown6;
			DWORD unknown7;
			IDirect3DTexture9* tex;
			BYTE unknown9;
			BYTE unknown10;
			short unknown11;
			short unknown12;
		};
		struct {
			DWORD dwordData[0x6C / 4];
			UNUSED(6);
		};
		BYTE data[0x72];
	};
} UnknownDrawData;
#pragma pack(pop)

#define CHECKOFFSET(v, n) static_assert(offsetof(UnknownDrawData, v) == n, "UnknownDrawData offset incorrect for " #v);

CHECKOFFSET(unknownHasTexture, 0x54);
CHECKOFFSET(tex, 0x68);
CHECKOFFSET(unknown9, 0x6C);

static_assert(sizeof(UnknownDrawData) == 0x72, "UnknownDrawData should have size 0x72");

#undef CHECKOFFSET

bool shouldDebugImportantDraw = false;
void debugImportantDraw() {

	if (!shouldDebugImportantDraw) {
		return;
	}
	
	int bufOffset = 0;
	const int bufLen = 1024;
	char buffer[bufLen];

	auto tempPrint = [&buffer, &bufLen, &bufOffset](DWORD val) mutable -> bool {

		bool isValid = isValidResource(val);

		if (isValid && mDown) {
			saveTexture((IDirect3DBaseTexture9*)val);
		}

		bufOffset += snprintf(buffer + bufOffset, bufLen - bufOffset, "%s %08X\n", isValid ? "VAL" : "???", val);

		return isValid;
	};

	// when calling this right after callsImportantDraw1(), printebxindex 0 contains the frame. pretty funny 

	float font = 12.0f;
	float y = 20.0f;
	for (int printDrawEBXIndex = 0; printDrawEBXIndex < 3; printDrawEBXIndex++) {
		
		UnknownDrawData* drawData = (UnknownDrawData*)*(DWORD*)((printDrawEBXIndex * 4) + 0x00767430);

		for (int i = 0; i < 0x6C / 4; i++) {
			bufOffset = 0;
			bufOffset += snprintf(buffer + bufOffset, bufLen - bufOffset, "%02X ", i * 4);
			bool isValid = tempPrint(drawData->dwordData[i]);
			TextDraw(0, y, font, isValid ? 0xFF00FF00 : 0xFFFF00FF, buffer);
			y += font;
		}

		if (isAddrValid((DWORD)drawData->unknownHasTexture)) {
			bufOffset = 0;
			bufOffset += snprintf(buffer + bufOffset, bufLen - bufOffset, "   ");
			bool isValid = tempPrint((DWORD)drawData->unknownHasTexture->tex);
			TextDraw(0, y, font, isValid ? 0xFF00FF00 : 0xFFFF00FF, buffer);
			y += font;
		}
		

		break;
	}


}

int getComboCount() {
	// go read 00478c38
	// this only gets the combo count for p1, and doesnt reset properly without more code

	if (playerDataArr[1].subObj.notInCombo) {
		return 0;
	}

	DWORD iVar2 = *(BYTE*)(0x0055df0f);
	iVar2 *= 0x20C;
	int iVar1 = *(int*)(0x00557e20 + iVar2);

	DWORD res = *(DWORD*)(0x00557e28 + iVar1 * 0x18 + iVar2 + 4);

	return res;
}

bool useWind = false;
int xWindVel = 0;
int changeWindDir = 0;
void setWind() {

	if (!useWind) {
		return;
	}


	static bool invertDir = false;
	static int count = 0;
	
	static int what = 0;
	if (changeWindDir == 0) {
		invertDir = false;
	} else {
		count++;
		if (count > changeWindDir) {
			invertDir = !invertDir;
			count = 0;
		}
	}

	for (int i = 0; i < 4; i++) {
		if (playerDataArr[i].exists) {
			playerDataArr[i].subObj.xVelChange = invertDir ? -xWindVel : xWindVel;
		}
	}

}
