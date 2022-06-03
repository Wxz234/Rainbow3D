Texture2D g_Tex : register(t0);
SamplerState g_Sampler : register(s0);
RWTexture2D<float4> g_Output : register(u0);

cbuffer texture_message : register(b0)
{
	uint4 _para;
}


[numthreads(32, 32, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	g_Output[DTid.xy] = g_Tex.SampleLevel(g_Sampler, float2((float(DTid.x)) / ((float)_para.z), (float(DTid.y)) / ((float)_para.w)), 0);
	//g_Output[DTid.xy] = float4(1, 1, 1, 1);
}