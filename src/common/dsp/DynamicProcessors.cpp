#include "DynamicProcessors.h"

namespace vOTT {

float Compressor::processSample(int channel, float inputValue) {
    auto env = envelope.processSample(channel, inputValue);

    auto gain = (env < thresholdGain) ? 1.f
                                           : std::pow(env * (1 / thresholdGain), (1 / ratio) - 1.f);

    return gain * inputValue;
}

void Compressor::process(const juce::dsp::ProcessContextReplacing<float>& context) {
    const auto& inputBlock = context.getInputBlock();
    auto& outputBlock = context.getOutputBlock();
    const auto numChannels = outputBlock.getNumChannels();
    const auto numSamples = outputBlock.getNumSamples();

    if (context.isBypassed) {
        outputBlock.copyFrom(inputBlock);
        return;
    }

    for (size_t i = 0; i < numSamples; ++i) {
        // update ratio / threshold here, so that the smoothing can happen during the process
        if (smoothedRatio.isSmoothing()) {
            ratio = smoothedRatio.getNextValue();
        }
        if (smoothedThreshold.isSmoothing()) {
            thresholdDb = smoothedThreshold.getNextValue();
            // thresholdGain = juce::Decibels::gainToDecibels(thresholdDb, -100.f);
        }

        for (size_t channel = 0; channel < numChannels; ++channel) {
            auto inputSamples = inputBlock.getChannelPointer(channel);
            auto outputSamples = outputBlock.getChannelPointer(channel);

            outputSamples[i] = processSample((int)channel, inputSamples[i]);
        }
    }
}
} // vOTT