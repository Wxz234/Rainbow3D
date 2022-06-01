struct VSOUT {
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};
Texture2D g_Tex : register(t0);
SamplerState g_Sampler : register(s0);


float4 main(VSOUT vs_out) : SV_TARGET
{
    return g_Tex.Sample(g_Sampler, vs_out.uv);
}