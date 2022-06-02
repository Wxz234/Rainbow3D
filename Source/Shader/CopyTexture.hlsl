Texture2D g_Tex : register(t0);
SamplerState g_Sampler : register(s0);
RWTexture2D<float4> g_Output : register(u0);

cbuffer texture_message : register(b0)
{
	uint width;
	uint height;
	uint w;
	uint h;
}


[numthreads(8, 8, 1)]
void main(uint2 DTid : SV_DispatchThreadID)
{
	//g_Output[DTid.xy] = g_Tex.SampleLevel(g_Sampler, float2(DTid.x / ((float)w), DTid.y / ((float)h)), 0);
	g_Output[DTid.xy] = float4(1, 1, 1, 1);
}