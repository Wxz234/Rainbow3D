struct VSQuadOut {
	float4 position : SV_Position;
	float2 texcoord: TexCoord;
};
Texture2D g_Tex : register(t0);
SamplerState g_Sampler : register(s0);
RWBuffer<float> LuminanceOutput : register(u2);

float3 ACESToneMapping(float3 color, float adapted_lum)
{
	const float A = 2.51f;
	const float B = 0.03f;
	const float C = 2.43f;
	const float D = 0.59f;
	const float E = 0.14f;

	color *= adapted_lum;
	return (color * (A * color + B)) / (color * (C * color + D) + E);
}

float4 main(VSQuadOut vs_out) : SV_TARGET
{
	float3 color = g_Tex.Sample(g_Sampler,vs_out.texcoord).xyz;
	float adapted_lum = LuminanceOutput[0];
	float3 newcolor = ACESToneMapping(color, adapted_lum);
	return g_Tex.Sample(g_Sampler, vs_out.texcoord);
}