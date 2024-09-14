#pragma once

class Texture {
public:

	// maintain a list of all loaded textures for memory management.
	static std::vector<Texture*> _loadedTextures;

	Texture(const std::string& fileName) {
		// should i load from file? or would it be better to bake the pngs in? 
		// we would have to ship with the textures. im not sure



		_loadedTextures.push_back(this);
	}


	static void _cleanForReset() {


	}
	

private:

	bool inited = false; 

};

enum class DrawType {
	Line,
	Rect,
};

typedef struct DrawCallInfo {
	DrawType drawType;
	float v1;
	float v2;
	float v3;
	float v4;
	DWORD ARGB = 0x8042e5f4;
} DrawCallInfo;

// would a queue be better here?
std::vector<DrawCallInfo> drawCalls;

bool _hasStateToRestore = false;
IDirect3DPixelShader9* _pixelShaderBackup;
IDirect3DVertexShader9* _vertexShaderBackup;
IDirect3DBaseTexture9* _textureBackup;
DWORD _D3DRS_BLENDOP;
DWORD _D3DRS_ALPHABLENDENABLE;
DWORD _D3DRS_SRCBLEND;
DWORD _D3DRS_DESTBLEND;
DWORD _D3DRS_SEPARATEALPHABLENDENABLE;
DWORD _D3DRS_SRCBLENDALPHA;
DWORD _D3DRS_DESTBLENDALPHA;

float vWidth = 640;
float vHeight = 480;
float wWidth = 640;
float wHeight = 480;
bool isWide = false;
float factor = 1.0f;

void scaleVertex(D3DVECTOR& v) {	
	if (isWide) {
		v.x /= factor;
	} else {
		v.y /= factor;
	}
}

void backupRenderState() {

	if (_hasStateToRestore) {
		return;
	}

	_hasStateToRestore = true;

	// store state
	device->GetPixelShader(&_pixelShaderBackup);
	device->GetVertexShader(&_vertexShaderBackup);
	device->GetTexture(0, &_textureBackup);

	//device->GetRenderState(D3DRS_BLENDOP, &_D3DRS_BLENDOP);
	//device->GetRenderState(D3DRS_ALPHABLENDENABLE, &_D3DRS_ALPHABLENDENABLE);
	//device->GetRenderState(D3DRS_SRCBLEND, &_D3DRS_SRCBLEND);
	//device->GetRenderState(D3DRS_DESTBLEND, &_D3DRS_DESTBLEND);
	//device->GetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, &_D3DRS_SEPARATEALPHABLENDENABLE);
	//device->GetRenderState(D3DRS_SRCBLENDALPHA, &_D3DRS_SRCBLENDALPHA);
	//device->GetRenderState(D3DRS_DESTBLENDALPHA, &_D3DRS_DESTBLENDALPHA);

	// set state to "normal"
	// these are just directx defaults. i have no idea if they are the best case.
	device->SetPixelShader(NULL);
	device->SetVertexShader(NULL);
	device->SetTexture(0, NULL);

	// this should be done with a matrix transform!
	// D3DXMatrixOrthoLH(
	// D3DXMatrixPerspectiveFovLH
	// device->SetTransform(
	// this works well enough tho

	D3DVIEWPORT9 viewport;
	device->GetViewport(&viewport);
	vWidth = viewport.Width;
	vHeight = viewport.Height;

	HWND hwnd = (HWND)*(DWORD*)(0x0074dfac);

	if (hwnd != NULL) {
		RECT rect;
		if (GetClientRect(hwnd, &rect)) {
			wWidth = rect.right - rect.left;
			wHeight = rect.bottom - rect.top;
		}
	}

	const float ratio = 4.0f / 3.0f;

	isWide = wWidth / wHeight > ratio;

	if (isWide) {
		factor = wWidth / (wHeight * ratio);
	} else {
		factor = wHeight / (wWidth / ratio);
	}

	//device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	//device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	//device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	//device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, false);
	//device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
	//device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ZERO);
	
}
	
void restoreRenderState() {	

	if (!_hasStateToRestore) {
		return;
	}

	_hasStateToRestore = false;

	device->SetPixelShader(_pixelShaderBackup);
	device->SetVertexShader(_vertexShaderBackup);
	
	if (_textureBackup != NULL) {
		_textureBackup->Release();
	}
	
	device->SetTexture(0, _textureBackup);

}

void drawTri(const D3DVECTOR& v1, const D3DVECTOR& v2, const D3DVECTOR& v3, D3DCOLOR col) {
	if (device == NULL) return;

	struct CUSTOMVERTEX {
		D3DVECTOR position;
		D3DCOLOR color;
	};

	// todo, redo drawing and text with this
	// make sure it works fullscreen and not, and scaled fullscreen and not.

	

	constexpr float whatIsThis = 0.5f; // what is this?

	// Define the vertices of the triangle
	CUSTOMVERTEX vertices[] = {
		{ D3DVECTOR(v1.x, v1.y, whatIsThis), col },
		{ D3DVECTOR(v2.x, v2.y, whatIsThis), col },
		{ D3DVECTOR(v3.x, v3.y, whatIsThis), col },
	};

	scaleVertex(vertices[0].position);
	scaleVertex(vertices[1].position);
	scaleVertex(vertices[2].position);
	
	// todo, figure out why colors, dont

	#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)
	
	// Set up the vertex buffer and draw
	// i dislike allocing and unallocing stuff like this.
	LPDIRECT3DVERTEXBUFFER9 v_buffer;
	if (SUCCEEDED(device->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_MANAGED, &v_buffer, NULL))) {
		VOID* pVoid;
		
		v_buffer->Lock(0, 0, (void**)&pVoid, 0);
		memcpy(pVoid, vertices, sizeof(vertices));
		v_buffer->Unlock();

		device->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
		device->SetFVF(D3DFVF_CUSTOMVERTEX);
		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		v_buffer->Release();
	}
}

void drawRect2(float x, float y, float w, float h, DWORD ARGB = 0x8042e5f4) { // top left is 0.0, bottom right is 1.0. 
	
	y = 1 - y;
	
	D3DVECTOR v1 = { ((x + 0) * 2.0) - 1.0, ((y + 0) * 2.0) - 1.0, 0.0f };
	D3DVECTOR v2 = { ((x + w) * 2.0) - 1.0, ((y + 0) * 2.0) - 1.0, 0.0f };
	D3DVECTOR v3 = { ((x + 0) * 2.0) - 1.0, ((y - h) * 2.0) - 1.0, 0.0f };
	D3DVECTOR v4 = { ((x + w) * 2.0) - 1.0, ((y - h) * 2.0) - 1.0, 0.0f };
	
	drawTri(v1, v2, v3, ARGB);
	drawTri(v2, v3, v4, ARGB);
}

void drawLine2(float x1, float y1, float x2, float y2, DWORD ARGB = 0x8042e5f4) { // top left is 0.0, bottom right is 1.0. 
	
	// i am,,, i bit confused on how exactly to do this. 
	// current vibes say,,, two very thin triangles.

	// i would like,,, diag lines please too.
	// there are,,, angle issues with that tho

	y1 = 1 - y1;
	y2 = 1 - y2;

	// this var may need to be set dynamically based on resolution, but for now 0.005 works fine.
	const float lineWidth = 0.005;

	D3DVECTOR v1 = { ((x1 + 0) * 2.0) - 1.0, ((y1 + 0) * 2.0) - 1.0, 0.0f };
	D3DVECTOR v2 = { ((x2 + 0) * 2.0) - 1.0, ((y2 + 0) * 2.0) - 1.0, 0.0f };
	D3DVECTOR v3 = { ((x1 + lineWidth) * 2.0) - 1.0, ((y1 + 0) * 2.0) - 1.0, 0.0f };
	D3DVECTOR v4 = { ((x2 + lineWidth) * 2.0) - 1.0, ((y2 + 0) * 2.0) - 1.0, 0.0f };

	drawTri(v1, v2, v3, ARGB);
	drawTri(v2, v3, v4, ARGB);
}

void _doDrawCalls() {

	backupRenderState();

	device->BeginScene(); // should i start a new scene per call, or is one thing enough

	log("drawCallsLen %d", drawCalls.size());

	for (const DrawCallInfo& drawCallInfo : drawCalls) {

		// there is def a better and more readable way to do this
		switch (drawCallInfo.drawType) {
		case DrawType::Line:
			drawLine2(drawCallInfo.v1, drawCallInfo.v2, drawCallInfo.v3, drawCallInfo.v4, drawCallInfo.ARGB);
			break;
		case DrawType::Rect:
			drawRect2(drawCallInfo.v1, drawCallInfo.v2, drawCallInfo.v3, drawCallInfo.v4, drawCallInfo.ARGB);
			break;
		default:
			log("unknown drawcall type %d", drawCallInfo.drawType);
			break;
		}

	}

	/*
	drawRect2(0.25, 0.25, 0.5, 0.5);
	drawRect2(0.80, 0.80, 0.1, 0.1);
	drawRect2(0.10, 0.10, 0.1, 0.8);

	drawLine2(0.1, 0.1, 0.2, 0.9, 0x80FF0000);
	drawLine2(0.1, 0.1, 0.9, 0.9, 0x80FF0000); 
	*/

	device->EndScene();

	restoreRenderState();

	drawCalls.clear();
}

// -----

__declspec(naked) void _naked_PresentHook() {
	// just in case, im only hooking this one present call
	PUSH_ALL;
	_doDrawCalls();
	POP_ALL;

	__asm {
		push 00000000h;
		push 00000000h;
		push 00000000h;
		push 00000000h;
		push eax;
		call edx;

		push 004bdd16h;
		ret;
	}
}

void cleanForDirectXReset() {
	// all textures need to be reset here this occurs during the transition to and from fullscreen
	// i should make a class to manage all of them, make it easier for ppl
	Texture::_cleanForReset();
}

DWORD _RESET_HOOKS = 0;
DWORD _DirectX_Reset_Func_ret;
DWORD _DirectX_Reset_Func_Addr = 0;
__declspec(naked) void _DirectX_Reset_Func() {
	PUSH_ALL;
	log("DIRECTX RESET CALLED, CLEANING");
	cleanForDirectXReset();
	POP_ALL;
	__asm {
		pop _DirectX_Reset_Func_ret;
		//jmp[_DirectX_Reset_Func_Addr];
		call[_DirectX_Reset_Func_Addr];
	}

	__asm {
		push _DirectX_Reset_Func_ret;
		ret;
	}
}

DWORD _DirectX_BeginStateBlock_Func_Addr = 0;
__declspec(naked) void _DirectX_BeginStateBlock_Func() {
	PUSH_ALL;
	log("beginstateblockfunc called");
	POP_ALL;
	__asm {
		mov _RESET_HOOKS, 1;
		jmp[_DirectX_BeginStateBlock_Func_Addr];
	}
}

DWORD _DirectX_EvictManagedResources_Func_Addr = 0;
__declspec(naked) void _DirectX_EvictManagedResources_Func() {
	PUSH_ALL;
	log("_DirectX_EvictManagedResources_Func called, cleaning up memory");
	cleanForDirectXReset();
	POP_ALL;
	__asm {
		jmp[_DirectX_EvictManagedResources_Func_Addr];
	}
}

DWORD _DirectX_Present_Func_Addr = 0;
__declspec(naked) void _DirectX_Present_Func() {
	// IMPORTANT. i am unsure if the hooked areas where ppl would be adding draw calls here,, 
	// some hooks for certain things might be after present. if so, things would get drawn a frame late. 
	// figure it out.
	PUSH_ALL;
	log("IDirect3DDevice9_Present called!");
	_doDrawCalls();
	POP_ALL;
	__asm {
		jmp[_DirectX_Present_Func_Addr];
	}
}

__declspec(naked) void _naked_InitDirectXHooks() {

	PUSH_ALL;
	log("hooking directx");
	POP_ALL;

	__asm {

		push eax;
		push ecx;
		push edx;

		// hook directx reset, we need to free all alloced textures when this is called
		mov eax, [device];
		mov ecx, [eax];
		mov edx, [ecx + 00000040h];
		mov _DirectX_Reset_Func_Addr, edx;
		mov edx, _DirectX_Reset_Func;
		mov[ecx + 00000040h], edx;

		// hook beginstate block, as it overwrites my hooks.
		mov eax, [device];
		mov ecx, [eax];
		mov edx, [ecx + 000000F0h];
		mov _DirectX_BeginStateBlock_Func_Addr, edx;
		mov edx, _DirectX_BeginStateBlock_Func;
		mov[ecx + 000000F0h], edx;
		
		// hook present, so we can draw on the screen
		//mov eax, [device];
		//mov ecx, [eax];
		//mov edx, [ecx + 00000044h];
		//mov _DirectX_Present_Func_Addr, edx;
		//mov edx, _DirectX_Present_Func;
		//mov[ecx + 00000044h], edx;

		// hook evict, so i can manage memory properly for once
		mov eax, [device];
		mov ecx, [eax];
		mov edx, [ecx + 00000014h];
		mov _DirectX_EvictManagedResources_Func_Addr, edx;
		mov edx, _DirectX_EvictManagedResources_Func;
		mov[ecx + 00000014h], edx;

		pop edx;
		pop ecx;
		pop eax;

		ret;
	}
}

_declspec(naked) void _naked_RehookDirectX() {
	// this appears to reset the state issues as fast as possible, more things flicker (the meter bars) now
	__asm {

		cmp _RESET_HOOKS, 1;
		JNE _OK;
		mov _RESET_HOOKS, 0;

		call _naked_InitDirectXHooks;

	_OK:

		test eax, eax;
		JZ LAB_004be3d2;

		mov dword ptr[esp + 10h], 00000000h;

		push 004be3e3h;
		ret;

	LAB_004be3d2:
		push 004be3d2h;
		ret;
	}
}

bool HookDirectX() {


	if (device == NULL) {
		return false;
	}


	_naked_InitDirectXHooks();
	// this specifically checks if our hooks were overwritten, and then rehooks
	patchJump(0x004be3c4, _naked_RehookDirectX);

	patchJump(0x004bdd0b, _naked_PresentHook);

	log("directX hooked");
	return true;
}