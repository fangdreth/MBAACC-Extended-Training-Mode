
#include <set>

#include "DirectX.h"
#include "resource.h"

void _naked_InitDirectXHooks();
void dualInputDisplay();
//void BorderDraw(float x, float y, float w, float h, DWORD ARGB = 0x8042e5f4);
void cursorDraw();
unsigned directxFrameCount = 0;

D3DXVECTOR2 mousePos; // no use getting this multiple times a frame

// my inconsistent use of D3DXVECTOR2 vs point is bad. i should use point

DragManager dragManager;

void printDirectXError(HRESULT hr) {
	const char* errorStr = DXGetErrorStringA(hr);
	const char* errorDesc = DXGetErrorDescriptionA(hr);

	log("err: %s\n    %s", errorStr, errorDesc);
}

void logMatrix(const D3DMATRIX& matrix) {
	for (int i = 0; i < 4; i++) {
		log("%7.3f %7.3f %7.3f %7.3f", matrix.m[i][0], matrix.m[i][1], matrix.m[i][2], matrix.m[i][3]);
	}
	log("-----");
}

void writeClipboard(const std::string& text) {

	if (!OpenClipboard(nullptr)) {
		return;
	}

	EmptyClipboard();

	HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
	if (hGlob) {
		memcpy(GlobalLock(hGlob), text.c_str(), text.size() + 1);
		GlobalUnlock(hGlob);

		SetClipboardData(CF_TEXT, hGlob);
	}

	CloseClipboard();
}

// -----

size_t fontBufferSize = 0;
BYTE* fontBuffer = NULL; // this is purposefully not freed on evict btw
IDirect3DTexture9* fontTexture = NULL;

size_t fontBufferSizeWithOutline = 0;
BYTE* fontBufferWithOutline = NULL;
IDirect3DTexture9* fontTextureWithOutline = NULL;

BYTE* fontBufferMelty = NULL;
size_t fontBufferMeltySize = 0;
IDirect3DTexture9* fontTextureMelty = NULL;

VertexData<PosColVert, 3 * 2048> posColVertData(D3DFVF_XYZ | D3DFVF_DIFFUSE);
VertexData<PosTexVert, 3 * 2048> posTexVertData(D3DFVF_XYZ | D3DFVF_TEX1, &fontTexture);
// need to rework font rendering, 4096 is just horrid
VertexData<PosColTexVert, 3 * 4096 * 2> posColTexVertData(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, &fontTextureMelty);

VertexData<MeltyVert, 3 * 4096 * 2> meltyVertData(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1, &fontTextureMelty);
VertexData<MeltyVert, 2 * 16384, D3DPT_LINELIST> meltyLineData(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1, &fontTextureMelty); // 8192 is overkill

unsigned _vertexBytesTotal = 0;
unsigned _vertexBytesTransferedThisFrame = 0;

// ----

//const int fontTexWidth = 256;
//const int fontTexHeight = 256;
//int fontSize = 32;
const int fontTexWidth = 512;
const int fontTexHeight = 512;
int fontSize = 64;
int fontHeight = fontSize;
int fontWidth = (((float)fontSize) / 2.0f) - 1; // 1 extra pixel is put between each char and the next. take this into account
float fontRatio = ((float)fontWidth) / ((float)fontSize);

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

bool loadResource(int id, BYTE*& buffer, unsigned& bufferSize) {

	HMODULE hModule = GetModuleHandle(TEXT("Extended-Training-Mode-DLL.dll"));

	HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(id), L"PNG");
	if (!hResource) {
		log("couldnt find embedded resource?");
		return false;
	}

	bufferSize = SizeofResource(hModule, hResource);
	if (!bufferSize) {
		log("getting resource size failed");
		return false;
	}

	HGLOBAL hMemory = LoadResource(hModule, hResource);
	if (!hMemory) {
		log("Failed to load resource");
		return false;
	}

	buffer = (BYTE*)LockResource(hMemory); // for some reason, this lock never has to be unlocked.
	if (!buffer) {
		log("Failed to lock resource");
		return false;
	}

	log("loaded resource %d successfully", id);

	return true;
}

void _initDefaultFont(IDirect3DTexture9*& resTexture) {

	HRESULT hr;

	BYTE* pngBuffer = NULL;
	unsigned pngSize = 0;
	bool res = loadResource(IDB_PNG2, pngBuffer, pngSize);

	hr = D3DXCreateTextureFromFileInMemory(device, pngBuffer, pngSize, &resTexture);
	if (FAILED(hr)) {
		log("font createtexfromfileinmem failed??");
		resTexture = NULL;
		return;
	}

}

IDirect3DPixelShader9* getFontOutlinePixelShader() {
	return createPixelShader(R"(
			sampler2D textureSampler : register(s0);
			float4 texSize : register(c219);

			float4 main(float2 texCoordIn : TEXCOORD0) : COLOR {
									
					float2 texOffset = 4.0 / texSize;
	
					//texOffset.y /= (4.0 / 3.0);

					float2 texCoord = texCoordIn + (texOffset * 0.5);

					float4 texColor = tex2D(textureSampler, texCoord);
				
					// this outline needs to be a bit different, since im trying to have it be outside the bounds of the drawn color.

					if(texColor.a > 0.0) {
						return float4(texColor.rgb, 1.0);
					}
	
					float2 offsets[8] = { // order is adjusted to check diags last. performance.
						
						texCoord + float2(-texOffset.x, 0.0),
						texCoord + float2(0.0, -texOffset.y),
						texCoord + float2(0.0, texOffset.y),
						texCoord + float2(texOffset.x, 0.0),						

						texCoord + float2(texOffset.x, -texOffset.y),
						texCoord + float2(-texOffset.x, -texOffset.y),
						texCoord + float2(-texOffset.x, texOffset.y),
						texCoord + float2(texOffset.x, texOffset.y)
					};


					float4 tempColor;

					[unroll(8)] for(int i=0; i<8; i++) {

						tempColor = tex2D(textureSampler, offsets[i]);
						if(tempColor.a > 0) {
							return float4(0.0, 0.0, 0.0, 1.0);
						}

					}
			
					return float4(0.0, 0.0, 0.0, 0.0);

			}

	)");
}

IDirect3DVertexShader9* getFontOutlineVertexShader() {
	return createVertexShader(R"(
			struct VS_INPUT {
				float4 position : POSITION;
				float2 texCoord : TEXCOORD0;
			};
    
			struct VS_OUTPUT {
				float4 position : POSITION;
				float2 texCoord : TEXCOORD0;
			};
    
			VS_OUTPUT main(VS_INPUT input) {
				VS_OUTPUT output;
				output.position = input.position;
				output.texCoord = input.texCoord;
				return output;
			}

	)");
}

void _initDefaultFontOutline(IDirect3DTexture9*& fontTex) {

	IDirect3DSurface9* surface = NULL;
	ID3DXBuffer* buffer = NULL;
	IDirect3DTexture9* texture = NULL;

	HRESULT hr;

	hr = device->CreateTexture(fontTexWidth, fontTexHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, NULL);
	if (FAILED(hr)) {
		log("font createtexture failed");
		return;
	}

	IDirect3DSurface9* oldRenderTarget = nullptr;
	hr = device->GetRenderTarget(0, &oldRenderTarget);
	if (FAILED(hr)) {
		log("font getrendertarget failed");
		return;
	}

	hr = texture->GetSurfaceLevel(0, &surface);
	if (FAILED(hr)) {
		log("font getsurfacelevel failed");
		return;
	}

	hr = device->SetRenderTarget(0, surface);
	if (FAILED(hr)) {
		log("font setrendertarget failed");
		return;
	}

	IDirect3DPixelShader9* textOutlinePixelShader = getFontOutlinePixelShader();
	IDirect3DVertexShader9* textOutlineVertexShader = getFontOutlineVertexShader();


	device->SetPixelShader(textOutlinePixelShader);
	device->SetVertexShader(textOutlineVertexShader);

	D3DXVECTOR4 textureSize((float)fontTexWidth, (float)fontTexHeight, 0.0, 0.0);
	device->SetPixelShaderConstantF(219, (float*)&textureSize, 1);

	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);



	VertexData<PosTexVert, 3 * 2> tempVertData(D3DFVF_XYZ | D3DFVF_TEX1, &fontTex); // the deconstructor should handle this.	
	tempVertData.alloc();

	PosTexVert v1 = { D3DVECTOR(-1.0f, 1.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f) };
	PosTexVert v2 = { D3DVECTOR(1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 0.0f) };
	PosTexVert v3 = { D3DVECTOR(-1.0f, -1.0f, 0.5f), D3DXVECTOR2(0.0f, 1.0f) };
	PosTexVert v4 = { D3DVECTOR(1.0f, -1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f) };

	tempVertData.add(v1, v2, v3);
	tempVertData.add(v2, v3, v4);

	device->BeginScene();

	tempVertData.draw();

	device->EndScene();

	//hr = D3DXSaveTextureToFileA("fontTest.png", D3DXIFF_PNG, texture, NULL);
	hr = D3DXSaveTextureToFileInMemory(&buffer, D3DXIFF_PNG, texture, NULL);

	BYTE* bufferPtr = (BYTE*)buffer->GetBufferPointer();
	size_t bufferSize = buffer->GetBufferSize();

	fontBufferWithOutline = (BYTE*)malloc(bufferSize);
	if (fontBufferWithOutline == NULL) {
		log("font malloc failed, what are you doing");
		return;
	}

	memcpy(fontBufferWithOutline, bufferPtr, bufferSize);
	fontBufferSizeWithOutline = bufferSize;

	device->SetRenderTarget(0, oldRenderTarget);

	device->SetPixelShader(NULL);
	device->SetVertexShader(NULL);

	oldRenderTarget->Release();
	surface->Release();
	buffer->Release();
	texture->Release();

	textOutlinePixelShader->Release();
	textOutlineVertexShader->Release();
}

void _initMeltyFont() {

	IDirect3DSurface9* surface = NULL;
	ID3DXBuffer* buffer = NULL;
	IDirect3DTexture9* texture = NULL;

	HRESULT hr;

	BYTE* pngBuffer = NULL;
	unsigned pngSize = 0;
	bool res = loadResource(IDB_PNG1, pngBuffer, pngSize);

	IDirect3DTexture9* meltyTex = NULL;

	hr = D3DXCreateTextureFromFileInMemory(device, pngBuffer, pngSize, &meltyTex);
	if (FAILED(hr)) {
		log("font createtexfromfileinmem failed??");
		return;
	}

	if (!res) {
		log("failed to load melty font resource");
		return;
	}

	hr = device->CreateTexture(fontTexWidth, fontTexHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, NULL);
	if (FAILED(hr)) {
		log("font createtexture failed");
		return;
	}

	IDirect3DSurface9* oldRenderTarget = nullptr;
	hr = device->GetRenderTarget(0, &oldRenderTarget);
	if (FAILED(hr)) {
		log("font getrendertarget failed");
		return;
	}

	hr = texture->GetSurfaceLevel(0, &surface);
	if (FAILED(hr)) {
		log("font getsurfacelevel failed");
		return;
	}

	hr = device->SetRenderTarget(0, surface);
	if (FAILED(hr)) {
		log("font setrendertarget failed");
		return;
	}

	IDirect3DPixelShader9* textOutlinePixelShader = getFontOutlinePixelShader();
	IDirect3DVertexShader9* textOutlineVertexShader = getFontOutlineVertexShader();

	//device->SetPixelShader(textOutlinePixelShader);
	//device->SetVertexShader(textOutlineVertexShader);

	D3DXVECTOR4 textureSize((float)fontTexWidth, (float)fontTexHeight, 0.0, 0.0);
	device->SetPixelShaderConstantF(219, (float*)&textureSize, 1);

	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	//device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0xFF, 0, 0, 0xFF), 1.0f, 0);

	VertexData<PosTexVert, 3 * 2> tempVertData(D3DFVF_XYZ | D3DFVF_TEX1, &meltyTex); // the deconstructor should handle this.	
	tempVertData.alloc();

	PosTexVert v1 = { D3DVECTOR(-1.0f, 1.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f) };
	PosTexVert v2 = { D3DVECTOR(1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 0.0f) };
	PosTexVert v3 = { D3DVECTOR(-1.0f, -1.0f, 0.5f), D3DXVECTOR2(0.0f, 1.0f) };
	PosTexVert v4 = { D3DVECTOR(1.0f, -1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f) };

	tempVertData.add(v1, v2, v3);
	tempVertData.add(v2, v3, v4);

	device->BeginScene();

	tempVertData.draw();

	device->EndScene();

	//hr = D3DXSaveTextureToFileA("meltyFontTest.png", D3DXIFF_PNG, texture, NULL);
	hr = D3DXSaveTextureToFileInMemory(&buffer, D3DXIFF_PNG, texture, NULL);

	BYTE* bufferPtr = (BYTE*)buffer->GetBufferPointer();
	size_t bufferSize = buffer->GetBufferSize();

	fontBufferMelty = (BYTE*)malloc(bufferSize);
	if (fontBufferMelty == NULL) {
		log("font malloc failed, what are you doing");
		return;
	}

	memcpy(fontBufferMelty, bufferPtr, bufferSize);
	fontBufferMeltySize = bufferSize;

	device->SetRenderTarget(0, oldRenderTarget);

	device->SetPixelShader(NULL);
	device->SetVertexShader(NULL);

	oldRenderTarget->Release();
	surface->Release();
	buffer->Release();
	texture->Release();

	textOutlinePixelShader->Release();
	textOutlineVertexShader->Release();

	meltyTex->Release();

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

	DWORD antiAliasBackup;
	device->GetRenderState(D3DRS_MULTISAMPLEANTIALIAS, &antiAliasBackup);
	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);

	//IDirect3DTexture9* fontTex = NULL;
	_initDefaultFont(fontTexture);
	if (fontTexture == NULL) {
		return;
	}

	//_initDefaultFontOutline(fontTex);
	_initMeltyFont();

	//fontTex->Release();

	log("loaded font BUFFER!!!");
	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, antiAliasBackup);
}

void initFont() {

	_initFontFirstLoad();

	/*
	if (fontBuffer == NULL) {
		log("initfont font buffer was null, dipping");
		return;
	}

	if (fontTexture != NULL) {
		fontTexture->Release();
		fontTexture = NULL;
	}
	*/

	if (fontTextureWithOutline != NULL) {
		fontTextureWithOutline->Release();
		fontTextureWithOutline = NULL;
	}

	if (fontTextureMelty != NULL) {
		fontTextureMelty->Release();
		fontTextureMelty = NULL;
	}

	HRESULT hr;

	// this texture is D3DPOOL_MANAGED, and so doesnt need a reset on every reset call! yipee
	// going to be real tho, what guarentee do i have that it is????
	/*hr = D3DXCreateTextureFromFileInMemory(device, fontBuffer, fontBufferSize, &fontTexture);
	if (FAILED(hr)) {
		log("default font createtexfromfileinmem failed??");
		return;
	}*/

	/*
	hr = D3DXCreateTextureFromFileInMemory(device, fontBufferWithOutline, fontBufferSizeWithOutline, &fontTextureWithOutline); // this texture is D3DPOOL_MANAGED, and so doesnt need a reset on every reset call! yipee
	if (FAILED(hr)) {
		log("default outline font createtexfromfileinmem failed??");
		return;
	}
	*/

	hr = D3DXCreateTextureFromFileInMemory(device, fontBufferMelty, fontBufferMeltySize, &fontTextureMelty);
	if (FAILED(hr)) {
		log("melty font createtexfromfileinmem failed??");
		return;
	}

	//hr = D3DXSaveTextureToFileA("fontTest.png", D3DXIFF_PNG, fontTexture, NULL);

	log("FONT TEXTURE LOADED");
}

// -----

typedef std::vector<BoxData> BoxList;

//typedef std::vector<BoxData> BoxObjects;
typedef std::array<BoxList, static_cast<int>(BoxType::_Count)> BoxObjects; // 2d array, index is what type of box

std::vector<BoxObjects*> boxObjectList;

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
DWORD _D3DRS_MULTISAMPLEANTIALIAS;
DWORD _D3DRS_ALPHATESTENABLE;
DWORD _D3DRS_ALPHAREF;
DWORD _D3DRS_ALPHAFUNC;
D3DMATRIX _D3DTS_VIEW;

float vWidth = 640;
float vHeight = 480;
float wWidth = 640;
float wHeight = 480;
bool isWide = false;
//float factor = 1.0f;
D3DVECTOR factor = { 1.0f, 1.0f, 1.0f };
D3DVECTOR topLeftPos = { 0.0f, 0.0f, 0.0f }; // top left of the screen, in pixel coords. maybe should be in directx space but idk
D3DVECTOR renderModificationFactor = { 1.0f, 1.0f, 1.0f };
//D3DVECTOR renderModificationOffset = { 1.0f, 1.0f, 1.0f };
D3DXVECTOR2 mouseTopLeft;
D3DXVECTOR2 mouseBottomRight;
D3DXVECTOR2 mouseFactor;

// -----

void __stdcall backupRenderState() {

	if (_hasStateToRestore) {
		return;
	}

	_hasStateToRestore = true;

	//IDirect3DSurface9* prevRenderTarget = NULL;
	//device->GetRenderTarget(0, &prevRenderTarget);
	//log("rendertarget: %08X", prevRenderTarget);
	//prevRenderTarget->Release();


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

	device->GetRenderState(D3DRS_MULTISAMPLEANTIALIAS, &_D3DRS_MULTISAMPLEANTIALIAS);
	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);

	device->GetRenderState(D3DRS_ALPHATESTENABLE, &_D3DRS_ALPHATESTENABLE);
	device->GetRenderState(D3DRS_ALPHAREF, &_D3DRS_ALPHAREF);
	device->GetRenderState(D3DRS_ALPHAFUNC, &_D3DRS_ALPHAFUNC);

	device->SetPixelShader(NULL);
	device->SetVertexShader(NULL);
	device->SetTexture(0, NULL);

	D3DVIEWPORT9 viewport;
	device->GetViewport(&viewport);
	vWidth = viewport.Width;
	vHeight = viewport.Height;
	//vWidth = (4.0f / 3.0f) * vHeight;

	HWND hwnd = (HWND) * (DWORD*)(0x0074dfac);

	if (hwnd != NULL) {
		RECT rect;
		if (GetClientRect(hwnd, &rect)) {
			wWidth = rect.right - rect.left;
			wHeight = rect.bottom - rect.top;
		}
	}

	const float ratio = 4.0f / 3.0f;

	isWide = wWidth / wHeight > ratio;

	factor.x = 1.0f;
	factor.y = 1.0f;

	if (isWide) {
		factor.x = (wHeight * ratio) / wWidth;
	}
	else {
		factor.y = (wWidth / ratio) / wHeight;
	}

	renderModificationFactor.x = 1.0f;
	renderModificationFactor.y = 1.0f;

	//renderModificationFactor.x = (vHeight * (4.0f / 3.0f)) / 640.0f;
	//renderModificationFactor.y = (vWidth / (4.0f / 3.0f)) / 480.0f;

	renderModificationFactor.x = (vHeight * (vWidth / vHeight)) / 640.0f;
	renderModificationFactor.y = (vWidth / (vWidth / vHeight)) / 480.0f;

	renderModificationFactor.x *= factor.x;
	renderModificationFactor.y *= factor.y;

	topLeftPos.x = 0.0f;
	topLeftPos.y = 0.0f;

	if (isWide) {
		topLeftPos.x = 640.0f / factor.x;
		topLeftPos.x *= (wWidth - (wHeight * ratio)) / (2.0f * wWidth);

	}
	else {
		topLeftPos.y = 480.0f / factor.y;
		topLeftPos.y *= (wHeight - (wWidth / ratio)) / (2.0f * wHeight);
	}

	mouseTopLeft.x = 0.0f;
	mouseTopLeft.y = 0.0f;

	// the border on top of the window is taken into account with this. 
	if (isWide) {
		mouseTopLeft.x = (wWidth - (wHeight * (4.0f / 3.0f))) / 2.0f;
	}
	else {
		mouseTopLeft.y = (wHeight - (wWidth / (4.0f / 3.0f))) / 2.0f;
	}

	mouseBottomRight.x = mouseTopLeft.x + (wHeight * (4.0f / 3.0f));
	mouseBottomRight.y = mouseTopLeft.y + (wWidth / (4.0f / 3.0f));

	if (isWide) {
		mouseFactor.x = 640.0f / (wHeight * (4.0f / 3.0f));
		mouseFactor.y = 480.0f / wHeight;
	}
	else {
		mouseFactor.x = 640.0f / wWidth;
		mouseFactor.y = 480.0f / (wWidth / (4.0f / 3.0f));
	}

	// this deals with caster setting the transform matrices. they only seem to use the view matrix? but it would maybe be good to do the others
	device->GetTransform(D3DTS_VIEW, &_D3DTS_VIEW);

	const D3DMATRIX defaultViewMatrix = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	device->SetTransform(D3DTS_VIEW, &defaultViewMatrix);


	POINT tempMousePos;
	RECT clientRect;
	if (GetCursorPos(&tempMousePos)) {
		HWND hwnd = (HWND) * (DWORD*)(0x0074dfac);
		ScreenToClient(hwnd, &tempMousePos);

		GetClientRect(hwnd, &clientRect);
		if (tempMousePos.x >= clientRect.left && tempMousePos.x < clientRect.right &&
			tempMousePos.y >= clientRect.top && tempMousePos.y < clientRect.bottom) {

			if (tempMousePos.x < mouseTopLeft.x ||
				tempMousePos.y < mouseTopLeft.y ||
				tempMousePos.x > mouseBottomRight.x ||
				tempMousePos.y > mouseBottomRight.y
				) {
				mousePos = { -100.0f, -100.0f };
			}

			mousePos = D3DXVECTOR2((float)tempMousePos.x, (float)tempMousePos.y);

			mousePos.x -= mouseTopLeft.x;
			mousePos.y -= mouseTopLeft.y;

			mousePos.x *= mouseFactor.x;
			mousePos.y *= mouseFactor.y;


		}
	}
	else {
		mousePos = { -100.0f, -100.0f };
	}

	//if (dragManager.hasDrag == NULL) {
	//	TextDraw(mousePos.x - 4.0f, mousePos.y - 4.0f, 32, 0x8042e5f4, "%c", CURSOR);
	//} else {										   
	//	TextDraw(mousePos.x - 4.0f, mousePos.y - 4.0f, 32, 0x8042e5f4, "%c", CURSOR_LOADING);
	//}

	// -----

	/*
	if (wWidth < (wHeight * 4.0f / 3.0f)) {
		x = xVal;
	}
	else {
		if (xVal < 300.0f) {
			x = -((wWidth - (wHeight * 4.0f / 3.0f)) / 2.0f) / wWidth;

			x *= (wWidth / (wHeight * 4.0f / 3.0f));
			x *= 640.0f;

			x += xVal;
		}
		else {
			x = ((wWidth - (wHeight * 4.0f / 3.0f)) / 2.0f) / wWidth;

			x *= (wWidth / (wHeight * 4.0f / 3.0f));
			x *= 640.0f;

			x = 640.0 + x - 100.0f;
		}
	}
	*/

}

void __stdcall restoreRenderState() {

	if (!_hasStateToRestore) {
		return;
	}

	_hasStateToRestore = false;

	device->SetTexture(0, _textureBackup);
	device->SetPixelShader(_pixelShaderBackup);
	device->SetVertexShader(_vertexShaderBackup);

	if (_textureBackup != NULL) {
		_textureBackup->Release();
	}

	if (_pixelShaderBackup != NULL) {
		_pixelShaderBackup->Release();
	}
	if (_vertexShaderBackup != NULL) {
		_vertexShaderBackup->Release();
	}

	device->SetRenderState(D3DRS_BLENDOP, _D3DRS_BLENDOP);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, _D3DRS_ALPHABLENDENABLE);
	device->SetRenderState(D3DRS_SRCBLEND, _D3DRS_SRCBLEND);
	device->SetRenderState(D3DRS_DESTBLEND, _D3DRS_DESTBLEND);
	device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, _D3DRS_SEPARATEALPHABLENDENABLE);
	device->SetRenderState(D3DRS_SRCBLENDALPHA, _D3DRS_SRCBLENDALPHA);
	device->SetRenderState(D3DRS_DESTBLENDALPHA, _D3DRS_DESTBLENDALPHA);

	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, _D3DRS_MULTISAMPLEANTIALIAS);

	device->SetRenderState(D3DRS_ALPHATESTENABLE, _D3DRS_ALPHATESTENABLE);
	device->SetRenderState(D3DRS_ALPHAREF, _D3DRS_ALPHAREF);
	device->SetRenderState(D3DRS_ALPHAFUNC, _D3DRS_ALPHAFUNC);

	device->SetTransform(D3DTS_VIEW, &_D3DTS_VIEW);

}

// -----

void LineDraw(float x1, float y1, float x2, float y2, DWORD ARGB, bool side) {

	x1 /= 480.0f;
	x2 /= 480.0f;
	y1 /= 480.0f;
	y2 /= 480.0f;

	// this is going to need to be changed at different resolutions
	float lineWidth = 1.0f / vHeight;

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
	}
	else {
		p3.x -= offset.x;
		p3.y -= offset.y;
		p4.x -= offset.x;
		p4.y -= offset.y;
	}

	p1.y = 1 - p1.y;
	p2.y = 1 - p2.y;
	p3.y = 1 - p3.y;
	p4.y = 1 - p4.y;

	PosColVert v1 = { D3DVECTOR((p1.x * 1.5f) - 1.0f, (p1.y * 2.0f) - 1.0f, 0.5f), ARGB };
	PosColVert v2 = { D3DVECTOR((p2.x * 1.5f) - 1.0f, (p2.y * 2.0f) - 1.0f, 0.5f), ARGB };
	PosColVert v3 = { D3DVECTOR((p3.x * 1.5f) - 1.0f, (p3.y * 2.0f) - 1.0f, 0.5f), ARGB };
	PosColVert v4 = { D3DVECTOR((p4.x * 1.5f) - 1.0f, (p4.y * 2.0f) - 1.0f, 0.5f), ARGB };

	scaleVertex(v1.position);
	scaleVertex(v2.position);
	scaleVertex(v3.position);
	scaleVertex(v4.position);

	posColVertData.add(v1, v2, v3);
	posColVertData.add(v2, v3, v4);

}

void RectDraw(float x, float y, float w, float h, DWORD ARGB) {

	x /= 480.0f;
	w /= 480.0f;
	y /= 480.0f;
	h /= 480.0f;

	y = 1 - y;

	PosColVert v1 = { ((x + 0) * 1.5f) - 1.0f, ((y + 0) * 2.0f) - 1.0f, 0.5f, ARGB };
	PosColVert v2 = { ((x + w) * 1.5f) - 1.0f, ((y + 0) * 2.0f) - 1.0f, 0.5f, ARGB };
	PosColVert v3 = { ((x + 0) * 1.5f) - 1.0f, ((y - h) * 2.0f) - 1.0f, 0.5f, ARGB };
	PosColVert v4 = { ((x + w) * 1.5f) - 1.0f, ((y - h) * 2.0f) - 1.0f, 0.5f, ARGB };

	scaleVertex(v1.position);
	scaleVertex(v2.position);
	scaleVertex(v3.position);
	scaleVertex(v4.position);

	posColVertData.add(v1, v2, v3);
	posColVertData.add(v2, v3, v4);
}

void BorderDraw(float x, float y, float w, float h, DWORD ARGB) {

	//x /= 480.0f;
	//w /= 480.0f;
	//y /= 480.0f;
	//h /= 480.0f;

	// this mult occurs here because linedraw has a div. need to be sure this is ok
	float lineWidth = 480.0f * (1.0f / vHeight);

	h -= lineWidth;
	w -= lineWidth;

	LineDraw(x + lineWidth, y, x + w, y, ARGB, true);
	LineDraw(x, y, x, y + h, ARGB, false);
	LineDraw(x + w, y, x + w, y + h, ARGB, false);
	LineDraw(x, y + h, x + w + lineWidth, y + h, ARGB, true);
}

void BorderRectDraw(float x, float y, float w, float h, DWORD ARGB) {

	RectDraw(x, y, w, h, ARGB);
	BorderDraw(x, y, w, h, ARGB | 0xFF000000);

}

// -----

void LineDrawBlend(float x1, float y1, float x2, float y2, DWORD ARGB, bool side) {


	MeltyVert v1 = { x1, y1, ARGB };
	MeltyVert v2 = { x2, y2, ARGB };

	scaleVertex(v1);
	scaleVertex(v2);

	//meltyVertData
	meltyLineData.add(v1, v2);

	return;

	/*
	x1 /= 480.0f;
	x2 /= 480.0f;
	y1 /= 480.0f;
	y2 /= 480.0f;

	// this is going to need to be changed at different resolutions
	float lineWidth = 1.0f / vHeight;

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
	}
	else {
		p3.x -= offset.x;
		p3.y -= offset.y;
		p4.x -= offset.x;
		p4.y -= offset.y;
	}

	p1.y = 1 - p1.y;
	p2.y = 1 - p2.y;
	p3.y = 1 - p3.y;
	p4.y = 1 - p4.y;

	PosColVert v1 = { D3DVECTOR((p1.x * 1.5f) - 1.0f, (p1.y * 2.0f) - 1.0f, 0.5f), ARGB };
	PosColVert v2 = { D3DVECTOR((p2.x * 1.5f) - 1.0f, (p2.y * 2.0f) - 1.0f, 0.5f), ARGB };
	PosColVert v3 = { D3DVECTOR((p3.x * 1.5f) - 1.0f, (p3.y * 2.0f) - 1.0f, 0.5f), ARGB };
	PosColVert v4 = { D3DVECTOR((p4.x * 1.5f) - 1.0f, (p4.y * 2.0f) - 1.0f, 0.5f), ARGB };

	scaleVertex(v1.position);
	scaleVertex(v2.position);
	scaleVertex(v3.position);
	scaleVertex(v4.position);

	posColVertData.add(v1, v2, v3);
	posColVertData.add(v2, v3, v4);
	*/

}

void RectDrawBlend(float x, float y, float w, float h, DWORD ARGB) {

	MeltyVert v1 = { x,     y,     ARGB };
	MeltyVert v2 = { x + w, y,     ARGB };
	MeltyVert v3 = { x,     y + h, ARGB };
	MeltyVert v4 = { x + w, y + h, ARGB };

	scaleVertex(v1);
	scaleVertex(v2);
	scaleVertex(v3);
	scaleVertex(v4);

	meltyVertData.add(v1, v2, v3);
	meltyVertData.add(v2, v3, v4);
}

void BorderDrawBlend(float x, float y, float w, float h, DWORD ARGB) {

	float lineWidth = 480.0f * (1.0f / vHeight);

	h -= lineWidth;
	w -= lineWidth;

	LineDrawBlend(x + lineWidth, y, x + w, y, ARGB, true);
	LineDrawBlend(x, y, x, y + h, ARGB, false);
	LineDrawBlend(x + w, y, x + w, y + h, ARGB, false);
	LineDrawBlend(x, y + h, x + w + lineWidth, y + h, ARGB, true);
}

void BorderRectDrawBlend(float x, float y, float w, float h, DWORD ARGB) {
	RectDrawBlend(x, y, w, h, ARGB);
	BorderDrawBlend(x, y, w, h, ARGB | 0xFF000000);
}

// -----

void TextDraw(float x, float y, float size, DWORD ARGB, const char* format, ...) {
	// i do hope that this allocing does not slow things down. i tried saving the va_args for when the actual print func was called, but it would not work

	if (format == NULL) {
		return;
	}

	size *= 2.0f;

	x /= 480.0f;
	y /= 480.0f;
	size /= 480.0f;

	static char buffer[1024];

	va_list args;
	va_start(args, format);
	vsnprintf(buffer, 1024, format, args);
	va_end(args);


	DWORD TempARGB = ARGB;

	if (fontTexture == NULL) {
		log("fontTexture was null, im not drawing");
		return;
	}

	DWORD antiAliasBackup;
	//device->GetRenderState(D3DRS_MULTISAMPLEANTIALIAS, &antiAliasBackup);
	//device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);

	float origX = x;
	float origY = y;

	float charWidthOffset = (fontRatio * size) * 1.0f;
	float charHeightOffset = size / 2.0; // melty font is weird

	float w = charWidthOffset;
	float h = charHeightOffset;

	float charWidth = ((float)(fontSize >> 1)) / (float)fontTexWidth;
	float charHeight = (((float)fontSize) / (float)fontTexWidth) / 2.0f;

	char* str = buffer;

	while (*str) {

		BYTE c = *str;

		charWidthOffset = w * 0.75;

		D3DXVECTOR2 charTopLeft(charWidth * (c & 0xF), charHeight * ((c - 0x00) / 0x10));

		D3DXVECTOR2 charW(charWidth, 0.0);
		D3DXVECTOR2 charH(0.0, charHeight);

		switch (c) {
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
		case '$': // gray
			TempARGB = 0xFF888888;
			str++;
			continue;
		case '}': // reset 
			TempARGB = ARGB;
			str++;
			continue;
		case '\\': // in case you want to print one of the above chars, you can escape them
			str++;
			if (c == '\0') {
				return;
			}
			break;
		case ARROW_1:
		case ARROW_2:
		case ARROW_3:
		case ARROW_4:
		case ARROW_6:
		case ARROW_7:
		case ARROW_8:
		case ARROW_9:
		case BUTTON_A:
		case BUTTON_B:
		case BUTTON_C:
		case BUTTON_D:
		case BUTTON_A_GRAY:
		case BUTTON_B_GRAY:
		case BUTTON_C_GRAY:
		case BUTTON_D_GRAY:
			charWidthOffset = w;
			break;
		case ARROW_0:
		case ARROW_5:
			charWidthOffset = w;
			break;
		case JOYSTICK:
			charWidthOffset = w;
			charW = D3DXVECTOR2(2.0f / 16.0f, 0.0);
			charH = D3DXVECTOR2(0.0, 2.0f / 16.0f);
			break;
		case CURSOR:
			charTopLeft.y = (8.0f + ((float)(directxFrameCount & 0b1))) / 16.0f;
			charWidthOffset = w * 4;
			charW = D3DXVECTOR2(4.0f / 16.0f, 0.0);
			charH = D3DXVECTOR2(0.0, 1.0f / 16.0f);
			break;
		case CURSOR_LOADING:
			charTopLeft.x = ((float)(directxFrameCount & 0b111)) / 8.0f;
			charWidthOffset = w * 2;
			charW = D3DXVECTOR2(2.0f / 16.0f, 0.0);
			charH = D3DXVECTOR2(0.0, 2.0f / 16.0f);
			break;
		default:
			break;
		}

		y = 1 - origY;

		PosColTexVert v1{ D3DVECTOR(((x + 0) * 1.5f) - 1.0f, ((y + 0) * 2.0f) - 1.0f, 0.5f), TempARGB, charTopLeft };
		PosColTexVert v2{ D3DVECTOR(((x + w) * 1.5f) - 1.0f, ((y + 0) * 2.0f) - 1.0f, 0.5f), TempARGB, charTopLeft + charW };
		PosColTexVert v3{ D3DVECTOR(((x + 0) * 1.5f) - 1.0f, ((y - h) * 2.0f) - 1.0f, 0.5f), TempARGB, charTopLeft + charH };
		PosColTexVert v4{ D3DVECTOR(((x + w) * 1.5f) - 1.0f, ((y - h) * 2.0f) - 1.0f, 0.5f), TempARGB, charTopLeft + charW + charH };

		scaleVertex(v1.position);
		scaleVertex(v2.position);
		scaleVertex(v3.position);
		scaleVertex(v4.position);

		posColTexVertData.add(v1, v2, v3);
		posColTexVertData.add(v2, v3, v4);

		x += charWidthOffset;
		str++;
	}

	//device->SetTexture(0, NULL);

	//device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, antiAliasBackup);

}

void TextDrawSimple(float x, float y, float size, DWORD ARGB, const char* format, ...) {

	if (format == NULL) {
		return;
	}


	x /= 480.0f;
	y /= 480.0f;
	size /= 480.0f;

	static char buffer[1024];

	va_list args;
	va_start(args, format);
	vsnprintf(buffer, 1024, format, args);
	va_end(args);


	DWORD TempARGB = ARGB;

	if (fontTexture == NULL) {
		log("fontTexture was null, im not drawing");
		return;
	}

	DWORD antiAliasBackup;

	float origX = x;
	float origY = y;

	float charWidthOffset = (fontRatio * size) * 1.01f; // this 1.01 might cause issues when aligning stuff, not sure
	float charHeightOffset = size;

	float w = charWidthOffset;
	float h = charHeightOffset;

	char* str = buffer;

	float maxX = 1.0f + ((wWidth - (wHeight * 4.0f / 3.0f)) / 2.0f) / wWidth;

	const float charWidth = ((float)(fontSize >> 1)) / (float)fontTexWidth;
	const float charHeight = ((float)fontSize) / (float)fontTexWidth;

	const float symbolWidth = charWidth;
	const float symbolHeight = charHeight / 2.0f;

	BYTE c;
	bool needDoubleWidth = false;

	D3DXVECTOR2 charTopLeft;

	D3DXVECTOR2 charW;
	D3DXVECTOR2 charH;

	while (*str) {

		c = *str;

		y = 1 - origY;

		switch (c) {
		case ' ':
			x += charWidthOffset;
			str++;
			continue;
		case ARROW_0:
		case ARROW_5:
			x += charWidthOffset;
			x += charWidthOffset;
			str++;
			continue;
		case ARROW_1:
		case ARROW_2:
		case ARROW_3:
		case ARROW_4:
		case ARROW_6:
		case ARROW_7:
		case ARROW_8:
		case ARROW_9:
		case BUTTON_A:
		case BUTTON_B:
		case BUTTON_C:
		case BUTTON_D:

			charTopLeft.x = charWidth * (*str & 0xF);
			charTopLeft.y = (charHeight * 0x6) + (symbolHeight * ((c - 0x80) / 0x10));

			charW = D3DXVECTOR2(symbolWidth, 0.0);
			charH = D3DXVECTOR2(0.0, symbolHeight);

			needDoubleWidth = true;
			w *= 2.0f;

			break;
		default:
			charTopLeft = D3DXVECTOR2(charWidth * (c & 0xF), charHeight * ((c - 0x20) / 0x10));
			charW = D3DXVECTOR2(charWidth, 0.0);
			charH = D3DXVECTOR2(0.0, charHeight);
			break;
		}

		PosTexVert v1{ D3DVECTOR(((x + 0) * 1.5f) - 1.0f, ((y + 0) * 2.0f) - 1.0f, 0.5f), charTopLeft };
		PosTexVert v2{ D3DVECTOR(((x + w) * 1.5f) - 1.0f, ((y + 0) * 2.0f) - 1.0f, 0.5f), charTopLeft + charW };
		PosTexVert v3{ D3DVECTOR(((x + 0) * 1.5f) - 1.0f, ((y - h) * 2.0f) - 1.0f, 0.5f), charTopLeft + charH };
		PosTexVert v4{ D3DVECTOR(((x + w) * 1.5f) - 1.0f, ((y - h) * 2.0f) - 1.0f, 0.5f), charTopLeft + charW + charH };

		scaleVertex(v1.position);
		scaleVertex(v2.position);
		scaleVertex(v3.position);
		scaleVertex(v4.position);

		if (v1.position.x < maxX) {
			posTexVertData.add(v1, v2, v3);
			posTexVertData.add(v2, v3, v4);
		}

		x += charWidthOffset;
		if (needDoubleWidth) {
			needDoubleWidth = false;
			x += charWidthOffset;
			w /= 2.0f;
		}

		str++;
	}
}

void joystickDraw(float x, float y, float size, DWORD ARGB) {

	D3DXVECTOR2 charTopLeft(7.0f / 16.0f, 9.0f / 16.0f);
	D3DXVECTOR2 charW(2.0f / 16.0f, 0.0f / 16.0f);
	D3DXVECTOR2 charH(0.0f / 16.0f, 2.0f / 16.0f);

	Quad<MeltyVert> joystick(
		MeltyVert(x - 0, y - 0, charTopLeft),
		MeltyVert(x + size, y - 0, charTopLeft + charW),
		MeltyVert(x - 0, y + size, charTopLeft + charH),
		MeltyVert(x + size, y + size, charTopLeft + charW + charH),
		ARGB
	);

	meltyVertData.add(joystick);
}

void cursorDraw() {

	D3DXVECTOR2 charTopLeft;
	D3DXVECTOR2 charW;
	D3DXVECTOR2 charH;

	float posX = mousePos.x;
	float posY = mousePos.y;
	// if the compiler doesnt precompute these floats i will scream.

	//if (dragManager.hasDrag == NULL) {

	float size = 16.0f;

	posX += size / 3.9f;
	posY += size / 2.1f;

	charTopLeft = D3DXVECTOR2(10.0f / 16.0f, 8.0f / 16.0f);
	//charTopLeft.y += ((float)(directxFrameCount & 0b1)) / 16.0f;
	charW = D3DXVECTOR2(1.0f / 16.0f, 0.0);
	charH = D3DXVECTOR2(0.0, 1.0f / 16.0f);

	Quad<MeltyVert> cursor(
		MeltyVert(-size, -size, charTopLeft),
		MeltyVert(size, -size, charTopLeft + charW),
		MeltyVert(-size, size, charTopLeft + charH),
		MeltyVert(size, size, charTopLeft + charW + charH),
		0xFFFFFFFF
	);

	float angle = 00.0f * (3.1415926535 / 180.0f);
	float c = cos(angle);
	float s = sin(angle);

	for (int i = 0; i < 4; i++) {

		float xBack = cursor.verts[i].x;
		float yBack = cursor.verts[i].y;

		cursor.verts[i].x = posX + (xBack * c) - (yBack * s);
		cursor.verts[i].y = posY + (xBack * s) + (yBack * c);
	}

	meltyVertData.add(cursor);

	/*
	} else {

		float size = 22.627;
		DWORD col = 0xFFe0000a;

		posX -= size / 64.0f;
		posY -= size / 64.0f;

		posX += size / 8.0f;
		posY += size / 8.0f;

		charTopLeft = D3DXVECTOR2(((float)(directxFrameCount & 0b111)) / 8.0f, 0.0f / 16.0f);
		charTopLeft.x += (1.0f / 512.0f); // for the loading cursor, make sure to add 1/512 to x TO ACCOUNT FOR THAT ONE WHITE PIXEL!
		charW = D3DXVECTOR2(2.0f / 16.0f, 0.0);
		charH = D3DXVECTOR2(0.0, 2.0f / 16.0f);


		Quad<MeltyVert> cursor(
			MeltyVert(posX - size, posY - size, charTopLeft),
			MeltyVert(posX + size, posY - size, charTopLeft + charW),
			MeltyVert(posX - size, posY + size, charTopLeft + charH),
			MeltyVert(posX + size, posY + size, charTopLeft + charW + charH),
			col
		);

		meltyVertData.add(cursor);
	}
	*/


}

IDirect3DPixelShader9* getOutlinePixelShader() {
	return createPixelShader(R"(
			sampler2D textureSampler : register(s0);
			float4 texSize : register(c219);

			float4 main(float2 texCoordIn : TEXCOORD0) : COLOR {

					float2 texOffset = 1.0 / texSize;

					texOffset.y /= (4.0 / 3.0); // THIS LINE IS CORRECT. 

					float2 texCoord = texCoordIn + (texOffset * 0.5); // move the tex coord into the "center" of the pixel, should it be plus or minus???
					
					float4 texColor = tex2D(textureSampler, texCoord);
				
					if(texColor.a <= 0.0) {
						return texColor;
					}
					
					// should diags have a radic(2) instead of 1,1?
	
					float2 offsets[8] = { // order is adjusted to check diags last. performance.
						
						texCoord + float2(-texOffset.x, 0.0),
						texCoord + float2(0.0, -texOffset.y),
						texCoord + float2(0.0, texOffset.y),
						texCoord + float2(texOffset.x, 0.0),						

						texCoord + float2(texOffset.x, -texOffset.y),
						texCoord + float2(-texOffset.x, -texOffset.y),
						texCoord + float2(-texOffset.x, texOffset.y),
						texCoord + float2(texOffset.x, texOffset.y)
					};


					float4 tempColor;
					
					// having to go through all 8 cases fucks perfomance
					// ooooo 
					[unroll(8)] for(int i=0; i<8; i++) {

						tempColor = tex2D(textureSampler, offsets[i]);
						if(tempColor.a == 0) {
							return float4(texColor.rgb, 0.875); // putting this alpha at 0.75 might be a bit whack, but will help with overlay
						}

					}
			
					return float4(texColor.rgb, 0.1875);
			}

		)");
}

IDirect3DVertexShader9* getOutlineVertexShader() {
	return createVertexShader(R"(
			struct VS_INPUT {
				float4 position : POSITION;
				float2 texCoord : TEXCOORD0;
			};
    
			struct VS_OUTPUT {
				float4 position : POSITION;
				float2 texCoord : TEXCOORD0;
			};
    
			VS_OUTPUT main(VS_INPUT input) {
				VS_OUTPUT output;
				output.position = input.position;
				output.texCoord = input.texCoord;
				return output;
			}

		)");
}

IDirect3DPixelShader9* getColorPixelShader() {
	return createPixelShader(R"(
			float4 shadeColor : register(c218);
			float4 main() : COLOR {
				return shadeColor;
			}
	)");
}

IDirect3DVertexShader9* getColorVertexShader() {
	return createVertexShader(R"(
			struct VS_INPUT {
				float4 position : POSITION;
			};
    
			struct VS_OUTPUT {
				float4 position : POSITION;
			};
    
			VS_OUTPUT main(VS_INPUT input) {
				VS_OUTPUT output;
				output.position = input.position;
				return output;
			}

		)");
}

IDirect3DTexture9* renderTargetTex = NULL;

void drawSingleHitbox(const BoxData& box, DWORD ARGB, bool shade) {
	if (shade) {
		RectDraw(box.x, box.y, box.w, box.h, (ARGB & 0x00FFFFFF) | 0x30000000); // make sure to sync these alpha values with the ones in the outline shader
	}
	BorderDraw(box.x, box.y, box.w, box.h, (ARGB & 0x00FFFFFF) | 0xE0000000);
}

void drawSingleHitboxBlend(const BoxData& box, DWORD ARGB, bool shade) {
	if (shade) {
		RectDrawBlend(box.x, box.y, box.w, box.h, (ARGB & 0x00FFFFFF) | 0x30000000); // make sure to sync these alpha values with the ones in the outline shader
	}
	BorderDrawBlend(box.x, box.y, box.w, box.h, (ARGB & 0x00FFFFFF) | 0xE0000000);
}

void drawBatchHitboxes(const BoxList& boxList, DWORD ARGB) {
	HRESULT hr;

	/*
	static HRESULT prevhr = 0xFFFFFFFF;
	hr = device->TestCooperativeLevel();

	if (prevhr != hr) {
		log("hresult: %08X", hr);
		printDirectXError(hr);
		prevhr = hr;
	}


	if (hr == D3DERR_DEVICELOST ) { // this is critical, and should probs be put at the start of every directx func
		if (renderTargetTex != NULL) {
			renderTargetTex->Release();
			renderTargetTex = NULL;
		}
		return;
	}
	*/

	if (boxList.size() == 0) {
		return;
	}

	if (boxList.size() == 1) {
		drawSingleHitboxBlend(boxList[0], ARGB);
		return;
	}

	// draws a set of hitboxes of ONE color for ONE object. 


	IDirect3DSurface9* renderTargetSurf = NULL;
	static int width = 0;
	static int height = 0;
	if (renderTargetTex == NULL) {

		IDirect3DSwapChain9* pSwapChain = NULL;
		D3DPRESENT_PARAMETERS presentParams;

		hr = device->GetSwapChain(0, &pSwapChain);

		if (FAILED(hr)) {
			log("get swap chain failed?");
			printDirectXError(hr);
		}

		if (pSwapChain != NULL) {

			hr = pSwapChain->GetPresentParameters(&presentParams);

			width = presentParams.BackBufferWidth;
			height = presentParams.BackBufferHeight;

			//log("creating texture with %d %d", width, height);

			pSwapChain->Release();

			device->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &renderTargetTex, NULL);

			// shader registers are RESET on device loss!
			// i should have been a farmer.
			// it gets traumatic after a while.
			D3DXVECTOR4 textureSize((float)(height * 4.0f / 3.0f), (float)height, 0.0, 0.0);
			device->SetPixelShaderConstantF(219, (float*)&textureSize, 1);

			//hr = device->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &renderTargetTex, NULL);
			if (FAILED(hr)) {
				printDirectXError(hr);
			}
		}
	}

	if (renderTargetTex == NULL) {
		log("hitbox rendertarget null?");
		return;
	}

	static VertexData<PosVert, 3 * 2 * 256> vertData(D3DFVF_XYZ);
	//static VertexData<PosColVert, 3 * 2 * 256> vertData(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	vertData.alloc();

	IDirect3DSurface9* prevRenderTarget = NULL;
	device->GetRenderTarget(0, &prevRenderTarget);

	renderTargetTex->GetSurfaceLevel(0, &renderTargetSurf);
	device->SetRenderTarget(0, renderTargetSurf);

	// might need to do this?
	//device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	// one could save, possibly time, but only clearing what needs to be cleared.
	// but is it worth?

	//device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	/*
	static D3DRECT needClearRects[32];
	static unsigned needClearRectsIndex = 0;
	if (needClearRectsIndex != 0) {
		device->Clear(needClearRectsIndex, &needClearRects[0], D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
		needClearRectsIndex = 0;
	}*/

	static D3DRECT clearRect = { 0, 0, width, height };
	//device->Clear(1, &clearRect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	// WHEN RESETING TO A NON CENTERED CAMERA, THIS FUCKS UP. FIX IT
	device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);


	clearRect = { width, height, 0, 0 }; // these are the hypothetical opposite max/min values each could have

	static IDirect3DPixelShader9* pColorShader = NULL;
	static IDirect3DVertexShader9* vColorShader = NULL;

	if (pColorShader == NULL) {
		pColorShader = getColorPixelShader();
	}

	if (vColorShader == NULL) {
		vColorShader = getColorVertexShader();
	}

	// this can and maybe should be done with a pixel shader

	for (int i = 0; i < boxList.size(); i++) {
		//_drawRectUnscaled(boxList[i].x / 640.0f, boxList[i].y / 480.0f, boxList[i].w / 640.0f, boxList[i].h / 480.0f, ARGB);

		float x = boxList[i].x / 640.0f;
		float y = 1 - (boxList[i].y / 480.0f);

		float w = boxList[i].w / 640.0f;
		float h = boxList[i].h / 480.0f;

		clearRect.x1 = MIN(clearRect.x1, floor(x * width));
		clearRect.y1 = MIN(clearRect.y1, floor((1 - y) * height));
		clearRect.x2 = MAX(clearRect.x2, ceil(width * (x + w)));
		clearRect.y2 = MAX(clearRect.y2, ceil(height * ((1 - y) + h)));

		/*
		needClearRects[needClearRectsIndex].x1 = floor(x * width);
		needClearRects[needClearRectsIndex].y1 = floor( (1 - y)* height);
		needClearRects[needClearRectsIndex].x2 = ceil(width * (x + w));
		needClearRects[needClearRectsIndex].y2 = ceil(height * ((1 - y) + h));
		needClearRectsIndex++;
		*/

		PosVert v1 = { ((x + 0) * 2.0f) - 1.0f, ((y + 0) * 2.0f) - 1.0f, 0.5f };
		PosVert v2 = { ((x + w) * 2.0f) - 1.0f, ((y + 0) * 2.0f) - 1.0f, 0.5f };
		PosVert v3 = { ((x + 0) * 2.0f) - 1.0f, ((y - h) * 2.0f) - 1.0f, 0.5f };
		PosVert v4 = { ((x + w) * 2.0f) - 1.0f, ((y - h) * 2.0f) - 1.0f, 0.5f };

		vertData.add(v1, v2, v3);
		vertData.add(v2, v3, v4);
	}

	device->BeginScene();
	device->SetTexture(0, NULL);
	device->SetPixelShader(pColorShader);
	device->SetVertexShader(vColorShader);

	D3DXVECTOR4 rectColor;
	rectColor.x = (float)((ARGB & 0x00FF0000) >> 16) / 255.0f;
	rectColor.y = (float)((ARGB & 0x0000FF00) >> 8) / 255.0f;
	rectColor.z = (float)((ARGB & 0x000000FF) >> 0) / 255.0f;
	rectColor.w = 1.0f;

	device->SetPixelShaderConstantF(218, (float*)&rectColor, 1);

	vertData.draw();

	device->EndScene();

	if (prevRenderTarget != NULL) {
		device->SetRenderTarget(0, prevRenderTarget);
		prevRenderTarget->Release();
		prevRenderTarget = NULL;
	}
	else {
		device->SetRenderTarget(0, NULL);
	}

	if (renderTargetSurf != NULL) {
		renderTargetSurf->Release();
		renderTargetSurf = NULL;
	}

	// -----

	static IDirect3DPixelShader9* pOutlineShader = NULL; // edge shader
	static IDirect3DVertexShader9* vOutlineShader = NULL;

	if (pOutlineShader == NULL) {
		pOutlineShader = getOutlinePixelShader();
	}

	if (vOutlineShader == NULL) {
		vOutlineShader = getOutlineVertexShader();
	}

	device->BeginScene();

	// actually go and render what we just made to a real rendertarget
	// depressingly expensive.

	device->SetTexture(0, renderTargetTex);
	device->SetPixelShader(pOutlineShader);
	device->SetVertexShader(vOutlineShader);

	// actually draw that texture to the screen now

	const DWORD vertFormat = (D3DFVF_XYZ | D3DFVF_TEX1);

	struct CUSTOMVERTEX {
		D3DVECTOR position;
		D3DXVECTOR2 texCoord;
	};

	constexpr float whatIsThis = 0.5f; // what is this?


	CUSTOMVERTEX vertices[] = {
		{ D3DVECTOR(-1.0f, 1.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DVECTOR(1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DVECTOR(-1.0f, -1.0f, 0.5f), D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DVECTOR(1.0f, -1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f) }
	};

	scaleVertex(vertices[0].position);
	scaleVertex(vertices[1].position);
	scaleVertex(vertices[2].position);
	scaleVertex(vertices[3].position);

	static LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL; // not reallocing this buffer constantly was HUGE for performance!

	if (v_buffer == NULL) {
		if (FAILED(device->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX), 0, vertFormat, D3DPOOL_MANAGED, &v_buffer, NULL))) {
			v_buffer = NULL;
			log("drawBatchHitboxes createvertbuffer failed");
			return;
		}
	}

	VOID* pVoid;

	v_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	v_buffer->Unlock();

	device->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	device->SetFVF(vertFormat);
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	device->EndScene();

	device->SetPixelShader(NULL);
	device->SetVertexShader(NULL);
	device->SetTexture(0, NULL);
}

void HitboxBatchDrawNoBlend(const BoxObjects* b) {

	const DWORD* arrColors;
	if (*(uint8_t*)(dwBaseAddress + adSharedColorBlindMode))
		arrColors = arrColorBlindColors;
	else
		arrColors = arrNormalColors;

	int i;


	i = static_cast<int>(BoxType::Collision);
	if ((*b)[i].size() == 1) {
		drawSingleHitbox((*b)[i][0], arrColors[i], false);
	}

	i = static_cast<int>(BoxType::Hurtbox);
	for (int j = 0; j < (*b)[i].size(); j++) {
		drawSingleHitbox((*b)[i][j], arrColors[i]);
	}

	i = static_cast<int>(BoxType::Hitbox);
	for (int j = 0; j < (*b)[i].size(); j++) {
		drawSingleHitbox((*b)[i][j], arrColors[i]);
		//BoxData box = (*b)[i][j];
		//log("%6.3f %6.3f %6.3f %6.3f", box.x / 480.0f, box.y / 480.0f, (box.x / 480.0f) + (box.w / 480.0f), (box.y	 / 480.0f) + (box.h / 480.0f));
	}
	//log("-----");

	i = static_cast<int>(BoxType::Blue);
	if ((*b)[i].size() == 1) {
		drawSingleHitbox((*b)[i][0], arrColors[i]);
	}

	i = static_cast<int>(BoxType::Clash);
	if ((*b)[i].size() == 1) {
		drawSingleHitbox((*b)[i][0], arrColors[i]);
	}

	i = static_cast<int>(BoxType::Shield);
	if ((*b)[i].size() == 1) {
		drawSingleHitbox((*b)[i][0], arrColors[i]);
	}

	i = static_cast<int>(BoxType::Origin);
	if ((*b)[i].size() == 1) {

		if (*(uint8_t*)(dwBaseAddress + adSharedExtendOrigins)) {
			//_drawLine2(0.0f, ((*b)[i][0].y + (*b)[i][0].h) / 480.0f, 1.3333f, ((*b)[i][0].y + (*b)[i][0].h) / 480.0f, arrColors[i]);
			//_drawLine2(((*b)[i][0].x + (*b)[i][0].w / 2.0f) / 480.0f, 0.0f, ((*b)[i][0].x + (*b)[i][0].w / 2.0f) / 480.0f, 1.0f, arrColors[i]);
			LineDraw(0.0f, ((*b)[i][0].y + (*b)[i][0].h), 640.0f, ((*b)[i][0].y + (*b)[i][0].h), arrColors[i]);
			LineDraw(((*b)[i][0].x + (*b)[i][0].w / 2.0f), 0.0f, ((*b)[i][0].x + (*b)[i][0].w / 2.0f), 480.0f, arrColors[i]);
		}
		else {
			//_drawLine2((*b)[i][0].x / 480.0f, ((*b)[i][0].y + (*b)[i][0].h) / 480.0f, ((*b)[i][0].x + (*b)[i][0].w) / 480.0f, ((*b)[i][0].y + (*b)[i][0].h) / 480.0f, arrColors[i]);
			//_drawLine2(((*b)[i][0].x + (*b)[i][0].w / 2.0f) / 480.0f, (*b)[i][0].y / 480.0f, ((*b)[i][0].x + (*b)[i][0].w / 2.0f) / 480.0f, ((*b)[i][0].y + (*b)[i][0].h) / 480.0f, arrColors[i]);
			LineDraw((*b)[i][0].x, ((*b)[i][0].y + (*b)[i][0].h), ((*b)[i][0].x + (*b)[i][0].w), ((*b)[i][0].y + (*b)[i][0].h), arrColors[i]);
			LineDraw(((*b)[i][0].x + (*b)[i][0].w / 2.0f), (*b)[i][0].y, ((*b)[i][0].x + (*b)[i][0].w / 2.0f), ((*b)[i][0].y + (*b)[i][0].h), arrColors[i]);
		}
	}

}

void HitboxBatchDrawBlend(const BoxObjects* b) {

	// attemmpt with CPU for box collisions

	/*

	instead of checking rect collisions, check lines instead?
	i could graph theory it as well? (might be overkill with memory alloc

	altho, how many boxes on average am i getting per list (and per entity)
	hell i could also use a tristrip(maybe just tris to not overcomplicate things) to draw each box at opacity and not need a pixel shader step

	my desire for optimization has instead left me with trash code

	i can do multiple passes over the list for each box type, or like, have an array again

	*/


	// i could have avoided a div stage, but ugh, another time

	const DWORD* arrColors;
	if (*(uint8_t*)(dwBaseAddress + adSharedColorBlindMode))
		arrColors = arrColorBlindColors;
	else
		arrColors = arrNormalColors;


	// todo, consolidate all these calls! grouping prim draws is better.

	int i;

	i = static_cast<int>(BoxType::Collision);
	if ((*b)[i].size() == 1) {
		drawSingleHitboxBlend((*b)[i][0], arrColors[i], false);
	}

	i = static_cast<int>(BoxType::Hurtbox);
	drawBatchHitboxes((*b)[i], arrColors[i]);

	i = static_cast<int>(BoxType::Hitbox);
	drawBatchHitboxes((*b)[i], arrColors[i]);

	i = static_cast<int>(BoxType::Blue);
	if ((*b)[i].size() == 1) {
		drawSingleHitboxBlend((*b)[i][0], arrColors[i]);
	}

	i = static_cast<int>(BoxType::Clash);
	if ((*b)[i].size() == 1) {
		drawSingleHitboxBlend((*b)[i][0], arrColors[i]);
	}

	i = static_cast<int>(BoxType::Shield);
	if ((*b)[i].size() == 1) {
		drawSingleHitboxBlend((*b)[i][0], arrColors[i]);
	}

	i = static_cast<int>(BoxType::Origin);
	if ((*b)[i].size() == 1) {
		if (*(uint8_t*)(dwBaseAddress + adSharedExtendOrigins)) {
			//LineDrawBlend(0.0f, ((*b)[i][0].y + (*b)[i][0].h), 640.0f, ((*b)[i][0].y + (*b)[i][0].h), arrColors[i]);
			//LineDrawBlend(((*b)[i][0].x + (*b)[i][0].w / 2.0f), 0.0f, ((*b)[i][0].x + (*b)[i][0].w / 2.0f), 480.0f, arrColors[i]);
			LineDrawBlend(0.0f, ((*b)[i][0].y + (*b)[i][0].h), 640.0f, ((*b)[i][0].y + (*b)[i][0].h), arrColors[i]);
			LineDrawBlend(((*b)[i][0].x + (*b)[i][0].w / 2.0f), 0.0f, ((*b)[i][0].x + (*b)[i][0].w / 2.0f), 480.0f, arrColors[i]);
		}
		else {
			//LineDrawBlend((*b)[i][0].x, ((*b)[i][0].y + (*b)[i][0].h), ((*b)[i][0].x + (*b)[i][0].w), ((*b)[i][0].y + (*b)[i][0].h), arrColors[i]);
			//LineDrawBlend(((*b)[i][0].x + (*b)[i][0].w / 2.0f), (*b)[i][0].y, ((*b)[i][0].x + (*b)[i][0].w / 2.0f), ((*b)[i][0].y + (*b)[i][0].h), arrColors[i]);
			LineDrawBlend((*b)[i][0].x, ((*b)[i][0].y + (*b)[i][0].h), ((*b)[i][0].x + (*b)[i][0].w), ((*b)[i][0].y + (*b)[i][0].h), arrColors[i]);
			LineDrawBlend(((*b)[i][0].x + (*b)[i][0].w / 2.0f), (*b)[i][0].y, ((*b)[i][0].x + (*b)[i][0].w / 2.0f), ((*b)[i][0].y + (*b)[i][0].h), arrColors[i]);
		}
	}

}

// ----- horrid Profiler, as a treat 

std::vector<std::function<void(void)>> drawCalls;
std::map<const char*, ProfileInfo > profilerData;

// -----

void DrawHitboxes(BoxObjects* b) { // didnt need to be a pointer, but i would rather know im not passing by copy. also the optimizer better inline this(who am i kiding, its def not)
	//boxDataList[static_cast<int>(type)].emplace_back(BoxData{x / 640.0f, y / 480.0f, w / 640.0f, h / 480.0f, player });
	//boxDataList[static_cast<int>(type)].emplace_back(BoxData{ x, y , w , h }); 
	boxObjectList.emplace_back(b);
}

char* logHistory[logHistorySize] = { NULL };
int logHistoryIndex = 0;
void DrawLog(char* s) {
	// will be alloced in the log func, and dealloced here.
	//logHistory.push_front(s);
	if (logHistory[logHistoryIndex] != NULL) {
		free(logHistory[logHistoryIndex]);
	}
	logHistory[logHistoryIndex] = s;
	logHistoryIndex = (logHistoryIndex + 1) % logHistorySize;
}

// -----

void _drawHitboxes() {

	profileFunction();

	DWORD antiAliasBackup;
	device->GetRenderState(D3DRS_MULTISAMPLEANTIALIAS, &antiAliasBackup);
	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);

	if (*(uint8_t*)(dwBaseAddress + adSharedHitboxStyle)) {
		for (int i = 0; i < boxObjectList.size(); i++) {
			HitboxBatchDrawBlend(boxObjectList[i]);
			delete boxObjectList[i];
		}
	}
	else {
		for (int i = 0; i < boxObjectList.size(); i++) {
			HitboxBatchDrawNoBlend(boxObjectList[i]);
			delete boxObjectList[i];
		}
	}

	boxObjectList.clear();

	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, antiAliasBackup);

}

void _drawProfiler() {
	static char buffer[1024];
	static bool drawDebug = false;
	static KeyState F10Key(VK_F10);

	if (F10Key.keyDown()) {
		drawDebug = !drawDebug;
	}

	DWORD col = 0xFF00FFFF;

	if (drawDebug) {
		// how slow is this,,, im not sure how expensive map lookups are, but the keys to it could be constexpr. ugh 
		// i also have the pointer strat. might go do that tbh
		float profileInfoY = 256;
		for (auto& [name, info] : profilerData) {

			info.times[info.index] = info.currentFrameTime;

			long long totalTime = 0;
			for (int i = 0; i < 64; i++) {
				totalTime += info.times[i];
			}
			totalTime >>= 6; // div 64

			col = 0xFF000000;

			col |= CLAMP(0xFF - ((totalTime) >> 1), 0, 0xFF);
			col |= CLAMP(-(abs(totalTime - 500) >> 1) + 0xFF, 0, 0xFF) << 8;
			col |= CLAMP(((totalTime) >> 1) - 0xFF, 0, 0xFF) << 16;

			//TextDraw(0, profileInfoY, 12, 0xFFFFFFFF, "%3lld.%03lld %5d %.32s", totalTime / 1000, totalTime % 1000, info.callCount, name == NULL ? "NULL" : name);
			snprintf(buffer, 1024, "%3lld.%03lld %5d %.32s", totalTime / 1000, totalTime % 1000, info.callCount, name == NULL ? "NULL" : name);
			TextDraw(-0.0f, (float)profileInfoY, 12.0f, col, buffer);

			//TextDraw(200, profileInfoY, 12, 0x7FFFFFFF, "%3lld.%03lld %5d %.32s", totalTime / 1000, totalTime % 1000, info.callCount, name == NULL ? "NULL" : name);

			//TextDraw(400, profileInfoY, 12, 0x10FFFFFF, "%3lld.%03lld %5d %.32s", totalTime / 1000, totalTime % 1000, info.callCount, name == NULL ? "NULL" : name);

			info.currentFrameTime = 0;
			info.callCount = 0;

			// i dont trust MSVC to actually do this
			info.index = (info.index + 1) & 63;

			profileInfoY += 14;
		}
	}
	else {
		for (auto& [name, info] : profilerData) {
			info.currentFrameTime = 0;
		}
	}
}

void _drawLog() {

	static bool drawDebug = false;
	static KeyState F11Key(VK_F11);

	if (F11Key.keyDown()) {
		drawDebug = !drawDebug;
	}

	if (!drawDebug) {
		return;
	}

	float y = 0.0f;

	for (int i = 0; i < logHistorySize; i++) {
		int index = (i + logHistoryIndex) % logHistorySize;
		if (logHistory[index] == NULL) {
			continue;
		}
		TextDrawSimple(640.0f, y, 10.0f, 0xFF42e5f4, logHistory[index]);
		y += 10.0f;
		if (y > 480.0f) {
			break;
		}
	}

}

bool debugMode = false;
bool verboseMode = false;
bool overkillVerboseMode = false;
void _drawMiscInfo() {

	static bool drawDebug = false;
	static KeyState F12Key(VK_F12);

	if (F12Key.keyDown()) {
		drawDebug = !drawDebug;
	}

	static KeyState F9Key(VK_F9);
	static KeyState F8Key(VK_F8);
	static KeyState F7Key(VK_F7);

	float x;

	if (F9Key.keyDown()) {
		debugMode = !debugMode;
	}

	if (F8Key.keyDown()) {
		verboseMode = !verboseMode;
	}

	if (F7Key.keyDown()) {
		overkillVerboseMode = !overkillVerboseMode;
	}

	if (debugMode) {


		PosColVert v1 = { D3DVECTOR(0.0,       0.0, 0.5f), 0xFFfefa79 };
		PosColVert v2 = { D3DVECTOR(0.471,   0.463, 0.5f), 0xFFfefa79 };
		PosColVert v3 = { D3DVECTOR(0.3,       0.2, 0.5f), 0xFFfefa79 };
		PosColVert v4 = { D3DVECTOR(0.0,       0.8, 0.5f), 0xFFfefa79 };
		PosColVert v5 = { D3DVECTOR(0.23,    0.889, 0.5f), 0xFFfefa79 };
		PosColVert v6 = { D3DVECTOR(0.112,   0.872, 0.5f), 0xFFfefa79 };
		PosColVert v7 = { D3DVECTOR(0.38,     0.86, 0.5f), 0xFFfefa79 };
		PosColVert v8 = { D3DVECTOR(0.48,     0.74, 0.5f), 0xFFfefa79 };

		PosColVert* data[8] = { &v1, &v2, &v3, &v4, &v5, &v6, &v7, &v8 };

#define _drawIdkDefine(c) for(int i=0; i<8; i++) { c; }

		_drawIdkDefine(data[i]->position.x /= 16.0f);
		_drawIdkDefine(data[i]->position.y /= 16.0f);
		_drawIdkDefine(data[i]->position.x /= 1.3333f);

		scaleVertex(v1.position);
		scaleVertex(v2.position);
		scaleVertex(v3.position);
		scaleVertex(v4.position);
		scaleVertex(v5.position);
		scaleVertex(v6.position);
		scaleVertex(v7.position);
		scaleVertex(v8.position);

		_drawIdkDefine(data[i]->position.x += 0.95f);
		_drawIdkDefine(data[i]->position.y -= 0.95f);

		posColVertData.add(v1, v2, v3);
		posColVertData.add(v1, v2, v4);
		posColVertData.add(v2, v4, v5);
		posColVertData.add(v2, v5, v7);
		posColVertData.add(v2, v7, v8);
		posColVertData.add(v4, v5, v6);

		_drawIdkDefine(data[i]->position.x *= -1.0f);
		_drawIdkDefine(data[i]->position.x += 1.9f);

		posColVertData.add(v1, v2, v3);
		posColVertData.add(v1, v2, v4);
		posColVertData.add(v2, v4, v5);
		posColVertData.add(v2, v5, v7);
		posColVertData.add(v2, v7, v8);
		posColVertData.add(v4, v5, v6);

	}

	if (!drawDebug) {
		return;
	}

	// reversing these transforms as i am doing feels gross.
	x = -((wWidth - (wHeight * 4.0f / 3.0f)) / 2.0f) / wWidth;

	if (wWidth < (wHeight * 4.0f / 3.0f)) {
		x = 0.0f;
	}

	x *= (wWidth / (wHeight * 4.0f / 3.0f));
	x *= 640.0f;

	float y = 0.2f * 480.0f;

	unsigned availMem = device->GetAvailableTextureMem();

	TextDrawSimple(x, y, 10.0f, 0xFF42e5f4, "VRAM AVAIL: %.2f GB", ((float)availMem) / ((float)(1 << 30)));
	y += 10.0f;

	TextDrawSimple(x, y, 10.0f, 0xFF42e5f4, "vertex bytes total: %.2f KB", ((float)_vertexBytesTotal) / ((float)(1 << 10)));
	y += 10.0f;

	TextDrawSimple(x, y, 10.0f, 0xFF42e5f4, "vertex bytes transfered: %.2f KB", ((float)_vertexBytesTransferedThisFrame) / ((float)(1 << 10)));
	y += 10.0f;
	_vertexBytesTransferedThisFrame = 0;

}

// -----

void allocVertexBuffers() {

	posColVertData.alloc();
	posTexVertData.alloc();
	posColTexVertData.alloc();
	meltyVertData.alloc();
	meltyLineData.alloc();
}

void _drawGeneralCalls() {
	profileFunction();

	allocVertexBuffers();

	device->BeginScene();


	posColVertData.draw();

	_drawHitboxes();

	posColTexVertData.draw();
	posTexVertData.draw();

	cursorDraw();
	meltyVertData.draw();
	meltyLineData.draw();






	//device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
	//device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	//device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//device->SetRenderState(D3DRS_ALPHAREF, 0);
	//device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);



	// horrid way to do priority
	//
	//meltyVertData.draw();


	device->EndScene();
}

// -----

void _drawDebugMenu();

void __stdcall _doDrawCalls() {

	/*
	todo, consolidate all drawprims into as little calls as possible
	multiple for each rect with a prealloced vert buffer is good, one big call is better
	i can either rewrite all my funcs, or go into each func, have them add tris to a list, and go from there
	and,, ugh i hate both options.
	also i wonder how much cpu time is taken up by stupid levels of division
	plan: rewrite old funcs.
	ill have,,,
	3 different,, actually 4 styles

	all fancy hitbox stuff (if enabled)

	all non fancy blend hitbox colored tris, those require a
	all general text
	ill assume all text will be above rects
	all debug text

	i should put everything into one vert buffer, the format doesnt matter i set that with a seperate func
	i understand now


	draw order:

	BEGINSCENE
		vert format = (Pos, Col)
		blended hitboxes (should i include non blended clash, etc here as well? most likely not)
	ENDSCENE

	BEGINSCENE
		vert format = (Pos, Col)
		general untextured draws draws (rects, borders, lines)

		// having to use this format,,, causes issues with the size of the buffer i will be using
		// but tbh the other call, just isnt as good except for when i need fast, bulk text (for example, the log)
		vert format = (Pos, Col, Tex)
		multicolor legacy text draws using the old format.

		vert format = (Pos, Tex)
		new single color text draws via pixel shader
	ENDSCENE

	*/

	//TextDraw(300, 300, 16, 0xFFFFFFFF, "%7.2f %7.2f", mousePos.x, mousePos.y);


	//dualInputDisplay();

	profileFunction();

	directxFrameCount++;

	HRESULT hr = device->TestCooperativeLevel();

	if (hr != S_OK) {
		//log("skipping doDrawCalls");
		//printDirectXError(hr);
		return;
	}

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

	if (shouldDrawHud) {
#ifdef NDEBUG 
		//TextDraw(631, 0.0, 9, 0xFF42e5f4, "REL", res);
#else
		TextDraw(500, 0.0, 9, 0xFFbd1a0b, "THIS IS A DEBUG BUILD", res);
#endif
		TextDraw(0.0, 0.0, 10, 0xFF42e5f4, "%5.2lf", res);
	}

	startTime = endTime;

	// ??? what coord space do these lines use?
	// or wait was rhw the only thing i needed to draw on the screen?? none of the stupid bs i have been doing this entire time
	// omfg

	/*
	MeltyTestVert a = {-100.0f, 200.0f, 0.0f, 1.0f, 0xFF0000FF};
	MeltyTestVert b = { 100.0f, 200.0f, 0.0f, 1.0f, 0xFF0000FF };

	meltyLineData.add(a, b);
	meltyLineData.add(a, b);
	meltyLineData.add(a, b);

	MeltyTestVert temp = meltyLineData.vertexData[0];

	log("%f %f %f %f %08X", temp.position.x, temp.position.y, temp.position.z, temp.rhw, temp.color);
	*/

	// predraw stuff goes here.
	drawFancyInputDisplay();
	_drawProfiler();
	_drawLog();
	_drawMiscInfo();
	_drawDebugMenu();

	// -- ACTUAL RENDERING --
	backupRenderState();

	_drawGeneralCalls();

	restoreRenderState();

	// -- END ACTUAL RENDERING --

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
	//Texture::_cleanForReset();

	HRESULT hr = device->TestCooperativeLevel();
	//log("RESET HR VAL");
	//printDirectXError(hr);

	if (hr == D3DERR_DEVICELOST) {
		return;
	}

	// for unknown reasons, after pressing caster f4, if you alt tab/fullscreen unfull screen, it fixes it

	log("reset resources");

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

	// disable their fps counter
	patchByte(0x00554128, 0x00);

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
	log("<3");
	return true;
}