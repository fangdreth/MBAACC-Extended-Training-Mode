#pragma once

#include <set>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include "textureModificationData.h"
#include <filesystem>


namespace fs = std::filesystem;

unsigned textureFrameCount = 0;

extern IDirect3DDevice9* device;

const char* pixelShaderCode3 = R"(

sampler2D textureSampler : register(s0); // is using this low of a reg ok?
sampler2D textureSampler2 : register(s1); 


float4 dynamicColor : register(c223); // using register 223 bc i dont want to mess something up
DWORD blendMode : register(c222);

float4 Hue : register(c221);

float4 ActiveColor : register(c220);

float4 texSize : register(c219);

float4 main(float2 texCoord : TEXCOORD0) : COLOR
{
	
	float4 orig = tex2D(textureSampler, texCoord);
	
	float4 newCol = tex2D(textureSampler2, texCoord);

	newCol.a = orig.a;
	
	return newCol;
}

)";


// shader vars
D3DXVECTOR4 dynamicColor(0.0f, 0.0f, 0.0f, 1.0f);
D3DXVECTOR4 blendMode(0.0f, 0.0f, 0.0f, 1.0f);

D3DXVECTOR4 Hue(0.0f, 0.0f, 0.0f, 1.0f);
D3DXVECTOR4 ActiveColor(0.0f, 0.0f, 0.0f, 1.0f);


bool pixelShaderNeedsReset = false;
IDirect3DPixelShader9* pPixelShader_backup = nullptr;

bool renderStateNeedsReset = false;
DWORD renderState_backup;

IDirect3DPixelShader9* pPixelShader = nullptr;
ID3DXBuffer* pShaderBuffer = nullptr;
ID3DXBuffer* pErrorBuffer = nullptr;

void initShader(const char* pixelShaderCode) {

	if (pPixelShader != NULL) {
		pPixelShader->Release();
		pPixelShader = NULL;
	}

	//const char* pixelShaderCode = pixelShaderCode2;
	//const char* pixelShaderCode = pixelShaderCode3;

	HRESULT hr = D3DXCompileShader(
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

	if (FAILED(hr))
	{
		if (pErrorBuffer)
		{
			// Print the error message if compilation fails
			log((char*)pErrorBuffer->GetBufferPointer());
			//while (true) {}
			pErrorBuffer->Release();
		}
		// Handle the error appropriately
		pPixelShader = NULL;
		return;
	}

	// Create the pixel shader
	hr = device->CreatePixelShader((DWORD*)pShaderBuffer->GetBufferPointer(), &pPixelShader);

	if (FAILED(hr))
	{
		log("createpix died");
		//while (true) {} // do i need to release here???
		pPixelShader = NULL;
		return;
	}

	// Release the shader buffer as it is no longer needed
	pShaderBuffer->Release();

	log("shader compile worked!");
}

void initShader() {
	initShader(pixelShaderCode3);
}

void initShaderFromFile() {

	//std::string loadPath = std::string(__FILE__);
	//size_t pos = loadPath.find_last_of('\\');
	//loadPath = loadPath.substr(0, pos);
	//loadPath += "\\testShader.hlsl";

	std::string loadPath = getExtraDataPath() + "testShader.hlsl";

	log("path: %s", loadPath.c_str());


	std::ifstream file(loadPath, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	char* buffer = (char*)malloc(size + 1);

	file.read(buffer, size);

	buffer[size] = '\0';

	initShader(buffer);

	free(buffer);

}

D3DXVECTOR3 gradientArray[360] = { D3DXVECTOR3(0.26, 0.89, 0.95), D3DXVECTOR3(0.26, 0.88, 0.95), D3DXVECTOR3(0.27, 0.86, 0.95), D3DXVECTOR3(0.27, 0.84, 0.95), D3DXVECTOR3(0.27, 0.82, 0.95), D3DXVECTOR3(0.27, 0.80, 0.95), D3DXVECTOR3(0.27, 0.79, 0.95), D3DXVECTOR3(0.27, 0.77, 0.95), D3DXVECTOR3(0.28, 0.75, 0.95), D3DXVECTOR3(0.28, 0.73, 0.95), D3DXVECTOR3(0.28, 0.71, 0.95), D3DXVECTOR3(0.28, 0.70, 0.95), D3DXVECTOR3(0.29, 0.68, 0.95), D3DXVECTOR3(0.29, 0.66, 0.95), D3DXVECTOR3(0.29, 0.65, 0.95), D3DXVECTOR3(0.29, 0.63, 0.95), D3DXVECTOR3(0.29, 0.62, 0.95), D3DXVECTOR3(0.30, 0.60, 0.95), D3DXVECTOR3(0.30, 0.58, 0.95), D3DXVECTOR3(0.30, 0.57, 0.95), D3DXVECTOR3(0.30, 0.55, 0.95), D3DXVECTOR3(0.30, 0.54, 0.95), D3DXVECTOR3(0.30, 0.52, 0.95), D3DXVECTOR3(0.31, 0.50, 0.95), D3DXVECTOR3(0.31, 0.48, 0.95), D3DXVECTOR3(0.31, 0.47, 0.95), D3DXVECTOR3(0.32, 0.45, 0.95), D3DXVECTOR3(0.32, 0.44, 0.95), D3DXVECTOR3(0.32, 0.43, 0.95), D3DXVECTOR3(0.32, 0.41, 0.95), D3DXVECTOR3(0.32, 0.39, 0.96), D3DXVECTOR3(0.32, 0.38, 0.96), D3DXVECTOR3(0.33, 0.36, 0.96), D3DXVECTOR3(0.33, 0.35, 0.96), D3DXVECTOR3(0.33, 0.34, 0.96), D3DXVECTOR3(0.35, 0.33, 0.96), D3DXVECTOR3(0.37, 0.34, 0.96), D3DXVECTOR3(0.38, 0.34, 0.96), D3DXVECTOR3(0.40, 0.34, 0.96), D3DXVECTOR3(0.42, 0.34, 0.96), D3DXVECTOR3(0.44, 0.34, 0.96), D3DXVECTOR3(0.46, 0.35, 0.96), D3DXVECTOR3(0.48, 0.35, 0.96), D3DXVECTOR3(0.49, 0.35, 0.96), D3DXVECTOR3(0.51, 0.35, 0.96), D3DXVECTOR3(0.53, 0.36, 0.96), D3DXVECTOR3(0.55, 0.36, 0.96), D3DXVECTOR3(0.56, 0.36, 0.96), D3DXVECTOR3(0.58, 0.36, 0.96), D3DXVECTOR3(0.60, 0.36, 0.96), D3DXVECTOR3(0.62, 0.36, 0.96), D3DXVECTOR3(0.63, 0.37, 0.96), D3DXVECTOR3(0.65, 0.37, 0.96), D3DXVECTOR3(0.67, 0.37, 0.96), D3DXVECTOR3(0.68, 0.38, 0.96), D3DXVECTOR3(0.70, 0.38, 0.96), D3DXVECTOR3(0.72, 0.38, 0.96), D3DXVECTOR3(0.73, 0.38, 0.96), D3DXVECTOR3(0.75, 0.38, 0.96), D3DXVECTOR3(0.77, 0.38, 0.96), D3DXVECTOR3(0.78, 0.39, 0.96), D3DXVECTOR3(0.80, 0.39, 0.96), D3DXVECTOR3(0.82, 0.39, 0.96), D3DXVECTOR3(0.83, 0.39, 0.96), D3DXVECTOR3(0.85, 0.39, 0.96), D3DXVECTOR3(0.86, 0.40, 0.96), D3DXVECTOR3(0.88, 0.40, 0.96), D3DXVECTOR3(0.89, 0.40, 0.96), D3DXVECTOR3(0.91, 0.40, 0.96), D3DXVECTOR3(0.92, 0.41, 0.96), D3DXVECTOR3(0.94, 0.41, 0.96), D3DXVECTOR3(0.95, 0.41, 0.96), D3DXVECTOR3(0.96, 0.41, 0.95), D3DXVECTOR3(0.96, 0.41, 0.94), D3DXVECTOR3(0.96, 0.41, 0.92), D3DXVECTOR3(0.96, 0.42, 0.91), D3DXVECTOR3(0.96, 0.42, 0.89), D3DXVECTOR3(0.96, 0.42, 0.88), D3DXVECTOR3(0.96, 0.43, 0.87), D3DXVECTOR3(0.96, 0.43, 0.85), D3DXVECTOR3(0.96, 0.43, 0.84), D3DXVECTOR3(0.96, 0.43, 0.82), D3DXVECTOR3(0.96, 0.43, 0.81), D3DXVECTOR3(0.96, 0.43, 0.80), D3DXVECTOR3(0.96, 0.44, 0.79), D3DXVECTOR3(0.96, 0.44, 0.77), D3DXVECTOR3(0.96, 0.44, 0.76), D3DXVECTOR3(0.96, 0.44, 0.75), D3DXVECTOR3(0.96, 0.45, 0.73), D3DXVECTOR3(0.96, 0.45, 0.72), D3DXVECTOR3(0.96, 0.45, 0.72), D3DXVECTOR3(0.96, 0.46, 0.73), D3DXVECTOR3(0.96, 0.46, 0.74), D3DXVECTOR3(0.96, 0.47, 0.75), D3DXVECTOR3(0.95, 0.48, 0.76), D3DXVECTOR3(0.95, 0.48, 0.77), D3DXVECTOR3(0.95, 0.49, 0.78), D3DXVECTOR3(0.95, 0.49, 0.79), D3DXVECTOR3(0.95, 0.50, 0.80), D3DXVECTOR3(0.95, 0.50, 0.81), D3DXVECTOR3(0.95, 0.51, 0.82), D3DXVECTOR3(0.95, 0.52, 0.83), D3DXVECTOR3(0.94, 0.52, 0.84), D3DXVECTOR3(0.94, 0.53, 0.84), D3DXVECTOR3(0.94, 0.53, 0.85), D3DXVECTOR3(0.94, 0.54, 0.86), D3DXVECTOR3(0.94, 0.54, 0.87), D3DXVECTOR3(0.94, 0.55, 0.88), D3DXVECTOR3(0.93, 0.55, 0.88), D3DXVECTOR3(0.93, 0.56, 0.89), D3DXVECTOR3(0.93, 0.56, 0.89), D3DXVECTOR3(0.93, 0.57, 0.90), D3DXVECTOR3(0.93, 0.57, 0.91), D3DXVECTOR3(0.93, 0.58, 0.91), D3DXVECTOR3(0.93, 0.59, 0.92), D3DXVECTOR3(0.93, 0.59, 0.93), D3DXVECTOR3(0.92, 0.60, 0.93), D3DXVECTOR3(0.92, 0.60, 0.93), D3DXVECTOR3(0.91, 0.61, 0.93), D3DXVECTOR3(0.90, 0.61, 0.92), D3DXVECTOR3(0.90, 0.62, 0.92), D3DXVECTOR3(0.89, 0.62, 0.92), D3DXVECTOR3(0.89, 0.62, 0.92), D3DXVECTOR3(0.88, 0.63, 0.92), D3DXVECTOR3(0.88, 0.64, 0.92), D3DXVECTOR3(0.87, 0.64, 0.92), D3DXVECTOR3(0.86, 0.65, 0.92), D3DXVECTOR3(0.86, 0.65, 0.92), D3DXVECTOR3(0.86, 0.66, 0.92), D3DXVECTOR3(0.85, 0.66, 0.92), D3DXVECTOR3(0.85, 0.67, 0.91), D3DXVECTOR3(0.84, 0.67, 0.91), D3DXVECTOR3(0.84, 0.68, 0.91), D3DXVECTOR3(0.84, 0.68, 0.91), D3DXVECTOR3(0.84, 0.68, 0.91), D3DXVECTOR3(0.83, 0.69, 0.91), D3DXVECTOR3(0.83, 0.70, 0.91), D3DXVECTOR3(0.83, 0.70, 0.91), D3DXVECTOR3(0.82, 0.70, 0.91), D3DXVECTOR3(0.82, 0.71, 0.91), D3DXVECTOR3(0.82, 0.71, 0.91), D3DXVECTOR3(0.82, 0.72, 0.91), D3DXVECTOR3(0.82, 0.72, 0.91), D3DXVECTOR3(0.82, 0.73, 0.91), D3DXVECTOR3(0.82, 0.73, 0.91), D3DXVECTOR3(0.81, 0.73, 0.91), D3DXVECTOR3(0.81, 0.74, 0.91), D3DXVECTOR3(0.81, 0.74, 0.91), D3DXVECTOR3(0.81, 0.75, 0.91), D3DXVECTOR3(0.81, 0.75, 0.91), D3DXVECTOR3(0.81, 0.76, 0.91), D3DXVECTOR3(0.81, 0.76, 0.91), D3DXVECTOR3(0.81, 0.77, 0.91), D3DXVECTOR3(0.81, 0.77, 0.91), D3DXVECTOR3(0.81, 0.77, 0.91), D3DXVECTOR3(0.81, 0.78, 0.91), D3DXVECTOR3(0.81, 0.78, 0.91), D3DXVECTOR3(0.81, 0.79, 0.91), D3DXVECTOR3(0.82, 0.79, 0.91), D3DXVECTOR3(0.82, 0.79, 0.91), D3DXVECTOR3(0.82, 0.80, 0.91), D3DXVECTOR3(0.82, 0.80, 0.91), D3DXVECTOR3(0.82, 0.80, 0.91), D3DXVECTOR3(0.82, 0.81, 0.91), D3DXVECTOR3(0.82, 0.81, 0.91), D3DXVECTOR3(0.82, 0.82, 0.91), D3DXVECTOR3(0.82, 0.82, 0.91), D3DXVECTOR3(0.83, 0.82, 0.91), D3DXVECTOR3(0.83, 0.83, 0.91), D3DXVECTOR3(0.83, 0.83, 0.91), D3DXVECTOR3(0.84, 0.84, 0.91), D3DXVECTOR3(0.84, 0.84, 0.92), D3DXVECTOR3(0.84, 0.85, 0.92), D3DXVECTOR3(0.85, 0.85, 0.92), D3DXVECTOR3(0.85, 0.86, 0.92), D3DXVECTOR3(0.85, 0.86, 0.92), D3DXVECTOR3(0.86, 0.86, 0.92), D3DXVECTOR3(0.86, 0.87, 0.92), D3DXVECTOR3(0.86, 0.87, 0.92), D3DXVECTOR3(0.87, 0.88, 0.92), D3DXVECTOR3(0.87, 0.88, 0.92), D3DXVECTOR3(0.86, 0.87, 0.92), D3DXVECTOR3(0.86, 0.87, 0.92), D3DXVECTOR3(0.86, 0.86, 0.92), D3DXVECTOR3(0.85, 0.86, 0.92), D3DXVECTOR3(0.85, 0.86, 0.92), D3DXVECTOR3(0.85, 0.85, 0.92), D3DXVECTOR3(0.84, 0.85, 0.92), D3DXVECTOR3(0.84, 0.84, 0.92), D3DXVECTOR3(0.84, 0.84, 0.91), D3DXVECTOR3(0.83, 0.83, 0.91), D3DXVECTOR3(0.83, 0.83, 0.91), D3DXVECTOR3(0.83, 0.82, 0.91), D3DXVECTOR3(0.82, 0.82, 0.91), D3DXVECTOR3(0.82, 0.82, 0.91), D3DXVECTOR3(0.82, 0.81, 0.91), D3DXVECTOR3(0.82, 0.81, 0.91), D3DXVECTOR3(0.82, 0.80, 0.91), D3DXVECTOR3(0.82, 0.80, 0.91), D3DXVECTOR3(0.82, 0.80, 0.91), D3DXVECTOR3(0.82, 0.79, 0.91), D3DXVECTOR3(0.82, 0.79, 0.91), D3DXVECTOR3(0.81, 0.79, 0.91), D3DXVECTOR3(0.81, 0.78, 0.91), D3DXVECTOR3(0.81, 0.78, 0.91), D3DXVECTOR3(0.81, 0.77, 0.91), D3DXVECTOR3(0.81, 0.77, 0.91), D3DXVECTOR3(0.81, 0.77, 0.91), D3DXVECTOR3(0.81, 0.76, 0.91), D3DXVECTOR3(0.81, 0.76, 0.91), D3DXVECTOR3(0.81, 0.75, 0.91), D3DXVECTOR3(0.81, 0.75, 0.91), D3DXVECTOR3(0.81, 0.74, 0.91), D3DXVECTOR3(0.81, 0.74, 0.91), D3DXVECTOR3(0.81, 0.73, 0.91), D3DXVECTOR3(0.82, 0.73, 0.91), D3DXVECTOR3(0.82, 0.73, 0.91), D3DXVECTOR3(0.82, 0.72, 0.91), D3DXVECTOR3(0.82, 0.72, 0.91), D3DXVECTOR3(0.82, 0.71, 0.91), D3DXVECTOR3(0.82, 0.71, 0.91), D3DXVECTOR3(0.82, 0.70, 0.91), D3DXVECTOR3(0.83, 0.70, 0.91), D3DXVECTOR3(0.83, 0.70, 0.91), D3DXVECTOR3(0.83, 0.69, 0.91), D3DXVECTOR3(0.84, 0.68, 0.91), D3DXVECTOR3(0.84, 0.68, 0.91), D3DXVECTOR3(0.84, 0.68, 0.91), D3DXVECTOR3(0.84, 0.67, 0.91), D3DXVECTOR3(0.85, 0.67, 0.91), D3DXVECTOR3(0.85, 0.66, 0.92), D3DXVECTOR3(0.86, 0.66, 0.92), D3DXVECTOR3(0.86, 0.65, 0.92), D3DXVECTOR3(0.86, 0.65, 0.92), D3DXVECTOR3(0.87, 0.64, 0.92), D3DXVECTOR3(0.88, 0.64, 0.92), D3DXVECTOR3(0.88, 0.63, 0.92), D3DXVECTOR3(0.89, 0.62, 0.92), D3DXVECTOR3(0.89, 0.62, 0.92), D3DXVECTOR3(0.90, 0.62, 0.92), D3DXVECTOR3(0.90, 0.61, 0.92), D3DXVECTOR3(0.91, 0.61, 0.93), D3DXVECTOR3(0.92, 0.60, 0.93), D3DXVECTOR3(0.92, 0.60, 0.93), D3DXVECTOR3(0.93, 0.59, 0.93), D3DXVECTOR3(0.93, 0.59, 0.92), D3DXVECTOR3(0.93, 0.58, 0.91), D3DXVECTOR3(0.93, 0.57, 0.91), D3DXVECTOR3(0.93, 0.57, 0.90), D3DXVECTOR3(0.93, 0.56, 0.89), D3DXVECTOR3(0.93, 0.56, 0.89), D3DXVECTOR3(0.93, 0.55, 0.88), D3DXVECTOR3(0.94, 0.55, 0.88), D3DXVECTOR3(0.94, 0.54, 0.87), D3DXVECTOR3(0.94, 0.54, 0.86), D3DXVECTOR3(0.94, 0.53, 0.85), D3DXVECTOR3(0.94, 0.53, 0.84), D3DXVECTOR3(0.94, 0.52, 0.84), D3DXVECTOR3(0.95, 0.52, 0.83), D3DXVECTOR3(0.95, 0.51, 0.82), D3DXVECTOR3(0.95, 0.50, 0.81), D3DXVECTOR3(0.95, 0.50, 0.80), D3DXVECTOR3(0.95, 0.49, 0.79), D3DXVECTOR3(0.95, 0.49, 0.78), D3DXVECTOR3(0.95, 0.48, 0.77), D3DXVECTOR3(0.95, 0.48, 0.76), D3DXVECTOR3(0.96, 0.47, 0.75), D3DXVECTOR3(0.96, 0.46, 0.74), D3DXVECTOR3(0.96, 0.46, 0.73), D3DXVECTOR3(0.96, 0.45, 0.72), D3DXVECTOR3(0.96, 0.45, 0.72), D3DXVECTOR3(0.96, 0.45, 0.73), D3DXVECTOR3(0.96, 0.44, 0.75), D3DXVECTOR3(0.96, 0.44, 0.76), D3DXVECTOR3(0.96, 0.44, 0.77), D3DXVECTOR3(0.96, 0.44, 0.79), D3DXVECTOR3(0.96, 0.43, 0.80), D3DXVECTOR3(0.96, 0.43, 0.81), D3DXVECTOR3(0.96, 0.43, 0.82), D3DXVECTOR3(0.96, 0.43, 0.84), D3DXVECTOR3(0.96, 0.43, 0.85), D3DXVECTOR3(0.96, 0.43, 0.87), D3DXVECTOR3(0.96, 0.42, 0.88), D3DXVECTOR3(0.96, 0.42, 0.89), D3DXVECTOR3(0.96, 0.42, 0.91), D3DXVECTOR3(0.96, 0.41, 0.92), D3DXVECTOR3(0.96, 0.41, 0.94), D3DXVECTOR3(0.96, 0.41, 0.95), D3DXVECTOR3(0.95, 0.41, 0.96), D3DXVECTOR3(0.94, 0.41, 0.96), D3DXVECTOR3(0.92, 0.41, 0.96), D3DXVECTOR3(0.91, 0.40, 0.96), D3DXVECTOR3(0.89, 0.40, 0.96), D3DXVECTOR3(0.88, 0.40, 0.96), D3DXVECTOR3(0.86, 0.40, 0.96), D3DXVECTOR3(0.85, 0.39, 0.96), D3DXVECTOR3(0.83, 0.39, 0.96), D3DXVECTOR3(0.82, 0.39, 0.96), D3DXVECTOR3(0.80, 0.39, 0.96), D3DXVECTOR3(0.78, 0.39, 0.96), D3DXVECTOR3(0.77, 0.38, 0.96), D3DXVECTOR3(0.75, 0.38, 0.96), D3DXVECTOR3(0.73, 0.38, 0.96), D3DXVECTOR3(0.72, 0.38, 0.96), D3DXVECTOR3(0.70, 0.38, 0.96), D3DXVECTOR3(0.68, 0.38, 0.96), D3DXVECTOR3(0.67, 0.37, 0.96), D3DXVECTOR3(0.65, 0.37, 0.96), D3DXVECTOR3(0.63, 0.37, 0.96), D3DXVECTOR3(0.62, 0.36, 0.96), D3DXVECTOR3(0.60, 0.36, 0.96), D3DXVECTOR3(0.58, 0.36, 0.96), D3DXVECTOR3(0.56, 0.36, 0.96), D3DXVECTOR3(0.55, 0.36, 0.96), D3DXVECTOR3(0.53, 0.36, 0.96), D3DXVECTOR3(0.51, 0.35, 0.96), D3DXVECTOR3(0.49, 0.35, 0.96), D3DXVECTOR3(0.48, 0.35, 0.96), D3DXVECTOR3(0.46, 0.35, 0.96), D3DXVECTOR3(0.44, 0.34, 0.96), D3DXVECTOR3(0.42, 0.34, 0.96), D3DXVECTOR3(0.40, 0.34, 0.96), D3DXVECTOR3(0.38, 0.34, 0.96), D3DXVECTOR3(0.37, 0.34, 0.96), D3DXVECTOR3(0.35, 0.33, 0.96), D3DXVECTOR3(0.33, 0.34, 0.96), D3DXVECTOR3(0.33, 0.35, 0.96), D3DXVECTOR3(0.33, 0.36, 0.96), D3DXVECTOR3(0.32, 0.38, 0.96), D3DXVECTOR3(0.32, 0.39, 0.96), D3DXVECTOR3(0.32, 0.41, 0.95), D3DXVECTOR3(0.32, 0.43, 0.95), D3DXVECTOR3(0.32, 0.44, 0.95), D3DXVECTOR3(0.32, 0.45, 0.95), D3DXVECTOR3(0.31, 0.47, 0.95), D3DXVECTOR3(0.31, 0.48, 0.95), D3DXVECTOR3(0.31, 0.50, 0.95), D3DXVECTOR3(0.30, 0.52, 0.95), D3DXVECTOR3(0.30, 0.54, 0.95), D3DXVECTOR3(0.30, 0.55, 0.95), D3DXVECTOR3(0.30, 0.57, 0.95), D3DXVECTOR3(0.30, 0.58, 0.95), D3DXVECTOR3(0.30, 0.60, 0.95), D3DXVECTOR3(0.29, 0.62, 0.95), D3DXVECTOR3(0.29, 0.63, 0.95), D3DXVECTOR3(0.29, 0.65, 0.95), D3DXVECTOR3(0.29, 0.66, 0.95), D3DXVECTOR3(0.29, 0.68, 0.95), D3DXVECTOR3(0.28, 0.70, 0.95), D3DXVECTOR3(0.28, 0.71, 0.95), D3DXVECTOR3(0.28, 0.73, 0.95), D3DXVECTOR3(0.28, 0.75, 0.95), D3DXVECTOR3(0.27, 0.77, 0.95), D3DXVECTOR3(0.27, 0.79, 0.95), D3DXVECTOR3(0.27, 0.80, 0.95), D3DXVECTOR3(0.27, 0.82, 0.95), D3DXVECTOR3(0.27, 0.84, 0.95), D3DXVECTOR3(0.27, 0.86, 0.95), D3DXVECTOR3(0.26, 0.88, 0.95), D3DXVECTOR3(0.26, 0.89, 0.95) };
void frameCountCallback() {
	// textureFrameCount

	//float fr = ((float)(((int)(((float)textureFrameCount) * 2.0)) % 360)) / 360.0;
	//D3DXVECTOR3 temp = hsl2rgb(fr, 0.89, 0.61);

	DWORD offset = ((int)(((float)textureFrameCount) * 4.0)) % 360;

	D3DXVECTOR3 temp = gradientArray[offset];

	dynamicColor.x = temp.x;
	dynamicColor.y = temp.y;
	dynamicColor.z = temp.z;

	device->SetPixelShaderConstantF(223, (float*)&dynamicColor, 1);

	Hue.x = ((float)offset) / 360.0f;

	device->SetPixelShaderConstantF(221, (float*)&Hue, 1);

}

class TextureModification {
public:
	TextureModification() {
		blend = 0;
	}

	TextureModification(DWORD blend_) {
		blend = blend_;
	}
	
	DWORD blend;

private:
};

// this is a set/map intentionally, not unordered, as i believe that the size of this set will be reasonably small < 256 DWORDS
std::map<DWORD, TextureModification> textureAddrs; 
//std::set<DWORD> textureAddrs; 
// key is the unknown address im using to match between the setup and render funcs (most likely just a linked list ptr?), it may have really helpful data tho?

/*
options are:
004331D9
004332e1
0043306F
*/

typedef struct BadAppleFrame {
	char* data = NULL;
	size_t size = 0;
} BadAppleFrame;

int badAppleFrame = 0;
std::vector<BadAppleFrame> badAppleFrames;

IDirect3DTexture9* pBadAppleTex = NULL;

// ffmpeg funcs

void loadMP4(const std::string& filePath) {

	int res;

	const AVCodec* outCodec = avcodec_find_encoder(AV_CODEC_ID_MJPEG);
	const AVCodec* inCodec = NULL; // input codec of the file 

	AVFormatContext* inFmtCtx = NULL;
	AVFormatContext* outFmtCtx = NULL;

	AVCodecContext* inCtx = NULL;
	AVCodecContext* outCtx = avcodec_alloc_context3(outCodec);
	outCtx->strict_std_compliance = FF_COMPLIANCE_UNOFFICIAL; 

	if (!outCtx) {
		log("initing the output codex failed??? how??");
		exit(1);
	}

	// open the actual file
	// https://ffmpeg.org/doxygen/2.8/group__lavf__decoding.html#details

	
	res = avformat_open_input(&inFmtCtx, filePath.c_str(), NULL, NULL);
	if (res < 0) {
		log("avformat_open_input failed");
		exit(1);
	}

	res = avformat_find_stream_info(inFmtCtx, NULL);
	if (res < 0) {
		log("avformat_find_stream_info failed");
		exit(1);
	}

	res = av_find_best_stream(inFmtCtx, AVMEDIA_TYPE_VIDEO, -1, -1, &inCodec, 0);
	if (res < 0) {
		log("av_find_best_stream failed");
		exit(1);
	}

	int videoStreamIndex = res;

	// create a context for the input video codec 
	inCtx = avcodec_alloc_context3(inCodec);
	if (!inCtx) {
		log("avcodec_alloc_context3 failed");
		exit(1);
	}

	// give the codecs params (width, height, etc i think) to the context
	res = avcodec_parameters_to_context(inCtx, inFmtCtx->streams[videoStreamIndex]->codecpar);
	if (res < 0) {
		log("avcodec_parameters_to_context failed");
		exit(1);
	}

	// init the decoder for the input
	res = avcodec_open2(inCtx, inCodec, NULL);
	if (res < 0) {
		log("avcodec_open2 failed");
		exit(1);
	}

	// init the decoder for the output
	outCtx->pix_fmt = inCtx->pix_fmt;
	outCtx->height  = inCtx->height;
	outCtx->width   = inCtx->width;
	outCtx->time_base = AVRational{ 1,10 };

	res = avcodec_open2(outCtx, outCodec, NULL);
	if (res < 0) {
		log("opening the output decoder failed????");
		exit(1);
	}

	AVPacket* outPkt = av_packet_alloc();
	if (!outPkt) {
		log("couldnt even alloc a packet?");
		return;
	}
	
	AVPacket* pkt = av_packet_alloc();
	if (!pkt) {
		log("couldnt even alloc a packet?");
		return;
	}

	AVFrame* frame = av_frame_alloc();

	while (true) {
		
		if ((res = av_read_frame(inFmtCtx, pkt)) < 0) {
			break;
		}

		if (pkt->stream_index == videoStreamIndex) {
			log("uhhh frame %d", badAppleFrames.size());

			res = avcodec_send_packet(inCtx, pkt);
			if (res < 0) {
				log("failed to send packet to inCtx decoder?");
				exit(1);
			}

			// decode that one packet
			while (true) { 
				// get the frame from that packet
				res = avcodec_receive_frame(inCtx, frame);
				if (res == AVERROR(EAGAIN) || res == AVERROR_EOF) { // end of packet
					break;
				} else if (res < 0) {
					log("avcodec_receive_frame failed");
					exit(1);
				}

				// at this point, we now (should) have a frame of the video. do something with it

				// send the video frame to the jpeg encoder
				res = avcodec_send_frame(outCtx, frame);
				if (res < 0) {
					log("sending frame to output decoder failed");
					exit(1);
				}

				// get the resulting packet from said frame
				res = avcodec_receive_packet(outCtx, outPkt);
				if (res < 0) {
					log("jpeg encoder failed to give us a packet?");
					exit(1);
				}

				BadAppleFrame tempFrame = { (char*)malloc(outPkt->size), outPkt->size };

				if (tempFrame.data == NULL) {
					log("you malloced like an idiot");
					exit(1);
				}

				memcpy(tempFrame.data, outPkt->data, tempFrame.size);

				badAppleFrames.push_back(tempFrame);

				av_packet_unref(outPkt);

				av_frame_unref(frame);
			}
		}

		av_packet_unref(pkt);
	}

}

void loadBadApple() {
	
	/*
	
	todo, at this point, just require ffmpeg to be installed on system.
	just require a video file in melty path.
	
	todo, have, ffmpeg be,, idk where i should put it, can i have it as a submodule?
	and then use that. make sure to link statically.

	*/

	//return;
	
	if (device == NULL) {
		return;
	}

	static bool loadBadApple = false;

	if (loadBadApple) {
		return;
	}

	loadBadApple = true;
	
	log("desperately attempting to load a mp4");
	
	std::string filePath = "./EffectsVideo.mp4";
	//std::string filePath = "./EffectsVideoNoAudio.mp4";

	// is doing this via ffmpeg worth it.
	// tbh i could just,,, idek
	// i would have to stream the mp4, i would need another thread, i would,,
	// this is the correct way. i should do it the correct way

	loadMP4(filePath);
	
	log("there are,,,, %d frame somehow?", badAppleFrames.size());
	
	

	log("textures loaded successfully");
}

void initSong(bool force = false) {

	static bool isPlaying = false;

	if (force) {
		isPlaying = false;
	}

	if (isPlaying) {
		return;
	}

	isPlaying = true;

	//std::string folderPath = std::string(__FILE__);
	//size_t pos = folderPath.find_last_of('\\');
	//folderPath = folderPath.substr(0, pos);
	//folderPath += "\\BadApple.wav";

	std::string folderPath = getExtraDataPath() + "BadApple.wav";

	std::string command = "open \"" + folderPath + "\" type waveaudio alias BadApple";

	mciSendStringA(command.c_str(), NULL, 0, NULL);

	
	// todo, the volume setting in memory is read when loading into a battle
	// whats happening(most likely) is dsound buffers are being created, and their vol is being set to that val
	// find whats creating those buffers, find the dsound thing, convert this to use that
	// alternatively, conv that vol value to this?

	mciSendStringA("play BadApple", NULL, 0, NULL);
	//mciSendStringA("setaudio BadApple volume to 10", NULL, 0, NULL);	

	// this code somehow maintains state over multiple runs, and can and will fuck your melty up permanently
	// i need to find where melty calls volume stuff
	//waveOutSetVolume(NULL, 0xFFFFFFFF);
	//waveOutSetVolume(NULL, 0x0000);
}

void pauseSong() {
	mciSendStringA("pause BadApple", NULL, 0, NULL);
}

void playSong() {
	initSong();
	mciSendStringA("resume BadApple", NULL, 0, NULL);
}

void restartSong() {
	mciSendStringA("stop BadApple", NULL, 0, NULL);
	mciSendStringA("seek BadApple to start", NULL, 0, NULL);
	mciSendStringA("play BadApple", NULL, 0, NULL);
}

// actual funcs

DWORD leadToDrawPrimHook_ret = 0;
IDirect3DPixelShader9* pPixelShader_backup_test = nullptr;
DWORD drawPrimHook_texAddr = 0;
void drawPrimHook() {

	loadBadApple();
	
	device->GetPixelShader(&pPixelShader_backup_test);
	if (pPixelShader_backup_test != NULL) {
		log("something else was using the pixel shader??");
		pPixelShader_backup_test = NULL;
	}

	static unsigned index = 0;

	if (leadToDrawPrimHook_ret != 0x004331D9) {
		index = 0;
		textureAddrs.clear(); // calling this repeatedly is wasteful!
		return;
	}

	//bool cond = (index >= 5 && index <= 8);
	if (textureAddrs.contains(drawPrimHook_texAddr)) {
		
		/*device->GetPixelShader(&pPixelShader_backup);
		if (pPixelShader_backup != NULL) {
			log("non null shader??");
		}*/

		pixelShaderNeedsReset = true;
		device->SetPixelShader(pPixelShader);

		//blendMode.x = (float)textureAddrs[drawPrimHook_texAddr].blend;
		//device->SetPixelShaderConstantF(222, (float*)&blendMode, 1);

		// this better not cause slowdown
		static unsigned lastTextureFrameCount = -1;
		if (lastTextureFrameCount != textureFrameCount) {

			lastTextureFrameCount = textureFrameCount;

			if (textureFrameCount & 1) {

				if (pBadAppleTex != NULL) {
					pBadAppleTex->Release();
					pBadAppleTex = NULL;
				}

				D3DXCreateTextureFromFileInMemoryEx(device,
					badAppleFrames[badAppleFrame].data, badAppleFrames[badAppleFrame].size,
					D3DX_DEFAULT, D3DX_DEFAULT, 
					// 480, 360,
					//640, 360,
					0, // MIP
					D3DUSAGE_DYNAMIC,
					D3DFMT_A8R8G8B8,
					D3DPOOL_DEFAULT,
					//D3DX_DEFAULT, //filter?
					D3DX_FILTER_NONE, // default filter is whatwas slowing things down https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dx-filter
					//D3DX_DEFAULT, // mip filter????
					D3DX_FILTER_NONE, 
					0, // replace color
					NULL, // img info
					NULL, // palette
					&pBadAppleTex
				);
			
				badAppleFrame++;
				if (badAppleFrame >= badAppleFrames.size()) {
					badAppleFrame = 0;
					restartSong();
				}
			}
		}

		shouldPlaySong = true;
		

		device->SetTexture(1, pBadAppleTex);

		//IDirect3DBaseTexture9* pTex = (IDirect3DBaseTexture9*)drawPrimHook_texAddr;
		IDirect3DTexture9* pTex = (IDirect3DTexture9*)drawPrimHook_texAddr;

		// bad apple is 480x360, 30fps
		// i could have used ffmpeg in c++, but then i would have to go deal with library compiler error bs and wish i was on linux
		// just use this ffmpeg -i '..\Touhou - Bad Apple.mp4' "BadApple%04d.jpg"
		// ffmpeg -i "..\Touhou - Bad Apple.mp4" -vf "scale=iw/2:ih/2" "BadApple%04d.jpg"
		// ffmpeg -i "..\Touhou - Bad Apple.mp4" -vf "scale=iw/2:ih/2" "BadApple%04d.jpg"
		// ffmpeg -i "..\Touhou - Bad Apple.mp4" -vf "scale=-1:255" "BadApple%04d.jpg"
		// ffmpeg -i "..\Hi_Fine_FOX_Original.mp4" -vf "scale=-1:360" "BadApple%04d.jpg"'
		// ideally, i would actually link ffmpeg with c++ but thats a massive pain
		// ill have to include the libs here so others dont go through my pain

		D3DSURFACE_DESC desc;
		pTex->GetLevelDesc(0, &desc); // Get the texture size
		
		D3DXVECTOR4 textureSize((float)desc.Width, (float)desc.Height, 0.0, 0.0);
		device->SetPixelShaderConstantF(219, (float*)&textureSize, 1);

		//pTex->Release();
		// 		
		//renderStateNeedsReset = true;

	}

	index++;
}

void drawPrimCallback() {
	if (pixelShaderNeedsReset) {
		pixelShaderNeedsReset = false;
		device->SetPixelShader(pPixelShader_backup);
	}
}
 
DWORD listAppendHook_effectRetAddr = 0;
DWORD listAppendHook_objAddr = 0;
DWORD listAppendHook_texAddr = 0;

DWORD listAppendHook_effectRetAddr_pat = 0;
DWORD listAppendHook_objAddr_pat = 0;

void listAppendHook() {
	//log("listAppendHook: %08X  objAddr: %08X retAddr: %08X", listAppendHook_texAddr, listAppendHook_objAddr, listAppendHook_effectRetAddr);

	if (listAppendHook_effectRetAddr == 0x0045410F || listAppendHook_effectRetAddr_pat == 0x0045410F) {

		if (listAppendHook_effectRetAddr_pat == 0x0045410F) {
			listAppendHook_objAddr = listAppendHook_objAddr_pat;
		}

		if (listAppendHook_objAddr >= 0x0067BDE8) { // effect
			// source is how its listed in the cheat engine file, im not actually sure what it is
			char source = *(char*)(listAppendHook_objAddr - 8);


			//log("source : %d", source);

			//log("listAppendHook_objAddr: %08X", listAppendHook_objAddr);
			//log("source is %d %02X", source, source);

			// todo, switch to a WHITELIST, notblacklist, of valid things to color

			DWORD pattern = *(DWORD*)(listAppendHook_objAddr + 0x0);
			DWORD state   = *(DWORD*)(listAppendHook_objAddr + 0x4);

			if (source >= 0) {
				
				BYTE blendMode = 0;
				
				// override blend mode to 0 so the shader works properly
				DWORD _animDataAddr = listAppendHook_objAddr + 0x310;
				if (*(DWORD*)_animDataAddr == 0) {
					return; // is this ret here smart?
				}
				
				
				DWORD _animDataPtr = *(DWORD*)_animDataAddr;
				// blend mode addr
				blendMode = *(BYTE*)(_animDataPtr + 0x1B);
				

				BYTE owner = *(BYTE*)(listAppendHook_objAddr - 0x10 + 0x02F4);
			
				BYTE charID;

				if (owner == 0) {
					charID = *(BYTE*)0x555135;
				} else {
					charID = *(BYTE*)0x555C31;
				}

				if (*(DWORD*)(listAppendHook_objAddr - 0x10 + 0x20) == 0x00000101) {
					// im just looking for patterns here
					// this prevents heat from making things weird
					// warc AAD still causes rave, but tbh im keeping that in
					// this almost definitely messes something else up. only thing that will help is per char testing, and tbh per char pattern checks.
					//log("heat shortcut");
					return;
				}

				if (!shouldThisBeColored(charID, pattern)) {
					return;
				}

				//log("char: %3d pattern: %5d", charID, pattern);

				textureAddrs.insert({ listAppendHook_texAddr, TextureModification(blendMode)});
				
			}

		} else { // char? possibly or the hit effects

		}	
	}
}

// naked funcs

DWORD _naked_drawPrimHook_reg;
DWORD _naked_drawPrimHook_jmp = 0x004be296;
__declspec(naked) void _naked_drawPrimHook() {
	__asm {
		mov _naked_drawPrimHook_reg, eax;

		// this MAY need null checking!
		// code taken from 004be293
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

__declspec(naked) void _naked_drawPrimCallback2() {
	// see if reseting the pixel shader early has benifits
	PUSH_ALL;
	drawPrimCallback();
	POP_ALL;
	
	// overwritten asm from 0x004be35a
	__asm _emit 0x8B;
	__asm _emit 0x55;
	__asm _emit 0x00;

	__asm _emit 0x8B;
	__asm _emit 0x82;
	__asm _emit 0xA4;
	__asm _emit 0x00;
	__asm _emit 0x00;
	__asm _emit 0x00;

	__asm {
		push 004be363h;
		ret;
	}
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

__declspec(naked) void _naked_listAppendHook() {

	__asm {
		mov listAppendHook_texAddr, eax;

		
		// stack checking NEEDS to be more consistent here!
		// i could copy, a bunch of it, but would it be worth?
		// .pat stuff has a different stack/code path. fix it
		// pceil 214 is also .pat
		// 3c/2c on flen shouldnt color
		// cwlen dash forward, hold back, etc
		// fhime fly???
		// cmech 236a is .pat
		// cmech j214a crossbow changes color when dropped??
		// wara 236c (red orb)
		// every shield fucks things up
		// kouma 22b j22a/b/c should be transparent. am i OVERWRITING A PIXEL SHADER?? THAT WOULD BE BAD
		// cvaki j22a somehow, i have no clue, but look towards the bottom of the screen, theres just a weird thing at the end of it 

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

		//mov stackAddr, esp;
	};

	//PUSH_ALL;
	//saveStack();
	//logStack();
	//POP_ALL;
	
	// down here so it doesnt mess with my above stuff
	
	// i do not trust the assembler :)
	// bytes taken from 004c026b

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

__declspec(naked) void _naked_frameCountCallback() {
	PUSH_ALL;
	frameCountCallback();
	POP_ALL;
	__asm {
		add textureFrameCount, 1;
		ret 4;
	}
}

// directX hooking (this should maybe be in a seperate file)

void cleanForDirectXReset() {
	if (pBadAppleTex != NULL) {
		pBadAppleTex->Release();
		pBadAppleTex = NULL;
		//device->SetTexture(1, NULL); // this may not be neccessary
	}
}

DWORD _RESET_HOOKS = 0;

DWORD _DirectX_Reset_Func_Addr = 0;
__declspec(naked) void _DirectX_Reset_Func() {
	PUSH_ALL;
	log("DIRECTX RESET CALLED, CLEANING");
	cleanForDirectXReset();
	POP_ALL;
	__asm {
		jmp[_DirectX_Reset_Func_Addr];
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

// init 

void initDrawPrimHook() {
	patchJump(0x004be290, _naked_drawPrimHook);
}

void initDrawPrimCallback() {
	patchJump(0x004be524, _naked_drawPrimCallback);
	//patchJump(0x004be35a, _naked_drawPrimCallback2);
}

void initLeadToDrawPrimHook() {
	patchJump(0x004c0380, _naked_leadToDrawPrimHook);
}

void initListAppendHook() {
	patchJump(0x004c026b, _naked_listAppendHook);
}

void initFrameCountCallback() {
	patchJump(0x004238f5, _naked_frameCountCallback);
}

void initDirectXHooks() {


	// i need a hook specifically for when reset is called in here.
	// i also need to 

	_naked_InitDirectXHooks();

	// this specifically checks if our hooks were overwritten, and then rehooks
	patchJump(0x004be3c4, _naked_RehookDirectX);

}

bool initTextureModifications() {

	if (device == NULL) {
		return false;
	}

	static bool init = false;
	if (init) { // im paranoid
		return true;
	}

	initShader();
	
	initDrawPrimHook();
	initDrawPrimCallback();
	initLeadToDrawPrimHook();
	initListAppendHook();
	initFrameCountCallback();

	initDirectXHooks();

	log("initTextureModifications ran");
	
	init = true;
	return true;
}