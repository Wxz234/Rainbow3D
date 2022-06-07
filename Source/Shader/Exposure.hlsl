#define NUM_HISTOGRAM_BINS 256
#define HISTOGRAM_AVERAGE_THREADS_PER_DIMENSION 16
RWByteAddressBuffer LuminanceHistogram : register(u0);
RWBuffer<float> LuminanceOutput : register(u1);

cbuffer LuminanceHistogramAverageBuffer : register(b0)
{
    uint inputWidth;
    uint inputHeight;
    //float minLogLuminance;
    //float logLuminanceRange;
    float timeDelta;
    uint firstframe;
};

groupshared float HistogramShared[NUM_HISTOGRAM_BINS];

[numthreads(HISTOGRAM_AVERAGE_THREADS_PER_DIMENSION, HISTOGRAM_AVERAGE_THREADS_PER_DIMENSION, 1)]
void main(uint groupIndex : SV_GroupIndex)
{
    float countForThisBin = (float)LuminanceHistogram.Load(groupIndex * 4);
    HistogramShared[groupIndex] = countForThisBin * (float)groupIndex;

    GroupMemoryBarrierWithGroupSync();

    [unroll]
    for (uint histogramSampleIndex = (NUM_HISTOGRAM_BINS >> 1); histogramSampleIndex > 0; histogramSampleIndex >>= 1)
    {
        if (groupIndex < histogramSampleIndex)
        {
            HistogramShared[groupIndex] += HistogramShared[groupIndex + histogramSampleIndex];
        }

        GroupMemoryBarrierWithGroupSync();
    }
    if (firstframe) {
        LuminanceOutput[0] = 0.0f;
    }
  
    if (groupIndex == 0)
    {
        const float tau = 1.1;
        const float logLuminanceRange = 12.0;
        const float minLogLuminance = -10.0;
        uint pixelCount = inputWidth * inputHeight;

        float weightedLogAverage = (HistogramShared[0].x / max((float)pixelCount - countForThisBin, 1.0)) - 1.0;
        float weightedAverageLuminance = exp2(((weightedLogAverage / 254.0) * logLuminanceRange) + minLogLuminance);
        float luminanceLastFrame = LuminanceOutput[0];
        float adaptedLuminance = luminanceLastFrame + (weightedAverageLuminance - luminanceLastFrame) * (1 - exp(-timeDelta * tau));
        LuminanceOutput[0] = adaptedLuminance;
    }
}
