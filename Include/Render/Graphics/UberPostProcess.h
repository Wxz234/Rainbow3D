#pragma once
#include "Core/CorePreDef.h"
namespace Rainbow3D {
inline std::string GetUberPostProcessVS() {
return R"(
struct VSQuadOut {
	float4 position : SV_Position;
	float2 texcoord: TexCoord;
};
VSQuadOut main(uint VertexID: SV_VertexID) {
	VSQuadOut Out;

	Out.texcoord = float2((VertexID << 1) & 2, VertexID & 2);
	Out.position = float4(Out.texcoord * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);
	return Out;
}
)";
}

inline std::string GetUberPostProcessPS() {
return R"(
struct VSQuadOut {
	float4 position : SV_Position;
	float2 texcoord: TexCoord;
};

#ifdef RAINBOW_LOG_LUMINANCE_SHADER
Texture2D tex : register(t0);
SamplerState linear_sampler : register(s0);
#endif

#ifdef RAINBOW_ACES_SHADER
Texture2D tex : register(t0);
Texture2D luminance : register(t1);
SamplerState linear_sampler : register(s0);
#endif

#ifdef RAINBOW_LOG_LUMINANCE_SHADER
float GetLuminance(float4 color)
{
	return dot(color.xyz, float3(0.2125, 0.7154, 0.0721));
}

float4 GetLogLuminance(float4 color) {
	float lum = GetLuminance(color);
	if (lum < 0.0005) {
		lum = 0.0005;
	}
	return float4(log(lum), 1, 1, 1);
}
#endif

#ifdef RAINBOW_ACES_SHADER
float4 ACESToneMapping(float4 color, float adapted_lum)
{
	const float A = 2.51f;
	const float B = 0.03f;
	const float C = 2.43f;
	const float D = 0.59f;
	const float E = 0.14f;

	float3 newcolor = color.xyz;
	newcolor *= adapted_lum;
	return float4((newcolor * (A * newcolor + B)) / (newcolor * (C * newcolor + D) + E), 1);
}
#endif


float4 main(VSQuadOut vs_out) : SV_TARGET
{
	float4 color = float4(1,1,1,1);
#ifdef RAINBOW_LOG_LUMINANCE_SHADER
	color = GetLogLuminance(tex.Sample(linear_sampler, vs_out.texcoord));
#endif
#ifdef RAINBOW_ACES_SHADER
	float log_lum = exp(luminance.mips[9][uint2(0, 0)].x);
	float alpha = 1.03 - 2.0 / (2.0 + log10(log_lum + 1.0));
	float exposure = alpha / log_lum;
	color = clamp(ACESToneMapping(tex.Sample(linear_sampler, vs_out.texcoord), exposure), float4(0, 0, 0, 1), float4(1, 1, 1, 1));
#endif
	return color;
}
)";
}


}