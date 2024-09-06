

sampler2D textureSampler : register(s0); // is using this low of a reg ok?
sampler2D textureSampler2 : register(s1); 


float4 dynamicColor : register(c223); // using register 223 bc i dont want to mess something up
DWORD blendMode : register(c222);

float4 Hue : register(c221);

float4 ActiveColor : register(c220);

float4 texSize : register(c219);

//const float EPSILON = 1e-10;

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
	float h = abs((q.w - q.y) / (6. * c + 1e-10) + q.z);
	return float3(h, c, q.x);
}

float3 RGBtoHSV(float3 rgb)
{
	// RGB [0..1] to Hue-Saturation-Value [0..1]
	float3 hcv = RGBtoHCV(rgb);
	float s = hcv.y / (hcv.z + 1e-10);
	return float3(hcv.x, s, hcv.z);
}

float3 HSVtoRGB(float3 hsv)
{
	// Hue-Saturation-Value [0..1] to RGB [0..1]
	float3 rgb = HUEtoRGB(hsv.x);
	return ((rgb - 1.) * hsv.y + 1.) * hsv.z;
}

float4 gay(float2 texCoord : TEXCOORD0) : COLOR
{
	float4 texColor = tex2D(textureSampler, texCoord);

	float3 hsvVal = RGBtoHSV(texColor.rgb);

	// rotate through rainbow
	// hsvVal.r = Hue.r;

	// weird but cool
	//hsvVal.r = texColor.x;
	
	// make whole texture a rainbow
	//hsvVal.r = texCoord.x;

	// rainbow but different
	//hsvVal.r = (texCoord.x + texCoord.y) / 2.0;

	hsvVal.r = texCoord.x;
	
	float3 rgbVal = HSVtoRGB(hsvVal.rgb);

	texColor.rgb = rgbVal;

	return texColor;
}

float4 trans(float2 texCoord : TEXCOORD0) : COLOR
{

	// this shader is horrid, optimize it

	float4 texColor = tex2D(textureSampler, texCoord);

	// this cant be global. idek why
	const float3 col1 = float3(0.35546875, 0.80078125, 0.9765625);
	const float3 col2 = float3(0.95703125, 0.66015625, 0.71875);
	const float3 col3 = float3(1.0, 1.0, 1.0);
	
	texCoord = frac(texCoord + 4.0 * Hue.r); // same as mod 1
	
	
	float3 res = float3(0.0, 0.0, 0.0); // actual trans color
	
	texCoord *= 2.0;
	
	if (texCoord.x > 1.0)
	{
		texCoord.x = 2.0 - texCoord.x;
	}

	if (texCoord.y > 1.0)
	{
		texCoord.y = 2.0 - texCoord.y;
	}
	
	float val = (texCoord.x + texCoord.y) / 2.0;
	
	if (val < 0.5)
	{
		res = (col2 * 2.0 * val) + (col1 * (1.0 - 2.0 * val));
	}
	else
	{
		val -= 0.5;
		res = (col3 * 2.0 * val) + (col2 * (1.0 - 2.0 * val));
	}

	
	float3 newHSV = RGBtoHSV(res);
	

	float3 origHSV = RGBtoHSV(texColor.rgb);
	//origHSV.r = newHSV.r;
	//origHSV.r = newHSV.r;
	
	origHSV.xy = newHSV.xy;
	texColor.rgb = HSVtoRGB(origHSV);
	
	//texColor.rgb = res.rgb;
	
	//texColor.rgb = HSVtoRGB(RGBtoHSV(res));
	
	return texColor;
}

float4 infrared(float2 texCoord : TEXCOORD0) : COLOR
{
	float4 texColor = tex2D(textureSampler, texCoord);

	//texColor.b = texSize.x;
	
	  // Compute offset for sampling neighboring pixels
	float2 texOffset = 6.0 / texSize;
	
	// Sample surrounding pixels to determine outline
	float4 colorLeft = tex2D(textureSampler, texCoord - float2(texOffset.x, 0));
	float4 colorRight = tex2D(textureSampler, texCoord + float2(texOffset.x, 0));
	float4 colorUp = tex2D(textureSampler, texCoord - float2(0, texOffset.y));
	float4 colorDown = tex2D(textureSampler, texCoord + float2(0, texOffset.y));
	
	const float borderDelta = 4 * (0.05f);
	const float insideDelta = 4 * (1.0 - 0.75f);
	
	float alpha = colorLeft.a + colorRight.a + colorUp.a + colorDown.a;
	
	float3 res = RGBtoHSV(texColor.rgb);
	
	//res.x = 0.0;
	
	if (alpha > insideDelta) {
		res.x -= 0.5 * (alpha / 4.0) * (alpha / 4.0) * (alpha / 4.0);
	}
	
	if (res.x < 0.0) {
		res.x += 1.0;
	}
	
	texColor.rgb = HSVtoRGB(res);
 
	return texColor;
}

float4 main(float2 texCoord : TEXCOORD0) : COLOR {
	
	float4 orig = tex2D(textureSampler, texCoord);
	
	float4 newCol = tex2D(textureSampler2, texCoord);
	
	float2 texOffset = 2.0 / texSize;
	float4 colorUp   = tex2D(textureSampler2, texCoord - float2(0, texOffset.y));
	float4 colorDown = tex2D(textureSampler2, texCoord + float2(0, texOffset.y));
	float4 colorLeft = tex2D(textureSampler2, texCoord - float2(texOffset.x, 0));
	float4 colorRight = tex2D(textureSampler2, texCoord + float2(texOffset.x, 0));
	
	float4 upDif    = abs(colorUp - newCol);
	float4 downDif  = abs(colorDown - newCol);
    float4 leftDif  = abs(colorLeft - newCol);
    float4 rightDif = abs(colorRight - newCol);
	
	float up    = upDif.r      * upDif.g      * upDif.b;
	float down  = downDif.r    * downDif.g    * downDif.b;
    float left  = leftDif.r    * leftDif.g    * leftDif.b;
    float right = rightDif.r   * rightDif.g   * rightDif.b;
	
	const float idrk = 0.01;
	
    float3 newHSV = float3(0.0, 0.0, 0.0);

	if (down > idrk) {
        newHSV.xyz = float3(0.9, 1.0, 1.0);
    }
	
	if (up > idrk) {
        newHSV.xyz = float3(0.5, 1.0, 1.0);
    }
	
	if (left > idrk) {
        newHSV.xyz = float3(0.3, 1.0, 1.0);
    }
	
    if (right > idrk) {
        newHSV.xyz = float3(0.0, 1.0, 1.0);
    }
	
    newCol.rgb = HSVtoRGB(newHSV);
	newCol.a = orig.a;
	
	return newCol;
}