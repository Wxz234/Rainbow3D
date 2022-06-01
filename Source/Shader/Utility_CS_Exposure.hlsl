Texture2D g_Tex : register(t0);
SamplerState g_Sampler : register(s0);

RWTexture2D<float4> g_Output : register(u0);

[numthreads(32, 32, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	for (uint i = 0;i < 8; ++i) {
		for (uint j = 0; j < 8; ++j) {
		//0.2126 R + 0.7152 G + 0.0722 B
			float3 color = g_Tex.SampleLevel(g_Sampler, float2((DTid.x * 8 + i) / 256.0, (DTid.y * 8 + j) / 256.0), 0);
			float lum = color.x * 0.2126 + color.y * 0.7152 + color.z * 0.0722;
			g_Output[uint2(DTid.x * 8 + i, DTid.y * 8 + j)] = float4(lum, lum, lum, 1);
		}
	}
}