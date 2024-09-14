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


// -----

typedef struct Point {
	float x = 0.0;
	float y = 0.0;
} Point;



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

	/*
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);  
	device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ZERO);

	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTSS_COLORARG1);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	*/
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

void __stdcall drawTri(const D3DVECTOR& v1, const D3DVECTOR& v2, const D3DVECTOR& v3, D3DCOLOR col) {
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

void __stdcall drawRect2(float x, float y, float w, float h, DWORD ARGB = 0x8042e5f4) { // top left is 0.0, bottom right is 1.0. 
	
	y = 1 - y;
	
	D3DVECTOR v1 = { ((x + 0) * 1.5) - 1.0, ((y + 0) * 2.0) - 1.0, 0.0f };
	D3DVECTOR v2 = { ((x + w) * 1.5) - 1.0, ((y + 0) * 2.0) - 1.0, 0.0f };
	D3DVECTOR v3 = { ((x + 0) * 1.5) - 1.0, ((y - h) * 2.0) - 1.0, 0.0f };
	D3DVECTOR v4 = { ((x + w) * 1.5) - 1.0, ((y - h) * 2.0) - 1.0, 0.0f };
	
	drawTri(v1, v2, v3, ARGB);
	drawTri(v2, v3, v4, ARGB);
}

// this var may need to be set dynamically based on resolution, but for now 0.005 works fine.
const float lineWidth = 0.0025;
void __stdcall drawLine2(float x1, float y1, float x2, float y2, DWORD ARGB = 0x8042e5f4, bool side=false) { // top left is (0.0, 0.0), bottom right is (1.3333, 0). 
	
	// i am,,, i bit confused on how exactly to do this. 
	// current vibes say,,, two very thin triangles.

	// i would like,,, diag lines please too.
	// there are,,, angle issues with that tho

	// feel free to play around with https://www.desmos.com/calculator/ppviwesili
	// side chooses which "side" of the input line is actually drawn
	// you will only really care about this,,, if you really care abt it?

	Point p1 = { x1, y1 };
	Point p2 = { x2, y2 };

	float mx = p2.x - p1.x;
	float my = p2.y - p1.y;
	float m = my / mx;

	float a = atan2(my, mx) + (3.1415926535 / 2.0f);
	
	float m2 = tan(a);

	Point p3 = { x1, y1 };
	Point p4 = { x2, y2 };

	Point offset = { lineWidth * cos(a), lineWidth * sin(a) };

	if (side) {
		p3.x += offset.x;
		p3.y += offset.y;
		p4.x += offset.x;
		p4.y += offset.y;
	} else {
		p3.x -= offset.x;
		p3.y -= offset.y;
		p4.x -= offset.x;
		p4.y -= offset.y;
	}

	p1.y = 1 - p1.y;
	p2.y = 1 - p2.y;
	p3.y = 1 - p3.y;
	p4.y = 1 - p4.y;

	D3DVECTOR v1 = { (p1.x * 1.5) - 1.0, (p1.y * 2.0) - 1.0, 0.0f };
	D3DVECTOR v2 = { (p2.x * 1.5) - 1.0, (p2.y * 2.0) - 1.0, 0.0f };
	D3DVECTOR v3 = { (p3.x * 1.5) - 1.0, (p3.y * 2.0) - 1.0, 0.0f };
	D3DVECTOR v4 = { (p4.x * 1.5) - 1.0, (p4.y * 2.0) - 1.0, 0.0f };

	drawTri(v1, v2, v3, ARGB);
	drawTri(v2, v3, v4, ARGB);



	/*
	y1 = 1 - y1;
	y2 = 1 - y2;
	
	D3DVECTOR v1 = { ((x1 + 0) * 2.0) - 1.0, ((y1 + 0) * 2.0) - 1.0, 0.0f };
	D3DVECTOR v2 = { ((x2 + 0) * 2.0) - 1.0, ((y2 + 0) * 2.0) - 1.0, 0.0f };
	D3DVECTOR v3 = { ((x1 + lineWidth) * 2.0) - 1.0, ((y1 + 0) * 2.0) - 1.0, 0.0f };
	D3DVECTOR v4 = { ((x2 + lineWidth) * 2.0) - 1.0, ((y2 + 0) * 2.0) - 1.0, 0.0f };

	drawTri(v1, v2, v3, ARGB);
	drawTri(v2, v3, v4, ARGB);
	*/
}

void __stdcall drawBorder2(float x, float y, float w, float h, DWORD ARGB = 0x8042e5f4) {

	h -= lineWidth;
	w -= lineWidth;

	drawLine2(x + lineWidth, y, x + w, y, ARGB, true);
	drawLine2(x, y, x, y + h, ARGB, false);
	drawLine2(x + w, y, x + w, y + h, ARGB, false);
	drawLine2(x, y + h, x + w + lineWidth, y + h, ARGB, true);
	

}

void __stdcall drawText2(float x, float y, float size, DWORD ARGB, const char* str) {
	// pass things in as you would with printf, like printf("%d %.2f %s", 1, 1.23f, "abcdefg");
	
	if (str != NULL) {
		log(str);
	}

	
	
	//va_end(*args); // args is already alloced, and needs to be cleaned up here.	

	// goals:
	// monospaced
	// txt viewable at small sized
	// non stupidity.
	// font used: Cascadia Code, its comfy, its familiar
	// i also need to statically link said font



}

// -----

// would a queue be better here?
// look, i know i like lambdas too much, but i was getting annoyed
// constantly allocing and deallocing lambdas is probs,,, bad.
// i could just have them be, refs? idek
std::vector<std::function<void(void)>*> drawCalls;

void LineDraw(float x1, float y1, float x2, float y2, DWORD ARGB = 0x8042e5f4, bool side = false) {
	drawCalls.emplace_back(
		new std::function<void(void)>(
		[x1, y1, x2, y2, ARGB, side]() -> void {
			drawLine2(x1, y1, x2, y2, ARGB, side);
		}
	));
}

void RectDraw(float x, float y, float w, float h, DWORD ARGB = 0x8042e5f4) {
	drawCalls.emplace_back(
		new std::function<void(void)>(
		[x, y, w, h, ARGB]() -> void {
			drawRect2(x, y, w, h, ARGB);
		}
	));
}

void BorderDraw(float x, float y, float w, float h, DWORD ARGB = 0x8042e5f4) {
	drawCalls.emplace_back(
		new std::function<void(void)>(
		[x, y, w, h, ARGB]() -> void {
			drawBorder2(x, y, w, h, ARGB);
		}
	));
}

void BorderRectDraw(float x, float y, float w, float h, DWORD ARGB = 0x8042e5f4) {
	drawCalls.emplace_back(
	new std::function<void(void)>(
		[x, y, w, h, ARGB]() -> void {
			drawRect2(x, y, w, h, ARGB);
			drawBorder2(x, y, w, h, ARGB | 0xFF000000);
		}
	));
}

void TextDraw(float x, float y, float size, DWORD ARGB, const char* format, ...) {
	// i do hope that this allocing does not slow things down. i tried saving the va_args for when the actual print func was called, but it would not work

	char* buffer = (char*)malloc(1024);

	if (buffer == NULL) {
		log("textdraw malloc somehow failed???");
		return;
	}

	va_list args;
	va_start(args, format);
	vsnprintf(buffer, 1024, format, args);
	va_end(args);

	drawCalls.emplace_back(
		new std::function<void(void)>(
		[x, y, size, ARGB, buffer]() mutable -> void {
			drawText2(x, y, size, ARGB, buffer);
			if (buffer != NULL) {
				free(buffer);
				buffer = NULL;
			}
		}
	));
}

// -----

void __stdcall _doDrawCalls() {

	backupRenderState();

	device->BeginScene(); // should i start a new scene per call, or is one thing enough

	for (std::function<void(void)>* drawCallInfo : drawCalls) {
		(*drawCallInfo)();
	}

	device->EndScene();

	restoreRenderState();

	for (int i = 0; i < drawCalls.size(); i++) {
		delete drawCalls[i];
	}

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