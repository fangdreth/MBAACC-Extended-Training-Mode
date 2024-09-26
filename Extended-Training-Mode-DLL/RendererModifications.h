#pragma once

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

// init 

void initDrawCallHook() {
	patchJump(0x004c03df, _naked_drawCallHook);
}

void initDrawIndexPrimHook() {
	patchJump(0x004be46e, _naked_drawIndexPrimHook);
}

bool initRenderModifications() {

	
	
	
	//initDrawIndexPrimHook();




	return true;
}