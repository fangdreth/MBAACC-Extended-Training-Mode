#pragma once

void _naked_InitDirectXHooks();

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

const int fontTexWidth = 512;
const int fontTexHeight = 512;
int fontSize = 64;
int fontHeight = 64;
int fontWidth = (((float)fontSize) / 2.0f) - 1; // 1 extra pixel is put between each char and the next. take this into account
float fontRatio = ((float)fontWidth) / ((float)fontSize);

size_t fontBufferSize = 0;
BYTE* fontBuffer = NULL; // this is purposefully not freed on evict btw
IDirect3DTexture9* fontTexture = NULL;

unsigned scaleNextPow2(unsigned v) {
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v;
}

void _initFontFirstLoad() {
	// i dont know why.
	// i dont know how.
	// but SOMEHOW
	// calling DrawTextA overwrites my hooks, and completely fucks EVERYTHING up.
	// i could rehook after it, but holy shit im already rehooking caster hooks, and rehooking beginstate hooks? i think that rehooking 3 times is to many hooks.
	// im going to need to render the font manually.
	// orrr,,, i could,,, try rehooking again? as a treat?
	// no, no treat, you are going to have to do this painfully.
	// never call this func more than once, or while hooked.

	static bool hasRan = false;
	if (hasRan) {
		return;
	}
	hasRan = true;

	ID3DXFont* font = NULL;
	IDirect3DSurface9* surface = NULL;
	ID3DXBuffer* buffer = NULL;
	IDirect3DTexture9* texture = NULL;

	// i still do not know what the hell MIP levels are. but it seems that they would be,, needed/a good idea
	// for variable font sizing

	//const int fontSize = 24;
	
	int fontSizePow2 = scaleNextPow2(fontSize);

	HRESULT hr = D3DXCreateFontA(
		device,
		fontSize, // height 
		fontWidth,  // width. this could be set to 0, but out of paranoia, i want it at a fixed value
		//FW_NORMAL, // weight?
		FW_BOLD, 
		1,  // mip? i should probs use these tbh
		FALSE, // italicized
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, // is this the best ver
		FIXED_PITCH | FF_MODERN, // pitch is monospace,,, modern is,,, for constant stroke witdth?
		"Courier New", // this appears to be the only monospaced font shipped by default on windows. i could also include another font in rom if needed tho
		&font
	);

	if (FAILED(hr)) {
		log("failed to create font?");
		font = NULL;
		return;
	}

	std::string dispChars = "";

	size_t charCount = 0;

	for (char c = ' '; c <= '~'; c++) {

		if (((int)c & 0xF) == 0 && c != ' ') {
			dispChars += "\n";
		}

		dispChars += c;
		charCount++;
	}

	int texWidth = scaleNextPow2(charCount * fontSize);
	int texHeight = fontSizePow2 * 16;

	//texWidth = 512;
	//texHeight = 512;

	log("attempting to load a font, reallllly attempting w: %d h: %d", fontTexWidth, fontTexHeight);

	hr = device->CreateTexture(fontTexWidth, fontTexHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, NULL);
	if (FAILED(hr)) {
		font->Release();
		log("font createtexture failed");
		return;
	}

	hr = texture->GetSurfaceLevel(0, &surface);
	if (FAILED(hr)) {
		texture->Release();
		texture = NULL;
		font->Release();
		log("font getsurfacelevel failed");
		return;
	}

	D3DVIEWPORT9 viewport;

	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = texWidth;
	viewport.Height = texHeight;
	viewport.MinZ = 0.0f;
	viewport.MaxZ = 1.0f;
	device->SetViewport(&viewport);

	IDirect3DSurface9* oldRenderTarget = nullptr;
	hr = device->GetRenderTarget(0, &oldRenderTarget);
	if (FAILED(hr)) {
		surface->Release();
		texture->Release();
		texture = NULL;
		font->Release();
		log("font getrendertarget failed");
		return;
	}

	hr = device->SetRenderTarget(0, surface);
	if (FAILED(hr)) {
		oldRenderTarget->Release();
		surface->Release();
		texture->Release();
		texture = NULL;
		font->Release();
		log("font setrendertarget failed");
		return;
	}


	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	hr = device->BeginScene();
	if (SUCCEEDED(hr)) {
		
		RECT rect = { 0, 0, texWidth, texHeight };
		font->DrawTextA(NULL, dispChars.c_str(), -1, &rect, DT_LEFT | DT_TOP, D3DCOLOR_XRGB(255, 255, 255)); // gross horrid disgusting hook removing call

		// End the scene
		device->EndScene();
	}
	else {
		log("beginscene failed?????");
	}

	device->SetRenderTarget(0, oldRenderTarget);
	oldRenderTarget->Release();
	surface->Release();

	hr = D3DXSaveTextureToFileA("fontTest.png", D3DXIFF_PNG, texture, NULL);
	hr = D3DXSaveTextureToFileInMemory(&buffer, D3DXIFF_PNG	, texture, NULL);
	texture->Release();
	font->Release();

	if (FAILED(hr)) {
		log("savetextomem failed??");
		return;
	}

	BYTE* bufferPtr = (BYTE*)buffer->GetBufferPointer();
	size_t bufferSize = buffer->GetBufferSize();

	fontBuffer = (BYTE*)malloc(bufferSize);
	if (fontBuffer == NULL) {
		log("font malloc failed, what are you doing");
		return;
	}

	memcpy(fontBuffer, bufferPtr, bufferSize);
	fontBufferSize = bufferSize;

	log("loaded font BUFFER!!!");
}

void initFont() {

	_initFontFirstLoad();

	if (fontBuffer == NULL) {
		log("initfont font buffer was null, dipping");
		return;
	}

	if (fontTexture != NULL) {
		fontTexture->Release();
		fontTexture = NULL;
	}

	HRESULT hr;

	hr = D3DXCreateTextureFromFileInMemory(device, fontBuffer, fontBufferSize, &fontTexture); // this texture is D3DPOOL_MANAGED, and so doesnt need a reset on every reset call! yipee
	if (FAILED(hr)) {
		log("font createtexfromfileinmem failed??");
		return;
	}

	log("FONT TEXTURE LOADED");
}

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

void __stdcall backupRenderState() {

	if (_hasStateToRestore) {
		return;
	}

	_hasStateToRestore = true;

	// store state
	device->GetPixelShader(&_pixelShaderBackup);
	device->GetVertexShader(&_vertexShaderBackup);
	device->GetTexture(0, &_textureBackup);

	device->GetRenderState(D3DRS_BLENDOP, &_D3DRS_BLENDOP);
	device->GetRenderState(D3DRS_ALPHABLENDENABLE, &_D3DRS_ALPHABLENDENABLE);
	device->GetRenderState(D3DRS_SRCBLEND, &_D3DRS_SRCBLEND);
	device->GetRenderState(D3DRS_DESTBLEND, &_D3DRS_DESTBLEND);
	device->GetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, &_D3DRS_SEPARATEALPHABLENDENABLE);
	device->GetRenderState(D3DRS_SRCBLENDALPHA, &_D3DRS_SRCBLENDALPHA);
	device->GetRenderState(D3DRS_DESTBLENDALPHA, &_D3DRS_DESTBLENDALPHA);

	// 1 1 5 6 1 5 2

	//log("%d %d %d %d %d %d %d", _D3DRS_BLENDOP, _D3DRS_ALPHABLENDENABLE, _D3DRS_SRCBLEND, _D3DRS_DESTBLEND, _D3DRS_SEPARATEALPHABLENDENABLE, _D3DRS_SRCBLENDALPHA, _D3DRS_DESTBLENDALPHA);

	
	device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);



	/*
	device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
	device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ZERO);
	*/

	//device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	

	//device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTSS_COLORARG1);
	//device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

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
	//device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	//device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	//device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	//device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, true);
	///device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
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
	//device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTSS_COLORARG1);
	//device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

}
	
void __stdcall restoreRenderState() {	

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

	device->SetRenderState(D3DRS_BLENDOP, _D3DRS_BLENDOP);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, _D3DRS_ALPHABLENDENABLE);
	device->SetRenderState(D3DRS_SRCBLEND, _D3DRS_SRCBLEND);
	device->SetRenderState(D3DRS_DESTBLEND, _D3DRS_DESTBLEND);
	device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, _D3DRS_SEPARATEALPHABLENDENABLE);
	device->SetRenderState(D3DRS_SRCBLENDALPHA, _D3DRS_SRCBLENDALPHA);
	device->SetRenderState(D3DRS_DESTBLENDALPHA, _D3DRS_DESTBLENDALPHA);

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

void __stdcall drawChar(const D3DVECTOR& v1, const D3DVECTOR& v2, const D3DVECTOR& v3, const D3DVECTOR& v4, float size, D3DCOLOR col, char c) {
	if (device == NULL) return;

	if (c < ' ' || c > '~') {
		return;
	}

	const DWORD vertFormat = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	
	// the order of params is not communicated. i think. so it must be in this order
	// stupidest bug of my life
	struct CUSTOMVERTEX { 
		D3DVECTOR position;
		D3DCOLOR color;
		D3DXVECTOR2 texCoord;
	};

	constexpr float whatIsThis = 0.5f; // what is this?

	const float charWidth = ((float)(fontWidth + 1)) / ((float)fontTexWidth);
	const float charHeight = ((float)(fontHeight + 0)) / ((float)fontTexHeight);

	D3DXVECTOR2 charTopLeft(charWidth * (c & 0xF), charHeight * ((c - 0x20) / 0x10));
	D3DXVECTOR2 charW(charWidth, 0.0);
	D3DXVECTOR2 charH(0.0, charHeight);


	// Define the vertices of the triangle
	CUSTOMVERTEX vertices[] = {
		{ D3DVECTOR(v1.x, v1.y, whatIsThis), col, charTopLeft },
		{ D3DVECTOR(v2.x, v2.y, whatIsThis), col, charTopLeft + charW },
		{ D3DVECTOR(v3.x, v3.y, whatIsThis), col, charTopLeft + charH },
		{ D3DVECTOR(v4.x, v4.y, whatIsThis), col, charTopLeft + charW + charH }
	};

	scaleVertex(vertices[0].position);
	scaleVertex(vertices[1].position);
	scaleVertex(vertices[2].position);
	scaleVertex(vertices[3].position);

	// Set up the vertex buffer and draw
	// i dislike allocing and unallocing stuff like this.
	// ESPECIALLY HERE.
	// if anythings going to lag the gpu, its this.

	LPDIRECT3DVERTEXBUFFER9 v_buffer;
	if (SUCCEEDED(device->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX), 0, vertFormat, D3DPOOL_MANAGED, &v_buffer, NULL))) {
		VOID* pVoid;

		v_buffer->Lock(0, 0, (void**)&pVoid, 0);
		memcpy(pVoid, vertices, sizeof(vertices));
		v_buffer->Unlock();

		device->SetTexture(0, fontTexture);

		device->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
		device->SetFVF(vertFormat);
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		v_buffer->Release();

		device->SetTexture(0, NULL);
	} else {
		log("drawCharAlloc failed?");	
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

// this var may. need to be set dynamically based on resolution, but for now 0.005 works fine.
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
	
	if (str == NULL) {
		log("str was null, not drawing text");
		return;
	}

	if (fontTexture == NULL) {
		log("fontTexture was null, im not drawing");
		return;
	}

	float origX = x;
	float origY = y;
	
	

	float charWidthOffset = (fontRatio * size) * 1.01f; // this 1.01 might cause issues when aligning stuff, not sure
	float charHeightOffset = size;

	float w = charWidthOffset;
	float h = charHeightOffset;

	while (*str) {
		if (*str == '\r' || *str == '\n') {
			x = origX;
			origY += charHeightOffset;
			str++;
			continue;
		}

		if (*str == ' ') {
			x += charWidthOffset;
			str++;
			continue;
		}

		if (*str == '\t') { // please dont use tabs. please
			str++;
			continue;
		}

		y = 1 - origY;
		
		D3DVECTOR v1 = { ((x + 0) * 1.5) - 1.0, ((y + 0) * 2.0) - 1.0, 0.0f };
		D3DVECTOR v2 = { ((x + w) * 1.5) - 1.0, ((y + 0) * 2.0) - 1.0, 0.0f };
		D3DVECTOR v3 = { ((x + 0) * 1.5) - 1.0, ((y - h) * 2.0) - 1.0, 0.0f };
		D3DVECTOR v4 = { ((x + w) * 1.5) - 1.0, ((y - h) * 2.0) - 1.0, 0.0f };

		drawChar(v1, v2, v3, v4, size, ARGB, *str);

		x += charWidthOffset;
		str++;
	}
	
}

// -----

// would a queue be better here?
// look, i know i like lambdas too much, but i was getting annoyed
// constantly allocing and deallocing lambdas is probs,,, bad.
// i could just have them be, refs? idek
std::vector<std::function<void(void)>*> drawCalls;

void LineDraw(float x1, float y1, float x2, float y2, DWORD ARGB = 0x8042e5f4, bool side = false) {
	
	x1 /= 480.0;
	x2 /= 480.0;
	y1 /= 480.0;
	y1 /= 480.0;
	
	drawCalls.emplace_back(
		new std::function<void(void)>(
		[x1, y1, x2, y2, ARGB, side]() -> void {
			drawLine2(x1, y1, x2, y2, ARGB, side);
		}
	));
}

void RectDraw(float x, float y, float w, float h, DWORD ARGB = 0x8042e5f4) {
	
	x /= 480.0;
	w /= 480.0;
	y /= 480.0;
	h /= 480.0;
	
	drawCalls.emplace_back(
		new std::function<void(void)>(
		[x, y, w, h, ARGB]() -> void {
			drawRect2(x, y, w, h, ARGB);
		}
	));
}

void BorderDraw(float x, float y, float w, float h, DWORD ARGB = 0x8042e5f4) {
	
	x /= 480.0;
	w /= 480.0;
	y /= 480.0;
	h /= 480.0;
	
	drawCalls.emplace_back(
		new std::function<void(void)>(
		[x, y, w, h, ARGB]() -> void {
			drawBorder2(x, y, w, h, ARGB);
		}
	));
}

void BorderRectDraw(float x, float y, float w, float h, DWORD ARGB = 0x8042e5f4) {
	
	x /= 480.0;
	w /= 480.0;
	y /= 480.0;
	h /= 480.0;	
	
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

	x /= 480.0;
	y /= 480.0;
	size /= 480.0;

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

	// on my machine at least, i swear we are having some slowdown
	// lots of the areas that are hooked occur,,, between when the 60fps timing checks are done?
	// id appreciate it if we kept this on until release
	// its in here bc present is guarenteed to only be called once a frame
	static long long startTime = 0.0;
	static long long endTime = 0.0;
	const int timeBufferLen = 60;
	static double timeBuffer[timeBufferLen];
	static int timeBufferIndex = 0;

	endTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

	timeBuffer[timeBufferIndex] = (double)1000000.0 / ((double)endTime - startTime);

	timeBufferIndex = (timeBufferIndex + 1) % timeBufferLen;

	double res = 0.0;
	for (int i = 0; i < timeBufferLen; i++) {
		res += timeBuffer[i];
	}
	res /= ((double)timeBufferLen);

	//TextDraw(1.21, 0.0, 0.025, 0xFF00FFFF, "FPS: %5.2lf", res);
	TextDraw(580, 0.0, 12, 0xFF00FFFF, "FPS: %5.2lf", res);

	//TextDraw(1.11, 0.2, 0.050, 0xFF00FFFF, "FPS: %5.2lf", res);

	startTime = endTime;

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
	//log("doDrawCalls start");
	_doDrawCalls();
	//log("doDrawCalls good");
	POP_ALL;

	__asm {
		push 00000000h;
		push 00000000h;
		push 00000000h;
		push 00000000h;
		push eax;
		call edx;
	};

	PUSH_ALL;
	frameStartCallback(); 
	POP_ALL;

	__asm {

		push 004bdd16h;
		ret;
	}
}

void cleanForDirectXReset() {
	// all textures need to be reset here this occurs during the transition to and from fullscreen
	// i should make a class to manage all of them, make it easier for ppl
	Texture::_cleanForReset();

	// fonttexture is managed, doesnt need release

}

void reInitAfterDirectXReset() {

	/*
	if (font != NULL) {
		font->OnResetDevice();
	}

	if (fontSprite != NULL) {
		fontSprite->OnResetDevice();
	}
	*/

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

	PUSH_ALL;
	reInitAfterDirectXReset();
	POP_ALL;

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

DWORD _DirectX_Present_Func_ret;
DWORD _DirectX_Present_Func_Addr = 0;
__declspec(naked) void _DirectX_Present_Func() {
	// IMPORTANT. i am unsure if the hooked areas where ppl would be adding draw calls here,, 
	// some hooks for certain things might be after present. if so, things would get drawn a frame late. 
	// figure it out.
	PUSH_ALL;
	//log("IDirect3DDevice9_Present called!");
	//_doDrawCalls();
	POP_ALL;
	__asm {
		pop _DirectX_Present_Func_ret;
		//jmp[_DirectX_Present_Func_Addr];
		call[_DirectX_Present_Func_Addr];
	}

	PUSH_ALL;
	//log("present success");
	POP_ALL;

	__asm {
		push _DirectX_Present_Func_ret;
		ret;
	}

}

__declspec(naked) void _naked_InitDirectXHooks() {

	PUSH_ALL;
	log("hooking directx");
	POP_ALL;

	__asm {

		push eax;
		push ebx;
		push ecx;
		push edx;

		// hook directx reset, we need to free all alloced textures when this is called
		mov eax, [device];
		mov ecx, [eax];
		mov edx, [ecx + 00000040h];
		//cmp edx, _DirectX_Reset_Func_Addr; // check if the current hook is already ours.
		//JE _DirectX_Reset_Func_DONT;
		mov _DirectX_Reset_Func_Addr, edx;
		mov edx, _DirectX_Reset_Func;
		mov[ecx + 00000040h], edx;
	_DirectX_Reset_Func_DONT:

		// hook beginstate block, as it overwrites my hooks.
		mov eax, [device];
		mov ecx, [eax];
		mov edx, [ecx + 000000F0h];
		//cmp edx, _DirectX_BeginStateBlock_Func_Addr; // check if the current hook is already ours.
		//JE _DirectX_BeginStateBlock_Func_DONT;
		mov _DirectX_BeginStateBlock_Func_Addr, edx;
		mov edx, _DirectX_BeginStateBlock_Func;
		mov[ecx + 000000F0h], edx;
	_DirectX_BeginStateBlock_Func_DONT:
		
		/*
		// hook present, so we can draw on the screen
		mov eax, [device];
		mov ecx, [eax];
		mov edx, [ecx + 00000044h];
		//cmp edx, _DirectX_Present_Func_Addr; // check if the current hook is already ours.
		//JE _DirectX_Present_Func_DONT;
		mov _DirectX_Present_Func_Addr, edx;
		mov edx, _DirectX_Present_Func;
		mov[ecx + 00000044h], edx;
	_DirectX_Present_Func_DONT:
		*/

		// hook evict, so i can manage memory properly for once
		mov eax, [device];
		mov ecx, [eax];
		mov edx, [ecx + 00000014h];
		//cmp edx, _DirectX_EvictManagedResources_Func_Addr; // check if the current hook is already ours.
		//JE _DirectX_EvictManagedResources_Func_DONT;
		mov _DirectX_EvictManagedResources_Func_Addr, edx;
		mov edx, _DirectX_EvictManagedResources_Func;
		mov[ecx + 00000014h], edx;
	_DirectX_EvictManagedResources_Func_DONT:

		pop edx;
		pop ecx;
		pop ebx;
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
	

	// this specifically checks if our hooks were overwritten, and then rehooks
	patchJump(0x004be3c4, _naked_RehookDirectX);

	patchJump(0x004bdd0b, _naked_PresentHook);

	// this NEEDS to be first, before directx is hooked
	initFont();

	// idrk why, but ig im just 
	// why does my code still work even when, nothing is hooked, and my resets arent there
	// like,, the font texture is not being recreated and doesnt crash
	// the only thing i can think of is i installed the steam ver this morning???
	// did the steam ver do something to directx? did it change melty's settings??? why is this happening?
	// additionally, INITING the directx hook in pausecallback vs in framedonecallback 
	// causes different things to happen. 
	// the ONLY WAY this occurs, is if caster is,,, hooking and unhooking it periodically? on every call?
	// actually, ok here. 
	// either caster is calling my hook, or im calling caster's hook. caster doesnt seem to sanitize
	// or actually maybe i was relying on casters unsanitized state as my default?
	// however, it actually seems like caster does not call my hooks,, and that was the issue?
	_naked_InitDirectXHooks();

	log("directX hooked");
	return true;
}