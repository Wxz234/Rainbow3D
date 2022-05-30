struct VSOUT {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};
Texture2D g_Tex : register(t0);
SamplerState g_Sampler : register(s0);


float3 ACESFilm(float3 x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return saturate((x * (a * x + b)) / (x * (c * x + d) + e));
}
float4 main(VSOUT vs_out) : SV_TARGET
{
	float4 color = g_Tex.Sample(g_Sampler, vs_out.uv) * 0.6;
	return float4(ACESFilm(color), 1);
}