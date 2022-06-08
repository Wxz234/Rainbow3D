struct VSQuadOut {
	float4 position : SV_Position;
	float2 texcoord: TexCoord;
};


Texture2D g_Tex : register(t0);
Texture2D Luminance : register(t1);
SamplerState g_Sampler : register(s0);

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
	float log_lum = exp(Luminance.mips[9][uint2(0,0)].x);
	float alpha = 1.03 - 2.0 / (2.0 + log10(log_lum + 1.0));
	float exposure = alpha / log_lum;
	float4 color = g_Tex.Sample(g_Sampler, vs_out.texcoord);
	float3 newcolor = ACESToneMapping(color.xyz, exposure);
	return float4(newcolor, 1.0f);
}