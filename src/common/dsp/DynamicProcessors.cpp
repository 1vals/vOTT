#include "DynamicProcessors.h"

namespace vOTT {

float Compressor::processSample(int channel, float inputValue, float threshold_, float ratio_) {
    auto env = envelope.processSample(channel, inputValue);

    auto gain = (env < threshold_) ? 1.f
                                           : std::pow(env * (1 / threshold_), (1 / ratio_) - 1.f);

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

    float threshold_ = smoothedThreshold.getCurrentValue();
    float ratio_ = smoothedRatio.getCurrentValue();

    for (size_t i = 0; i < numSamples; ++i) {
        if (smoothedRatio.isSmoothing())
            ratio_ = smoothedRatio.getNextValue();

        if (smoothedThreshold.isSmoothing()) {
            threshold_ = smoothedThreshold.getNextValue();
        }

        for (size_t channel = 0; channel < numChannels; ++channel) {
            auto inputSamples = inputBlock.getChannelPointer(channel);
            auto outputSamples = outputBlock.getChannelPointer(channel);
            outputSamples[i] = processSample((int)channel, inputSamples[i], threshold_, ratio_);
        }
    }
}
} // vOTT