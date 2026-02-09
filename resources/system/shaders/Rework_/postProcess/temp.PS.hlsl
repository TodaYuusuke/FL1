
#include "../utility/Random.hlsli"

struct PSInput
{
    float32_t4 position : SV_POSITION;
    float32_t2 texcoord : TEXCOORD0;
};
struct Parameter
{
    float time;
    int rWidth;
    int rHeight;
};

ConstantBuffer<Parameter> gPara : register(b0);
Texture2D<float32_t4> gTexture : register(t0);
Texture2D<float32_t> gDepth : register(t1);
SamplerState gSampler : register(s0);
SamplerState gPointSampler : register(s1);

struct RGBShiftData {
	float32_t2 shiftR;
	float32_t2 shiftG;
	float32_t2 shiftB;
};
ConstantBuffer<RGBShiftData> rgbShiftData : register(b1);

float32_t3 RGBShift(float32_t2 uv) {
    float32_t r = gTexture.Sample(gSampler, uv + rgbShiftData.shiftR).r;
    float32_t g = gTexture.Sample(gSampler, uv + rgbShiftData.shiftG).g;
    float32_t b = gTexture.Sample(gSampler, uv + rgbShiftData.shiftB).b;
    return float32_t3(r, g, b);
}

struct RadialBlurData {
	float32_t2 center;
	float32_t blurWidth;
};
ConstantBuffer<RadialBlurData> radialData : register(b2);

float32_t3 RadialBlur(float32_t3 rgb, float32_t2 uv) {
	const int32_t kNumSamples = 10;
	float32_t3 result = rgb;

	float32_t2 direction = uv - radialData.center;
	for(int32_t sampleIndex = 1; sampleIndex < kNumSamples; sampleIndex++) {
		float32_t2 texcoord = uv + direction * radialData.blurWidth * float32_t(sampleIndex);
		result += gTexture.Sample(gSampler, texcoord).rgb;
	}

	return result * rcp(kNumSamples);
}
struct VignettingData {
	float32_t intensity;
};
ConstantBuffer<VignettingData> vData : register(b3);

float32_t Vignetting(float32_t2 uv) {
	float32_t2 correct = uv * (1.0f - uv.xy);
	float vignette = correct.x * correct.y * 16.0f;
	vignette = saturate(pow(vignette, vData.intensity));
	return vignette;
}
struct FogData {
	float32_t4x4 projectionInverse;
	float32_t4 color;
	float32_t fogNear;
	float32_t fogFar;
};
ConstantBuffer<FogData> fogData : register(b4);

float32_t3 ApplyFog(float32_t2 uv, float32_t3 color) {
	float32_t ndcDepth = gDepth.Sample(gPointSampler, uv);
	
	// NDCからビュー空間へ変換（Zのみ）
	float32_t4 viewSpace = mul(float32_t4(0.0f, 0.0f, ndcDepth, 1.0f), fogData.projectionInverse);
	float32_t viewZ = viewSpace.z * rcp(viewSpace.w);
	
	// 霧の係数を計算（線形）
	float32_t fogFactor = saturate((viewZ - fogData.fogNear) / (fogData.fogFar - fogData.fogNear));
	
	return lerp(color, fogData.color.rgb, fogFactor);
}

float32_t4 main(PSInput input) : SV_TARGET {
	float32_t4 output;
	float2 uv = input.texcoord;

    output = gTexture.Sample(gSampler, uv);

	output.rgb = RGBShift(uv);

	output.rgb = RadialBlur(output.rgb, uv);

	output.rgb *= Vignetting(uv);

	output.rgb = ApplyFog(uv, output.rgb);

	output.a = 1.0f;
	return output;
}
