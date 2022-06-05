struct VSQuadOut {
	float4 position : SV_Position;
	float2 texcoord: TexCoord;
};
Texture2D g_Tex : register(t0);
SamplerState g_Sampler : register(s0);

float4 main(VSQuadOut vs_out) : SV_TARGET
{
	return g_Tex.Sample(g_Sampler,vs_out.texcoord);
}