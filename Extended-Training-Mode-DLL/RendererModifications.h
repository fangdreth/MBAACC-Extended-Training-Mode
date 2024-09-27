#pragma once
#include <set>

// contains things which modify melty's rendering system internally.

// 0x1C4 000111000100
// 0x004			0x040				0x080			0x100
// D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1
typedef struct MeltyVertex {
	float x, y, z, rhw; 
	DWORD diffuse;      
	DWORD specular;     
	float u, v;
} MeltyVertex;

// general (non patching) funcs
DWORD linkedListLength = 0;
float drawY = 0.0f;
void _drawDebugMenu() {

	linkedListLength = 0;

	drawY = 0.0f;

}


std::set<DWORD> textureAddrs;

void renderModificationsFrameDone() {
	//textureAddrs.clear();
}

// actual funcs

DWORD _naked_drawCallHook_eax;
DWORD _naked_drawCallHook_ebx;
DWORD _naked_drawCallHook_ecx;
DWORD _naked_drawCallHook_edx;
void drawLoopHook() {
	
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
	MeltyVertex* ptrVertexStreamZeroData = (MeltyVertex*)(pVertexStreamZeroData);

	VertexStreamZeroStride = *(DWORD*)(_naked_drawCallHook_ebx + 0x14);
	VertexStreamZeroStride = *(DWORD*)(pVertexStreamZeroData + 0x10);

	MeltyTestVert outVerts[4];

	if (vertFormat != 0x1C4 && vertFormat != 0) {
		log("vertFormat %08X", vertFormat);
	}

	if(vertFormat == 0x1C4 && PrimitiveCountTestVar == 4) {

		if (NumVertices % 4 != 0) {
			log("ohno");
		}

		DWORD index = 0;

		for (int j = 0; j < NumVertices; j += 3) {
			for (int i = 0; i < 3; i++) {


				DWORD tempIndex = ptrIndexData[i + j];
				MeltyVertex v = ptrVertexStreamZeroData[tempIndex];

				outVerts[i].x = v.x;
				outVerts[i].y = v.y;
				outVerts[i].z = v.z;
				outVerts[i].rhw = v.rhw;

				outVerts[i].color = 0x3000FF00;

			}

			meltyTestVertData.add(outVerts[0], outVerts[1], outVerts[2]);
			//meltyTestVertData.add(outVerts[1], outVerts[2], outVerts[3]);
		}

		

		
		
		//*(DWORD*)(*(DWORD*)(_naked_drawCallHook_ebx + 0x14)) = 3;
		//*(DWORD*)(*(DWORD*)(_naked_drawCallHook_ebx + 0x14)) = 3;

	}

	// VertexStreamZeroStride might not be accurate?
	

	//TextDraw(0, drawY, 6, 0xFFFFFFFF, "%4d %08X %08X %08X %08X %08X", linkedListLength, _naked_drawCallHook_ebx, unknown1, unknown2, unknown3, unknown4);

	TextDraw(0, drawY, 6, 0xFFFFFFFF, "%4d %08X %08X %08X %08X %08X %08X", linkedListLength, PrimitiveType, NumVertices, PrimitiveCount, pIndexData, pVertexStreamZeroData, VertexStreamZeroStride);

	drawY += 6.0f;



	linkedListLength++;

}

DWORD listAppendHook_effectRetAddr = 0;
DWORD listAppendHook_objAddr = 0;
DWORD listAppendHook_texAddr = 0;

DWORD listAppendHook_effectRetAddr_pat = 0;
DWORD listAppendHook_objAddr_pat = 0;
void listAppendHook() { // for the life of me, why didnt i just not append this thing to the list??? i feel like that would have been better

	if (listAppendHook_effectRetAddr == 0x0045410F || listAppendHook_effectRetAddr_pat == 0x0045410F) {

		if (listAppendHook_effectRetAddr_pat == 0x0045410F) {
			listAppendHook_objAddr = listAppendHook_objAddr_pat;
		}

		if (listAppendHook_objAddr >= 0x0067BDE8) { // effect
			
			char source = *(char*)(listAppendHook_objAddr - 8);
			DWORD pattern = *(DWORD*)(listAppendHook_objAddr + 0x0);
			DWORD state = *(DWORD*)(listAppendHook_objAddr + 0x4);

			if (source == -2) {
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
						break;
					default:
						return;
				}
			} else {
				return;
			}

			textureAddrs.insert(listAppendHook_texAddr);
		}
	}
}

DWORD leadToDrawPrimHook_ret = 0;
DWORD drawPrimHook_texAddr = 0;
DWORD skipTextureDraw = 0;
void drawPrimHook() {

	if (leadToDrawPrimHook_ret != 0x004331D9) {
		textureAddrs.clear(); // calling this repeatedly is wasteful!
		return;
	}

	if (textureAddrs.contains(drawPrimHook_texAddr)) {
		skipTextureDraw = 1;
	}
}


// naked funcs

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

DWORD _naked_drawCallHook_Func = 0x004be290;
__declspec(naked) void _naked_drawCallHook() {

	__asm {
		mov _naked_drawCallHook_eax, eax;
		mov _naked_drawCallHook_ebx, ebx;
		mov _naked_drawCallHook_ecx, ecx;
		mov _naked_drawCallHook_edx, edx;
	}

	PUSH_ALL;
	drawLoopHook();
	POP_ALL;

	__asm {
		call[_naked_drawCallHook_Func];
	}

	__asm {
		push 004c03e4h;
		ret;
	}

}

__declspec(naked) void _naked_listAppendHook() {

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

		mov eax, listAppendHook_texAddr;
	};

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

void initDrawCallHook() {
	patchJump(0x004c03df, _naked_drawCallHook);
}

void initDrawIndexPrimHook() {
	patchJump(0x004be46e, _naked_drawIndexPrimHook);
}

void initEffectSelector() {

	patchJump(0x004c026b, _naked_listAppendHook);
	patchJump(0x004be290, _naked_drawPrimHook);
	patchJump(0x004c0380, _naked_leadToDrawPrimHook);

}

bool initRenderModifications() {

	
	
	
	//initDrawIndexPrimHook();
	initEffectSelector();



	return true;
}