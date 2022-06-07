#define HISTOGRAM_BINS          128
#define HISTOGRAM_TEXELS        HISTOGRAM_BINS / 4
#define HISTOGRAM_THREAD_X              16
#define HISTOGRAM_THREAD_Y              16
#define HISTOGRAM_REDUCTION_THREAD_X    HISTOGRAM_THREAD_X
#define HISTOGRAM_REDUCTION_THREAD_Y    HISTOGRAM_BINS / HISTOGRAM_THREAD_Y
#define HISTOGRAM_REDUCTION_ALT_PATH    0

#define HISTOGRAM_REDUCTION_BINS HISTOGRAM_REDUCTION_THREAD_X * HISTOGRAM_REDUCTION_THREAD_Y

RWStructuredBuffer<uint> _HistogramBuffer :register(u0);
Texture2D _Source : register(t0);
SamplerState sampler_LinearClamp: register(s0);
groupshared uint gs_histogram[HISTOGRAM_BINS];

cbuffer Params  : register(b0) {
    float4 _ScaleOffsetRes;
};

float GetHistogramBinFromLuminance(float value, float2 scaleOffset)
{
    return saturate(log2(value) * scaleOffset.x + scaleOffset.y);
}

[numthreads(HISTOGRAM_THREAD_X, HISTOGRAM_THREAD_Y, 1)]
void main(uint2 dispatchThreadId : SV_DispatchThreadID, uint2 groupThreadId : SV_GroupThreadID)
{

    const uint localThreadId = groupThreadId.y * HISTOGRAM_THREAD_X + groupThreadId.x;

    // Clears the shared memory

    if (localThreadId < HISTOGRAM_BINS)
    {
        gs_histogram[localThreadId] = 0u;
    }


    float2 ipos = float2(dispatchThreadId) * 2.0;

    GroupMemoryBarrierWithGroupSync();

    // Gather local group histogram
    if (ipos.x < _ScaleOffsetRes.z && ipos.y < _ScaleOffsetRes.w)
    {
        uint weight = 1u;
        float2 sspos = ipos / _ScaleOffsetRes.zw;

        // Vignette weighting to put more focus on what's in the center of the screen

        {
            float2 d = abs(sspos - (0.5).xx);
            float vfactor = saturate(1.0 - dot(d, d));
            vfactor *= vfactor;
            weight = (uint)(64.0 * vfactor);
        }


        float3 color = _Source.SampleLevel(sampler_LinearClamp, sspos, 0.0).xyz; // Bilinear downsample 2x
        float luminance = dot(color, float3(0.2125, 0.7154, 0.0721));
        float logLuminance = GetHistogramBinFromLuminance(luminance, _ScaleOffsetRes.xy);
        uint idx = (uint)(logLuminance * (HISTOGRAM_BINS - 1u));
        InterlockedAdd(gs_histogram[idx], weight);
    }

    GroupMemoryBarrierWithGroupSync();

    // Merge everything


    if (localThreadId < HISTOGRAM_BINS)
    {
        InterlockedAdd(_HistogramBuffer[localThreadId], gs_histogram[localThreadId]);
    }

}

[numthreads(HISTOGRAM_THREAD_X, 1, 1)]
void KEyeHistogramClear(uint dispatchThreadId : SV_DispatchThreadID)
{
    if (dispatchThreadId < HISTOGRAM_BINS)
        _HistogramBuffer[dispatchThreadId] = 0u;
}