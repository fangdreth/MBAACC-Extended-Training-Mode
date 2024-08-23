#pragma once
#include <Windows.h>

void __stdcall _log(const char* s) {
	//log(s);
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
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_Reset_addr];
	}
}
DWORD _IDirect3DDevice9_Present_addr = 0;
__declspec(naked) void _IDirect3DDevice9_Present_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_Present called!");
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
DWORD _IDirect3DDevice9_CreateTexture_addr_ret;
DWORD _IDirect3DDevice9_CreateTexture_addr = 0;
__declspec(naked) void _IDirect3DDevice9_CreateTexture_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_CreateTexture called!");
	POP_ALL;
	__asm {
		//pop _IDirect3DDevice9_CreateTexture_addr_ret;

		//call[_IDirect3DDevice9_CreateTexture_addr];
		jmp[_IDirect3DDevice9_CreateTexture_addr];
	}
	/*
	PUSH_ALL;
	log("IDirect3DDevice9_CreateTexture ret was %08X", _IDirect3DDevice9_CreateTexture_addr_ret);
	POP_ALL;

	__asm {
		push _IDirect3DDevice9_CreateTexture_addr_ret;
		ret;
	}
	*/
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
DWORD _IDirect3DDevice9_SetRenderTarget_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetRenderTarget_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetRenderTarget called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetRenderTarget_addr];
	}
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
DWORD _IDirect3DDevice9_EndScene_addr = 0;
__declspec(naked) void _IDirect3DDevice9_EndScene_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_EndScene called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_EndScene_addr];
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
DWORD _IDirect3DDevice9_SetRenderState_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetRenderState_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetRenderState called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetRenderState_addr];
	}
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
DWORD _IDirect3DDevice9_BeginStateBlock_addr = 0;
__declspec(naked) void _IDirect3DDevice9_BeginStateBlock_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_BeginStateBlock called!");
	POP_ALL;
	__asm {
		mov needHookReset, 1;
		jmp[_IDirect3DDevice9_BeginStateBlock_addr];
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
DWORD _IDirect3DDevice9_SetTexture_retAddr = 0;
DWORD _IDirect3DDevice9_SetTexture_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetTexture_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetTexture called!");
	POP_ALL;
	__asm {
		pop _IDirect3DDevice9_SetTexture_retAddr;

		call[_IDirect3DDevice9_SetTexture_addr];
	}

	PUSH_ALL;
	//log("IDirect3DDevice9_SetTexture ret was %08X", _IDirect3DDevice9_SetTexture_retAddr);
	POP_ALL;

	__asm {
		push _IDirect3DDevice9_SetTexture_retAddr;
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
DWORD _IDirect3DDevice9_SetSamplerState_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetSamplerState_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetSamplerState called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetSamplerState_addr];
		//mov eax, 0;
		//mov edx, 0;
		//ret 10h;
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
	POP_ALL;
	__asm {
		pop _IDirect3DDevice9_DrawIndexedPrimitiveUP_ret;
	};

	__asm {
		mov _IDirect3DDevice9_DrawIndexedPrimitiveUP_reg, eax;


		mov eax, [esp + 16];
		shr eax, 1;
		cmp eax, 0;

		JNE _OK;
		mov eax, 1;

	_OK:

		mov [esp + 16], eax;

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
	//log("         PrimitiveType: %d", _IDirect3DDevice9_DrawIndexedPrimitiveUP_params[0]);
	//log("        MinVertexIndex: %d", _IDirect3DDevice9_DrawIndexedPrimitiveUP_params[1]);
	//log("           NumVertices: %d", _IDirect3DDevice9_DrawIndexedPrimitiveUP_params[2]);
	//log("        PrimitiveCount: %d", _IDirect3DDevice9_DrawIndexedPrimitiveUP_params[3]);
	//log("           *pIndexData: %08X", _IDirect3DDevice9_DrawIndexedPrimitiveUP_params[4]);
	//log("       IndexDataFormat: %d", _IDirect3DDevice9_DrawIndexedPrimitiveUP_params[5]);
	//log("*pVertexStreamZeroData: %08X", _IDirect3DDevice9_DrawIndexedPrimitiveUP_params[6]);
	//log("VertexStreamZeroStride: %d", _IDirect3DDevice9_DrawIndexedPrimitiveUP_params[7]);
	//log("-----");
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
DWORD _IDirect3DDevice9_SetPixelShader_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetPixelShader_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetPixelShader called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetPixelShader_addr];
		//mov eax, 0;
		//mov edx, 0;
		//ret 08h;
	}
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
DWORD _IDirect3DDevice9_SetPixelShaderConstantF_addr = 0;
__declspec(naked) void _IDirect3DDevice9_SetPixelShaderConstantF_func() {
	PUSH_ALL;
	_log("IDirect3DDevice9_SetPixelShaderConstantF called!");
	POP_ALL;
	__asm {
		jmp[_IDirect3DDevice9_SetPixelShaderConstantF_addr];
		//mov eax, 0;
		//mov edx, 0;
		//ret 10h;
	}
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
IDirect3DDevice9* __device = NULL;
void HookThisShit(IDirect3DDevice9* _device) {
	__device = _device;
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
