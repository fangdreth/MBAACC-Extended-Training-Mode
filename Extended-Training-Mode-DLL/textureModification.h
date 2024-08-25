#pragma once

#include <set>

extern IDirect3DDevice9* device;

// the shader
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



// this is a set intentionally, not unordered, as i believe that the size of this set will be reasonably small < 256 DWORDS
std::set<DWORD> textureAddrs;

/*
options are:
004331D9
004332e1
0043306F
*/
DWORD leadToDrawPrimHook_ret = 0;

// actual funcs

DWORD drawPrimHook_texAddr;
void drawPrimHook() {

	static unsigned index = 0;

	if (leadToDrawPrimHook_ret != 0x004331D9) {
		index = 0;
		textureAddrs.clear(); // calling this repeatedly is wasteful!
		return;
	}

	if (textureAddrs.contains(drawPrimHook_texAddr)) {
		device->SetPixelShader(pPixelShader);
	}



	log("%3d primHook tex addr %08X ret: %08X", index, drawPrimHook_texAddr, leadToDrawPrimHook_ret);

	index++;
}

void drawPrimCallback() {

}
 
// naked funcs

DWORD _naked_drawPrimHook_reg;
DWORD _naked_drawPrimHook_jmp = 0x004be296;
__declspec(naked) void _naked_drawPrimHook() {
	__asm {
		mov _naked_drawPrimHook_reg, eax;

		// this MAY need null checking!

		mov eax, dword ptr [ebx + 54h];
		mov eax, dword ptr [eax + 0Ch];

		mov drawPrimHook_texAddr, eax;

		mov eax, _naked_drawPrimHook_reg;
	};
	PUSH_ALL;
	drawPrimHook();
	POP_ALL;
	__asm {
		sub esp, 8h;
		mov ecx, dword ptr[ebx + 54h];
		jmp[_naked_drawPrimHook_jmp];
	}
}

__declspec(naked) void _naked_drawPrimCallback() {
	PUSH_ALL;
	drawPrimCallback();
	POP_ALL;
	__asm {
		ret;
	};
}

//DWORD* __absolutelyUseless = (DWORD*)0x0076e7c8;
DWORD _naked_leadToDrawPrimHook_jmp = 0x004c0385;
__declspec(naked) void _naked_leadToDrawPrimHook() {
	__asm {
		// eax is clobbered on the first instr of the func, so i can do what i desire with it

		//int 3;

		mov eax, [esp + 8h];
		mov leadToDrawPrimHook_ret, eax
		// i despise masm. 
		// thank you so much for taking my dword ptr, and [] and deciding that no, those mean nothing
		// i totally need to load an immediate here! because dword ptr and [] just give off such immediate vibes!
		//mov eax, dword ptr [0076e7c8h];
		// somehow, with a pointer, it then, doesnt load pointer data, and loads the pointer addr?
		// so NOW it wants me to [] and dword ptr?
		// mov eax, __absolutelyUseless; // doesnt work
		// mov eax, dword ptr [__absolutelyUseless]; // doesnt work
	}

	// the assembler does not deserve to assemble this code:
	__asm _emit 0xA1;
	__asm _emit 0xC8;
	__asm _emit 0xE7;
	__asm _emit 0x76;
	__asm _emit 0x00;


	__asm {
		jmp[_naked_leadToDrawPrimHook_jmp];
	}

}

// init 

void initDrawPrimHook() {
	patchJump(0x004be290, _naked_drawPrimHook);
}

void initDrawPrimCallback() {
	patchJump(0x004be524, _naked_drawPrimCallback);
}

void initLeadToDrawPrimHook() {
	patchJump(0x004c0380, _naked_leadToDrawPrimHook);
}

bool initTextureModifications() {

	if (device == NULL) {
		return false;
	}

	static bool init = false;
	if (init) { // im paranoid
		return;
	}

	initShader();

	initDrawPrimHook();
	initDrawPrimCallback();
	initLeadToDrawPrimHook();




	log("initTextureModifications ran");
	
	init = true;
	return true;
}