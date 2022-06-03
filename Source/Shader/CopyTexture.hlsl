Texture2D g_Tex : register(t0);
SamplerState g_Sampler : register(s0);
RWTexture2D<float4> g_Output : register(u0);

cbuffer texture_message : register(b0)
{
	uint4 _para;
}

[numthreads(8, 8, 1)]
void main(uint2 DTid : SV_DispatchThreadID)
{
	if (DTid.x < _para.x && DTid.y < _para.y) {
		g_Output[DTid] = g_Tex.SampleLevel(g_Sampler, float2(DTid.x / (float)_para.x, DTid.y / (float)_para.y), 0);
	}
}