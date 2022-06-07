#define EPSILON 1.0e-4


// Don't forget to update 'LogHistogram.cs' if you change these values !
#define HISTOGRAM_BINS          128
#define HISTOGRAM_TEXELS        HISTOGRAM_BINS / 4
#define HISTOGRAM_THREAD_X              16
#define HISTOGRAM_THREAD_Y              16
#define HISTOGRAM_REDUCTION_THREAD_X    HISTOGRAM_THREAD_X
#define HISTOGRAM_REDUCTION_THREAD_Y    HISTOGRAM_BINS / HISTOGRAM_THREAD_Y
#define HISTOGRAM_REDUCTION_ALT_PATH    0

#define HISTOGRAM_REDUCTION_BINS HISTOGRAM_REDUCTION_THREAD_X * HISTOGRAM_REDUCTION_THREAD_Y

StructuredBuffer<uint> _HistogramBuffer : register(t0);
groupshared uint gs_pyramid[HISTOGRAM_REDUCTION_BINS];
RWTexture2D<float> _Destination : register(u0);
float GetExposureMultiplier(float avgLuminance)
{
    avgLuminance = max(EPSILON, avgLuminance);
    //float keyValue = 1.03 - (2.0 / (2.0 + log2(avgLuminance + 1.0)));
    float keyValue = 1.0f;
    float exposure = keyValue / avgLuminance;
    return exposure;
}

float GetBinValue(StructuredBuffer<uint> buffer, uint index, float maxHistogramValue)
{
    return float(buffer[index]) * maxHistogramValue;
}

float GetLuminanceFromHistogramBin(float bin, float2 scaleOffset)
{
    return exp2((bin - scaleOffset.y) / scaleOffset.x);
}

void FilterLuminance(StructuredBuffer<uint> buffer, uint i, float maxHistogramValue, float2 scaleOffset, inout float4 filter)
{
    float binValue = GetBinValue(buffer, i, maxHistogramValue);

    // Filter dark areas
    float offset = min(filter.z, binValue);
    binValue -= offset;
    filter.zw -= offset.xx;

    // Filter highlights
    binValue = min(filter.w, binValue);
    filter.w -= binValue;

    // Luminance at the bin
    float luminance = GetLuminanceFromHistogramBin(float(i) / float(HISTOGRAM_BINS), scaleOffset);

    filter.xy += float2(luminance * binValue, binValue);
}


float GetAverageLuminance(StructuredBuffer<uint> buffer, float4 params, float maxHistogramValue, float2 scaleOffset)
{
    // Sum of all bins
    uint i;
    float totalSum = 0.0;

    [unroll]
    for (i = 0; i < HISTOGRAM_BINS; i++)
        totalSum += GetBinValue(buffer, i, maxHistogramValue);

    // Skip darker and lighter parts of the histogram to stabilize the auto exposure
    // x: filtered sum
    // y: accumulator
    // zw: fractions
    float4 filter = float4(0.0, 0.0, totalSum * params.xy);

    [unroll]
    for (i = 0; i < HISTOGRAM_BINS; i++)
        FilterLuminance(buffer, i, maxHistogramValue, scaleOffset, filter);

    // Clamp to user brightness range
    return clamp(filter.x / max(filter.y, EPSILON), params.z, params.w);
}


[numthreads(HISTOGRAM_REDUCTION_THREAD_X, HISTOGRAM_REDUCTION_THREAD_Y, 1)]
void main(uint2 groupThreadId : SV_GroupThreadID)
{
    const uint thread_id = groupThreadId.y * HISTOGRAM_REDUCTION_THREAD_X + groupThreadId.x;
    gs_pyramid[thread_id] = _HistogramBuffer[thread_id];

    GroupMemoryBarrierWithGroupSync();

    [unroll]
    for (uint i = HISTOGRAM_REDUCTION_BINS >> 1u; i > 0u; i >>= 1u)
    {
        if (thread_id < i)
            gs_pyramid[thread_id] = max(gs_pyramid[thread_id], gs_pyramid[thread_id + i]);

        GroupMemoryBarrierWithGroupSync();
    }

    GroupMemoryBarrierWithGroupSync();

    if (thread_id == 0u)
    {
        float maxValue = 1.0 / float(gs_pyramid[0]);

        const float4 _Params1 = float4(0.5, 0.95, 1.0, 1.0);
        const float2 _ScaleOffsetRes = float2(1.0 / 18.0, 0.5);
        float avgLuminance = GetAverageLuminance(_HistogramBuffer, _Params1, maxValue, _ScaleOffsetRes.xy);
        float exposure = GetExposureMultiplier(avgLuminance);
        _Destination[uint2(0u, 0u)].x = exposure.x;

    }
}
