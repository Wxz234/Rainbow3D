struct VSQuadOut {
	float4 position : SV_Position;
	float2 texcoord: TexCoord;
};
Texture2D g_Tex : register(t0);
SamplerState g_Sampler : register(s0);

float GetLuminance(float3 color)
{
	return dot(color, float3(0.2125, 0.7154, 0.0721));
}
float main(VSQuadOut vs_out) : SV_TARGET
{
	float lum = log(GetLuminance(g_Tex.Sample(g_Sampler, vs_out.texcoord).xyz));
	return lum;
}