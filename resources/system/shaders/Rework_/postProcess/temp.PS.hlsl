
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

struct GlitchData {
	float32_t strength;
};
ConstantBuffer<GlitchData> glitchData : register(b1);


float32_t3 Glitch(float32_t2 uv, float32_t time) {
    float32_t2 pixelUV = uv;
    
    // 繝悶Ο繝・け蛻・ｧ｣閭ｽ縺ｮ險育ｮ暦ｼ域凾髢薙〒螟牙喧縺輔○繧具ｼ・
    float32_t blockTime = floor(time * 10.0);
    RandomGenerator resRand;
    resRand.seed = float32_t3(blockTime, 0, 0);
    float32_t2 resScale = 0.5 + resRand.Generate2d() * 2.0;
    float32_t2 blockRes = float32_t2(10.0, 30.0) * resScale;

    // 繝悶Ο繝・け繝弱う繧ｺ縺ｮ險育ｮ・
    float32_t2 blockPos = floor(uv * blockRes);
    RandomGenerator rand;
    rand.seed = float32_t3(blockTime, blockPos.x, blockPos.y);
    float32_t noise = rand.Generate1d();
    
    if (noise < glitchData.strength * 0.2) {
        // 讓ｪ譁ｹ蜷代∈縺ｮ繧ｺ繝ｬ
        RandomGenerator shiftRand;
        shiftRand.seed = float32_t3(blockPos.y, blockTime, 0);
        float32_t shift = (shiftRand.Generate1d() - 0.5) * 0.1 * glitchData.strength;
        pixelUV.x += shift;
    }
    
    // 繧ｫ繝ｩ繝ｼ繝√Ε繝ｳ繝阪Ν縺ｮ蛻・屬繧ょｰ代＠豺ｷ縺懊ｋ
    float32_t3 color;
    color.r = gTexture.Sample(gSampler, pixelUV + float32_t2(0.002, 0.0) * glitchData.strength).r;
    color.g = gTexture.Sample(gSampler, pixelUV).g;
    color.b = gTexture.Sample(gSampler, pixelUV - float32_t2(0.002, 0.0) * glitchData.strength).b;
    
    // 縺溘∪縺ｫ繝悶Ο繝・け迥ｶ縺ｫ濶ｲ繧貞渚霆｢縺輔○縺溘ｊ縺吶ｋ
    //if (noise < glitchData.strength * 0.05) {
    //    color = 1.0 - color;
    //}
    
    return color;
}

float32_t4 main(PSInput input) : SV_TARGET {
	float32_t4 output;
	float2 uv = input.texcoord;

    output = gTexture.Sample(gSampler, uv);

	output.rgb = Glitch(uv, gPara.time);

	output.a = 1.0f;
	return output;
}
