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

IDirect3DPixelShader9* createPixelShader(const char* pixelShaderCode) {

	IDirect3DPixelShader9* res = NULL;
	ID3DXBuffer* pShaderBuffer = NULL;
	ID3DXBuffer* pErrorBuffer = NULL;

	HRESULT hr;

	hr = D3DXCompileShader(
		pixelShaderCode,
		strlen(pixelShaderCode),
		NULL,
		NULL,
		"main",
		//"trans",					 
		"ps_3_0",
		//"ps_4_0",
		0,
		&pShaderBuffer,
		&pErrorBuffer,
		NULL
	);

	if (FAILED(hr)) {
		if (pErrorBuffer) {
			log((char*)pErrorBuffer->GetBufferPointer());
			pErrorBuffer->Release();
		}
		return NULL;
	}

	hr = device->CreatePixelShader((DWORD*)pShaderBuffer->GetBufferPointer(), &res);

	if (FAILED(hr)) {
		log("createShader died");
		return NULL;
	}

	pShaderBuffer->Release();

	return res;
}

IDirect3DVertexShader9* createVertexShader(const char* shaderCode) {

	IDirect3DVertexShader9* res = NULL;
	ID3DXBuffer* pShaderBuffer = NULL;
	ID3DXBuffer* pErrorBuffer = NULL;

	HRESULT hr;

	hr = D3DXCompileShader(
		shaderCode,
		strlen(shaderCode),
		NULL,
		NULL,
		"main",
		"vs_3_0",
		0,
		&pShaderBuffer,
		&pErrorBuffer,
		NULL
	);

	if (FAILED(hr)) {
		if (pErrorBuffer) {
			log((char*)pErrorBuffer->GetBufferPointer());
			pErrorBuffer->Release();
		}
		return NULL;
	}

	hr = device->CreateVertexShader((DWORD*)pShaderBuffer->GetBufferPointer(), &res);

	if (FAILED(hr)) {
		log("createShader died");
		return NULL;
	}

	pShaderBuffer->Release();

	return res;
}

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

	//hr = D3DXSaveTextureToFileA("fontTest.png", D3DXIFF_PNG, texture, NULL);
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

	//hr = D3DXSaveTextureToFileA("fontTest.png", D3DXIFF_PNG, fontTexture, NULL);

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


	
	
	// good
	
	/*
	device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
	*/
	
	/*
	device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_SUBTRACT);
	//device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); // mult the input by the source alpha 
	//device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); // in the event that there is 0 alpha black, invsrcalpha will make it such taht it instead uses the dest color

	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);

	device->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);

	device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
	device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ZERO);
	*/
	


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

	// how wasteful is this branch :(

	scaleVertex(vertices[0].position);
	scaleVertex(vertices[1].position);
	scaleVertex(vertices[2].position);
	

	// todo, figure out why colors, dont

	const DWORD vertFormat = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	
	// Set up the vertex buffer and draw
	// i dislike allocing and unallocing stuff like this.
	LPDIRECT3DVERTEXBUFFER9 v_buffer;
	if (SUCCEEDED(device->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX), 0, vertFormat, D3DPOOL_MANAGED, &v_buffer, NULL))) {
		VOID* pVoid;
		
		v_buffer->Lock(0, 0, (void**)&pVoid, 0);
		memcpy(pVoid, vertices, sizeof(vertices));
		v_buffer->Unlock();

		device->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
		device->SetFVF(vertFormat);
		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		v_buffer->Release();
	}
}

void __stdcall drawChar(const D3DVECTOR& v1, const D3DVECTOR& v2, const D3DVECTOR& v3, const D3DVECTOR& v4, float size, D3DCOLOR col, char c) {
	if (device == NULL) return;

	if (c < ' ' || c > '~') {
		return;
	}

	const DWORD vertFormat = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1  );
	
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

	// when drawing a o, the underscore would be to low, and cut into the top of it, this fixes that?
	charTopLeft.y += 0.0001f;
	charH.y -= 0.0001f;

	// LOWERCASE LETTERS ALL HAVE ISSUES
	//charTopLeft.x += 0.00050f;
	//charW.x -= 0.0001f;



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


	//D3DSURFACE_DESC desc;
	//fontTexture->GetLevelDesc(0, &desc); // Get the texture size
	//D3DXVECTOR4 textureSize((float)desc.Width, (float)desc.Height, 0.0, 0.0);
	//device->SetPixelShaderConstantF(219, (float*)&textureSize, 1);

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
		//device->SetPixelShader(pShader);

		device->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
		device->SetFVF(vertFormat);
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		v_buffer->Release();

		device->SetPixelShader(NULL);
		device->SetTexture(0, NULL);
	} else {
		log("drawCharAlloc failed?");	
	}
}

void __stdcall drawTextureRect(float x, float y, float w, float h) {

	// in the future i could overload this for a color option

	y = 1 - y;

	D3DVECTOR v1 = { ((x + 0) * 1.5f) - 1.0f, ((y + 0) * 2.0f) - 1.0f, 0.0f };
	D3DVECTOR v2 = { ((x + w) * 1.5f) - 1.0f, ((y + 0) * 2.0f) - 1.0f, 0.0f };
	D3DVECTOR v3 = { ((x + 0) * 1.5f) - 1.0f, ((y - h) * 2.0f) - 1.0f, 0.0f };
	D3DVECTOR v4 = { ((x + w) * 1.5f) - 1.0f, ((y - h) * 2.0f) - 1.0f, 0.0f };
	
	//const DWORD vertFormat = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	const DWORD vertFormat = (D3DFVF_XYZ | D3DFVF_TEX1);

	struct CUSTOMVERTEX {
		D3DVECTOR position;
		D3DXVECTOR2 texCoord;
	};

	constexpr float whatIsThis = 0.5f; // what is this?

	CUSTOMVERTEX vertices[] = { // why did this need a diffuse color. tbh i should use this to color all the boxes.
		{ D3DVECTOR(v1.x, v1.y, whatIsThis), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DVECTOR(v2.x, v2.y, whatIsThis), D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DVECTOR(v3.x, v3.y, whatIsThis), D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DVECTOR(v4.x, v4.y, whatIsThis), D3DXVECTOR2(1.0f, 1.0f) }
	};

	scaleVertex(vertices[0].position);
	scaleVertex(vertices[1].position);
	scaleVertex(vertices[2].position);
	scaleVertex(vertices[3].position);

	LPDIRECT3DVERTEXBUFFER9 v_buffer;
	if (SUCCEEDED(device->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX), 0, vertFormat, D3DPOOL_MANAGED, &v_buffer, NULL))) {
		VOID* pVoid;

		v_buffer->Lock(0, 0, (void**)&pVoid, 0);
		memcpy(pVoid, vertices, sizeof(vertices));
		v_buffer->Unlock();

		device->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
		device->SetFVF(vertFormat);
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		v_buffer->Release();
	}
}

void __stdcall drawRect2(float x, float y, float w, float h, DWORD ARGB = 0x8042e5f4) { // top left is 0.0, bottom right is 1.0. 
	
	y = 1 - y;
	
	D3DVECTOR v1 = { ((x + 0) * 1.5f) - 1.0f, ((y + 0) * 2.0f) - 1.0f, 0.0f };
	D3DVECTOR v2 = { ((x + w) * 1.5f) - 1.0f, ((y + 0) * 2.0f) - 1.0f, 0.0f };
	D3DVECTOR v3 = { ((x + 0) * 1.5f) - 1.0f, ((y - h) * 2.0f) - 1.0f, 0.0f };
	D3DVECTOR v4 = { ((x + w) * 1.5f) - 1.0f, ((y - h) * 2.0f) - 1.0f, 0.0f };
	
	drawTri(v1, v2, v3, ARGB);
	drawTri(v2, v3, v4, ARGB);
}

void __stdcall drawRectUnscaled(float x, float y, float w, float h, DWORD ARGB = 0x8042e5f4) {

	const DWORD vertFormat = (D3DFVF_XYZ | D3DFVF_DIFFUSE);

	struct CUSTOMVERTEX {
		D3DVECTOR position;
		D3DCOLOR color;
	};

	y = 1 - y;

	D3DVECTOR v1 = { ((x + 0) * 2.0f) - 1.0f, ((y + 0) * 2.0f) - 1.0f, 0.0f };
	D3DVECTOR v2 = { ((x + w) * 2.0f) - 1.0f, ((y + 0) * 2.0f) - 1.0f, 0.0f };
	D3DVECTOR v3 = { ((x + 0) * 2.0f) - 1.0f, ((y - h) * 2.0f) - 1.0f, 0.0f };
	D3DVECTOR v4 = { ((x + w) * 2.0f) - 1.0f, ((y - h) * 2.0f) - 1.0f, 0.0f };

	constexpr float whatIsThis = 0.5f; // what is this?

	CUSTOMVERTEX vertices[] = { // why did this need a diffuse color. tbh i should use this to color all the boxes.
		{ D3DVECTOR(v1.x, v1.y, whatIsThis), ARGB },
		{ D3DVECTOR(v2.x, v2.y, whatIsThis), ARGB },
		{ D3DVECTOR(v3.x, v3.y, whatIsThis), ARGB },
		{ D3DVECTOR(v4.x, v4.y, whatIsThis), ARGB }
	};

	LPDIRECT3DVERTEXBUFFER9 v_buffer;
	if (SUCCEEDED(device->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX), 0, vertFormat, D3DPOOL_MANAGED, &v_buffer, NULL))) {
		VOID* pVoid;

		v_buffer->Lock(0, 0, (void**)&pVoid, 0);
		memcpy(pVoid, vertices, sizeof(vertices));
		v_buffer->Unlock();

		device->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
		device->SetFVF(vertFormat);
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		v_buffer->Release();

		device->SetPixelShader(NULL);
		device->SetTexture(0, NULL);
	}

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

	float a = atan2(my, mx) + (3.1415926535f / 2.0f);
	
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

	D3DVECTOR v1 = { (p1.x * 1.5f) - 1.0f, (p1.y * 2.0f) - 1.0f, 0.0f };
	D3DVECTOR v2 = { (p2.x * 1.5f) - 1.0f, (p2.y * 2.0f) - 1.0f, 0.0f };
	D3DVECTOR v3 = { (p3.x * 1.5f) - 1.0f, (p3.y * 2.0f) - 1.0f, 0.0f };
	D3DVECTOR v4 = { (p4.x * 1.5f) - 1.0f, (p4.y * 2.0f) - 1.0f, 0.0f };

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
	
	DWORD TempARGB = ARGB;

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

		switch (*str) {
			case '\r':
			case '\n':
				x = origX;
				origY += charHeightOffset;
				str++;
				continue;
			case ' ':
				x += charWidthOffset;
				str++;
				continue;
			case '\t': // please dont use tabs. please
				str++;
				continue;
			case '{': // blue
				TempARGB = 0xFF8F8FFF;
				str++;
				continue;
			case '~': // red
				TempARGB = 0xFFFF8F8F;
				str++;
				continue;
			case '@': // green 
				TempARGB = 0xFF8FFF8F;
				str++;
				continue;
			case '`': // yellow
				TempARGB = 0xFFFFFF8F;
				str++;
				continue;
			case '^': // purple
				TempARGB = 0xFFFF8FFF;
				str++;
				continue;
			case '*': // black
				TempARGB = 0xFF8F8F8F;
				str++;
				continue;
			case '}': // reset 
				TempARGB = ARGB;
				str++;
				continue;
			case '\\': // in case you want to print one of the above chars, you can escape them
				str++;
				if (*str == '\0') {
					return;
				}
				break;
			default:
				break;
		}

		y = 1 - origY;
		
		D3DVECTOR v1 = { ((x + 0) * 1.5f) - 1.0f, ((y + 0) * 2.0f) - 1.0f, 0.0f };
		D3DVECTOR v2 = { ((x + w) * 1.5f) - 1.0f, ((y + 0) * 2.0f) - 1.0f, 0.0f };
		D3DVECTOR v3 = { ((x + 0) * 1.5f) - 1.0f, ((y - h) * 2.0f) - 1.0f, 0.0f };
		D3DVECTOR v4 = { ((x + w) * 1.5f) - 1.0f, ((y - h) * 2.0f) - 1.0f, 0.0f };

		drawChar(v1, v2, v3, v4, size, TempARGB, *str);

		x += charWidthOffset;
		str++;
	}

	/*
	if (pShader != NULL) {
		pShader->Release();
		pShader = NULL;
	}
	*/

	//device->SetPixelShader(NULL);
	
}

// -----

// would a queue be better here?
// look, i know i like lambdas too much, but i was getting annoyed
// constantly allocing and deallocing lambdas is probs,,, bad.
// i could just have them be, refs? idek
std::vector<std::function<void(void)>> drawCalls;
typedef struct ProfileInfo {
	unsigned callCount = 0; // amount called this frame 
	long long currentFrameTime = 0; // time spent here this frame
	unsigned index = 0; // having a index for each value might be overkill
	std::array<long long, 64> times; // past log of times, to make number smoother
} ProfileInfo;
std::map<const char*, ProfileInfo > profilerData;

void LineDraw(float x1, float y1, float x2, float y2, DWORD ARGB = 0x8042e5f4, bool side = false) {
	
	x1 /= 480.0f;
	x2 /= 480.0f;
	y1 /= 480.0f;
	y1 /= 480.0f;
	
	drawCalls.emplace_back(
		std::function<void(void)>(
		[x1, y1, x2, y2, ARGB, side]() -> void {
			drawLine2(x1, y1, x2, y2, ARGB, side);
		}
	));
}

void RectDraw(float x, float y, float w, float h, DWORD ARGB = 0x8042e5f4) {
	
	x /= 480.0f;
	w /= 480.0f;
	y /= 480.0f;
	h /= 480.0f;
	
	drawCalls.emplace_back(
		std::function<void(void)>(
		[x, y, w, h, ARGB]() -> void {
			drawRect2(x, y, w, h, ARGB);
		}
	));
}

void BorderDraw(float x, float y, float w, float h, DWORD ARGB = 0x8042e5f4) {
	
	x /= 480.0f;
	w /= 480.0f;
	y /= 480.0f;
	h /= 480.0f;
	
	drawCalls.emplace_back(
		std::function<void(void)>(
		[x, y, w, h, ARGB]() -> void {
			drawBorder2(x, y, w, h, ARGB);
		}
	));
}

void BorderRectDraw(float x, float y, float w, float h, DWORD ARGB = 0x8042e5f4) {
	
	x /= 480.0f;
	w /= 480.0f;
	y /= 480.0f;
	h /= 480.0f;	
	
	drawCalls.emplace_back(
	std::function<void(void)>(
		[x, y, w, h, ARGB]() -> void {
			drawRect2(x, y, w, h, ARGB);
			drawBorder2(x, y, w, h, ARGB | 0xFF000000);
		}
	));
}

void TextDraw(float x, float y, float size, DWORD ARGB, const char* format, ...) {
	// i do hope that this allocing does not slow things down. i tried saving the va_args for when the actual print func was called, but it would not work

	x /= 480.0f;
	y /= 480.0f;
	size /= 480.0f;

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
		std::function<void(void)>(
		[x, y, size, ARGB, buffer]() mutable -> void {
			drawText2(x, y, size, ARGB, buffer);
			if (buffer != NULL) {
				free(buffer);
				buffer = NULL;
			}
		}
	));
}

enum class BoxType {
	None,
	Origin, // 0xFF42E5F4
	Collision, // grey
	Hitbox, // red
	Hurtbox, // green
	Clash, // yellow
	Blue, // what is this
	Shield, // Purple, also like,,, vaki??

	_Count, // dont use
};

typedef struct BoxData {
	float x = 0.0f;
	float y = 0.0f;
	float w = 0.0f;
	float h = 0.0f;
	int player = 0; // what player this is being drawn for. should i,,, have each projectile be a seperate color as well?
} BoxData;

std::array<std::vector<BoxData>, static_cast<int>(BoxType::_Count)> boxDataList;

IDirect3DTexture9* renderTargetTex = NULL;
void HitboxBatchDraw(const std::vector<BoxData>& boxData, DWORD ARGB) {
	if (device == NULL) {
		return;
	}

	if (boxData.size() == 0) {
		return;
	}
	
	HRESULT hr;

	static IDirect3DSurface9* renderTargetSurf = NULL;
	static int width = 0;
	static int height = 0;
	static bool haveRenderTarget = false;
	if (renderTargetTex == NULL) {

		IDirect3DSwapChain9* pSwapChain = NULL;
		D3DPRESENT_PARAMETERS presentParams;

		hr = device->GetSwapChain(0, &pSwapChain);

		if (pSwapChain != NULL) {

			hr = pSwapChain->GetPresentParameters(&presentParams);

			width = presentParams.BackBufferWidth;
			height = presentParams.BackBufferHeight;

			pSwapChain->Release();

			//device->CreateRenderTarget(width, height, presentParams.BackBufferFormat, presentParams.MultiSampleType, presentParams.MultiSampleQuality, false, &renderTarget, NULL);
			//device->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &renderTargetTex, NULL);
			device->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT	, &renderTargetTex, NULL);

			
		}
	}
	
	if (renderTargetTex == NULL) {
		log("hitbox rendertarget null?");
		return;
	}
	
	
	IDirect3DSurface9* prevRenderTarget = NULL;
	device->GetRenderTarget(0, &prevRenderTarget);
	
	renderTargetTex->GetSurfaceLevel(0, &renderTargetSurf);
	device->SetRenderTarget(0, renderTargetSurf);

	// might need to do this?
	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	device->BeginScene();
	device->SetTexture(0, NULL);

	for (int i = 0; i < boxData.size(); i++) {

		// by making the color we draw in different, i can, 
		// wait hmm am i making a really stupid assumption here
		// can green overlay green?
		// yes. i no longer know what to do
		// i need to figure out directx blend modes, blend based on COLOR ONLY, and then use that to see where things overlay.

		drawRectUnscaled(boxData[i].y, boxData[i].x, boxData[i].h, boxData[i].w, ARGB);
	}
	
	device->EndScene();

	if (prevRenderTarget != NULL) {
		device->SetRenderTarget(0, prevRenderTarget);
		prevRenderTarget->Release();
		prevRenderTarget = NULL;
	} else {
		device->SetRenderTarget(0, NULL);
	}

	if (renderTargetSurf != NULL) {
		renderTargetSurf->Release();
		renderTargetSurf = NULL;
	}

	static IDirect3DPixelShader9* pShader = NULL; // edge shader
	if (pShader == NULL) {
		pShader = createPixelShader(R"(
			sampler2D textureSampler : register(s0);
			float4 texSize : register(c219);

			float4 main(float2 texCoord : TEXCOORD0) : COLOR {

					// are pixel's positions in the center of pixel or top left???

					float2 texOffset = 1.0 / texSize;
					texOffset *= 2;
					texOffset.y *= 0.6666666666666;
	

					float4 texColor = tex2D(textureSampler, texCoord);
				
					if(texColor.a < 0.1) {
						return texColor;
					}
					
					float2 offsets[8] = {
						texCoord + float2(-texOffset.x, -texOffset.y),
						texCoord + float2(-texOffset.x, 0.0),
						texCoord + float2(-texOffset.x, texOffset.y),
						
						texCoord + float2(0.0, -texOffset.y),
						texCoord + float2(0.0, texOffset.y),
						
						texCoord + float2(texOffset.x, -texOffset.y),
						texCoord + float2(texOffset.x, 0.0),
						texCoord + float2(texOffset.x, texOffset.y)
					};

					

					float4 tempColor;
					// ooooo 
					[unroll(8)] for(int i=0; i<8; i++) {

						tempColor = tex2D(textureSampler, offsets[i]);
						if(tempColor.a == 0) {
							return float4(texColor.rgb, 1.0);
						}

					}
			
					return float4(texColor.rgb, 0.25);
			}

		)");

		// even with resizes, the texture size did not change

		D3DXVECTOR4 textureSize((float)width, (float)height, 0.0, 0.0);
		device->SetPixelShaderConstantF(219, (float*)&textureSize, 1);

	}

	// i have no fucking clue if im required to have a vertex shader or not.
	// clue: yes
	
	static IDirect3DVertexShader9* vShader = NULL; 
	if (vShader == NULL) {
		vShader = createVertexShader(R"(
			struct VS_INPUT
			{
			float4 position : POSITION;
			float2 texCoord : TEXCOORD0;
			};
    
			struct VS_OUTPUT
			{
			float4 position : POSITION;
			float2 texCoord : TEXCOORD0;
			};
    
			VS_OUTPUT main(VS_INPUT input)
			{
			VS_OUTPUT output;
			output.position = input.position;
			output.texCoord = input.texCoord;
			return output;
			}

		)");
	}

	device->BeginScene();

	// actually go and render what we just made to a real rendertarget

	device->SetTexture(0, renderTargetTex);
	device->SetPixelShader(pShader);
	device->SetVertexShader(vShader);

	drawTextureRect(0.0, 0.0, 1.333333, 1.0);

	device->EndScene();

	device->SetPixelShader(NULL);
	device->SetVertexShader(NULL);
	device->SetTexture(0, NULL);


}

// ----- horrid Profiler, as a treat 

class Profiler {
public:

	const char* name;
	long long startTime;

	Profiler(const char* name_) {
		name = name_;
		startTime = getMicroSec();
	}

	~Profiler() {

		auto it = profilerData.find(name);

		if (it == profilerData.end()) [[unlikely]] {
			profilerData[name] = ProfileInfo();
		}

		profilerData[name].callCount++;
		profilerData[name].currentFrameTime += getMicroSec() - startTime;
	}

};


#define profileFunction() \
    volatile Profiler profiler(__func__);

// -----

void DrawHitbox(float x, float y, float w, float h, BoxType type, int player) {
	boxDataList[static_cast<int>(type)].emplace_back(BoxData{x / 640.0f, y / 480.0f, w / 640.0f, h / 480.0f, player });
}

void drawHitboxes() {

	profileFunction();

	constexpr DWORD colors[] = {
		0xFF111111, // None
		0xFF42E5F4, // origin
		0xFFD0D0D0, // collision
		0xFFFF0000, // hitbox
		0xFF00FF00, // hurtbox
		0xFFFF0000, // clash
		0xFF0000FF, // blue
		0xFFF54298 // shield
	};

	// i could have avoided a div stage, but ugh, another time

	int i;

	i = static_cast<int>(BoxType::Hurtbox);
	HitboxBatchDraw(boxDataList[i], colors[i]);
	boxDataList[i].clear();

	i = static_cast<int>(BoxType::Hitbox);
	HitboxBatchDraw(boxDataList[i], colors[i]);
	boxDataList[i].clear();

	i = static_cast<int>(BoxType::Clash);
	HitboxBatchDraw(boxDataList[i], colors[i]);
	boxDataList[i].clear();

	i = static_cast<int>(BoxType::Blue);
	HitboxBatchDraw(boxDataList[i], colors[i]);
	boxDataList[i].clear();

	i = static_cast<int>(BoxType::Shield);
	HitboxBatchDraw(boxDataList[i], colors[i]);
	boxDataList[i].clear();

	// origin and collision never need to be overlaied. right? but tbh,, ugh i dont want more one off draw funcs
	i = static_cast<int>(BoxType::Collision);
	HitboxBatchDraw(boxDataList[i], colors[i]);
	boxDataList[i].clear();

	i = static_cast<int>(BoxType::Origin);
	HitboxBatchDraw(boxDataList[i], colors[i]);
	boxDataList[i].clear();

}

void drawProfiler() {
	static char buffer[1024];
	static bool drawDebug = false;
	static KeyState F10Key(VK_F10);

	if (F10Key.keyDown()) {
		drawDebug = !drawDebug;
	}

	if (drawDebug) {
		// how slow is this,,, im not sure how expensive map lookups are, but the keys to it could be constexpr. ugh 
		// i also have the pointer strat. might go do that tbh
		float profileInfoY = 128;
		for (auto& [name, info] : profilerData) {

			info.times[info.index] = info.currentFrameTime;

			long long totalTime = 0;
			for (int i = 0; i < 64; i++) {
				totalTime += info.times[i];
			}
			totalTime >>= 6; // div 64

			//TextDraw(0, profileInfoY, 12, 0xFFFFFFFF, "%3lld.%03lld %5d %.32s", totalTime / 1000, totalTime % 1000, info.callCount, name == NULL ? "NULL" : name);
			snprintf(buffer, 1024, "%3lld.%03lld %5d %.32s", totalTime / 1000, totalTime % 1000, info.callCount, name == NULL ? "NULL" : name);
			drawText2(-0.0f, (float)profileInfoY / 480.0f, (float)12 / 480.0f, 0xFF00FFFF, buffer);

			//TextDraw(200, profileInfoY, 12, 0x7FFFFFFF, "%3lld.%03lld %5d %.32s", totalTime / 1000, totalTime % 1000, info.callCount, name == NULL ? "NULL" : name);

			//TextDraw(400, profileInfoY, 12, 0x10FFFFFF, "%3lld.%03lld %5d %.32s", totalTime / 1000, totalTime % 1000, info.callCount, name == NULL ? "NULL" : name);

			info.currentFrameTime = 0;
			info.callCount = 0;

			// i dont trust MSVC to actually do this
			info.index = (info.index + 1) & 63;

			profileInfoY += 14;
		}
	}
}

void __stdcall _doDrawCalls() {
	

	//Sleep(1);


	//profileDrawCalls.clear();

	profileFunction();

	// on my machine at least, i swear we are having some slowdown
	// lots of the areas that are hooked occur,,, between when the 60fps timing checks are done?
	// id appreciate it if we kept this on until release
	// its in here bc present is guarenteed to only be called once a frame
	static long long startTime = 0.0;
	static long long endTime = 0.0;
	const int timeBufferLen = 64;
	static double timeBuffer[timeBufferLen];
	static int timeBufferIndex = 0;

	endTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

	timeBuffer[timeBufferIndex] = (double)1000000.0 / ((double)endTime - startTime);

	timeBufferIndex = (timeBufferIndex + 1) & (timeBufferLen - 1);

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

	// im purposefully having these come before misc calls.
	// and before we have an existing scene, since this has to do a lot of weird shit.
	// hopefully it isnt too slow

	drawHitboxes();

	device->BeginScene(); // should i start a new scene per call, or is one thing enough
	// i am unsure if stack alloced or heap allocing these things is better or worse
	
	for (const auto& drawCallInfo : drawCalls) {	
		drawCallInfo();
	}


	drawProfiler();

	device->EndScene();

	restoreRenderState();

	/*for (int i = 0; i < drawCalls.size(); i++) {
		delete drawCalls[i];
	}*/

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
	if (renderTargetTex != NULL) {
		renderTargetTex->Release();
		renderTargetTex = NULL;
	}
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