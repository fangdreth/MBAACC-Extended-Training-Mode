#pragma once
#include <set>
#include <map>

// todo, please move this over to a cpp
#include "DebugInfo.h"

// contains things which modify melty's rendering system internally.

extern bool useCustomShaders;
extern bool useDeerMode;

bool shouldThisBeColored(BYTE charID, DWORD pattern);


// 0x1C4 000111000100
// 0x004			0x040				0x080			0x100
// D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1
typedef struct MeltyVertexOrig {
	float x, y, z;
	float rhw; 
	DWORD diffuse;      
	DWORD specular;     
	float u, v;
} MeltyVertexOrig;

// general (non patching) funcs
DWORD linkedListLength = 0;
float drawY = 0.0f;
void _drawDebugMenu() {

	linkedListLength = 0;

	drawY = 0.0f;

}

// key is mystery texture addr, val is its obj offset
typedef struct LinkedListData {
	bool shouldColor = false;
	bool isDeer = false;
	DWORD object = 0;
	DWORD caller = 0;
} LinkedListData;
std::map<DWORD, LinkedListData> textureToObject;

bool pixelShaderNeedsReset = false;
IDirect3DPixelShader9* pPixelShader_backup = NULL;
IDirect3DPixelShader9* pPixelShader = NULL;
IDirect3DPixelShader9* pCustomShader = NULL;

void loadCustomShader();

bool hasTextureAddr(DWORD test) {
	return textureToObject.find(test) != textureToObject.end();
}

std::set<DWORD> skipTextureAddrs;

void renderModificationsFrameDone();

void describeObject(char* buffer, size_t buflen, const LinkedListData& info) {

	// -1 in this case means "not that type of object"
	int playerIndex = -1;
	int effectIndex = -1; 

	switch (info.object) {
		case 0x00555140:
			playerIndex = 0;
			break;
		case 0x00555C3C:
			playerIndex = 1;
			break;
		case 0x00556738:
			playerIndex = 2;
			break;
		case 0x00557234:
			playerIndex = 3;
			break;
		default:
			break;
	}

	if (playerIndex == -1 && info.object > 0x0067BDE8) { // check if this thing is an effect. remember that they start +0x10 from where they are in cheat table

		if ((info.object - 0x0067BDF8) % 0x33C == 0) {
			effectIndex = (info.object - 0x0067BDF8) / 0x33C;
		}
	}

	if (playerIndex != -1) {

		DWORD pattern = *(DWORD*)(info.object + 0x0);
		DWORD state = *(DWORD*)(info.object + 0x4);

		//snprintf(buffer, buflen, "PLAYER%d\nP:%d\nS:%d", playerIndex, pattern, state);
		//playerDataArr[playerIndex].describe(buffer, buflen);
		//int offset = snprintf(buffer, buflen, "PLAYER%d ");
		playerDataArr[playerIndex].describe(buffer, buflen);
		return;
	}

	if (effectIndex != -1) {

		DWORD pattern = *(DWORD*)(info.object + 0x0);
		DWORD state = *(DWORD*)(info.object + 0x4);

		//snprintf(buffer, buflen, "EFFECT: %d\nP:%d\nS:%d", effectIndex, pattern, state);
		//int offset = snprintf(buffer, buflen, "EFFECT%d ");
		effectDataArr[effectIndex].describe(buffer, buflen);
		return;
	}


	// default we dont know what this is case
	snprintf(buffer, buflen, "???: %08X ", info.object);

}

// actual funcs

DWORD listAppendHook_effectRetAddr = 0;
DWORD listAppendHook_objAddr = 0;
DWORD listAppendHook_texAddr = 0;

DWORD listAppendHook_effectRetAddr_pat = 0;
DWORD listAppendHook_objAddr_pat = 0;

DWORD listAppendHook_hitEffectRetAddr = 0;
DWORD listAppendHook_objAddr_hit = 0;

DWORD listAppendHook_callerAddr = 0;

DWORD _naked_drawCallHook_ebx;
void drawLoopHook() {

	/*
	if (!debugMode) {
		return;
	}
	*/


	// before we setup all the stupid vertex shit, lets figure out what we are actually drawing here
	DWORD lineCol = 0xFFFFFFFF;
	LinkedListData info;

	DWORD unknownTexAddr;
	unknownTexAddr = *(DWORD*)(_naked_drawCallHook_ebx + 0x54);
	unknownTexAddr = *(DWORD*)(unknownTexAddr + 0xC);

	bool isValidTex = false;
	if (isValidTexture(unknownTexAddr)) {
		isValidTex = true;
		//if (mDown) {
		//	saveTexture((IDirect3DBaseTexture9*)unknownTexAddr);
		//}
	}

	if (textureToObject.find(unknownTexAddr) != textureToObject.end()) {
		info = textureToObject[unknownTexAddr];
	}

	bool hasExtraDetail = false;
	static char extraDetail[2048];

	const char* infoString = "NULL";
	switch (info.caller) {
	case 0:
		break;
	default:
		log("unknown ret %08X", info.caller);
		break;
	case 0x00415448:
		infoString = "FUN_004153b0";
		break;
	case 0x00415572:
		infoString = "DrawMenuCursor";
		lineCol = 0xFFbd1a0b;
		break;
	case 0x004159f0:
		infoString = "DrawEffectsTopUIandCharaSelect";
		lineCol = 0xFF00FF00;
		break;
	case 0x00415b8d:
		infoString = "DrawHitEffect";
		lineCol = 0xFFF4AA42;
		break;
	case 0x00415cd6:
		infoString = "DrawBlur";
		break;
	case 0x00415ee5:
		infoString = "DrawCharacterSelectBackground";
		break;
	case 0x0041619f:
		infoString = "DrawCharactersAndBackground";
		lineCol = 0xFF42e5f4;
		hasExtraDetail = true;
		describeObject(extraDetail, 2048, info);
		break;
	case 0x0041621f:
		infoString = "FUN_004161b0";
		break;
	case 0x00416583:
		infoString = "RenderOnScreen";
		break;
	}
	
	/*
	BYTE resourceType = *(BYTE*)(_naked_drawLoopHook_esi + 0x4);

	if (resourceType != 1) {
		return;
	}
	*/

	// 004be293
	DWORD unknown1 = *(DWORD*)(_naked_drawCallHook_ebx + 0x54);

	// 004be296 // vertex data pointer???
	DWORD unknown2 = *(DWORD*)(_naked_drawCallHook_ebx + 0x14);
	// 004be2a4 
	unknown2 = *(DWORD*)(unknown2 + 0x8);

	// FVF vert format?
	DWORD vertFormat = *(DWORD*)(unknown1 + 0x4); // 0x1C4 when,, its a  character? or well just,,, anything being actually drawn? its 0 normally

	// 000111000100
	// 0x004			0x040				0x080			0x100
	// D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1



	DWORD unknown4 = *(DWORD*)(_naked_drawCallHook_ebx + 0x58);

	DWORD PrimitiveType;
	DWORD MinVertexIndex;
	DWORD NumVertices;
	DWORD PrimitiveCount;
	DWORD pIndexData;
	DWORD IndexDataFormat;
	DWORD pVertexStreamZeroData;
	DWORD VertexStreamZeroStride;
	
	PrimitiveType = *(DWORD*)(_naked_drawCallHook_ebx + 0x54);
	PrimitiveType = *(DWORD*)(PrimitiveType);
	
	MinVertexIndex = 0x0;

	NumVertices = *(DWORD*)(_naked_drawCallHook_ebx + 0x14);
	NumVertices = *(DWORD*)(NumVertices);

	PrimitiveCount = *(DWORD*)(_naked_drawCallHook_ebx + 0x14);
	PrimitiveCount = *(DWORD*)(PrimitiveCount + 0x8);

	DWORD PrimitiveCountTestVar;
	PrimitiveCountTestVar = *(DWORD*)(_naked_drawCallHook_ebx + 0x54);
	PrimitiveCountTestVar = *(DWORD*)(PrimitiveCountTestVar);
	
	DWORD PrimitiveCountBackup = PrimitiveCount;

	if (PrimitiveCount != 0) { // replicate code at 004be315
		if (PrimitiveCountTestVar == 2) {
			PrimitiveCount /= 2;
		} else if (PrimitiveCountTestVar == 4) {
			PrimitiveCount /= 3;
		}
	}
	
	pIndexData = *(DWORD*)(_naked_drawCallHook_ebx + 0x14);
	pIndexData = *(DWORD*)(pIndexData + 0xC);
	DWORD* ptrIndexData = (DWORD*)(pIndexData);

	IndexDataFormat = D3DFMT_INDEX32;

	pVertexStreamZeroData = *(DWORD*)(_naked_drawCallHook_ebx + 0x14);
	pVertexStreamZeroData = *(DWORD*)(pVertexStreamZeroData + 0x4);
	MeltyVertexOrig* ptrVertexStreamZeroData = (MeltyVertexOrig*)(pVertexStreamZeroData);

	VertexStreamZeroStride = *(DWORD*)(_naked_drawCallHook_ebx + 0x14);
	VertexStreamZeroStride = *(DWORD*)(pVertexStreamZeroData + 0x10);

	MeltyVert outVerts[4];
	MeltyVertexOrig firstOutVert;

	if (ptrVertexStreamZeroData[ptrIndexData[0]].x > ptrVertexStreamZeroData[ptrIndexData[1]].x) {
		firstOutVert = ptrVertexStreamZeroData[ptrIndexData[1]];
	} else {
		firstOutVert = ptrVertexStreamZeroData[ptrIndexData[0]];
	}

	if (vertFormat != 0x1C4 && vertFormat != 0) {
		//log("vertFormat %08X", vertFormat);
	}

	DWORD col = 0xFFFFFFFF;

	// ugh
	D3DVIEWPORT9 view;

	device->GetViewport(&view);

	DWORD X = view.X;
	DWORD Y = view.Y;
	DWORD Width = view.Width;
	DWORD Height = view.Height;
	float MinZ = view.MinZ;
	float MaxZ = view.MaxZ;

	if (debugMode && vertFormat == 0x1C4 && PrimitiveCountTestVar == 4 && !(Height != 512 || Width != 1024)) {

		if (NumVertices % 4 != 0) {
			//log("ohno");
		}

		// hacky not ideal solution
		if (Height != 512 || Width != 1024) {
			return;
		}

		//log("%6d %5d %5d %5d %5d %7.3f %7.3f", __frameDoneCount, view.X, view.Y, view.Width, view.Height, view.MinZ, view.MaxZ);

		DWORD w = Width - X;
		DWORD h = Height - Y;

		DWORD index = 0;

		/*
		for (int j = 0; j < NumVertices; j += 3) {
			for (int i = 0; i < 3; i++) {

				DWORD tempIndex = ptrIndexData[i + j];
				MeltyVertex v = ptrVertexStreamZeroData[tempIndex];

				outVerts[i].position.x = v.x;
				outVerts[i].position.y = v.y;
				outVerts[i].position.z = v.z;
				outVerts[i].rhw = v.rhw;

				outVerts[i].color = 0x0000FF00 | 0xFF000000;

				outVerts[i].position.x += topLeftPos.x;
				outVerts[i].position.y += topLeftPos.y;

				outVerts[i].position.x *= renderModificationFactor.x;
				outVerts[i].position.y *= renderModificationFactor.y;
			}

			meltyLineData.add(outVerts[0], outVerts[1]);
			meltyLineData.add(outVerts[0], outVerts[2]);
			meltyLineData.add(outVerts[1], outVerts[2]);
		}
		*/

		for (int j = 0; j < PrimitiveCount; j++) { // the fact i didnt use this sooner is abysmal on my part
			for (int i = 0; i < 3; i++) {

				DWORD tempIndex = ptrIndexData[i + (j *3)];
				MeltyVertexOrig v = ptrVertexStreamZeroData[tempIndex];

				outVerts[i].position.x = v.x;
				outVerts[i].position.y = v.y;
				outVerts[i].position.z = v.z;
				outVerts[i].rhw = v.rhw;

				//outVerts[i].color = 0x0000FF00 | 0xFF000000;
				outVerts[i].color = lineCol;

				outVerts[i].position.x += topLeftPos.x;
				outVerts[i].position.y += topLeftPos.y;

				outVerts[i].position.x *= renderModificationFactor.x;
				outVerts[i].position.y *= renderModificationFactor.y;
			}

			meltyLineData.add(outVerts[0], outVerts[1]);
			meltyLineData.add(outVerts[0], outVerts[2]);
			meltyLineData.add(outVerts[1], outVerts[2]);
			
			//meltyLineData.add(outVerts[1], outVerts[2]);
			//meltyLineData.add(outVerts[1], outVerts[3]);
			//meltyLineData.add(outVerts[2], outVerts[3]);

		}

		/*
		MeltyTestVert prev;
		MeltyTestVert curr;


		for (int j = 0; j < NumVertices; j++) {

			curr.position.x = ptrVertexStreamZeroData[ptrIndexData[j]].x;
			curr.position.y = ptrVertexStreamZeroData[ptrIndexData[j]].y;
			curr.position.z = ptrVertexStreamZeroData[ptrIndexData[j]].z;
			curr.rhw = ptrVertexStreamZeroData[ptrIndexData[j]].rhw;
			curr.color = 0x0000FF00 | 0xFF000000;

			curr.position.x += topLeftPos.x;
			curr.position.y += topLeftPos.y;

			curr.position.x *= renderModificationFactor.x;
			curr.position.y *= renderModificationFactor.y;

			// this makes the code cleaner. the optimizer BETTER, or else ill do a funky define
			if (j == 0) {
				prev = curr;
				continue;
			}

			meltyLineData.add(prev, curr);
			prev = curr;

		}

		*/

		if (PrimitiveCountTestVar == 4) {
			col = 0xFF42e5f4;
		} else {
			col = 0xFFFFFF00;
		}

		/*
		D3DMATRIX matrix;
		
		device->GetTransform(D3DTS_VIEW, &matrix);
		log("view:");
		logMatrix(matrix);

		device->GetTransform(D3DTS_PROJECTION, &matrix);
		log("proj:");
		logMatrix(matrix);

		device->GetTransform(D3DTS_WORLD, &matrix);
		log("world:");
		logMatrix(matrix);
		*/

	}

	// VertexStreamZeroStride might not be accurate?
	

	//TextDraw(0, drawY, 6, 0xFFFFFFFF, "%4d %08X %08X %08X %08X %08X", linkedListLength, _naked_drawCallHook_ebx, unknown1, unknown2, unknown3, unknown4);

	

	if (hasExtraDetail && verboseMode) {
		TextDraw(firstOutVert.x, firstOutVert.y, 6, lineCol, extraDetail);
	}

	if (overkillVerboseMode) {
		//TextDraw(0, drawY, 6, col, "%4d %08X %08X %08X %08X %08X %08X %08X %08X %s", linkedListLength, PrimitiveType, NumVertices, PrimitiveCount, pIndexData, pVertexStreamZeroData, VertexStreamZeroStride, unknownTexAddr, info.object, infoString);
		TextDraw(0, drawY, 6, lineCol, "%4d %s%08X %08X %s", linkedListLength, isValidTex ? "TEX" : "???", unknownTexAddr, info.object, infoString);
	}


	drawY += 6.0f;


	linkedListLength++;

}

void listAppendHook() { // for the life of me, why didnt i just not append this thing to the list??? i feel like that would have been better

	if (listAppendHook_effectRetAddr_pat == 0x0045410F) {
		listAppendHook_objAddr = listAppendHook_objAddr_pat;
	}

	if (listAppendHook_hitEffectRetAddr == 0x0042389C) { // i accidentally overwrote all my reversing on this :)
		listAppendHook_objAddr = 0x0061E170 + (listAppendHook_objAddr_hit * 0x60);
	}

	textureToObject.insert({ listAppendHook_texAddr, { false, false, listAppendHook_objAddr, listAppendHook_callerAddr } });

	if (listAppendHook_effectRetAddr == 0x0045410F || listAppendHook_effectRetAddr_pat == 0x0045410F) {

		if (listAppendHook_objAddr >= 0x0067BDE8) { // effect
			
			char source = *(char*)(listAppendHook_objAddr - 8);
			DWORD pattern = *(DWORD*)(listAppendHook_objAddr + 0x0);
			DWORD state = *(DWORD*)(listAppendHook_objAddr + 0x4);

			// what the hell is this doing??? is this doing something??? when did i write this????
			// ok maddy thanks for the shit comments
			// this is the code which prevents the flashing of the meter bar when hud is hidden.
			if (source == -2) {
				if (!shouldDrawHud) {
					// check effects.txt
					switch (pattern) {
					case 100:
					case 101:
					case 102:
					case 104:
					case 105:
					case 106:
					case 107:
					case 108:
					case 300:
					case 301:
					case 302:
					case 304:
					case 305:
					case 306:
					case 307:
					case 308:
						skipTextureAddrs.insert(listAppendHook_texAddr);
						return;
					default:
						return;
					}
				}
				return;
			}

			if (enableEffectColors) {
			
				BYTE owner = *(BYTE*)(listAppendHook_objAddr - 0x10 + 0x02F4);

				BYTE charID;

				if (owner == 0) {
					charID = *(BYTE*)0x555135;
				} else {
					charID = *(BYTE*)0x555C31;
				}

				if (*(DWORD*)(listAppendHook_objAddr - 0x10 + 0x20) != 0x00000101) { // mystery heat detection thingy
					textureToObject[listAppendHook_texAddr].shouldColor = shouldThisBeColored(charID, pattern); // tbh would having a seperate map for things to be colored be ideal.
				}
			}

			if (useDeerMode) {

				EffectData* effect = (EffectData*)(listAppendHook_objAddr - 0x10);

				if (effect->pattern == 103 || (effect->pattern >= 108 && effect->pattern <= 111)) {
					if (playerDataArr[effect->ownerIndex].charID == 10) { // we are nero.
						textureToObject[listAppendHook_texAddr].isDeer = true;
					}
				}
			}

		}
	}
}

DWORD leadToDrawPrimHook_ret = 0;
DWORD drawPrimHook_texAddr = 0;
DWORD skipTextureDraw = 0;
void drawPrimHook() {

	if (leadToDrawPrimHook_ret != 0x004331D9) {
		skipTextureAddrs.clear(); // calling this repeatedly is wasteful!
		textureToObject.clear();
		return;
	}



	// set lookups are trash. there has to be some way of,, getting the index of this texture or something??
	if (skipTextureAddrs.contains(drawPrimHook_texAddr)) {
		skipTextureDraw = 1;
	} else if (textureToObject.contains(drawPrimHook_texAddr)) {
		
		if (textureToObject[drawPrimHook_texAddr].isDeer) {
			device->GetPixelShader(&pPixelShader_backup); // does this inc a refcount?

			pixelShaderNeedsReset = true;
			device->SetPixelShader(pCustomShader);
		} else if (textureToObject[drawPrimHook_texAddr].shouldColor) {
			device->GetPixelShader(&pPixelShader_backup); // does this inc a refcount?

			pixelShaderNeedsReset = true;
			device->SetPixelShader(pPixelShader);
		}
	}

	if (useCustomShaders && !enableEffectColors && !useDeerMode) {
		pixelShaderNeedsReset = true;
		device->SetPixelShader(pCustomShader);
	}
	
}

void drawPrimCallback() {
	if (pixelShaderNeedsReset) {
		pixelShaderNeedsReset = false;
		device->SetPixelShader(pPixelShader_backup);
		pPixelShader_backup = NULL;
	}
}

// naked funcs

__declspec(naked) void _naked_drawPrimCallback() {
	PUSH_ALL;
	drawPrimCallback();
	POP_ALL;
	__asm {
		ret;
	};
}

__declspec(naked) void _naked_drawIndexPrimHook() {
	__asm {
		mov _naked_drawCallHook_ebx, ebx;
	}

	PUSH_ALL;
	drawLoopHook();
	POP_ALL;

	__asm {
		// overwritten asm

		mov eax, dword ptr[ebx + 0014h];
		mov edx, dword ptr[eax + 0010h];

		push 004be474h;
		ret;

	}


}

DWORD _naked_listAppendHook_reg;
__declspec(naked) void _naked_listAppendHook() { // at 0x004c026b

	__asm {
		mov listAppendHook_texAddr, eax;

		mov eax, [esp + 350h];
		mov listAppendHook_objAddr, eax;

		mov eax, [esp + 3E0h];
		mov listAppendHook_effectRetAddr, eax;

		// this testing is for .pat effects
		mov eax, [esp + 470h];
		mov listAppendHook_objAddr_pat, eax;

		mov eax, [esp + 500h];
		mov listAppendHook_effectRetAddr_pat, eax;

		// this is for hit effects, 0042389C
		// i could also use +C0 for 0045a77a drawhiteffectwrapper?
		mov eax, [esp + 30Ch];
		mov listAppendHook_hitEffectRetAddr, eax;
		// as for, how im getting the actual drawn hit. im not sure if its addr is on the stack. im going to just get the loop index
		mov eax, [esp + 1ECh];
		mov listAppendHook_objAddr_hit, eax;
		

		mov eax, listAppendHook_texAddr;
	};

	// sorta smarter approach here
	// checking the direct caller of the func 
	__asm {

		mov _naked_listAppendHook_reg, eax;

		mov eax, [esp + 4h]; // there is one push at the start of the func that needs to be bypassed
		mov listAppendHook_callerAddr, eax;

		mov eax, _naked_listAppendHook_reg;

	}

	// mov ecx, dword ptr [eax]
	__asm _emit 0x8B;
	__asm _emit 0x08;

	// mov edx, dword ptr [ecx + 4]
	__asm _emit 0x8B;
	__asm _emit 0x51;
	__asm _emit 0x04;

	// push eax 
	__asm _emit 0x50;

	PUSH_ALL;
	listAppendHook();
	POP_ALL;

	__asm {
		push 004c0273h;
	};

	// jmp edx
	__asm _emit 0xFF;
	__asm _emit 0xE2;

}

DWORD _naked_drawPrimHook_reg;
DWORD _naked_drawPrimHook_jmp = 0x004be296;
__declspec(naked) void _naked_drawPrimHook() {
	__asm {
		mov _naked_drawPrimHook_reg, eax;

		mov eax, dword ptr[ebx + 54h];
		mov eax, dword ptr[eax + 0Ch];

		mov drawPrimHook_texAddr, eax;

		mov eax, _naked_drawPrimHook_reg;
	};
	PUSH_ALL;
	drawPrimHook();
	POP_ALL;
	__asm {

		cmp skipTextureDraw, 0;
		JE _CONT;

		mov skipTextureDraw, 0;

		// skip this texture
		ret;

	_CONT:
		sub esp, 8h;
		mov ecx, dword ptr[ebx + 54h];
		
		push 004be296h;
		ret;
	}
}

__declspec(naked) void _naked_leadToDrawPrimHook() {
	__asm {
		mov eax, [esp + 8h];
		mov leadToDrawPrimHook_ret, eax;
	}

	__asm _emit 0xA1;
	__asm _emit 0xC8;
	__asm _emit 0xE7;
	__asm _emit 0x76;
	__asm _emit 0x00;

	__asm {
		push 004c0385h;
		ret;
	}

}

// init 

void initDrawIndexPrimHook();

void initEffectSelector();

bool initRenderModifications();

