#pragma once

#include <set>
#include <map>

unsigned textureFrameCount = 0;

extern IDirect3DDevice9* device;

// the shader
const char* pixelShaderCode2 = R"(
	
sampler2D textureSampler : register(s0); // is using this low of a reg ok?
float4 dynamicColor : register(c223); // using register 223 bc i dont want to mess something up
DWORD blendMode : register(c222);

float4 Hue : register(c221);

const float EPSILON = 1e-10;

float3 HUEtoRGB(float hue)
{
    // Hue [0..1] to RGB [0..1]
    // See http://www.chilliant.com/rgb2hsv.html
    float3 rgb = abs(hue * 6. - float3(3, 2, 4)) * float3(1, -1, -1) + float3(-1, 2, 2);
    return clamp(rgb, 0., 1.);
}

float3 RGBtoHCV(float3 rgb)
{
    // RGB [0..1] to Hue-Chroma-Value [0..1]
    // Based on work by Sam Hocevar and Emil Persson
    float4 p = (rgb.g < rgb.b) ? float4(rgb.bg, -1., 2. / 3.) : float4(rgb.gb, 0., -1. / 3.);
    float4 q = (rgb.r < p.x) ? float4(p.xyw, rgb.r) : float4(rgb.r, p.yzx);
    float c = q.x - min(q.w, q.y);
    float h = abs((q.w - q.y) / (6. * c + EPSILON) + q.z);
    return float3(h, c, q.x);
}

float3 RGBtoHSV(float3 rgb)
{
    // RGB [0..1] to Hue-Saturation-Value [0..1]
    float3 hcv = RGBtoHCV(rgb);
    float s = hcv.y / (hcv.z + EPSILON);
    return float3(hcv.x, s, hcv.z);
}

float3 HSVtoRGB(float3 hsv)
{
    // Hue-Saturation-Value [0..1] to RGB [0..1]
    float3 rgb = HUEtoRGB(hsv.x);
    return ((rgb - 1.) * hsv.y + 1.) * hsv.z;
}


float4 main(float2 texCoord : TEXCOORD0) : COLOR {
	float4 texColor = tex2D(textureSampler, texCoord);
	
	
	/*
	if(blendMode.r == 0.0 || blendMode.r == 1.0) {
		texColor.r = dynamicColor.r;
		texColor.g = dynamicColor.g;
		texColor.b = dynamicColor.b;
	} else {

	}
	*/

	

	float3 hcyVal = RGBtoHSV(texColor.rgb);

	hcyVal.r = Hue.r;

	float3 rgbVal = HSVtoRGB(hcyVal.rgb);

	texColor.rgb = rgbVal;

	return texColor;
}

)";

// shader vars
D3DXVECTOR4 dynamicColor(0.0f, 0.0f, 0.0f, 1.0f);
D3DXVECTOR4 blendMode(0.0f, 0.0f, 0.0f, 1.0f);

D3DXVECTOR4 Hue(0.0f, 0.0f, 0.0f, 1.0f);


bool pixelShaderNeedsReset = false;
IDirect3DPixelShader9* pPixelShader_backup = nullptr;

bool renderStateNeedsReset = false;
DWORD renderState_backup;

IDirect3DPixelShader9* pPixelShader = nullptr;
ID3DXBuffer* pShaderBuffer = nullptr;
ID3DXBuffer* pErrorBuffer = nullptr;

void initShader() {

	const char* pixelShaderCode = pixelShaderCode2;

	HRESULT hr = D3DXCompileShader(
		pixelShaderCode,               // Shader source code
		strlen(pixelShaderCode),       // Length of the shader code
		NULL,                          // Optional defines
		NULL,                          // Optional include interface
		"main",                 // Entry point function name
		"ps_3_0",                      // Target profile (pixel shader 2.0)
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

float hue2rgb(float p, float q, float t) {
	if (t < 0)
		t += 1;
	if (t > 1)
		t -= 1;
	if (t < 1. / 6)
		return p + (q - p) * 6 * t;
	if (t < 1. / 2)
		return q;
	if (t < 2. / 3)
		return p + (q - p) * (2. / 3 - t) * 6;

	return p;
}

D3DXVECTOR3 hsl2rgb(float h, float s, float l) {

	D3DXVECTOR3 res;

	if (0 == s) {
		res.x = res.y = res.z = l;
	}
	else {
		float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
		float p = 2 * l - q;
		res.x = hue2rgb(p, q, h + 1. / 3);
		res.y = hue2rgb(p, q, h);
		res.z = hue2rgb(p, q, h - 1. / 3);
	}

	return res;
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
DWORD leadToDrawPrimHook_ret = 0;

// actual funcs

DWORD drawPrimHook_texAddr = 0;
void drawPrimHook() {

	/*
	
	TODO:
	the random thing currently in works. 
	however:

	top UI elems were constantly inverted, why

	actual todo, investigate the stack, and figure out how to capture what is actually being drawn, and use that 
	
	*/

	static unsigned index = 0;

	if (leadToDrawPrimHook_ret != 0x004331D9) {
		index = 0;
		textureAddrs.clear(); // calling this repeatedly is wasteful!
		return;
	}

	//bool cond = (index >= 5 && index <= 8);
	if (textureAddrs.contains(drawPrimHook_texAddr)) {
		device->GetPixelShader(&pPixelShader_backup);
		if (pPixelShader_backup != NULL) {
			log("non null shader??");
		}
		pixelShaderNeedsReset = true;
		device->SetPixelShader(pPixelShader);

		blendMode.x = (float)textureAddrs[drawPrimHook_texAddr].blend;
		device->SetPixelShaderConstantF(222, (float*)&blendMode, 1);

		

		// make all things use normal color blending
		//device->SetRenderState(D3DRS_BLENDOP, 0);
		device->GetRenderState(D3DRS_ALPHABLENDENABLE, &renderState_backup);
		renderStateNeedsReset = true;

		//device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}

	//log("%3d primHook tex addr %08X ret: %08X", index, drawPrimHook_texAddr, leadToDrawPrimHook_ret);

	if (drawPrimHook_texAddr != 0) {
		//log("attempting to see if tex is valid");
		//IDirect3DBaseTexture9* pTex = (IDirect3DBaseTexture9*)(drawPrimHook_texAddr);
		//IDirect3DBaseTexture9* pTex = (IDirect3DBaseTexture9*)(*(DWORD*)drawPrimHook_texAddr);
		//
		//int type = pTex->GetType();
		//
		//log("ok %d", type);
	}

	index++;
}

void drawPrimCallback() {
	

	if (renderStateNeedsReset) {
		DWORD srcblend;
		DWORD dstblend;

		DWORD sepAlphaEnable;
		DWORD srcAlpha;
		DWORD dstAlpha;
		DWORD blendAlpha;
		DWORD otherBlendAlpha;


		// D3DRS_DESTBLEND fluctuates between 2 and 6
		// D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1
		 
		
		// D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  6 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1

		/*
		
		pattern: 165 state: 21
		D3DRS_BLENDOP:  1 D3DRS_ALPHABLENDENABLE:  1 D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2
		pattern: 165 state: 21
		D3DRS_BLENDOP:  1 D3DRS_ALPHABLENDENABLE:  1 D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2
		pattern: 165 state: 21
		D3DRS_BLENDOP:  1 D3DRS_ALPHABLENDENABLE:  1 D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2
		pattern: 165 state: 22
		D3DRS_BLENDOP:  1 D3DRS_ALPHABLENDENABLE:  1 D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2
		pattern: 165 state: 22
		D3DRS_BLENDOP:  1 D3DRS_ALPHABLENDENABLE:  1 D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2
		pattern: 165 state: 22
		D3DRS_BLENDOP:  1 D3DRS_ALPHABLENDENABLE:  1 D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2
		pattern: 165 state: 23
		D3DRS_BLENDOP:  1 D3DRS_ALPHABLENDENABLE:  1 D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2
		pattern: 165 state: 23
		D3DRS_BLENDOP:  1 D3DRS_ALPHABLENDENABLE:  1 D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2
		pattern: 165 state: 23
		D3DRS_BLENDOP:  1 D3DRS_ALPHABLENDENABLE:  1 D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2
		pattern: 165 state: 24
		pattern: 168 state: 0
		D3DRS_BLENDOP:  1 D3DRS_ALPHABLENDENABLE:  1 D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2
		pattern: 165 state: 24
		pattern: 168 state: 1
		D3DRS_BLENDOP:  1 D3DRS_ALPHABLENDENABLE:  1 D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2
		pattern: 165 state: 24
		pattern: 168 state: 1
		D3DRS_BLENDOP:  1 D3DRS_ALPHABLENDENABLE:  1 D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2
		pattern: 165 state: 25
		pattern: 168 state: 2
		D3DRS_BLENDOP:  1 D3DRS_ALPHABLENDENABLE:  1 D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2
		pattern: 165 state: 25
		pattern: 168 state: 2
		D3DRS_BLENDOP:  1 D3DRS_ALPHABLENDENABLE:  1 D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2

		cvaki air 22a 

		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  6 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  1 D3DSAMP_MINFILTER:  1
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  6 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  1 D3DSAMP_MINFILTER:  1
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  2 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		
		warc 236A 
		(REMEMBER THAT GROUND 214 A HAS THE TRAIL WITH THE NON0 BLEND

		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  6 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  6 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  6 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  6 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  6 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  6 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  6 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  6 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  6 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2
		D3DRS_SRCBLEND:  5 D3DRS_DESTBLEND:  6 D3DRS_SEPARATEALPHABLENDENABLE:  1 D3DRS_SRCBLENDALPHA:  5 D3DRS_DESTBLENDALPHA:  2 D3DRS_BLENDOPALPHA:  1 D3DRS_BLENDOP:  1 D3DSAMP_MAGFILTER:  2 D3DSAMP_MINFILTER:  2

		cvaki 22a 
		pattern: 168 state: 0
		pattern: 165 state: 24

		*/

		device->GetRenderState(D3DRS_SRCBLEND, &srcblend);
		device->GetRenderState(D3DRS_DESTBLEND, &dstblend);

		device->GetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, &sepAlphaEnable);
		device->GetRenderState(D3DRS_SRCBLENDALPHA, &srcAlpha);
		device->GetRenderState(D3DRS_DESTBLENDALPHA, &dstAlpha);
		device->GetRenderState(D3DRS_BLENDOPALPHA, &blendAlpha);
		device->GetRenderState(D3DRS_BLENDOP, &otherBlendAlpha);

		DWORD magFilter;
		DWORD minFilter;

		device->GetSamplerState(0, D3DSAMP_MAGFILTER, &magFilter);
		device->GetSamplerState(0, D3DSAMP_MINFILTER, &minFilter);

		//log("D3DRS_SRCBLEND: %2d D3DRS_DESTBLEND: %2d D3DRS_SEPARATEALPHABLENDENABLE: %2d D3DRS_SRCBLENDALPHA: %2d D3DRS_DESTBLENDALPHA: %2d D3DRS_BLENDOPALPHA: %2d D3DRS_BLENDOP: %2d D3DSAMP_MAGFILTER: %2d D3DSAMP_MINFILTER: %2d", srcblend, dstblend, sepAlphaEnable, srcAlpha, dstAlpha, blendAlpha, otherBlendAlpha, magFilter, minFilter);
	}

	if (pixelShaderNeedsReset) {
		pixelShaderNeedsReset = false;
		device->SetPixelShader(pPixelShader_backup);
	}
	

	if (renderStateNeedsReset) {
		renderStateNeedsReset = false;
		//device->SetRenderState(D3DRS_ALPHABLENDENABLE, renderState_backup);
	}

	
	//device->SetPixelShader(pPixelShader_backup);
}
 
DWORD listAppendHook_effectRetAddr = 0;
DWORD listAppendHook_objAddr = 0;
DWORD listAppendHook_texAddr = 0;
void listAppendHook() {
	//log("listAppendHook: %08X  objAddr: %08X retAddr: %08X", listAppendHook_texAddr, listAppendHook_objAddr, listAppendHook_effectRetAddr);

	if (listAppendHook_effectRetAddr == 0x0045410F) {

		if (listAppendHook_objAddr >= 0x0067BDE8) { // effect
			// source is how its listed in the cheat engine file, im not actually sure what it is
			char source = *(char*)(listAppendHook_objAddr - 8);


			log("source : %d", source);

			//log("listAppendHook_objAddr: %08X", listAppendHook_objAddr);
			//log("source is %d %02X", source, source);

			// todo, switch to a WHITELIST, notblacklist, of valid things to color

			DWORD pattern = *(DWORD*)(listAppendHook_objAddr + 0x0);
			DWORD state   = *(DWORD*)(listAppendHook_objAddr + 0x4);

			if (source >= 0) {
				
				BYTE blendMode = 0;
				
				// override blend mode to 0 so the shader works properly
				DWORD _animDataAddr = listAppendHook_objAddr + 0x310;
				if (*(DWORD*)_animDataAddr != 0) {
					DWORD _animDataPtr = *(DWORD*)_animDataAddr;
					// blend mode addr
					blendMode = *(BYTE*)(_animDataPtr + 0x1B);
				}
				
				log("pattern: %d state: %d", pattern, state);

				//log("obj: %08X", listAppendHook_objAddr);
				//if (blendMode == 0) { // for now, disable these. where is blend mode passed into a func to be used? 
					// changing it by this point is to late i believe, but it HAS to be put somewhere during this func!!
					// or somewhere else farther down the pipeline, or i find what actually is causing it
					// worst case scenario, and alternate shader could be written
					// actually, what should happen is piping that variable INTO the shader.
					textureAddrs.insert({ listAppendHook_texAddr, TextureModification(blendMode)});
				//}
			}

		} else { // char? possibly or the hit effects

		}	
	}
}

DWORD BLENDOP_Val;
DWORD ALPHABLENDENABLE_Val;
DWORD SRCBLEND_Val;
DWORD DESTBLEND_Val;
DWORD SEPARATEALPHABLENDENABLE_Val;
DWORD SRCBLENDALPHA_Val;
DWORD DESTBLENDALPHA_Val;
void setRenderStatesHook() {

	if (renderStateNeedsReset) {
		log("D3DRS_BLENDOP: %2d D3DRS_ALPHABLENDENABLE: %2d D3DRS_SRCBLEND: %2d D3DRS_DESTBLEND: %2d D3DRS_SEPARATEALPHABLENDENABLE: %2d D3DRS_SRCBLENDALPHA: %2d D3DRS_DESTBLENDALPHA: %2d", BLENDOP_Val, ALPHABLENDENABLE_Val, SRCBLEND_Val, DESTBLEND_Val, SEPARATEALPHABLENDENABLE_Val, SRCBLENDALPHA_Val, DESTBLENDALPHA_Val);
	
		if (D3DRS_DESTBLEND != 6) {
			//DESTBLEND_Val = 6;
			//log("changing DESTBLEND_Val to 6");
		}
	}
	
	

	device->SetRenderState(D3DRS_BLENDOP, BLENDOP_Val);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, ALPHABLENDENABLE_Val);
	device->SetRenderState(D3DRS_SRCBLEND, SRCBLEND_Val);
	device->SetRenderState(D3DRS_DESTBLEND, DESTBLEND_Val);
	device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, SEPARATEALPHABLENDENABLE_Val);
	device->SetRenderState(D3DRS_SRCBLENDALPHA, SRCBLENDALPHA_Val);
	device->SetRenderState(D3DRS_DESTBLENDALPHA, DESTBLENDALPHA_Val);

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
		//cmech 236a is .pat
		// cmech j214a crossbow changes color when dropped??

		mov eax, [esp + 350h];
		mov listAppendHook_objAddr, eax;

		mov eax, [esp + 3E0h];
		mov listAppendHook_effectRetAddr, eax;

		mov eax, listAppendHook_texAddr;
	};
	
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

__declspec(naked) void _naked_setRenderStatesHook() {
	__asm {
		test ecx, ecx;
		push edi;
		mov edi, eax;
		JBE _justLeave;
		cmp ecx, 7h;
		JA _justLeave;

		// iVar1 setup (MAKE SURE TO CHECK ASM) 004c06a4
		mov eax, dword ptr[edi + 4h];
		push esi;
		lea esi, [ecx * 8 + 0];
		sub esi, ecx;
		mov ecx, dword ptr[eax];
		add esi, esi;
		mov edx, dword ptr[esi + esi * 1 + 0054cc50h];
		add esi, esi; 
		// what is happening to esi here
		
		mov BLENDOP_Val, edx;
		
		mov edx, dword ptr[esi + 0054cc54h];
		mov ALPHABLENDENABLE_Val, edx;

		mov edx, dword ptr[esi + 0054cc58h];
		mov SRCBLEND_Val, edx;

		mov edx, dword ptr[esi + 0054cc5ch];
		mov DESTBLEND_Val, edx;

		mov edx, dword ptr[esi + 0054cc60h];
		mov SEPARATEALPHABLENDENABLE_Val, edx;

		mov edx, dword ptr[esi + 0054cc64h];
		mov SRCBLENDALPHA_Val, edx;

		mov edx, dword ptr[esi + 0054cc68h];
		mov DESTBLENDALPHA_Val, edx;

		// i cannot call my pushall/popall, doing it manually :(
		push esp;
		push ebp;
		push edi;
		push esi;
		push edx;
		push ecx;
		push ebx;
		push eax;
		push ebp;
		mov ebp, esp;

		call setRenderStatesHook;

		// pop all
		pop ebp;
		pop eax;
		pop ebx;
		pop ecx;
		pop edx;
		pop esi;
		pop edi;
		pop ebp;
		pop esp;

		pop esi;
	_justLeave:
		pop edi;
		ret;
	};
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

void initListAppendHook() {
	patchJump(0x004c026b, _naked_listAppendHook);
}

void initFrameCountCallback() {
	patchJump(0x004238f5, _naked_frameCountCallback);
}

void initSetRenderStatesHook() {
	patchFunction(0x004be2d2, _naked_setRenderStatesHook);
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
	initSetRenderStatesHook();



	log("initTextureModifications ran");
	
	init = true;
	return true;
}