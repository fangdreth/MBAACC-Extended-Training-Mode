#pragma once
#include <Windows.h>

IDirect3DDevice9* __device = NULL;

DWORD setTextureCount = 0;

void HookThisShit(IDirect3DDevice9* _device);
void UnhookThisShit();

void __stdcall _log(const char* format, ...) {
	return;
	static char buffer[1024]; // no more random char buffers everywhere.
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, 1024, format, args);
	___log(buffer);
	va_end(args);
}

const char* pixelShaderCode1 = R"(
	sampler2D textureSampler : register(s0);

	struct PS_INPUT
	{
		float2 TexCoord : TEXCOORD0;
	};

	float4 main(PS_INPUT input) : COLOR
	{
		float4 color = tex2D(textureSampler, input.TexCoord);
		
		//return float4(color.b, color.g, color.r, color.a);

		float idk = (color.r + color.g + color.b) / 3;

		return float4(idk, idk, idk, color.a);
	}
)";

const char* pixelShaderCode2 = R"(


sampler2D texSampler : register(s0);

struct PSInput
{
	float2 texCoord : TEXCOORD0;
};

float4 main(PSInput input) : COLOR
{
	// Sample the texture
	float4 color = tex2D(texSampler, input.texCoord);
	
	// Modify the color (for example, invert the colors)
	//color.rgb = 1.0 - color.rgb;

	color.r = 1 - color.r;
	color.g = 1 - color.g;
	color.b = 1 - color.b;


	
	// Return the modified color
	return color;
}

technique SimpleTechnique
{
	pass P0
	{
		PixelShader = compile ps_2_0 main();
	}
}


)"; 

IDirect3DPixelShader9* pPixelShader = nullptr;
ID3DXBuffer* pShaderBuffer = nullptr;
ID3DXBuffer* pErrorBuffer = nullptr;

void initShader() {

	IDirect3DDevice9* device = __device;

	const char* pixelShaderCode = pixelShaderCode2;

	HRESULT hr = D3DXCompileShader(
		pixelShaderCode,               // Shader source code
		strlen(pixelShaderCode),       // Length of the shader code
		NULL,                          // Optional defines
		NULL,                          // Optional include interface
		"main",                 // Entry point function name
		"ps_2_0",                      // Target profile (pixel shader 2.0)
		0,                             // Shader compile flags
		&pShaderBuffer,                // Compiled shader code
		&pErrorBuffer,                 // Error messages
		NULL                           // Optional constant table
	);

	if (FAILED(hr))
	{
		if (pErrorBuffer)
		{
			// Print the error message if compilation fails
			log((char*)pErrorBuffer->GetBufferPointer());
			while (true) {}
			pErrorBuffer->Release();
		}
		// Handle the error appropriately
		return;
	}

	// Create the pixel shader
	hr = device->CreatePixelShader((DWORD*)pShaderBuffer->GetBufferPointer(), &pPixelShader);

	if (FAILED(hr))
	{
		log("createpix died");
		return;
	}

	// Release the shader buffer as it is no longer needed
	pShaderBuffer->Release();

	log("shader compile worked!");
}

struct Vertex
{
	D3DXVECTOR4 position;
	D3DXVECTOR2 texCoord;
};

#define D3DFVF_VERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1)

DWORD _IDirect3DDevice9_CreateTexture_addr_fmt;
DWORD createTextureAddr;
void __stdcall doSomething() {

	return;

	IDirect3DTexture9* pTex = (IDirect3DTexture9*)createTextureAddr;

	IDirect3DDevice9* device = __device;

	if (device == NULL) {
		return;
	}
	
	if (pPixelShader == NULL) {
		return;
	}

	HRESULT hr;


	

	

	
	D3DSURFACE_DESC desc;
	hr = pTex->GetLevelDesc(0, &desc);
	D3DPOOL pool = desc.Pool;
	D3DFORMAT format = desc.Format;
	DWORD usage = desc.Usage;
	DWORD width = desc.Width;
	DWORD height = desc.Height;

	UnhookThisShit();

	

	IDirect3DTexture9* renderTargetTexture = nullptr;
	IDirect3DSurface9* renderTargetSurface = nullptr;

	hr = device->CreateTexture(
		width,
		height,
		1,
		D3DUSAGE_RENDERTARGET,
		format,
		D3DPOOL_DEFAULT,
		&renderTargetTexture,
		nullptr
	);

	if (FAILED(hr)) {
		log("fuck1");
		//while (true) {}
		HookThisShit(__device);
		return;
	}

	hr = renderTargetTexture->GetSurfaceLevel(0, &renderTargetSurface);

	if (FAILED(hr)) {
		log("fuck2");
		while (true) {}
	}

	IDirect3DSurface9* originalRenderTarget = nullptr;
	device->GetRenderTarget(0, &originalRenderTarget);
	device->SetRenderTarget(0, renderTargetSurface);

	device->BeginScene();

	device->SetPixelShader(pPixelShader);

	device->SetTexture(0, pTex);

	Vertex vertices[4] = {
		{ D3DXVECTOR4(-1.0f,  1.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(-1.0f, -1.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR4(1.0f,  1.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR4(1.0f, -1.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f) }
	};

	// Create a vertex buffer
	IDirect3DVertexBuffer9* vertexBuffer = nullptr;
	hr = device->CreateVertexBuffer(
		sizeof(vertices),
		D3DUSAGE_WRITEONLY,
		D3DFVF_VERTEX,
		D3DPOOL_MANAGED,
		&vertexBuffer,
		nullptr
	);

	if (FAILED(hr)) {
		log("fuck3");
		while (true) {}
	}

	void* pVertices;
	hr = vertexBuffer->Lock(0, sizeof(vertices), (void**)&pVertices, 0);


	if (FAILED(hr)) {
		log("fuck4");
		while (true) {}
	}

	if (SUCCEEDED(hr)) {
		memcpy(pVertices, vertices, sizeof(vertices));
		vertexBuffer->Unlock();
	}

	// Set the vertex buffer
	device->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
	device->SetFVF(D3DFVF_VERTEX);

	// Render the full-screen quad
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);



	device->EndScene();

	vertexBuffer->Release();

	IDirect3DSurface9* textureSurface = nullptr;
	pTex->GetSurfaceLevel(0, &textureSurface);

	//device->BeginScene();
	device->UpdateSurface(renderTargetSurface, nullptr, textureSurface, nullptr);
	//device->EndScene();

	
	textureSurface->Release();

	device->SetRenderTarget(0, originalRenderTarget);
	originalRenderTarget->Release();
	renderTargetSurface->Release();
	renderTargetTexture->Release();

	log("weird func ran successfully?");

	HookThisShit(__device);
}

DWORD _IDirect3DDevice9_SetRenderState_state;
DWORD _IDirect3DDevice9_SetRenderState_value;
void __stdcall alterRenderState() {
	
	//_IDirect3DDevice9_SetRenderState_value = 0;
	//  https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dblend
	// D3DBLEND_ONE (VAL OF 2)

	switch (_IDirect3DDevice9_SetRenderState_state) {
	case D3DRS_SRCBLEND:
	case D3DRS_DESTBLEND:
		//IDirect3DDevice9_SetRenderState_value = D3DBLEND_ONE;
		break;
	case D3DRS_SRCBLENDALPHA:
	case D3DRS_DESTBLENDALPHA:
		break;
	case D3DRS_SEPARATEALPHABLENDENABLE:
		//_IDirect3DDevice9_SetRenderState_value = 0;
		break;
	default:
		break;
	}

}

DWORD _ebxBackup;
void __stdcall pureConfusion() {

	if (__device == NULL) {
		return;
	}

	for (int i = 0; i < 0x58/4; i++) {
		log("     EBX %04X: %08X", i*4, *(DWORD*)(_ebxBackup + (i*4)));
	}


	log("---------------- leaving pureConfusion");
}

IDirect3DPixelShader9* shaderResetAddr = NULL;
__declspec(naked) void omfgResetShader() {
	__asm {
		mov _ebxBackup, ebx;
	}
	PUSH_ALL;
	pureConfusion();
	if (__device != NULL) {
		__device->SetPixelShader(shaderResetAddr);
	}
	POP_ALL;

	__asm {
		ret;
	};
}

DWORD _IDirect3DDevice9_SetTexture_eax;
DWORD _IDirect3DDevice9_SetTexture_ebx;
DWORD _IDirect3DDevice9_SetTexture_ecx;
DWORD _IDirect3DDevice9_SetTexture_edx;

DWORD _IDirect3DDevice9_SetTexture_esi;
DWORD _IDirect3DDevice9_SetTexture_edi;

DWORD _IDirect3DDevice9_SetTexture_ebp;
DWORD _IDirect3DDevice9_SetTexture_esp;

DWORD _IDirect3DDevice9_SetTexture_stack[32];
DWORD _IDirect3DDevice9_SetTexture_retAddr;
DWORD _IDirect3DDevice9_SetTexture_texAddr;
DWORD _IDirect3DDevice9_SetTexture_texStage;
bool breakCondition = false;
bool trigBreakCondition = false;
DWORD testOverride = 0;
void __stdcall setTextureFuckaround() {



	HRESULT hr;

	if (_IDirect3DDevice9_SetTexture_texAddr == 0) {
		return;
	}


	log("\n\n\nsetTextureCount was %d and TESTVAR1 was %d TESTVAR2 was %d", setTextureCount, TESTVAR1, TESTVAR2);

	IDirect3DTexture9* pTex = (IDirect3DTexture9*)_IDirect3DDevice9_SetTexture_texAddr;
	

	D3DSURFACE_DESC desc;
	
	hr = pTex->GetLevelDesc(0, &desc);

	D3DPOOL pool = desc.Pool;
	D3DFORMAT format = desc.Format;
	DWORD usage = desc.Usage;
	DWORD width = desc.Width;
	DWORD height = desc.Height;


	log("         width was %d", width);
	log("        height was %d", height);
	log("          pool was %d", pool);
	log("         usage was %d", usage);
	log("        format was %d", format);

	static unsigned what = 0;


	if (format == D3DFMT_A1R5G5B5) {

		//__device->SetPixelShader(pPixelShader);

		// ok. THIS(with == D3DFMT_A1R5G5B5). changes menu shit. and chars. AND PROJECTILES. 
		// as far as i can think/tell/know, end/beginscene reset the shader states.
		// there are 4 calls with this type per frame. 
		// although, if the,, shader isnt changed, then that means that the stuff AFTER this is what actually does shit?
		// no. these calls are for,,, each char and their shadow
		// or,,, actually i have no clue, sion summon did some shit
	}

	// test on classic home invade warc v sion
	// draw order seems to draw the top ui first 


	breakCondition = ((int)setTextureCount >= TESTVAR1) && ((int)setTextureCount <= TESTVAR2);

	log("testOverride was %08X breakCondition was %d", testOverride, breakCondition);
	if (breakCondition ) {
		__device->GetPixelShader(&shaderResetAddr);
		__device->SetPixelShader(pPixelShader);
	}
	else {
		//__device->SetPixelShader(NULL);
	}


	log("  tex stage was %08X", _IDirect3DDevice9_SetTexture_texStage);
	log("   tex addr was %08X", _IDirect3DDevice9_SetTexture_texAddr);
	log("        ret was %08X", _IDirect3DDevice9_SetTexture_retAddr);

	log("_IDirect3DDevice9_SetTexture_eax: %08X", _IDirect3DDevice9_SetTexture_eax);
	log("_IDirect3DDevice9_SetTexture_ebx: %08X", _IDirect3DDevice9_SetTexture_ebx);
	log("_IDirect3DDevice9_SetTexture_ecx: %08X", _IDirect3DDevice9_SetTexture_ecx);
	log("_IDirect3DDevice9_SetTexture_edx: %08X", _IDirect3DDevice9_SetTexture_edx);
	log("_IDirect3DDevice9_SetTexture_esi: %08X", _IDirect3DDevice9_SetTexture_esi);
	log("_IDirect3DDevice9_SetTexture_edi: %08X", _IDirect3DDevice9_SetTexture_edi);
	log("_IDirect3DDevice9_SetTexture_ebp: %08X", _IDirect3DDevice9_SetTexture_ebp);


	for (int i = 0; i < 32; i++) {
		log("            %02X: %08X", i*4, _IDirect3DDevice9_SetTexture_stack[i]);
	}

	if (trigBreak != 0 && breakCondition) {
		trigBreakCondition = true;
	}

	setTextureCount++;

	log("      leaving setTextureFuckaround");

}

DWORD _IDirect3DDevice9_TestCooperativeLevel_addr = 0;
__declspec(naked) void _IDirect3DDevice9_TestCooperativeLevel_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_TestCooperativeLevel called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_TestCooperativeLevel_addr];
	}
}
DWORD _IDirect3DDevice9_EvictManagedResources_addr = 0;
__declspec(naked) void _IDirect3DDevice9_EvictManagedResources_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_EvictManagedResources called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_EvictManagedResources_addr];
	}
}
DWORD _IDirect3DDevice9_GetDirect3D_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetDirect3D_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetDirect3D called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetDirect3D_addr];
	}
}
DWORD _IDirect3DDevice9_GetDeviceCaps_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetDeviceCaps_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetDeviceCaps called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetDeviceCaps_addr];
	}
}
DWORD _IDirect3DDevice9_GetDisplayMode_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetDisplayMode_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetDisplayMode called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetDisplayMode_addr];
	}
}
DWORD _IDirect3DDevice9_GetCreationParameters_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetCreationParameters_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetCreationParameters called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetCreationParameters_addr];
	}
}
DWORD _IDirect3DDevice9_SetCursorProperties_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetCursorProperties_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetCursorProperties called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetCursorProperties_addr];
	}
}
DWORD _IDirect3DDevice9_CreateAdditionalSwapChain_addr = 0;
__declspec(naked) void _IDirect3DDevice9_CreateAdditionalSwapChain_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_CreateAdditionalSwapChain called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_CreateAdditionalSwapChain_addr];
	}
}
DWORD _IDirect3DDevice9_GetSwapChain_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetSwapChain_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetSwapChain called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetSwapChain_addr];
	}
}
DWORD _IDirect3DDevice9_Reset_addr = 0;
__declspec(naked) void _IDirect3DDevice9_Reset_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_Reset called!");
	//__device->SetPixelShader(NULL);
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_Reset_addr];
	}
}
DWORD _IDirect3DDevice9_Present_addr = 0;
__declspec(naked) void _IDirect3DDevice9_Present_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_Present called!");
	setTextureCount = 0;
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_Present_addr];
	}
}
DWORD _IDirect3DDevice9_GetBackBuffer_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetBackBuffer_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetBackBuffer called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetBackBuffer_addr];
	}
}
DWORD _IDirect3DDevice9_GetRasterStatus_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetRasterStatus_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetRasterStatus called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetRasterStatus_addr];
	}
}
DWORD _IDirect3DDevice9_SetDialogBoxMode_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetDialogBoxMode_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetDialogBoxMode called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetDialogBoxMode_addr];
	}
}
DWORD _IDirect3DDevice9_CreateTexture_addr_reg;
DWORD _IDirect3DDevice9_CreateTexture_addr_ret;
DWORD _IDirect3DDevice9_CreateTexture_addr = 0;
__declspec(naked) void _IDirect3DDevice9_CreateTexture_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_CreateTexture called!");
	POP_ALL;
	__asm {
		pop _IDirect3DDevice9_CreateTexture_addr_ret;

		mov _IDirect3DDevice9_CreateTexture_addr_reg, eax;

		mov eax, [esp + 20];
		mov _IDirect3DDevice9_CreateTexture_addr_fmt, eax;

		mov eax, [esp + 28];
		//mov eax, [eax];
		mov createTextureAddr, eax;

		mov eax, _IDirect3DDevice9_CreateTexture_addr_reg;

		call[_IDirect3DDevice9_CreateTexture_addr];
		//jmp[_IDirect3DDevice9_CreateTexture_addr];
	}

	// conspiracy theory time. this seems to only be called(at least during gameplay) before we are in ANY scene. solution? make our own scene, do the fuck ass rendering bs, live life, yea
	// nvm, those loads are only called the first a texure is loaded. 
	// meaning, they are probs storeded somewhere?
	// i could also track said pointers myself, but my hooks get corrupted on startup. 
	

	PUSH_ALL;
	__asm {
		mov eax, createTextureAddr;
		mov eax, [eax];
		mov createTextureAddr, eax;
	}
	doSomething();
	_log("   tex addr: %08X\n"
	   "        ret: %08X", createTextureAddr, _IDirect3DDevice9_CreateTexture_addr_ret);
	POP_ALL;

	__asm {
		push _IDirect3DDevice9_CreateTexture_addr_ret;
		ret;
	}
}
DWORD _IDirect3DDevice9_CreateVolumeTexture_addr = 0;
__declspec(naked) void _IDirect3DDevice9_CreateVolumeTexture_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_CreateVolumeTexture called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_CreateVolumeTexture_addr];
	}
}
DWORD _IDirect3DDevice9_CreateCubeTexture_addr = 0;
__declspec(naked) void _IDirect3DDevice9_CreateCubeTexture_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_CreateCubeTexture called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_CreateCubeTexture_addr];
	}
}
DWORD _IDirect3DDevice9_CreateVertexBuffer_addr = 0;
__declspec(naked) void _IDirect3DDevice9_CreateVertexBuffer_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_CreateVertexBuffer called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_CreateVertexBuffer_addr];
	}
}
DWORD _IDirect3DDevice9_CreateIndexBuffer_addr = 0;
__declspec(naked) void _IDirect3DDevice9_CreateIndexBuffer_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_CreateIndexBuffer called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_CreateIndexBuffer_addr];
	}
}
DWORD _IDirect3DDevice9_CreateRenderTarget_addr = 0;
__declspec(naked) void _IDirect3DDevice9_CreateRenderTarget_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_CreateRenderTarget called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_CreateRenderTarget_addr];
	}
}
DWORD _IDirect3DDevice9_CreateDepthStencilSurface_addr = 0;
__declspec(naked) void _IDirect3DDevice9_CreateDepthStencilSurface_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_CreateDepthStencilSurface called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_CreateDepthStencilSurface_addr];
	}
}
DWORD _IDirect3DDevice9_UpdateSurface_addr = 0;
__declspec(naked) void _IDirect3DDevice9_UpdateSurface_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_UpdateSurface called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_UpdateSurface_addr];
	}
}
DWORD _IDirect3DDevice9_UpdateTexture_addr = 0;
__declspec(naked) void _IDirect3DDevice9_UpdateTexture_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_UpdateTexture called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_UpdateTexture_addr];
	}
}
DWORD _IDirect3DDevice9_GetRenderTargetData_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetRenderTargetData_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetRenderTargetData called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetRenderTargetData_addr];
	}
}
DWORD _IDirect3DDevice9_GetFrontBufferData_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetFrontBufferData_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetFrontBufferData called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetFrontBufferData_addr];
	}
}
DWORD _IDirect3DDevice9_StretchRect_addr = 0;
__declspec(naked) void _IDirect3DDevice9_StretchRect_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_StretchRect called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_StretchRect_addr];
	}
}
DWORD _IDirect3DDevice9_ColorFill_addr = 0;
__declspec(naked) void _IDirect3DDevice9_ColorFill_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_ColorFill called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_ColorFill_addr];
	}
}
DWORD _IDirect3DDevice9_CreateOffscreenPlainSurface_addr = 0;
__declspec(naked) void _IDirect3DDevice9_CreateOffscreenPlainSurface_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_CreateOffscreenPlainSurface called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_CreateOffscreenPlainSurface_addr];
	}
}
DWORD _IDirect3DDevice9_SetRenderTarget_Index;
DWORD _IDirect3DDevice9_SetRenderTarget_Target;
DWORD _IDirect3DDevice9_SetRenderTarget_ret;
DWORD _IDirect3DDevice9_SetRenderTarget_reg;
DWORD _IDirect3DDevice9_SetRenderTarget_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetRenderTarget_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetRenderTarget called!");
	POP_ALL;
	__asm {
		pop _IDirect3DDevice9_SetRenderTarget_ret;
		mov _IDirect3DDevice9_SetRenderTarget_reg, eax;

		mov eax, [esp + 4];
		mov _IDirect3DDevice9_SetRenderTarget_Index, eax;

		mov eax, [esp + 8];
		mov _IDirect3DDevice9_SetRenderTarget_Target, eax;

		mov eax, _IDirect3DDevice9_SetRenderTarget_reg
		call[_IDirect3DDevice9_SetRenderTarget_addr];
	}

	PUSH_ALL;
	_log("RenderTargetIndex: %d\n"
		"    pRenderTarget: %08X\n"
		"              ret: %08X", _IDirect3DDevice9_SetRenderTarget_Index, _IDirect3DDevice9_SetRenderTarget_Target, _IDirect3DDevice9_SetRenderTarget_ret);
	POP_ALL;

	__asm {
		push _IDirect3DDevice9_SetRenderTarget_ret;
		ret;
	};
}
DWORD _IDirect3DDevice9_GetRenderTarget_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetRenderTarget_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetRenderTarget called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetRenderTarget_addr];
	}
}
DWORD _IDirect3DDevice9_SetDepthStencilSurface_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetDepthStencilSurface_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetDepthStencilSurface called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetDepthStencilSurface_addr];
	}
}
DWORD _IDirect3DDevice9_GetDepthStencilSurface_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetDepthStencilSurface_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetDepthStencilSurface called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetDepthStencilSurface_addr];
	}
}
DWORD _IDirect3DDevice9_BeginScene_addr = 0;
__declspec(naked) void _IDirect3DDevice9_BeginScene_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_BeginScene called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_BeginScene_addr];
	}
}
DWORD _IDirect3DDevice9_EndScene_ret = 0;
DWORD _IDirect3DDevice9_EndScene_addr = 0;
__declspec(naked) void _IDirect3DDevice9_EndScene_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_EndScene called!");
	//__device->SetPixelShader(NULL);
	/*if (pPixelShader != NULL) {
		__device->SetPixelShader(pPixelShader);
	}*/
	POP_ALL;
	__asm {
		pop _IDirect3DDevice9_EndScene_ret;
		call[_IDirect3DDevice9_EndScene_addr];
	};

	PUSH_ALL;
	//__device->SetPixelShader(NULL);
	POP_ALL;
	__asm {
		push _IDirect3DDevice9_EndScene_ret;
		ret;
	}
}
DWORD _IDirect3DDevice9_Clear_addr = 0;
__declspec(naked) void _IDirect3DDevice9_Clear_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_Clear called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_Clear_addr];
	}
}
DWORD _IDirect3DDevice9_SetTransform_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetTransform_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetTransform called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetTransform_addr];
	}
}
DWORD _IDirect3DDevice9_GetTransform_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetTransform_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetTransform called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetTransform_addr];
	}
}
DWORD _IDirect3DDevice9_MultiplyTransform_addr = 0;
__declspec(naked) void _IDirect3DDevice9_MultiplyTransform_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_MultiplyTransform called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_MultiplyTransform_addr];
	}
}
DWORD _IDirect3DDevice9_SetViewport_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetViewport_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetViewport called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetViewport_addr];
	}
}
DWORD _IDirect3DDevice9_GetViewport_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetViewport_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetViewport called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetViewport_addr];
	}
}
DWORD _IDirect3DDevice9_SetMaterial_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetMaterial_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetMaterial called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetMaterial_addr];
	}
}
DWORD _IDirect3DDevice9_GetMaterial_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetMaterial_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetMaterial called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetMaterial_addr];
	}
}
DWORD _IDirect3DDevice9_SetLight_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetLight_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetLight called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetLight_addr];
	}
}
DWORD _IDirect3DDevice9_GetLight_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetLight_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetLight called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetLight_addr];
	}
}
DWORD _IDirect3DDevice9_LightEnable_addr = 0;
__declspec(naked) void _IDirect3DDevice9_LightEnable_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_LightEnable called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_LightEnable_addr];
	}
}
DWORD _IDirect3DDevice9_GetLightEnable_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetLightEnable_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetLightEnable called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetLightEnable_addr];
	}
}
DWORD _IDirect3DDevice9_SetClipPlane_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetClipPlane_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetClipPlane called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetClipPlane_addr];
	}
}
DWORD _IDirect3DDevice9_GetClipPlane_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetClipPlane_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetClipPlane called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetClipPlane_addr];
	}
}
DWORD _IDirect3DDevice9_SetRenderState_reg;
DWORD _IDirect3DDevice9_SetRenderState_ret;
DWORD _IDirect3DDevice9_SetRenderState_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetRenderState_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetRenderState called!");
	POP_ALL;
	// is this func possibly the one causing me all my issues?
	__asm {
		pop _IDirect3DDevice9_SetRenderState_ret;
		mov _IDirect3DDevice9_SetRenderState_reg, eax;

		mov eax, [esp + 4];
		mov _IDirect3DDevice9_SetRenderState_state, eax;

		mov eax, [esp + 8];
		mov _IDirect3DDevice9_SetRenderState_value, eax;

		mov eax, _IDirect3DDevice9_SetRenderState_reg;
	}

	PUSH_ALL;
	alterRenderState();
	POP_ALL;

	__asm {

		mov _IDirect3DDevice9_SetRenderState_reg, eax;

		mov eax, _IDirect3DDevice9_SetRenderState_state;
		mov [esp + 4], eax;
		
		mov eax, _IDirect3DDevice9_SetRenderState_value;
		mov [esp + 8], eax;

		mov eax, _IDirect3DDevice9_SetRenderState_reg;

		call[_IDirect3DDevice9_SetRenderState_addr];
	};

	PUSH_ALL;
	_log(
		"    State: %d\n"
		"    Value: %d\n"
		"      ret: %08X", _IDirect3DDevice9_SetRenderState_state, _IDirect3DDevice9_SetRenderState_value, _IDirect3DDevice9_SetRenderState_ret
	);
	POP_ALL;

	__asm {
		push _IDirect3DDevice9_SetRenderState_ret;
		ret;
	};
}
DWORD _IDirect3DDevice9_GetRenderState_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetRenderState_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetRenderState called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetRenderState_addr];
	}
}
DWORD _IDirect3DDevice9_CreateStateBlock_addr = 0;
__declspec(naked) void _IDirect3DDevice9_CreateStateBlock_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_CreateStateBlock called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_CreateStateBlock_addr];
	}
}
DWORD _IDirect3DDevice9_BeginStateBlock_ret = 0;
DWORD _IDirect3DDevice9_BeginStateBlock_addr = 0;
__declspec(naked) void _IDirect3DDevice9_BeginStateBlock_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_BeginStateBlock called!");
	POP_ALL;
	__asm {
		mov needHookReset, 1; // this reset is delayed, rehooking inside this func however seemed to fuck shit. (if we are in a directx stack, things would def be v messed up)
		// this really sucks bc as far as i can tell, we miss a lot of tex loading on the frame this is called. when is it called??
		pop _IDirect3DDevice9_BeginStateBlock_ret;

		//jmp[_IDirect3DDevice9_BeginStateBlock_addr];
		call[_IDirect3DDevice9_BeginStateBlock_addr];
	}

	//PUSH_ALL;
	//log("reseting hooks!");
	//HookThisShit(__device);
	//POP_ALL;
	__asm {
		push _IDirect3DDevice9_BeginStateBlock_ret;
		ret;
	}

}
DWORD _IDirect3DDevice9_EndStateBlock_addr = 0;
__declspec(naked) void _IDirect3DDevice9_EndStateBlock_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_EndStateBlock called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_EndStateBlock_addr];
	}
}
DWORD _IDirect3DDevice9_SetClipStatus_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetClipStatus_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetClipStatus called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetClipStatus_addr];
	}
}
DWORD _IDirect3DDevice9_GetClipStatus_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetClipStatus_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetClipStatus called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetClipStatus_addr];
	}
}
DWORD _IDirect3DDevice9_GetTexture_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetTexture_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetTexture called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetTexture_addr];
	}
}
DWORD _IDirect3DDevice9_SetTexture_reg;
DWORD _IDirect3DDevice9_SetTexture_reg2;
DWORD _IDirect3DDevice9_SetTexture_reg3;
DWORD _IDirect3DDevice9_SetTexture_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetTexture_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetTexture called!");

	// setting this shader is not enough. we need to do a subrender!!

	//__device->SetPixelShader(pPixelShader);
	POP_ALL;

	__asm {

		mov _IDirect3DDevice9_SetTexture_eax, eax;
		mov _IDirect3DDevice9_SetTexture_ebx, ebx;
		mov _IDirect3DDevice9_SetTexture_ecx, ecx;
		mov _IDirect3DDevice9_SetTexture_edx, edx;

		mov _IDirect3DDevice9_SetTexture_esi, esi;
		mov _IDirect3DDevice9_SetTexture_edi, edi;

		mov _IDirect3DDevice9_SetTexture_ebp, ebp;
		mov _IDirect3DDevice9_SetTexture_esp, esp;

		// check the return addr 
		mov _IDirect3DDevice9_SetTexture_reg, eax;
		mov _IDirect3DDevice9_SetTexture_reg2, ebx;
		mov _IDirect3DDevice9_SetTexture_reg3, ecx;

		mov eax, 0;
		mov ebx, 0;
		mov ecx, 0;
	_L:
		mov ecx, [esp + (ebx * 4)];
		mov _IDirect3DDevice9_SetTexture_stack[(ebx * 4)], ecx;

		add ebx, 1;

		cmp ebx, 31;
		JNE _L;

		mov eax, _IDirect3DDevice9_SetTexture_reg;
		mov ebx, _IDirect3DDevice9_SetTexture_reg2;
		mov ecx, _IDirect3DDevice9_SetTexture_reg3;
	};


	__asm {

		pop _IDirect3DDevice9_SetTexture_retAddr;
		mov _IDirect3DDevice9_SetTexture_reg, eax;

		mov eax, [esp + 8];
		mov _IDirect3DDevice9_SetTexture_texAddr, eax;

		mov eax, [esp + 4];
		mov _IDirect3DDevice9_SetTexture_texStage, eax;

		mov eax, _IDirect3DDevice9_SetTexture_reg;
		call[_IDirect3DDevice9_SetTexture_addr];
	}

	

	PUSH_ALL;

	// subrender!
	//__device->GetRenderTarget(0, &pScreenSurface);

	setTextureFuckaround();
	//__device->SetPixelShader(pPixelShader);
	//__device->SetRenderTarget(0, pScreenSurface);
	POP_ALL;

	__asm {
		push _IDirect3DDevice9_SetTexture_retAddr;

		cmp trigBreakCondition, 0;
		JE _DONTBREAK;

		mov trigBreakCondition, 0;

		nop;
		int 3;
		nop;

		_DONTBREAK:

		ret;
	}
}
DWORD _IDirect3DDevice9_GetTextureStageState_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetTextureStageState_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetTextureStageState called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetTextureStageState_addr];
	}
}
DWORD _IDirect3DDevice9_SetTextureStageState_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetTextureStageState_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetTextureStageState called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetTextureStageState_addr];
	}
}
DWORD _IDirect3DDevice9_GetSamplerState_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetSamplerState_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetSamplerState called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetSamplerState_addr];
	}
}
DWORD _IDirect3DDevice9_SetSamplerState_params[3];
DWORD _IDirect3DDevice9_SetSamplerState_reg;
DWORD _IDirect3DDevice9_SetSamplerState_ret;
DWORD _IDirect3DDevice9_SetSamplerState_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetSamplerState_func() {
	
	__asm {
		pop _IDirect3DDevice9_SetSamplerState_ret;
		mov _IDirect3DDevice9_SetSamplerState_reg, eax;

		mov eax, [esp + 4];
		mov _IDirect3DDevice9_SetSamplerState_params[0], eax;
		mov eax, [esp + 8];
		mov _IDirect3DDevice9_SetSamplerState_params[4], eax;
		mov eax, [esp + 12];
		mov _IDirect3DDevice9_SetSamplerState_params[8], eax;

		mov eax, _IDirect3DDevice9_SetSamplerState_reg;
	};

	PUSH_ALL;
	_log(
		"IDirect3DDevice9_SetSamplerState called!\n"
		"                             Sampler: %d\n"
		"                 D3DSAMPLERSTATETYPE: %d\n"
		"                               Value: %d\n"
		"                                 Ret: %08X\n",
		_IDirect3DDevice9_SetSamplerState_params[0], _IDirect3DDevice9_SetSamplerState_params[1], _IDirect3DDevice9_SetSamplerState_params[2], _IDirect3DDevice9_SetSamplerState_ret);
	POP_ALL;

	

	__asm {
		call[_IDirect3DDevice9_SetSamplerState_addr];
		push _IDirect3DDevice9_SetSamplerState_ret;
		ret;
	}
}
DWORD _IDirect3DDevice9_ValidateDevice_addr = 0;
__declspec(naked) void _IDirect3DDevice9_ValidateDevice_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_ValidateDevice called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_ValidateDevice_addr];
	}
}
DWORD _IDirect3DDevice9_SetPaletteEntries_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetPaletteEntries_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetPaletteEntries called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetPaletteEntries_addr];
	}
}
DWORD _IDirect3DDevice9_GetPaletteEntries_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetPaletteEntries_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetPaletteEntries called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetPaletteEntries_addr];
	}
}
DWORD _IDirect3DDevice9_SetCurrentTexturePalette_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetCurrentTexturePalette_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetCurrentTexturePalette called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetCurrentTexturePalette_addr];
	}
}
DWORD _IDirect3DDevice9_GetCurrentTexturePalette_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetCurrentTexturePalette_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetCurrentTexturePalette called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetCurrentTexturePalette_addr];
	}
}
DWORD _IDirect3DDevice9_SetScissorRect_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetScissorRect_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetScissorRect called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetScissorRect_addr];
	}
}
DWORD _IDirect3DDevice9_GetScissorRect_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetScissorRect_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetScissorRect called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetScissorRect_addr];
	}
}
DWORD _IDirect3DDevice9_SetSoftwareVertexProcessing_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetSoftwareVertexProcessing_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetSoftwareVertexProcessing called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetSoftwareVertexProcessing_addr];
	}
}
DWORD _IDirect3DDevice9_SetNPatchMode_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetNPatchMode_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetNPatchMode called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetNPatchMode_addr];
	}
}
DWORD _IDirect3DDevice9_DrawPrimitive_addr = 0;
__declspec(naked) void _IDirect3DDevice9_DrawPrimitive_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_DrawPrimitive called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_DrawPrimitive_addr];
	}
}
DWORD _IDirect3DDevice9_DrawIndexedPrimitive_addr = 0;
__declspec(naked) void _IDirect3DDevice9_DrawIndexedPrimitive_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_DrawIndexedPrimitive called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_DrawIndexedPrimitive_addr];
	}
}
DWORD _IDirect3DDevice9_DrawPrimitiveUP_addr = 0;
__declspec(naked) void _IDirect3DDevice9_DrawPrimitiveUP_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_DrawPrimitiveUP called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_DrawPrimitiveUP_addr];
	}
}
DWORD _IDirect3DDevice9_DrawIndexedPrimitiveUP_params[8];
DWORD _IDirect3DDevice9_DrawIndexedPrimitiveUP_ret;
DWORD _IDirect3DDevice9_DrawIndexedPrimitiveUP_reg;
DWORD _IDirect3DDevice9_DrawIndexedPrimitiveUP_addr = 0;
__declspec(naked) void _IDirect3DDevice9_DrawIndexedPrimitiveUP_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_DrawIndexedPrimitiveUP called!");

	//__device->SetPixelShader(pPixelShader);

	POP_ALL;
	__asm {
		pop _IDirect3DDevice9_DrawIndexedPrimitiveUP_ret;
	};

	__asm {
		/*
		mov _IDirect3DDevice9_DrawIndexedPrimitiveUP_reg, eax;

		mov eax, [esp + 16];
		cmp eax, 4;
		JE _OK;
		
		mov[esp + 16], 1;
	_OK:
		*/

		mov eax, [esp + 4];
		mov _IDirect3DDevice9_DrawIndexedPrimitiveUP_params[4 * 0], eax;

		mov eax, [esp + 8];
		mov _IDirect3DDevice9_DrawIndexedPrimitiveUP_params[4 * 1], eax;

		mov eax, [esp + 12];
		mov _IDirect3DDevice9_DrawIndexedPrimitiveUP_params[4 * 2], eax;

		mov eax, [esp + 16];
		mov _IDirect3DDevice9_DrawIndexedPrimitiveUP_params[4 * 3], eax;

		mov eax, [esp + 20];
		mov _IDirect3DDevice9_DrawIndexedPrimitiveUP_params[4 * 4], eax;

		mov eax, [esp + 24];
		mov _IDirect3DDevice9_DrawIndexedPrimitiveUP_params[4 * 5], eax;

		mov eax, [esp + 28];
		mov _IDirect3DDevice9_DrawIndexedPrimitiveUP_params[4 * 6], eax;

		mov eax, [esp + 32];
		mov _IDirect3DDevice9_DrawIndexedPrimitiveUP_params[4 * 7], eax;

		mov eax, _IDirect3DDevice9_DrawIndexedPrimitiveUP_reg;
	};

	PUSH_ALL;
	
	/*
	log(
	"         PrimitiveType: %d\n" 
	"        MinVertexIndex: %d\n" 
	"           NumVertices: %d\n" 
	"        PrimitiveCount: %d\n" 
	"           *pIndexData: %08X\n"
	"       IndexDataFormat: %d\n"
	"*pVertexStreamZeroData: %08X\n"
	"VertexStreamZeroStride: %d\n"
	"                   ret: %08X\n"
	"-----",
		_IDirect3DDevice9_DrawIndexedPrimitiveUP_params[0], 
		_IDirect3DDevice9_DrawIndexedPrimitiveUP_params[1],
		_IDirect3DDevice9_DrawIndexedPrimitiveUP_params[2],
		_IDirect3DDevice9_DrawIndexedPrimitiveUP_params[3],
		_IDirect3DDevice9_DrawIndexedPrimitiveUP_params[4],
		_IDirect3DDevice9_DrawIndexedPrimitiveUP_params[5],
		_IDirect3DDevice9_DrawIndexedPrimitiveUP_params[6],
		_IDirect3DDevice9_DrawIndexedPrimitiveUP_params[7],
		_IDirect3DDevice9_DrawIndexedPrimitiveUP_ret
		);

	// scene transitions seem to ,,, reset the shader state??
	// ok as far as i can tell, this shit renders 
	// the ui, and a few backgrounhd elements
	// the majority of the ui is PrimitiveCount==4. bg and a few ui elems is 2
	// also airdashes???
	// and charge ups
	// great, the func i put the most effort into doesnt do what i need
	
	if (_IDirect3DDevice9_DrawIndexedPrimitiveUP_params[3] == 2) {
		__device->SetPixelShader(pPixelShader);
	}
	*/
	
	POP_ALL;

	__asm {
		
		//jmp[_IDirect3DDevice9_DrawIndexedPrimitiveUP_addr];
		
		//int 3;
		call[_IDirect3DDevice9_DrawIndexedPrimitiveUP_addr];
		//int 3;
		
		//add esp, 024h;
		//mov eax, 0;
		//mov edx, 0;

		push _IDirect3DDevice9_DrawIndexedPrimitiveUP_ret
		ret;
	}
}
DWORD _IDirect3DDevice9_ProcessVertices_addr = 0;
__declspec(naked) void _IDirect3DDevice9_ProcessVertices_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_ProcessVertices called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_ProcessVertices_addr];
	}
}
DWORD _IDirect3DDevice9_CreateVertexDeclaration_addr = 0;
__declspec(naked) void _IDirect3DDevice9_CreateVertexDeclaration_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_CreateVertexDeclaration called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_CreateVertexDeclaration_addr];
	}
}
DWORD _IDirect3DDevice9_SetVertexDeclaration_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetVertexDeclaration_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetVertexDeclaration called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetVertexDeclaration_addr];
	}
}
DWORD _IDirect3DDevice9_GetVertexDeclaration_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetVertexDeclaration_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetVertexDeclaration called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetVertexDeclaration_addr];
	}
}
DWORD _IDirect3DDevice9_SetFVF_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetFVF_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetFVF called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetFVF_addr];
	}
}
DWORD _IDirect3DDevice9_GetFVF_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetFVF_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetFVF called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetFVF_addr];
	}
}
DWORD _IDirect3DDevice9_CreateVertexShader_addr = 0;
__declspec(naked) void _IDirect3DDevice9_CreateVertexShader_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_CreateVertexShader called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_CreateVertexShader_addr];
	}
}
DWORD _IDirect3DDevice9_SetVertexShader_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetVertexShader_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetVertexShader called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetVertexShader_addr];
		//mov eax, 0;
		//mov edx, 0;
		//ret 08h;
	}
}
DWORD _IDirect3DDevice9_GetVertexShader_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetVertexShader_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetVertexShader called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetVertexShader_addr];
	}
}
DWORD _IDirect3DDevice9_SetVertexShaderConstantF_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetVertexShaderConstantF_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetVertexShaderConstantF called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetVertexShaderConstantF_addr];
	}
}
DWORD _IDirect3DDevice9_GetVertexShaderConstantF_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetVertexShaderConstantF_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetVertexShaderConstantF called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetVertexShaderConstantF_addr];
	}
}
DWORD _IDirect3DDevice9_SetVertexShaderConstantI_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetVertexShaderConstantI_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetVertexShaderConstantI called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetVertexShaderConstantI_addr];
	}
}
DWORD _IDirect3DDevice9_GetVertexShaderConstantI_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetVertexShaderConstantI_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetVertexShaderConstantI called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetVertexShaderConstantI_addr];
	}
}
DWORD _IDirect3DDevice9_SetVertexShaderConstantB_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetVertexShaderConstantB_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetVertexShaderConstantB called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetVertexShaderConstantB_addr];
	}
}
DWORD _IDirect3DDevice9_GetVertexShaderConstantB_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetVertexShaderConstantB_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetVertexShaderConstantB called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetVertexShaderConstantB_addr];
	}
}
DWORD _IDirect3DDevice9_SetStreamSource_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetStreamSource_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetStreamSource called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetStreamSource_addr];
	}
}
DWORD _IDirect3DDevice9_GetStreamSource_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetStreamSource_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetStreamSource called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetStreamSource_addr];
	}
}
DWORD _IDirect3DDevice9_SetStreamSourceFreq_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetStreamSourceFreq_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetStreamSourceFreq called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetStreamSourceFreq_addr];
	}
}
DWORD _IDirect3DDevice9_GetStreamSourceFreq_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetStreamSourceFreq_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetStreamSourceFreq called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetStreamSourceFreq_addr];
	}
}
DWORD _IDirect3DDevice9_SetIndices_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetIndices_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetIndices called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetIndices_addr];
	}
}
DWORD _IDirect3DDevice9_GetIndices_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetIndices_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetIndices called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetIndices_addr];
	}
}
DWORD _IDirect3DDevice9_CreatePixelShader_addr = 0;
__declspec(naked) void _IDirect3DDevice9_CreatePixelShader_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_CreatePixelShader called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_CreatePixelShader_addr];
	}
}
DWORD _IDirect3DDevice9_SetPixelShader_reg = 0;
DWORD _IDirect3DDevice9_SetPixelShader_ret = 0;
DWORD _IDirect3DDevice9_SetPixelShader_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetPixelShader_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetPixelShader called!");
	POP_ALL;
	__asm {
		pop _IDirect3DDevice9_SetPixelShader_ret;
		mov eax, _IDirect3DDevice9_SetPixelShader_reg;

		//1mov eax, pPixelShader;
		//1mov[esp + 4], eax;

		mov _IDirect3DDevice9_SetPixelShader_reg, eax;
		call[_IDirect3DDevice9_SetPixelShader_addr];
	};

	PUSH_ALL;
	_log("      ret %08X", _IDirect3DDevice9_SetPixelShader_ret);
	POP_ALL;

	__asm {
		push _IDirect3DDevice9_SetPixelShader_ret;
		ret;
	};
}
DWORD _IDirect3DDevice9_GetPixelShader_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetPixelShader_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetPixelShader called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetPixelShader_addr];
	}
}

DWORD _startReg;
DWORD _floatData; // i hate MASM types sm
DWORD _vec4Count;
DWORD _IDirect3DDevice9_SetPixelShaderConstantF_ret = 0;
DWORD _IDirect3DDevice9_SetPixelShaderConstantF_reg = 0;
DWORD _IDirect3DDevice9_SetPixelShaderConstantF_addr = 0;

void __stdcall omfg() {
	_log("     startReg: %d", _startReg);
	_log("   _vec4Count: %d", _vec4Count);
	float temp;
	for (int i = 0; i < _vec4Count; i++) {

		//temp = ((float*)_floatData)[i * 4 + 0];
		//((float*)_floatData)[i * 4 + 0] = ((float*)_floatData)[i * 4 + 1];
		//((float*)_floatData)[i * 4 + 1] = temp;

		//((float*)_floatData)[i * 4 + 0] = 10.0;
		//((float*)_floatData)[i * 4 + 1] = 10.0;
		//((float*)_floatData)[i * 4 + 2] = 10.0;
		//((float*)_floatData)[i * 4 + 3] = 10.0;

		_log("              (%5.2f, %5.2f, %5.2f, %5.2f)", ((float*)_floatData)[i * 4 + 0], ((float*)_floatData)[i * 4 + 1], ((float*)_floatData)[i * 4 + 2], ((float*)_floatData)[i * 4 + 3]);

		


	}
	_log("          ret: %08X", _IDirect3DDevice9_SetPixelShaderConstantF_ret);
}

__declspec(naked) void _IDirect3DDevice9_SetPixelShaderConstantF_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetPixelShaderConstantF called!");
	POP_ALL;
	__asm {
		pop _IDirect3DDevice9_SetPixelShaderConstantF_ret;
		mov _IDirect3DDevice9_SetPixelShaderConstantF_reg, eax;

		mov eax, [esp + 4];
		mov _startReg, eax;
		mov eax, [esp + 8];
		mov _floatData, eax;
		mov eax, [esp + 12];
		mov _vec4Count, eax;

		mov eax, _IDirect3DDevice9_SetPixelShaderConstantF_reg;
		
	}

	PUSH_ALL;
	omfg();
	POP_ALL;

	__asm {

		call[_IDirect3DDevice9_SetPixelShaderConstantF_addr];

		push _IDirect3DDevice9_SetPixelShaderConstantF_ret;
		ret;
	};

}
DWORD _IDirect3DDevice9_GetPixelShaderConstantF_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetPixelShaderConstantF_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetPixelShaderConstantF called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetPixelShaderConstantF_addr];
	}
}
DWORD _IDirect3DDevice9_SetPixelShaderConstantI_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetPixelShaderConstantI_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetPixelShaderConstantI called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetPixelShaderConstantI_addr];
	}
}
DWORD _IDirect3DDevice9_GetPixelShaderConstantI_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetPixelShaderConstantI_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetPixelShaderConstantI called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetPixelShaderConstantI_addr];
	}
}
DWORD _IDirect3DDevice9_SetPixelShaderConstantB_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetPixelShaderConstantB_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetPixelShaderConstantB called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetPixelShaderConstantB_addr];
	}
}
DWORD _IDirect3DDevice9_GetPixelShaderConstantB_addr = 0;
__declspec(naked) void _IDirect3DDevice9_GetPixelShaderConstantB_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_GetPixelShaderConstantB called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_GetPixelShaderConstantB_addr];
	}
}
DWORD _IDirect3DDevice9_DrawRectPatch_addr = 0;
__declspec(naked) void _IDirect3DDevice9_DrawRectPatch_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_DrawRectPatch called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_DrawRectPatch_addr];
	}
}
DWORD _IDirect3DDevice9_DrawTriPatch_addr = 0;
__declspec(naked) void _IDirect3DDevice9_DrawTriPatch_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_DrawTriPatch called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_DrawTriPatch_addr];
	}
}
DWORD _IDirect3DDevice9_DeletePatch_addr = 0;
__declspec(naked) void _IDirect3DDevice9_DeletePatch_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_DeletePatch called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_DeletePatch_addr];
	}
}
DWORD _IDirect3DDevice9_CreateQuery_addr = 0;
__declspec(naked) void _IDirect3DDevice9_CreateQuery_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_CreateQuery called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_CreateQuery_addr];
	}
}
void HookThisShit(IDirect3DDevice9* _device) {

	if (_device == NULL) {
		return;
	}

	__device = _device;
	static bool init = false;

	if (!init) {
		init = true;

		initShader();

	}



	
	PUSH_ALL;
	__asm {
		// init IDirect3DDevice9_TestCooperativeLevel
		nop;
		mov eax, 0000000Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 0000000Ch];
		mov _IDirect3DDevice9_TestCooperativeLevel_addr, edx;
		mov edx, _IDirect3DDevice9_TestCooperativeLevel_func;
		mov[ecx + 0000000Ch], edx;
		// init IDirect3DDevice9_EvictManagedResources
		nop;
		mov eax, 00000014h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000014h];
		mov _IDirect3DDevice9_EvictManagedResources_addr, edx;
		mov edx, _IDirect3DDevice9_EvictManagedResources_func;
		mov[ecx + 00000014h], edx;
		// init IDirect3DDevice9_GetDirect3D
		nop;
		mov eax, 00000018h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000018h];
		mov _IDirect3DDevice9_GetDirect3D_addr, edx;
		mov edx, _IDirect3DDevice9_GetDirect3D_func;
		mov[ecx + 00000018h], edx;
		// init IDirect3DDevice9_GetDeviceCaps
		nop;
		mov eax, 0000001Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 0000001Ch];
		mov _IDirect3DDevice9_GetDeviceCaps_addr, edx;
		mov edx, _IDirect3DDevice9_GetDeviceCaps_func;
		mov[ecx + 0000001Ch], edx;
		// init IDirect3DDevice9_GetDisplayMode
		nop;
		mov eax, 00000020h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000020h];
		mov _IDirect3DDevice9_GetDisplayMode_addr, edx;
		mov edx, _IDirect3DDevice9_GetDisplayMode_func;
		mov[ecx + 00000020h], edx;
		// init IDirect3DDevice9_GetCreationParameters
		nop;
		mov eax, 00000024h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000024h];
		mov _IDirect3DDevice9_GetCreationParameters_addr, edx;
		mov edx, _IDirect3DDevice9_GetCreationParameters_func;
		mov[ecx + 00000024h], edx;
		// init IDirect3DDevice9_SetCursorProperties
		nop;
		mov eax, 00000028h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000028h];
		mov _IDirect3DDevice9_SetCursorProperties_addr, edx;
		mov edx, _IDirect3DDevice9_SetCursorProperties_func;
		mov[ecx + 00000028h], edx;
		// init IDirect3DDevice9_CreateAdditionalSwapChain
		nop;
		mov eax, 00000034h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000034h];
		mov _IDirect3DDevice9_CreateAdditionalSwapChain_addr, edx;
		mov edx, _IDirect3DDevice9_CreateAdditionalSwapChain_func;
		mov[ecx + 00000034h], edx;
		// init IDirect3DDevice9_GetSwapChain
		nop;
		mov eax, 00000038h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000038h];
		mov _IDirect3DDevice9_GetSwapChain_addr, edx;
		mov edx, _IDirect3DDevice9_GetSwapChain_func;
		mov[ecx + 00000038h], edx;
		// init IDirect3DDevice9_Reset
		nop;
		mov eax, 00000040h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000040h];
		mov _IDirect3DDevice9_Reset_addr, edx;
		mov edx, _IDirect3DDevice9_Reset_func;
		mov[ecx + 00000040h], edx;
		// init IDirect3DDevice9_Present
		nop;
		mov eax, 00000044h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000044h];
		mov _IDirect3DDevice9_Present_addr, edx;
		mov edx, _IDirect3DDevice9_Present_func;
		mov[ecx + 00000044h], edx;
		// init IDirect3DDevice9_GetBackBuffer
		nop;
		mov eax, 00000048h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000048h];
		mov _IDirect3DDevice9_GetBackBuffer_addr, edx;
		mov edx, _IDirect3DDevice9_GetBackBuffer_func;
		mov[ecx + 00000048h], edx;
		// init IDirect3DDevice9_GetRasterStatus
		nop;
		mov eax, 0000004Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 0000004Ch];
		mov _IDirect3DDevice9_GetRasterStatus_addr, edx;
		mov edx, _IDirect3DDevice9_GetRasterStatus_func;
		mov[ecx + 0000004Ch], edx;
		// init IDirect3DDevice9_SetDialogBoxMode
		nop;
		mov eax, 00000050h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000050h];
		mov _IDirect3DDevice9_SetDialogBoxMode_addr, edx;
		mov edx, _IDirect3DDevice9_SetDialogBoxMode_func;
		mov[ecx + 00000050h], edx;
		// init IDirect3DDevice9_CreateTexture
		nop;
		mov eax, 0000005Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 0000005Ch];
		mov _IDirect3DDevice9_CreateTexture_addr, edx;
		mov edx, _IDirect3DDevice9_CreateTexture_func;
		mov[ecx + 0000005Ch], edx;
		// init IDirect3DDevice9_CreateVolumeTexture
		nop;
		mov eax, 00000060h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000060h];
		mov _IDirect3DDevice9_CreateVolumeTexture_addr, edx;
		mov edx, _IDirect3DDevice9_CreateVolumeTexture_func;
		mov[ecx + 00000060h], edx;
		// init IDirect3DDevice9_CreateCubeTexture
		nop;
		mov eax, 00000064h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000064h];
		mov _IDirect3DDevice9_CreateCubeTexture_addr, edx;
		mov edx, _IDirect3DDevice9_CreateCubeTexture_func;
		mov[ecx + 00000064h], edx;
		// init IDirect3DDevice9_CreateVertexBuffer
		nop;
		mov eax, 00000068h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000068h];
		mov _IDirect3DDevice9_CreateVertexBuffer_addr, edx;
		mov edx, _IDirect3DDevice9_CreateVertexBuffer_func;
		mov[ecx + 00000068h], edx;
		// init IDirect3DDevice9_CreateIndexBuffer
		nop;
		mov eax, 0000006Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 0000006Ch];
		mov _IDirect3DDevice9_CreateIndexBuffer_addr, edx;
		mov edx, _IDirect3DDevice9_CreateIndexBuffer_func;
		mov[ecx + 0000006Ch], edx;
		// init IDirect3DDevice9_CreateRenderTarget
		nop;
		mov eax, 00000070h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000070h];
		mov _IDirect3DDevice9_CreateRenderTarget_addr, edx;
		mov edx, _IDirect3DDevice9_CreateRenderTarget_func;
		mov[ecx + 00000070h], edx;
		// init IDirect3DDevice9_CreateDepthStencilSurface
		nop;
		mov eax, 00000074h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000074h];
		mov _IDirect3DDevice9_CreateDepthStencilSurface_addr, edx;
		mov edx, _IDirect3DDevice9_CreateDepthStencilSurface_func;
		mov[ecx + 00000074h], edx;
		// init IDirect3DDevice9_UpdateSurface
		nop;
		mov eax, 00000078h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000078h];
		mov _IDirect3DDevice9_UpdateSurface_addr, edx;
		mov edx, _IDirect3DDevice9_UpdateSurface_func;
		mov[ecx + 00000078h], edx;
		// init IDirect3DDevice9_UpdateTexture
		nop;
		mov eax, 0000007Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 0000007Ch];
		mov _IDirect3DDevice9_UpdateTexture_addr, edx;
		mov edx, _IDirect3DDevice9_UpdateTexture_func;
		mov[ecx + 0000007Ch], edx;
		// init IDirect3DDevice9_GetRenderTargetData
		nop;
		mov eax, 00000080h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000080h];
		mov _IDirect3DDevice9_GetRenderTargetData_addr, edx;
		mov edx, _IDirect3DDevice9_GetRenderTargetData_func;
		mov[ecx + 00000080h], edx;
		// init IDirect3DDevice9_GetFrontBufferData
		nop;
		mov eax, 00000084h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000084h];
		mov _IDirect3DDevice9_GetFrontBufferData_addr, edx;
		mov edx, _IDirect3DDevice9_GetFrontBufferData_func;
		mov[ecx + 00000084h], edx;
		// init IDirect3DDevice9_StretchRect
		nop;
		mov eax, 00000088h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000088h];
		mov _IDirect3DDevice9_StretchRect_addr, edx;
		mov edx, _IDirect3DDevice9_StretchRect_func;
		mov[ecx + 00000088h], edx;
		// init IDirect3DDevice9_ColorFill
		nop;
		mov eax, 0000008Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 0000008Ch];
		mov _IDirect3DDevice9_ColorFill_addr, edx;
		mov edx, _IDirect3DDevice9_ColorFill_func;
		mov[ecx + 0000008Ch], edx;
		// init IDirect3DDevice9_CreateOffscreenPlainSurface
		nop;
		mov eax, 00000090h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000090h];
		mov _IDirect3DDevice9_CreateOffscreenPlainSurface_addr, edx;
		mov edx, _IDirect3DDevice9_CreateOffscreenPlainSurface_func;
		mov[ecx + 00000090h], edx;
		// init IDirect3DDevice9_SetRenderTarget
		nop;
		mov eax, 00000094h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000094h];
		mov _IDirect3DDevice9_SetRenderTarget_addr, edx;
		mov edx, _IDirect3DDevice9_SetRenderTarget_func;
		mov[ecx + 00000094h], edx;
		// init IDirect3DDevice9_GetRenderTarget
		nop;
		mov eax, 00000098h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000098h];
		mov _IDirect3DDevice9_GetRenderTarget_addr, edx;
		mov edx, _IDirect3DDevice9_GetRenderTarget_func;
		mov[ecx + 00000098h], edx;
		// init IDirect3DDevice9_SetDepthStencilSurface
		nop;
		mov eax, 0000009Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 0000009Ch];
		mov _IDirect3DDevice9_SetDepthStencilSurface_addr, edx;
		mov edx, _IDirect3DDevice9_SetDepthStencilSurface_func;
		mov[ecx + 0000009Ch], edx;
		// init IDirect3DDevice9_GetDepthStencilSurface
		nop;
		mov eax, 000000A0h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000A0h];
		mov _IDirect3DDevice9_GetDepthStencilSurface_addr, edx;
		mov edx, _IDirect3DDevice9_GetDepthStencilSurface_func;
		mov[ecx + 000000A0h], edx;
		// init IDirect3DDevice9_BeginScene
		nop;
		mov eax, 000000A4h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000A4h];
		mov _IDirect3DDevice9_BeginScene_addr, edx;
		mov edx, _IDirect3DDevice9_BeginScene_func;
		mov[ecx + 000000A4h], edx;
		// init IDirect3DDevice9_EndScene
		nop;
		mov eax, 000000A8h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000A8h];
		mov _IDirect3DDevice9_EndScene_addr, edx;
		mov edx, _IDirect3DDevice9_EndScene_func;
		mov[ecx + 000000A8h], edx;
		// init IDirect3DDevice9_Clear
		nop;
		mov eax, 000000ACh;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000ACh];
		mov _IDirect3DDevice9_Clear_addr, edx;
		mov edx, _IDirect3DDevice9_Clear_func;
		mov[ecx + 000000ACh], edx;
		// init IDirect3DDevice9_SetTransform
		nop;
		mov eax, 000000B0h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000B0h];
		mov _IDirect3DDevice9_SetTransform_addr, edx;
		mov edx, _IDirect3DDevice9_SetTransform_func;
		mov[ecx + 000000B0h], edx;
		// init IDirect3DDevice9_GetTransform
		nop;
		mov eax, 000000B4h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000B4h];
		mov _IDirect3DDevice9_GetTransform_addr, edx;
		mov edx, _IDirect3DDevice9_GetTransform_func;
		mov[ecx + 000000B4h], edx;
		// init IDirect3DDevice9_MultiplyTransform
		nop;
		mov eax, 000000B8h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000B8h];
		mov _IDirect3DDevice9_MultiplyTransform_addr, edx;
		mov edx, _IDirect3DDevice9_MultiplyTransform_func;
		mov[ecx + 000000B8h], edx;
		// init IDirect3DDevice9_SetViewport
		nop;
		mov eax, 000000BCh;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000BCh];
		mov _IDirect3DDevice9_SetViewport_addr, edx;
		mov edx, _IDirect3DDevice9_SetViewport_func;
		mov[ecx + 000000BCh], edx;
		// init IDirect3DDevice9_GetViewport
		nop;
		mov eax, 000000C0h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000C0h];
		mov _IDirect3DDevice9_GetViewport_addr, edx;
		mov edx, _IDirect3DDevice9_GetViewport_func;
		mov[ecx + 000000C0h], edx;
		// init IDirect3DDevice9_SetMaterial
		nop;
		mov eax, 000000C4h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000C4h];
		mov _IDirect3DDevice9_SetMaterial_addr, edx;
		mov edx, _IDirect3DDevice9_SetMaterial_func;
		mov[ecx + 000000C4h], edx;
		// init IDirect3DDevice9_GetMaterial
		nop;
		mov eax, 000000C8h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000C8h];
		mov _IDirect3DDevice9_GetMaterial_addr, edx;
		mov edx, _IDirect3DDevice9_GetMaterial_func;
		mov[ecx + 000000C8h], edx;
		// init IDirect3DDevice9_SetLight
		nop;
		mov eax, 000000CCh;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000CCh];
		mov _IDirect3DDevice9_SetLight_addr, edx;
		mov edx, _IDirect3DDevice9_SetLight_func;
		mov[ecx + 000000CCh], edx;
		// init IDirect3DDevice9_GetLight
		nop;
		mov eax, 000000D0h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000D0h];
		mov _IDirect3DDevice9_GetLight_addr, edx;
		mov edx, _IDirect3DDevice9_GetLight_func;
		mov[ecx + 000000D0h], edx;
		// init IDirect3DDevice9_LightEnable
		nop;
		mov eax, 000000D4h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000D4h];
		mov _IDirect3DDevice9_LightEnable_addr, edx;
		mov edx, _IDirect3DDevice9_LightEnable_func;
		mov[ecx + 000000D4h], edx;
		// init IDirect3DDevice9_GetLightEnable
		nop;
		mov eax, 000000D8h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000D8h];
		mov _IDirect3DDevice9_GetLightEnable_addr, edx;
		mov edx, _IDirect3DDevice9_GetLightEnable_func;
		mov[ecx + 000000D8h], edx;
		// init IDirect3DDevice9_SetClipPlane
		nop;
		mov eax, 000000DCh;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000DCh];
		mov _IDirect3DDevice9_SetClipPlane_addr, edx;
		mov edx, _IDirect3DDevice9_SetClipPlane_func;
		mov[ecx + 000000DCh], edx;
		// init IDirect3DDevice9_GetClipPlane
		nop;
		mov eax, 000000E0h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000E0h];
		mov _IDirect3DDevice9_GetClipPlane_addr, edx;
		mov edx, _IDirect3DDevice9_GetClipPlane_func;
		mov[ecx + 000000E0h], edx;
		// init IDirect3DDevice9_SetRenderState
		nop;
		mov eax, 000000E4h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000E4h];
		mov _IDirect3DDevice9_SetRenderState_addr, edx;
		mov edx, _IDirect3DDevice9_SetRenderState_func;
		mov[ecx + 000000E4h], edx;
		// init IDirect3DDevice9_GetRenderState
		nop;
		mov eax, 000000E8h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000E8h];
		mov _IDirect3DDevice9_GetRenderState_addr, edx;
		mov edx, _IDirect3DDevice9_GetRenderState_func;
		mov[ecx + 000000E8h], edx;
		// init IDirect3DDevice9_CreateStateBlock
		nop;
		mov eax, 000000ECh;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000ECh];
		mov _IDirect3DDevice9_CreateStateBlock_addr, edx;
		mov edx, _IDirect3DDevice9_CreateStateBlock_func;
		mov[ecx + 000000ECh], edx;
		// init IDirect3DDevice9_BeginStateBlock
		nop;
		mov eax, 000000F0h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000F0h];
		mov _IDirect3DDevice9_BeginStateBlock_addr, edx;
		mov edx, _IDirect3DDevice9_BeginStateBlock_func;
		mov[ecx + 000000F0h], edx;
		// init IDirect3DDevice9_EndStateBlock
		nop;
		mov eax, 000000F4h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000F4h];
		mov _IDirect3DDevice9_EndStateBlock_addr, edx;
		mov edx, _IDirect3DDevice9_EndStateBlock_func;
		mov[ecx + 000000F4h], edx;
		// init IDirect3DDevice9_SetClipStatus
		nop;
		mov eax, 000000F8h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000F8h];
		mov _IDirect3DDevice9_SetClipStatus_addr, edx;
		mov edx, _IDirect3DDevice9_SetClipStatus_func;
		mov[ecx + 000000F8h], edx;
		// init IDirect3DDevice9_GetClipStatus
		nop;
		mov eax, 000000FCh;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000000FCh];
		mov _IDirect3DDevice9_GetClipStatus_addr, edx;
		mov edx, _IDirect3DDevice9_GetClipStatus_func;
		mov[ecx + 000000FCh], edx;
		// init IDirect3DDevice9_GetTexture
		nop;
		mov eax, 00000100h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000100h];
		mov _IDirect3DDevice9_GetTexture_addr, edx;
		mov edx, _IDirect3DDevice9_GetTexture_func;
		mov[ecx + 00000100h], edx;
		// init IDirect3DDevice9_SetTexture
		nop;
		mov eax, 00000104h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000104h];
		mov _IDirect3DDevice9_SetTexture_addr, edx;
		mov edx, _IDirect3DDevice9_SetTexture_func;
		mov[ecx + 00000104h], edx;
		// init IDirect3DDevice9_GetTextureStageState
		nop;
		mov eax, 00000108h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000108h];
		mov _IDirect3DDevice9_GetTextureStageState_addr, edx;
		mov edx, _IDirect3DDevice9_GetTextureStageState_func;
		mov[ecx + 00000108h], edx;
		// init IDirect3DDevice9_SetTextureStageState
		nop;
		mov eax, 0000010Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 0000010Ch];
		mov _IDirect3DDevice9_SetTextureStageState_addr, edx;
		mov edx, _IDirect3DDevice9_SetTextureStageState_func;
		mov[ecx + 0000010Ch], edx;
		// init IDirect3DDevice9_GetSamplerState
		nop;
		mov eax, 00000110h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000110h];
		mov _IDirect3DDevice9_GetSamplerState_addr, edx;
		mov edx, _IDirect3DDevice9_GetSamplerState_func;
		mov[ecx + 00000110h], edx;
		// init IDirect3DDevice9_SetSamplerState
		nop;
		mov eax, 00000114h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000114h];
		mov _IDirect3DDevice9_SetSamplerState_addr, edx;
		mov edx, _IDirect3DDevice9_SetSamplerState_func;
		mov[ecx + 00000114h], edx;
		// init IDirect3DDevice9_ValidateDevice
		nop;
		mov eax, 00000118h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000118h];
		mov _IDirect3DDevice9_ValidateDevice_addr, edx;
		mov edx, _IDirect3DDevice9_ValidateDevice_func;
		mov[ecx + 00000118h], edx;
		// init IDirect3DDevice9_SetPaletteEntries
		nop;
		mov eax, 0000011Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 0000011Ch];
		mov _IDirect3DDevice9_SetPaletteEntries_addr, edx;
		mov edx, _IDirect3DDevice9_SetPaletteEntries_func;
		mov[ecx + 0000011Ch], edx;
		// init IDirect3DDevice9_GetPaletteEntries
		nop;
		mov eax, 00000120h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000120h];
		mov _IDirect3DDevice9_GetPaletteEntries_addr, edx;
		mov edx, _IDirect3DDevice9_GetPaletteEntries_func;
		mov[ecx + 00000120h], edx;
		// init IDirect3DDevice9_SetCurrentTexturePalette
		nop;
		mov eax, 00000124h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000124h];
		mov _IDirect3DDevice9_SetCurrentTexturePalette_addr, edx;
		mov edx, _IDirect3DDevice9_SetCurrentTexturePalette_func;
		mov[ecx + 00000124h], edx;
		// init IDirect3DDevice9_GetCurrentTexturePalette
		nop;
		mov eax, 00000128h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000128h];
		mov _IDirect3DDevice9_GetCurrentTexturePalette_addr, edx;
		mov edx, _IDirect3DDevice9_GetCurrentTexturePalette_func;
		mov[ecx + 00000128h], edx;
		// init IDirect3DDevice9_SetScissorRect
		nop;
		mov eax, 0000012Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 0000012Ch];
		mov _IDirect3DDevice9_SetScissorRect_addr, edx;
		mov edx, _IDirect3DDevice9_SetScissorRect_func;
		mov[ecx + 0000012Ch], edx;
		// init IDirect3DDevice9_GetScissorRect
		nop;
		mov eax, 00000130h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000130h];
		mov _IDirect3DDevice9_GetScissorRect_addr, edx;
		mov edx, _IDirect3DDevice9_GetScissorRect_func;
		mov[ecx + 00000130h], edx;
		// init IDirect3DDevice9_SetSoftwareVertexProcessing
		nop;
		mov eax, 00000134h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000134h];
		mov _IDirect3DDevice9_SetSoftwareVertexProcessing_addr, edx;
		mov edx, _IDirect3DDevice9_SetSoftwareVertexProcessing_func;
		mov[ecx + 00000134h], edx;
		// init IDirect3DDevice9_SetNPatchMode
		nop;
		mov eax, 0000013Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 0000013Ch];
		mov _IDirect3DDevice9_SetNPatchMode_addr, edx;
		mov edx, _IDirect3DDevice9_SetNPatchMode_func;
		mov[ecx + 0000013Ch], edx;
		// init IDirect3DDevice9_DrawPrimitive
		nop;
		mov eax, 00000144h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000144h];
		mov _IDirect3DDevice9_DrawPrimitive_addr, edx;
		mov edx, _IDirect3DDevice9_DrawPrimitive_func;
		mov[ecx + 00000144h], edx;
		// init IDirect3DDevice9_DrawIndexedPrimitive
		nop;
		mov eax, 00000148h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000148h];
		mov _IDirect3DDevice9_DrawIndexedPrimitive_addr, edx;
		mov edx, _IDirect3DDevice9_DrawIndexedPrimitive_func;
		mov[ecx + 00000148h], edx;
		// init IDirect3DDevice9_DrawPrimitiveUP
		nop;
		mov eax, 0000014Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 0000014Ch];
		mov _IDirect3DDevice9_DrawPrimitiveUP_addr, edx;
		mov edx, _IDirect3DDevice9_DrawPrimitiveUP_func;
		mov[ecx + 0000014Ch], edx;
		// init IDirect3DDevice9_DrawIndexedPrimitiveUP
		nop;
		mov eax, 00000150h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000150h];
		mov _IDirect3DDevice9_DrawIndexedPrimitiveUP_addr, edx;
		mov edx, _IDirect3DDevice9_DrawIndexedPrimitiveUP_func;
		mov[ecx + 00000150h], edx;
		// init IDirect3DDevice9_ProcessVertices
		nop;
		mov eax, 00000154h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000154h];
		mov _IDirect3DDevice9_ProcessVertices_addr, edx;
		mov edx, _IDirect3DDevice9_ProcessVertices_func;
		mov[ecx + 00000154h], edx;
		// init IDirect3DDevice9_CreateVertexDeclaration
		nop;
		mov eax, 00000158h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000158h];
		mov _IDirect3DDevice9_CreateVertexDeclaration_addr, edx;
		mov edx, _IDirect3DDevice9_CreateVertexDeclaration_func;
		mov[ecx + 00000158h], edx;
		// init IDirect3DDevice9_SetVertexDeclaration
		nop;
		mov eax, 0000015Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 0000015Ch];
		mov _IDirect3DDevice9_SetVertexDeclaration_addr, edx;
		mov edx, _IDirect3DDevice9_SetVertexDeclaration_func;
		mov[ecx + 0000015Ch], edx;
		// init IDirect3DDevice9_GetVertexDeclaration
		nop;
		mov eax, 00000160h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000160h];
		mov _IDirect3DDevice9_GetVertexDeclaration_addr, edx;
		mov edx, _IDirect3DDevice9_GetVertexDeclaration_func;
		mov[ecx + 00000160h], edx;
		// init IDirect3DDevice9_SetFVF
		nop;
		mov eax, 00000164h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000164h];
		mov _IDirect3DDevice9_SetFVF_addr, edx;
		mov edx, _IDirect3DDevice9_SetFVF_func;
		mov[ecx + 00000164h], edx;
		// init IDirect3DDevice9_GetFVF
		nop;
		mov eax, 00000168h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000168h];
		mov _IDirect3DDevice9_GetFVF_addr, edx;
		mov edx, _IDirect3DDevice9_GetFVF_func;
		mov[ecx + 00000168h], edx;
		// init IDirect3DDevice9_CreateVertexShader
		nop;
		mov eax, 0000016Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 0000016Ch];
		mov _IDirect3DDevice9_CreateVertexShader_addr, edx;
		mov edx, _IDirect3DDevice9_CreateVertexShader_func;
		mov[ecx + 0000016Ch], edx;
		// init IDirect3DDevice9_SetVertexShader
		nop;
		mov eax, 00000170h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000170h];
		mov _IDirect3DDevice9_SetVertexShader_addr, edx;
		mov edx, _IDirect3DDevice9_SetVertexShader_func;
		mov[ecx + 00000170h], edx;
		// init IDirect3DDevice9_GetVertexShader
		nop;
		mov eax, 00000174h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000174h];
		mov _IDirect3DDevice9_GetVertexShader_addr, edx;
		mov edx, _IDirect3DDevice9_GetVertexShader_func;
		mov[ecx + 00000174h], edx;
		// init IDirect3DDevice9_SetVertexShaderConstantF
		nop;
		mov eax, 00000178h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000178h];
		mov _IDirect3DDevice9_SetVertexShaderConstantF_addr, edx;
		mov edx, _IDirect3DDevice9_SetVertexShaderConstantF_func;
		mov[ecx + 00000178h], edx;
		// init IDirect3DDevice9_GetVertexShaderConstantF
		nop;
		mov eax, 0000017Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 0000017Ch];
		mov _IDirect3DDevice9_GetVertexShaderConstantF_addr, edx;
		mov edx, _IDirect3DDevice9_GetVertexShaderConstantF_func;
		mov[ecx + 0000017Ch], edx;
		// init IDirect3DDevice9_SetVertexShaderConstantI
		nop;
		mov eax, 00000180h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000180h];
		mov _IDirect3DDevice9_SetVertexShaderConstantI_addr, edx;
		mov edx, _IDirect3DDevice9_SetVertexShaderConstantI_func;
		mov[ecx + 00000180h], edx;
		// init IDirect3DDevice9_GetVertexShaderConstantI
		nop;
		mov eax, 00000184h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000184h];
		mov _IDirect3DDevice9_GetVertexShaderConstantI_addr, edx;
		mov edx, _IDirect3DDevice9_GetVertexShaderConstantI_func;
		mov[ecx + 00000184h], edx;
		// init IDirect3DDevice9_SetVertexShaderConstantB
		nop;
		mov eax, 00000188h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000188h];
		mov _IDirect3DDevice9_SetVertexShaderConstantB_addr, edx;
		mov edx, _IDirect3DDevice9_SetVertexShaderConstantB_func;
		mov[ecx + 00000188h], edx;
		// init IDirect3DDevice9_GetVertexShaderConstantB
		nop;
		mov eax, 0000018Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 0000018Ch];
		mov _IDirect3DDevice9_GetVertexShaderConstantB_addr, edx;
		mov edx, _IDirect3DDevice9_GetVertexShaderConstantB_func;
		mov[ecx + 0000018Ch], edx;
		// init IDirect3DDevice9_SetStreamSource
		nop;
		mov eax, 00000190h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000190h];
		mov _IDirect3DDevice9_SetStreamSource_addr, edx;
		mov edx, _IDirect3DDevice9_SetStreamSource_func;
		mov[ecx + 00000190h], edx;
		// init IDirect3DDevice9_GetStreamSource
		nop;
		mov eax, 00000194h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000194h];
		mov _IDirect3DDevice9_GetStreamSource_addr, edx;
		mov edx, _IDirect3DDevice9_GetStreamSource_func;
		mov[ecx + 00000194h], edx;
		// init IDirect3DDevice9_SetStreamSourceFreq
		nop;
		mov eax, 00000198h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 00000198h];
		mov _IDirect3DDevice9_SetStreamSourceFreq_addr, edx;
		mov edx, _IDirect3DDevice9_SetStreamSourceFreq_func;
		mov[ecx + 00000198h], edx;
		// init IDirect3DDevice9_GetStreamSourceFreq
		nop;
		mov eax, 0000019Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 0000019Ch];
		mov _IDirect3DDevice9_GetStreamSourceFreq_addr, edx;
		mov edx, _IDirect3DDevice9_GetStreamSourceFreq_func;
		mov[ecx + 0000019Ch], edx;
		// init IDirect3DDevice9_SetIndices
		nop;
		mov eax, 000001A0h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000001A0h];
		mov _IDirect3DDevice9_SetIndices_addr, edx;
		mov edx, _IDirect3DDevice9_SetIndices_func;
		mov[ecx + 000001A0h], edx;
		// init IDirect3DDevice9_GetIndices
		nop;
		mov eax, 000001A4h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000001A4h];
		mov _IDirect3DDevice9_GetIndices_addr, edx;
		mov edx, _IDirect3DDevice9_GetIndices_func;
		mov[ecx + 000001A4h], edx;
		// init IDirect3DDevice9_CreatePixelShader
		nop;
		mov eax, 000001A8h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000001A8h];
		mov _IDirect3DDevice9_CreatePixelShader_addr, edx;
		mov edx, _IDirect3DDevice9_CreatePixelShader_func;
		mov[ecx + 000001A8h], edx;
		// init IDirect3DDevice9_SetPixelShader
		nop;
		mov eax, 000001ACh;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000001ACh];
		mov _IDirect3DDevice9_SetPixelShader_addr, edx;
		mov edx, _IDirect3DDevice9_SetPixelShader_func;
		mov[ecx + 000001ACh], edx;
		// init IDirect3DDevice9_GetPixelShader
		nop;
		mov eax, 000001B0h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000001B0h];
		mov _IDirect3DDevice9_GetPixelShader_addr, edx;
		mov edx, _IDirect3DDevice9_GetPixelShader_func;
		mov[ecx + 000001B0h], edx;
		// init IDirect3DDevice9_SetPixelShaderConstantF
		nop;
		mov eax, 000001B4h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000001B4h];
		mov _IDirect3DDevice9_SetPixelShaderConstantF_addr, edx;
		mov edx, _IDirect3DDevice9_SetPixelShaderConstantF_func;
		mov[ecx + 000001B4h], edx;
		// init IDirect3DDevice9_GetPixelShaderConstantF
		nop;
		mov eax, 000001B8h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000001B8h];
		mov _IDirect3DDevice9_GetPixelShaderConstantF_addr, edx;
		mov edx, _IDirect3DDevice9_GetPixelShaderConstantF_func;
		mov[ecx + 000001B8h], edx;
		// init IDirect3DDevice9_SetPixelShaderConstantI
		nop;
		mov eax, 000001BCh;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000001BCh];
		mov _IDirect3DDevice9_SetPixelShaderConstantI_addr, edx;
		mov edx, _IDirect3DDevice9_SetPixelShaderConstantI_func;
		mov[ecx + 000001BCh], edx;
		// init IDirect3DDevice9_GetPixelShaderConstantI
		nop;
		mov eax, 000001C0h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000001C0h];
		mov _IDirect3DDevice9_GetPixelShaderConstantI_addr, edx;
		mov edx, _IDirect3DDevice9_GetPixelShaderConstantI_func;
		mov[ecx + 000001C0h], edx;
		// init IDirect3DDevice9_SetPixelShaderConstantB
		nop;
		mov eax, 000001C4h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000001C4h];
		mov _IDirect3DDevice9_SetPixelShaderConstantB_addr, edx;
		mov edx, _IDirect3DDevice9_SetPixelShaderConstantB_func;
		mov[ecx + 000001C4h], edx;
		// init IDirect3DDevice9_GetPixelShaderConstantB
		nop;
		mov eax, 000001C8h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000001C8h];
		mov _IDirect3DDevice9_GetPixelShaderConstantB_addr, edx;
		mov edx, _IDirect3DDevice9_GetPixelShaderConstantB_func;
		mov[ecx + 000001C8h], edx;
		// init IDirect3DDevice9_DrawRectPatch
		nop;
		mov eax, 000001CCh;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000001CCh];
		mov _IDirect3DDevice9_DrawRectPatch_addr, edx;
		mov edx, _IDirect3DDevice9_DrawRectPatch_func;
		mov[ecx + 000001CCh], edx;
		// init IDirect3DDevice9_DrawTriPatch
		nop;
		mov eax, 000001D0h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000001D0h];
		mov _IDirect3DDevice9_DrawTriPatch_addr, edx;
		mov edx, _IDirect3DDevice9_DrawTriPatch_func;
		mov[ecx + 000001D0h], edx;
		// init IDirect3DDevice9_DeletePatch
		nop;
		mov eax, 000001D4h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000001D4h];
		mov _IDirect3DDevice9_DeletePatch_addr, edx;
		mov edx, _IDirect3DDevice9_DeletePatch_func;
		mov[ecx + 000001D4h], edx;
		// init IDirect3DDevice9_CreateQuery
		nop;
		mov eax, 000001D8h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, [ecx + 000001D8h];
		mov _IDirect3DDevice9_CreateQuery_addr, edx;
		mov edx, _IDirect3DDevice9_CreateQuery_func;
		mov[ecx + 000001D8h], edx;
	}
	POP_ALL;
}

void UnhookThisShit() {
	PUSH_ALL;
	__asm {
		// unhook IDirect3DDevice9_TestCooperativeLevel
		nop;
		mov eax, 0000000Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_TestCooperativeLevel_addr;
		mov[ecx + 0000000Ch], edx;
		// unhook IDirect3DDevice9_EvictManagedResources
		nop;
		mov eax, 00000014h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_EvictManagedResources_addr;
		mov[ecx + 00000014h], edx;
		// unhook IDirect3DDevice9_GetDirect3D
		nop;
		mov eax, 00000018h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetDirect3D_addr;
		mov[ecx + 00000018h], edx;
		// unhook IDirect3DDevice9_GetDeviceCaps
		nop;
		mov eax, 0000001Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetDeviceCaps_addr;
		mov[ecx + 0000001Ch], edx;
		// unhook IDirect3DDevice9_GetDisplayMode
		nop;
		mov eax, 00000020h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetDisplayMode_addr;
		mov[ecx + 00000020h], edx;
		// unhook IDirect3DDevice9_GetCreationParameters
		nop;
		mov eax, 00000024h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetCreationParameters_addr;
		mov[ecx + 00000024h], edx;
		// unhook IDirect3DDevice9_SetCursorProperties
		nop;
		mov eax, 00000028h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetCursorProperties_addr;
		mov[ecx + 00000028h], edx;
		// unhook IDirect3DDevice9_CreateAdditionalSwapChain
		nop;
		mov eax, 00000034h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_CreateAdditionalSwapChain_addr;
		mov[ecx + 00000034h], edx;
		// unhook IDirect3DDevice9_GetSwapChain
		nop;
		mov eax, 00000038h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetSwapChain_addr;
		mov[ecx + 00000038h], edx;
		// unhook IDirect3DDevice9_Reset
		nop;
		mov eax, 00000040h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_Reset_addr;
		mov[ecx + 00000040h], edx;
		// unhook IDirect3DDevice9_Present
		nop;
		mov eax, 00000044h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_Present_addr;
		mov[ecx + 00000044h], edx;
		// unhook IDirect3DDevice9_GetBackBuffer
		nop;
		mov eax, 00000048h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetBackBuffer_addr;
		mov[ecx + 00000048h], edx;
		// unhook IDirect3DDevice9_GetRasterStatus
		nop;
		mov eax, 0000004Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetRasterStatus_addr;
		mov[ecx + 0000004Ch], edx;
		// unhook IDirect3DDevice9_SetDialogBoxMode
		nop;
		mov eax, 00000050h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetDialogBoxMode_addr;
		mov[ecx + 00000050h], edx;
		// unhook IDirect3DDevice9_CreateTexture
		nop;
		mov eax, 0000005Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_CreateTexture_addr;
		mov[ecx + 0000005Ch], edx;
		// unhook IDirect3DDevice9_CreateVolumeTexture
		nop;
		mov eax, 00000060h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_CreateVolumeTexture_addr;
		mov[ecx + 00000060h], edx;
		// unhook IDirect3DDevice9_CreateCubeTexture
		nop;
		mov eax, 00000064h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_CreateCubeTexture_addr;
		mov[ecx + 00000064h], edx;
		// unhook IDirect3DDevice9_CreateVertexBuffer
		nop;
		mov eax, 00000068h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_CreateVertexBuffer_addr;
		mov[ecx + 00000068h], edx;
		// unhook IDirect3DDevice9_CreateIndexBuffer
		nop;
		mov eax, 0000006Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_CreateIndexBuffer_addr;
		mov[ecx + 0000006Ch], edx;
		// unhook IDirect3DDevice9_CreateRenderTarget
		nop;
		mov eax, 00000070h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_CreateRenderTarget_addr;
		mov[ecx + 00000070h], edx;
		// unhook IDirect3DDevice9_CreateDepthStencilSurface
		nop;
		mov eax, 00000074h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_CreateDepthStencilSurface_addr;
		mov[ecx + 00000074h], edx;
		// unhook IDirect3DDevice9_UpdateSurface
		nop;
		mov eax, 00000078h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_UpdateSurface_addr;
		mov[ecx + 00000078h], edx;
		// unhook IDirect3DDevice9_UpdateTexture
		nop;
		mov eax, 0000007Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_UpdateTexture_addr;
		mov[ecx + 0000007Ch], edx;
		// unhook IDirect3DDevice9_GetRenderTargetData
		nop;
		mov eax, 00000080h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetRenderTargetData_addr;
		mov[ecx + 00000080h], edx;
		// unhook IDirect3DDevice9_GetFrontBufferData
		nop;
		mov eax, 00000084h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetFrontBufferData_addr;
		mov[ecx + 00000084h], edx;
		// unhook IDirect3DDevice9_StretchRect
		nop;
		mov eax, 00000088h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_StretchRect_addr;
		mov[ecx + 00000088h], edx;
		// unhook IDirect3DDevice9_ColorFill
		nop;
		mov eax, 0000008Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_ColorFill_addr;
		mov[ecx + 0000008Ch], edx;
		// unhook IDirect3DDevice9_CreateOffscreenPlainSurface
		nop;
		mov eax, 00000090h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_CreateOffscreenPlainSurface_addr;
		mov[ecx + 00000090h], edx;
		// unhook IDirect3DDevice9_SetRenderTarget
		nop;
		mov eax, 00000094h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetRenderTarget_addr;
		mov[ecx + 00000094h], edx;
		// unhook IDirect3DDevice9_GetRenderTarget
		nop;
		mov eax, 00000098h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetRenderTarget_addr;
		mov[ecx + 00000098h], edx;
		// unhook IDirect3DDevice9_SetDepthStencilSurface
		nop;
		mov eax, 0000009Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetDepthStencilSurface_addr;
		mov[ecx + 0000009Ch], edx;
		// unhook IDirect3DDevice9_GetDepthStencilSurface
		nop;
		mov eax, 000000A0h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetDepthStencilSurface_addr;
		mov[ecx + 000000A0h], edx;
		// unhook IDirect3DDevice9_BeginScene
		nop;
		mov eax, 000000A4h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_BeginScene_addr;
		mov[ecx + 000000A4h], edx;
		// unhook IDirect3DDevice9_EndScene
		nop;
		mov eax, 000000A8h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_EndScene_addr;
		mov[ecx + 000000A8h], edx;
		// unhook IDirect3DDevice9_Clear
		nop;
		mov eax, 000000ACh;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_Clear_addr;
		mov[ecx + 000000ACh], edx;
		// unhook IDirect3DDevice9_SetTransform
		nop;
		mov eax, 000000B0h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetTransform_addr;
		mov[ecx + 000000B0h], edx;
		// unhook IDirect3DDevice9_GetTransform
		nop;
		mov eax, 000000B4h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetTransform_addr;
		mov[ecx + 000000B4h], edx;
		// unhook IDirect3DDevice9_MultiplyTransform
		nop;
		mov eax, 000000B8h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_MultiplyTransform_addr;
		mov[ecx + 000000B8h], edx;
		// unhook IDirect3DDevice9_SetViewport
		nop;
		mov eax, 000000BCh;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetViewport_addr;
		mov[ecx + 000000BCh], edx;
		// unhook IDirect3DDevice9_GetViewport
		nop;
		mov eax, 000000C0h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetViewport_addr;
		mov[ecx + 000000C0h], edx;
		// unhook IDirect3DDevice9_SetMaterial
		nop;
		mov eax, 000000C4h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetMaterial_addr;
		mov[ecx + 000000C4h], edx;
		// unhook IDirect3DDevice9_GetMaterial
		nop;
		mov eax, 000000C8h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetMaterial_addr;
		mov[ecx + 000000C8h], edx;
		// unhook IDirect3DDevice9_SetLight
		nop;
		mov eax, 000000CCh;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetLight_addr;
		mov[ecx + 000000CCh], edx;
		// unhook IDirect3DDevice9_GetLight
		nop;
		mov eax, 000000D0h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetLight_addr;
		mov[ecx + 000000D0h], edx;
		// unhook IDirect3DDevice9_LightEnable
		nop;
		mov eax, 000000D4h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_LightEnable_addr;
		mov[ecx + 000000D4h], edx;
		// unhook IDirect3DDevice9_GetLightEnable
		nop;
		mov eax, 000000D8h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetLightEnable_addr;
		mov[ecx + 000000D8h], edx;
		// unhook IDirect3DDevice9_SetClipPlane
		nop;
		mov eax, 000000DCh;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetClipPlane_addr;
		mov[ecx + 000000DCh], edx;
		// unhook IDirect3DDevice9_GetClipPlane
		nop;
		mov eax, 000000E0h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetClipPlane_addr;
		mov[ecx + 000000E0h], edx;
		// unhook IDirect3DDevice9_SetRenderState
		nop;
		mov eax, 000000E4h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetRenderState_addr;
		mov[ecx + 000000E4h], edx;
		// unhook IDirect3DDevice9_GetRenderState
		nop;
		mov eax, 000000E8h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetRenderState_addr;
		mov[ecx + 000000E8h], edx;
		// unhook IDirect3DDevice9_CreateStateBlock
		nop;
		mov eax, 000000ECh;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_CreateStateBlock_addr;
		mov[ecx + 000000ECh], edx;
		// unhook IDirect3DDevice9_BeginStateBlock
		nop;
		mov eax, 000000F0h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_BeginStateBlock_addr;
		mov[ecx + 000000F0h], edx;
		// unhook IDirect3DDevice9_EndStateBlock
		nop;
		mov eax, 000000F4h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_EndStateBlock_addr;
		mov[ecx + 000000F4h], edx;
		// unhook IDirect3DDevice9_SetClipStatus
		nop;
		mov eax, 000000F8h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetClipStatus_addr;
		mov[ecx + 000000F8h], edx;
		// unhook IDirect3DDevice9_GetClipStatus
		nop;
		mov eax, 000000FCh;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetClipStatus_addr;
		mov[ecx + 000000FCh], edx;
		// unhook IDirect3DDevice9_GetTexture
		nop;
		mov eax, 00000100h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetTexture_addr;
		mov[ecx + 00000100h], edx;
		// unhook IDirect3DDevice9_SetTexture
		nop;
		mov eax, 00000104h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetTexture_addr;
		mov[ecx + 00000104h], edx;
		// unhook IDirect3DDevice9_GetTextureStageState
		nop;
		mov eax, 00000108h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetTextureStageState_addr;
		mov[ecx + 00000108h], edx;
		// unhook IDirect3DDevice9_SetTextureStageState
		nop;
		mov eax, 0000010Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetTextureStageState_addr;
		mov[ecx + 0000010Ch], edx;
		// unhook IDirect3DDevice9_GetSamplerState
		nop;
		mov eax, 00000110h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetSamplerState_addr;
		mov[ecx + 00000110h], edx;
		// unhook IDirect3DDevice9_SetSamplerState
		nop;
		mov eax, 00000114h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetSamplerState_addr;
		mov[ecx + 00000114h], edx;
		// unhook IDirect3DDevice9_ValidateDevice
		nop;
		mov eax, 00000118h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_ValidateDevice_addr;
		mov[ecx + 00000118h], edx;
		// unhook IDirect3DDevice9_SetPaletteEntries
		nop;
		mov eax, 0000011Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetPaletteEntries_addr;
		mov[ecx + 0000011Ch], edx;
		// unhook IDirect3DDevice9_GetPaletteEntries
		nop;
		mov eax, 00000120h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetPaletteEntries_addr;
		mov[ecx + 00000120h], edx;
		// unhook IDirect3DDevice9_SetCurrentTexturePalette
		nop;
		mov eax, 00000124h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetCurrentTexturePalette_addr;
		mov[ecx + 00000124h], edx;
		// unhook IDirect3DDevice9_GetCurrentTexturePalette
		nop;
		mov eax, 00000128h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetCurrentTexturePalette_addr;
		mov[ecx + 00000128h], edx;
		// unhook IDirect3DDevice9_SetScissorRect
		nop;
		mov eax, 0000012Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetScissorRect_addr;
		mov[ecx + 0000012Ch], edx;
		// unhook IDirect3DDevice9_GetScissorRect
		nop;
		mov eax, 00000130h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetScissorRect_addr;
		mov[ecx + 00000130h], edx;
		// unhook IDirect3DDevice9_SetSoftwareVertexProcessing
		nop;
		mov eax, 00000134h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetSoftwareVertexProcessing_addr;
		mov[ecx + 00000134h], edx;
		// unhook IDirect3DDevice9_SetNPatchMode
		nop;
		mov eax, 0000013Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetNPatchMode_addr;
		mov[ecx + 0000013Ch], edx;
		// unhook IDirect3DDevice9_DrawPrimitive
		nop;
		mov eax, 00000144h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_DrawPrimitive_addr;
		mov[ecx + 00000144h], edx;
		// unhook IDirect3DDevice9_DrawIndexedPrimitive
		nop;
		mov eax, 00000148h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_DrawIndexedPrimitive_addr;
		mov[ecx + 00000148h], edx;
		// unhook IDirect3DDevice9_DrawPrimitiveUP
		nop;
		mov eax, 0000014Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_DrawPrimitiveUP_addr;
		mov[ecx + 0000014Ch], edx;
		// unhook IDirect3DDevice9_DrawIndexedPrimitiveUP
		nop;
		mov eax, 00000150h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_DrawIndexedPrimitiveUP_addr;
		mov[ecx + 00000150h], edx;
		// unhook IDirect3DDevice9_ProcessVertices
		nop;
		mov eax, 00000154h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_ProcessVertices_addr;
		mov[ecx + 00000154h], edx;
		// unhook IDirect3DDevice9_CreateVertexDeclaration
		nop;
		mov eax, 00000158h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_CreateVertexDeclaration_addr;
		mov[ecx + 00000158h], edx;
		// unhook IDirect3DDevice9_SetVertexDeclaration
		nop;
		mov eax, 0000015Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetVertexDeclaration_addr;
		mov[ecx + 0000015Ch], edx;
		// unhook IDirect3DDevice9_GetVertexDeclaration
		nop;
		mov eax, 00000160h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetVertexDeclaration_addr;
		mov[ecx + 00000160h], edx;
		// unhook IDirect3DDevice9_SetFVF
		nop;
		mov eax, 00000164h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetFVF_addr;
		mov[ecx + 00000164h], edx;
		// unhook IDirect3DDevice9_GetFVF
		nop;
		mov eax, 00000168h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetFVF_addr;
		mov[ecx + 00000168h], edx;
		// unhook IDirect3DDevice9_CreateVertexShader
		nop;
		mov eax, 0000016Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_CreateVertexShader_addr;
		mov[ecx + 0000016Ch], edx;
		// unhook IDirect3DDevice9_SetVertexShader
		nop;
		mov eax, 00000170h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetVertexShader_addr;
		mov[ecx + 00000170h], edx;
		// unhook IDirect3DDevice9_GetVertexShader
		nop;
		mov eax, 00000174h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetVertexShader_addr;
		mov[ecx + 00000174h], edx;
		// unhook IDirect3DDevice9_SetVertexShaderConstantF
		nop;
		mov eax, 00000178h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetVertexShaderConstantF_addr;
		mov[ecx + 00000178h], edx;
		// unhook IDirect3DDevice9_GetVertexShaderConstantF
		nop;
		mov eax, 0000017Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetVertexShaderConstantF_addr;
		mov[ecx + 0000017Ch], edx;
		// unhook IDirect3DDevice9_SetVertexShaderConstantI
		nop;
		mov eax, 00000180h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetVertexShaderConstantI_addr;
		mov[ecx + 00000180h], edx;
		// unhook IDirect3DDevice9_GetVertexShaderConstantI
		nop;
		mov eax, 00000184h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetVertexShaderConstantI_addr;
		mov[ecx + 00000184h], edx;
		// unhook IDirect3DDevice9_SetVertexShaderConstantB
		nop;
		mov eax, 00000188h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetVertexShaderConstantB_addr;
		mov[ecx + 00000188h], edx;
		// unhook IDirect3DDevice9_GetVertexShaderConstantB
		nop;
		mov eax, 0000018Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetVertexShaderConstantB_addr;
		mov[ecx + 0000018Ch], edx;
		// unhook IDirect3DDevice9_SetStreamSource
		nop;
		mov eax, 00000190h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetStreamSource_addr;
		mov[ecx + 00000190h], edx;
		// unhook IDirect3DDevice9_GetStreamSource
		nop;
		mov eax, 00000194h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetStreamSource_addr;
		mov[ecx + 00000194h], edx;
		// unhook IDirect3DDevice9_SetStreamSourceFreq
		nop;
		mov eax, 00000198h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetStreamSourceFreq_addr;
		mov[ecx + 00000198h], edx;
		// unhook IDirect3DDevice9_GetStreamSourceFreq
		nop;
		mov eax, 0000019Ch;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetStreamSourceFreq_addr;
		mov[ecx + 0000019Ch], edx;
		// unhook IDirect3DDevice9_SetIndices
		nop;
		mov eax, 000001A0h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetIndices_addr;
		mov[ecx + 000001A0h], edx;
		// unhook IDirect3DDevice9_GetIndices
		nop;
		mov eax, 000001A4h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetIndices_addr;
		mov[ecx + 000001A4h], edx;
		// unhook IDirect3DDevice9_CreatePixelShader
		nop;
		mov eax, 000001A8h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_CreatePixelShader_addr;
		mov[ecx + 000001A8h], edx;
		// unhook IDirect3DDevice9_SetPixelShader
		nop;
		mov eax, 000001ACh;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetPixelShader_addr;
		mov[ecx + 000001ACh], edx;
		// unhook IDirect3DDevice9_GetPixelShader
		nop;
		mov eax, 000001B0h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetPixelShader_addr;
		mov[ecx + 000001B0h], edx;
		// unhook IDirect3DDevice9_SetPixelShaderConstantF
		nop;
		mov eax, 000001B4h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetPixelShaderConstantF_addr;
		mov[ecx + 000001B4h], edx;
		// unhook IDirect3DDevice9_GetPixelShaderConstantF
		nop;
		mov eax, 000001B8h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetPixelShaderConstantF_addr;
		mov[ecx + 000001B8h], edx;
		// unhook IDirect3DDevice9_SetPixelShaderConstantI
		nop;
		mov eax, 000001BCh;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetPixelShaderConstantI_addr;
		mov[ecx + 000001BCh], edx;
		// unhook IDirect3DDevice9_GetPixelShaderConstantI
		nop;
		mov eax, 000001C0h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetPixelShaderConstantI_addr;
		mov[ecx + 000001C0h], edx;
		// unhook IDirect3DDevice9_SetPixelShaderConstantB
		nop;
		mov eax, 000001C4h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_SetPixelShaderConstantB_addr;
		mov[ecx + 000001C4h], edx;
		// unhook IDirect3DDevice9_GetPixelShaderConstantB
		nop;
		mov eax, 000001C8h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_GetPixelShaderConstantB_addr;
		mov[ecx + 000001C8h], edx;
		// unhook IDirect3DDevice9_DrawRectPatch
		nop;
		mov eax, 000001CCh;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_DrawRectPatch_addr;
		mov[ecx + 000001CCh], edx;
		// unhook IDirect3DDevice9_DrawTriPatch
		nop;
		mov eax, 000001D0h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_DrawTriPatch_addr;
		mov[ecx + 000001D0h], edx;
		// unhook IDirect3DDevice9_DeletePatch
		nop;
		mov eax, 000001D4h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_DeletePatch_addr;
		mov[ecx + 000001D4h], edx;
		// unhook IDirect3DDevice9_CreateQuery
		nop;
		mov eax, 000001D8h;
		mov eax, [__device];
		mov ecx, [eax];
		mov edx, _IDirect3DDevice9_CreateQuery_addr;
		mov[ecx + 000001D8h], edx;
	}
	POP_ALL;
}