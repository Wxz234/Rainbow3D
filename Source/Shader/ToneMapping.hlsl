RWTexture2D<float> g_Output : register(u0);
Texture2D g_Input : register(t0);

cbuffer texture_message : register(b0)
{
	uint4 _para;
}

//groupshared uint gs_pyramid[HISTOGRAM_REDUCTION_BINS];

half Luminance(half3 linearRgb)
{
	return dot(linearRgb, float3(0.2126729, 0.7151522, 0.0721750));
}

half Luminance(half4 linearRgba)
{
	return Luminance(linearRgba.rgb);
}

[numthreads(8, 8, 1)]
void main(uint2 DTid : SV_DispatchThreadID)
{
	if (DTid.x < _para.x && DTid.y < _para.y) {
		g_Output[DTid] = Luminance(g_Input[DTid]);
	}
	GroupMemoryBarrierWithGroupSync();

	if (DTid.x == 0 && DTid.y == 0) {

	}
}
