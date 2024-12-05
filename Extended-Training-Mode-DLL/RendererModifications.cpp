
extern bool enableEffectColors;
extern float effectColorHue;
#include "DirectX.h"
#include "RendererModifications.h"
#include <string>
#include <functional>

bool useCustomShaders = false;
bool useDeerMode = false;

typedef struct {
	const WORD charID;
	const WORD pattern;
} CharPattern;

constexpr WORD sionID = 0;
constexpr WORD arcID = 1;
constexpr WORD ceilID = 2;
constexpr WORD akiID = 3;
constexpr WORD maidsID = 4;
constexpr WORD hisuID = 5;
constexpr WORD kohaID = 6;
constexpr WORD tonoID = 7;
constexpr WORD miyaID = 8;
constexpr WORD waraID = 9;
constexpr WORD neroID = 10;
constexpr WORD vsionID = 11;
constexpr WORD warcID = 12;
constexpr WORD vakiID = 13;
constexpr WORD mechID = 14;
constexpr WORD nanaID = 15;
constexpr WORD dukaID = 16;
constexpr WORD lenID = 17;
constexpr WORD pceilID = 18;
constexpr WORD necoID = 19;
constexpr WORD aokoID = 20;
constexpr WORD wlenID = 21;
constexpr WORD nacID = 22;
constexpr WORD koumaID = 23;
constexpr WORD seiD = 24;
constexpr WORD riesID = 25;
constexpr WORD roaID = 26;
constexpr WORD roogID = 27;
constexpr WORD necmecID = 28;
constexpr WORD kohmecID = 29;
constexpr WORD himeID = 30;

constexpr const CharPattern blacklist[] = {
	//{ warcID, 120  },

	// kouma:

	{ koumaID, 139 }, // ckouma 22c
	{ koumaID, 140 },
	{ koumaID, 141 },
	{ koumaID, 142 },

	{ koumaID, 103 }, // ckouma 22b

	{ koumaID, 447 }, // j22a/j22b
	{ koumaID, 451 }, // j22c

	{ koumaID, 223 }, // fkouma 421c

	// len
	{ lenID, 75 }, // clen 2c
	{ lenID, 80 }, // clen 3c

	{ lenID, 55 }, // happens when she crouches??
	{ lenID, 56 }, // uncrouch?

	{ lenID, 203 }, // clen 421B
	{ lenID, 204 }, // clen 421C

	{ lenID, 165 }, // clen AAD
	{ lenID, 166 }, // clen AAD
	{ lenID, 167 }, // clen AAD
	{ lenID, 168 }, // clen AAD
	{ lenID, 169 }, // clen AAD

	{ lenID, 183 }, // clen cake (last arc)

	{ lenID, 468 }, // bonk hammer

	// wlen

	// todo, need to check out her icicles
	// grounded 236a, uncharged, icicle is colored, it breaking isnt. why??

	{ wlenID, 170 }, // dash forward
	{ wlenID, 173 }, // dash forward
	{ wlenID, 175 }, // dash back

	{ wlenID, 212 }, // last arc
	{ wlenID, 213 }, // last arc???
	{ wlenID, 214 }, // last arc
	{ wlenID, 215 }, // last arc, dont know what this is, doing it anyway
	{ wlenID, 208 }, // last arc, dont know what this is, doing it anyway
	{ wlenID, 216 }, // last arc, dont know what this is, doing it anyway

	{ wlenID, 189 }, // unknown, 22a
	{ wlenID, 190 }, // 22b
	{ wlenID, 191 }, // 22c
	{ wlenID, 187 }, // 22d (assumed)
	{ wlenID, 189 }, // 22d

	{ wlenID, 157 }, // AAD
	{ wlenID, 152 }, // AAD
	{ wlenID, 153 }, // AAD
	{ wlenID, 155 }, // AAD

	{ wlenID, 496 }, // hwlen 22b

	// nanana

	{ nanaID, 206 }, // AAD
	{ nanaID, 501 }, // H 623C

	// hime 

	// TODO, FINISH THIS UP
	{ himeID, 143 }, // 214C
	{ himeID, 144 }, // 214C
	{ himeID, 145 }, // 214C
	{ himeID, 146 }, // 214C

	{ himeID, 147 }, // 214A-A
	{ himeID, 148 }, // 214A-A
	{ himeID, 149 }, // 214A-A

	{ himeID, 150 }, // 214A-A
	{ himeID, 151 }, // 214A-A
	{ himeID, 152 }, // 214A-A

	{ himeID, 76 }, // fly
	{ himeID, 208}, // fly
	{ himeID, 209 }, // fly
	{ himeID, 78 }, // misc fly?
	{ himeID, 77 }, // misc fly?

	{ himeID, 166 }, // 236a, rocks on the ground
	{ himeID, 168 }, // more rocks
	{ himeID, 167 }, // more rocks

	{ himeID, 298 }, // AAD
	{ himeID, 302 }, // AAD
	{ himeID, 301 }, // AAD
	{ himeID, 300 }, // AAD
	{ himeID, 312 }, // AAD
	{ himeID, 318 }, // AAD
	{ himeID, 306 }, // AAD
	{ himeID, 307 }, // AAD
	{ himeID, 319 }, // AAD
	{ himeID, 316 }, // AAD
	{ himeID, 310 }, // AAD

	{ himeID, 277 }, // grab??? what?

	// arc

	// TODO
	{ arcID, 125 }, // 214
	{ arcID, 126 }, // 214
	{ arcID, 127 }, // 214

	{ arcID, 116 }, // lil hime
	{ arcID, 115 }, // last arc
	{ arcID, 118 }, // last arc

	// pceil, her 214s are,,, way to bright? im just disabling them for now. sorry pciel players

	{ pceilID, 336 }, // 214
	{ pceilID, 335 }, // 214

	// aoko

	{ aokoID, 161 }, // last arc
	{ aokoID, 160 }, // last arc
	{ aokoID, 167 }, // last arc
	{ aokoID, 159 }, // last arc
	{ aokoID, 162 }, // last arc
	{ aokoID, 163 }, // last arc
	{ aokoID, 164 }, // last arc
	{ aokoID, 165 }, // last arc

	// ceil

	{ ceilID, 201 }, // AD/AAD, her cape shouldnt change color
	{ ceilID, 202 }, // AD/AAD, her cape shouldnt change color
	{ ceilID, 207 }, // what is this? more needs to be looked into 

	// ries

	{ riesID , 500 }, // last arc
	{ riesID , 496 }, // last arc
	{ riesID , 505 }, // AAD bg

	// vsion

	{ vsionID, 505 }, // last arc
	{ vsionID, 506 }, // last arc
	{ vsionID, 507 }, // last arc
	{ vsionID, 508 }, // last arc
	{ vsionID, 509 }, // last arc
	{ vsionID, 510 }, // last arc
	{ vsionID, 511 }, // last arc
	{ vsionID, 512 }, // last arc
	{ vsionID, 513 }, // last arc
	{ vsionID, 514 }, // last arc
	{ vsionID, 515 }, // last arc
	{ vsionID, 516 }, // last arc
	{ vsionID, 517 }, // last arc
	{ vsionID, 518 }, // last arc

	{ vsionID, 206 }, // AAD
	{ vsionID, 209 }, // AAD

	// i should maybe disable the coloration on her summons

	// wara

	{ waraID, 83 }, // last arc, 81 and 82 maybe should get it as well?
	{ waraID, 82 },
	{ waraID, 81 },
	{ waraID, 84 },
	{ waraID, 85 },
	{ waraID, 86 },
	{ waraID, 87 },
	{ waraID, 88 },

	{ waraID, 156 }, // AD

	// roa 

	{ roaID, 198 }, // last arc bg
	{ roaID, 196 }, // last arc
	{ roaID, 195 }, // last arc

	{ roaID, 169 }, // AD BG

	{ roaID, 155 }, // AD
	{ roaID, 156 }, // AD
	{ roaID, 157 }, // AD
	{ roaID, 177 }, // AD
	{ roaID, 171 }, // AD
	{ roaID, 172 }, // AD
	{ roaID, 173 }, // AD

	{ roaID, 160 }, // AAD
	{ roaID, 161 }, // AAD
	{ roaID, 162 }, // AAD
	{ roaID, 163 }, // AAD
	{ roaID, 164 }, // AAD
	{ roaID, 165 }, // AAD
	{ roaID, 166 }, // AAD
	{ roaID, 167 }, // AAD

	{ roaID, 184 }, // AAD
	{ roaID, 185 }, // AAD

	// maids. this is a confusing case. can i ,,,, duplicate hisu and koha stuff here? or like, will i have to do them seperately

	// all maids, koha, hisui, mech, and neco need to be looked over
	// check and see if can just,,, apply all maids blacklists to both maids? i think i can?

	// hisumaids

	{ maidsID, 153 }, // last arc when on hisui, are they different per char?
	{ maidsID, 139 },
	{ maidsID, 140 },
	{ maidsID, 141 },
	{ maidsID, 142 },
	{ maidsID, 143 },
	{ maidsID, 144 },
	{ maidsID, 145 },
	{ maidsID, 146 },
	{ maidsID, 147 },
	{ maidsID, 148 },
	{ maidsID, 154 },
	{ maidsID, 155 },

	{ maidsID, 98 }, // jc table
	{ maidsID, 96 }, // jc table
	{ maidsID, 90 }, // jc table
	{ maidsID, 91 }, // jc table
	{ maidsID, 92 }, // jc table
	{ maidsID, 93 }, // jc table
	{ maidsID, 94 }, // jc table
	{ maidsID, 95 }, // jc table

	{ maidsID, 45 }, // charge 5c chair
	{ maidsID, 46 }, // charge 5c chair
	{ maidsID, 47 }, // charge 5c chair

	{ maidsID, 414 }, // 22 picnic
	{ maidsID, 415 }, // 22 picnic
	{ maidsID, 416 }, // 22 picnic

	{ maidsID, 418 }, // 22 picnic
	{ maidsID, 419 }, // 22 picnic
	{ maidsID, 420 }, // 22 picnic
	{ maidsID, 423 }, // 22 picnic
	{ maidsID, 424 }, // 22 picnic
	{ maidsID, 481 }, // 22c picnic when it hits???

	{ maidsID, 202 }, // hisui AAD arms
	{ maidsID, 201 }, // hisui AAD arms

	{ maidsID, 431 }, // fmoon hisui 22 chair
	{ maidsID, 432 }, // fmoon hisui 22 chair
	{ maidsID, 433 }, // fmoon hisui 22 chair
	{ maidsID, 434 }, // fmoon hisui 22 chair
	{ maidsID, 435 }, // fmoon hisui 22 chair

	{ maidsID, 438 }, // fmoon hisui 22 chair
	{ maidsID, 439 }, // fmoon hisui 22 chair
	{ maidsID, 440 }, // fmoon hisui 22 chair
	{ maidsID, 441 }, // fmoon hisui 22 chair
	{ maidsID, 442 }, // fmoon hisui 22 chair

	{ maidsID, 446 }, // fmoon hisui 22 chair
	{ maidsID, 447 }, // fmoon hisui 22 chair
	{ maidsID, 448 }, // fmoon hisui 22 chair
	{ maidsID, 449 }, // fmoon hisui 22 chair
	{ maidsID, 450 }, // fmoon hisui 22 chair

	// kohamaids

	{ maidsID, 183 }, // koha last arc
	{ maidsID, 184 },
	{ maidsID, 185 },
	{ maidsID, 186 },
	{ maidsID, 169 },

	// koha plants
	{ maidsID, 129 }, // koha 22
	{ maidsID, 130 }, // koha 22
	{ maidsID, 131 }, // koha 22
	{ maidsID, 132 }, // koha 22
	{ maidsID, 133 }, // koha 22
	{ maidsID, 134 }, // koha 22
	{ maidsID, 135 }, // koha 22
	{ maidsID, 136 }, // koha 22
	{ maidsID, 460 }, // koha 22
	{ maidsID, 462 }, // koha 22

	{ maidsID, 202 }, // koha AAD
	{ maidsID, 203 }, // koha AAD
	{ maidsID, 228 }, // koha AAD
	{ maidsID, 212 }, // koha AAD
	{ maidsID, 213 }, // koha AAD
	{ maidsID, 214 }, // koha AAD
	{ maidsID, 226 }, // koha AAD

	// koha pretzel oh no i cant even input that


	// the things for each koha could probs/should be copied to each maid


	// hisui 

	{ hisuID, 202 }, // arms
	{ hisuID, 201 }, // arms
	{ hisuID, 414 }, // 22 picnic
	{ hisuID, 415 }, // 22 picnic
	{ hisuID, 416 }, // 22 picnic

	{ hisuID, 418 }, // 22 picnic
	{ hisuID, 419 }, // 22 picnic
	{ hisuID, 420 }, // 22 picnic
	{ hisuID, 423 }, // 22 picnic
	{ hisuID, 424 }, // 22 picnic
	{ hisuID, 481 }, // 22c picnic when it hits???

	{ hisuID, 431 }, // fmoon hisui 22 chair
	{ hisuID, 432 }, // fmoon hisui 22 chair
	{ hisuID, 433 }, // fmoon hisui 22 chair
	{ hisuID, 434 }, // fmoon hisui 22 chair
	{ hisuID, 435 }, // fmoon hisui 22 chair

	{ hisuID, 438 }, // fmoon hisui 22 chair
	{ hisuID, 439 }, // fmoon hisui 22 chair
	{ hisuID, 440 }, // fmoon hisui 22 chair
	{ hisuID, 441 }, // fmoon hisui 22 chair
	{ hisuID, 442 }, // fmoon hisui 22 chair

	{ hisuID, 446 }, // fmoon hisui 22 chair
	{ hisuID, 447 }, // fmoon hisui 22 chair
	{ hisuID, 448 }, // fmoon hisui 22 chair
	{ hisuID, 449 }, // fmoon hisui 22 chair
	{ hisuID, 450 }, // fmoon hisui 22 chair

	// koha

	{ kohaID, 183 }, // koha last arc
	{ kohaID, 184 },
	{ kohaID, 185 },
	{ kohaID, 186 },
	{ kohaID, 169 },

	// koha plants
	{ kohaID, 129 }, // koha 22
	{ kohaID, 130 }, // koha 22
	{ kohaID, 131 }, // koha 22
	{ kohaID, 132 }, // koha 22
	{ kohaID, 133 }, // koha 22
	{ kohaID, 134 }, // koha 22
	{ kohaID, 135 }, // koha 22
	{ kohaID, 136 }, // koha 22
	{ kohaID, 460 }, // koha 22
	{ kohaID, 462 }, // koha 22

	{ kohaID, 202 }, // koha AAD
	{ kohaID, 203 }, // koha AAD
	{ kohaID, 228 }, // koha AAD
	{ kohaID, 212 }, // koha AAD
	{ kohaID, 213 }, // koha AAD
	{ kohaID, 214 }, // koha AAD
	{ kohaID, 226 }, // koha AAD

	// kohamech (H: 214C, 624C

	// mech	todo, her rare summons?

	{ mechID, 134 }, // j214

	{ mechID, 110 }, // jetpack
	{ mechID, 113 }, // jetpack
	{ mechID, 116 }, // jetpack
	{ mechID, 117 }, // jetpack
	{ mechID, 118 }, // jetpack
	{ mechID, 119 }, // jetpack

	{ mechID, 98 }, // j22c dropped weapon

	{ mechID, 218 }, // the GOONS
	{ mechID, 219 }, // the GOONS
	{ mechID, 220 }, // the GOONS

	{ mechID, 45 }, // chainsaw on floor, its dark so doesnt show but still

	{ mechID, 544 }, // fmech 22a bomb thing 
	{ mechID, 422 },
	{ mechID, 428 },
	{ mechID, 541 }, // mech,,, bomb countdown
	{ mechID, 438 },
	{ mechID, 434 },

	// aki

	{ akiID, 212 }, // last arc
	//{ akiID, 240 }, // last arc (maybe?
	//{ akiID, 241 }, // last arc (maybe?
	//{ akiID, 242 }, // last arc (maybe?
	//{ akiID, 243 }, // last arc (maybe?
	//{ akiID, 244 }, // last arc (maybe?
	//{ akiID, 245 }, // last arc (maybe?
	//{ akiID, 246 }, // last arc (maybe?

	// warc
	{ warcID, 220 }, // chains in last arc
	{ warcID, 221 }, // chains in last arc

	{ warcID, 155 }, // teleport afterimage
	{ warcID, 156 }, // teleport afterimage
	{ warcID, 157 }, // teleport afterimage
	{ warcID, 158 }, // teleport afterimage
	{ warcID, 159 }, // teleport afterimage
	{ warcID, 160 }, // teleport afterimage
	{ warcID, 161 }, // teleport afterimage
	{ warcID, 162 }, // teleport afterimage

	// vaki

	{ vakiID, 537 }, // last arc
	{ vakiID, 168 }, // on c with a 22 out, she will sometimes flicker. is this the cause?

	// roog

	{ roogID, 20 }, // blinker
	{ roogID, 608 }, // last arc bg
	{ roogID, 640 }, // last arc 

	// nero 

	{ neroID, 202 }, // last arc
	{ neroID, 203 }, // last arc
	{ neroID, 169 }, // last arc
	{ neroID, 175 }, // AAD
	{ neroID, 178 }, // AAD

	// nac

	{ nacID, 115 }, // lil neros on 214

	{ nacID, 162 }, // last arc
	{ nacID, 163 }, // last arc

	{ nacID, 233 }, // AAD
	{ nacID, 234 }, // AAD
	{ nacID, 235 }, // AD
	{ nacID, 236 }, // AD
	{ nacID, 237 }, // AAD

	// nec

	{ necoID, 115 }, // lil necos on 214

	{ necoID, 162 }, // last arc
	{ necoID, 163 }, // last arc

	{ necoID, 219 }, // AD
	{ necoID, 220 }, // AD
	{ necoID, 225 }, // AAD
	{ necoID, 203 }, // AAD
	{ necoID, 204 }, // AAD

	// things are pretty much done, but more work needs to be done on assist/dual chars

	{0xFF, 0} // i was very tired of adding commas to prev lines
};

constexpr size_t blacklistLen = (sizeof(blacklist) / sizeof(blacklist[0])) - 1; // minus 1 bc of the trailing 0xFF, 0

constexpr auto textureModificationData = []() constexpr -> auto {
	std::array<std::array<BYTE, 125>, 32> res{}; // roughly 4kb, worth it vs using a map tbh

	for (int x = 0; x < 32; x++) {
		for (int y = 0; y < 125; y++) {
			res[x][y] = 0;
		}
	}

	for (size_t i = 0; i < blacklistLen; ++i) {
		if (blacklist[i].pattern >= 1000) {
			throw std::logic_error("???");
			// visual studio doesnt actually display the throw here, and just says it couldnt be evaled at constexpr. 
			// but this at least works while static_assert didnt
		}
	}

	for (size_t i = 0; i < blacklistLen; ++i) {
		res[blacklist[i].charID][blacklist[i].pattern / 8] |= (1 << (blacklist[i].pattern % 8));
	}

	return res;
	}();

#pragma push_macro("optimize")
#pragma optimize("t", on)
// go check godbolt. this opt level is shit. /O2 is a flag which exists, but has to be enabled for the whole project
// the asm for this with and without t on is the same. why.
// i write my shit to be fast, and this compiler spits on me

uint_fast8_t getPatternIndex(uint_fast8_t base) {
	// translate the games id system into the one listed above

	switch (base) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		return base;
	case 17:
	case 18:
	case 19:
	case 20:
		return base - 1;
	case 22:
	case 23:
		return base - 2;
	case 25:
		return base - 3;
	case 28:
	case 29:
	case 30:
	case 31:
		return base - 5;
	case 33:
	case 34:
	case 35:
		return base - 6;
	case 51:
		return 30; // hime
	default:
		return 0xFF;
	}

	return 0xFF;
}

bool shouldThisBeColored(BYTE charID, DWORD pattern) {

	BYTE index = getPatternIndex(charID);
	if (index == 0xFF) {
		return false;
	}

	BYTE temp = textureModificationData[index][pattern / 8];

	temp >>= (pattern % 8);

	return !(temp & 1);
}

void renderModificationsFrameDone() {
	//textureAddrs.clear();

	if (device == NULL) {
		return;
	}

	static D3DXVECTOR4 frameFloatOffset(0.0f, 0.0f, 0.0f, 0.0f);
	frameFloatOffset.x += (1.0f / 60.0f);
	if (frameFloatOffset.x > 1.0f) {
		frameFloatOffset.x = 0.0f;
	}

	device->SetPixelShaderConstantF(223, (float*)&frameFloatOffset, 1);

	if (!enableEffectColors) {
		return;
	}

	D3DXVECTOR4 temp(effectColorHue, 0.0f, 0.0f, 0.0f);

	device->SetPixelShaderConstantF(221, (float*)&temp, 1); // calling this every frame kinda sucks, but im tired
}

#pragma pop_macro("optimize")

void initDrawIndexPrimHook() {
	patchJump(0x004be46e, _naked_drawIndexPrimHook);
	patchJump(0x004be524, _naked_drawPrimCallback);
}

void initEffectSelector() {

	patchJump(0x004c026b, _naked_listAppendHook);
	patchJump(0x004be290, _naked_drawPrimHook);
	patchJump(0x004c0380, _naked_leadToDrawPrimHook);

}

bool initRenderModifications() {

	if (device == NULL) {
		return false;
	}

	initDrawIndexPrimHook();

	initEffectSelector();

	pPixelShader = createPixelShader(R"(

sampler2D textureSampler : register(s0);
float4 Hue : register(c221);

float3 HUEtoRGB(float hue)
{
	float3 rgb = abs(hue * 6. - float3(3, 2, 4)) * float3(1, -1, -1) + float3(-1, 2, 2);
	return clamp(rgb, 0., 1.);
}

float3 RGBtoHCV(float3 rgb)
{
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

float4 main(float2 texCoord : TEXCOORD0) : COLOR
{
		
	
	float4 orig = tex2D(textureSampler, texCoord);
	float3 hsvVal = RGBtoHSV(orig.rgb);
	hsvVal.x = Hue.x; // input hue from register.
	orig.rgb = HSVtoRGB(hsvVal);
	return orig;
	


}

	)");

	log("rendermodifications inited successfully");

	return true;
}

void loadCustomShader() {

	if (!useCustomShaders && !useDeerMode) {
		return;
	}

	if (pCustomShader != NULL) {
		pCustomShader->Release();
		pCustomShader = NULL;
	}

	pCustomShader = loadPixelShaderFromFile(L"testShader.hlsl");

}