#include "Envelope.h"

namespace vOTT {
void Envelope::prepare(int sr) {
    DBG("envelope.prepare ran");
    sampleRate = sr;
    expFactor = -1000.f / (float) sampleRate;
}

float Envelope::calculateConstant(float ms) const noexcept {
    // lol this could run on the audio thread and be better
    return ms < static_cast<float>(1.0e-3) ? 0
                                           : static_cast<float>(std::exp(expFactor / ms));
}

void Envelope::process(const juce::dsp::ProcessContextReplacing<float>& context) {
    const auto& inputBlock = context.getInputBlock();
    auto& outputBlock = context.getOutputBlock();
    const auto numChannels = outputBlock.getNumChannels();
    const auto numSamples = outputBlock.getNumSamples();

    if (context.isBypassed) {
        outputBlock.copyFrom(inputBlock);
        return;
    }

    for (size_t channel = 0; channel < numChannels; ++channel) {
        auto* inputSamples = inputBlock.getChannelPointer(channel);
        auto* outputSamples = outputBlock.getChannelPointer(channel);

        for (size_t i = 0; i < numSamples; ++i)
            outputSamples[i] = processSample((int) channel, inputSamples[i]);
    }
}

float Envelope::processSample(int channel, float inputValue) {
    // rms calculation
    inputValue *= inputValue;

    // when the value is above the old value it applies the attack constant,
    // otherwise the release constant is applied
    float cte = (inputValue > y[(size_t) channel] ? constantAtk : constantRls);

    // add the input value to the constant, then multiply it by the difference between the current
    // and old values
    float result = inputValue + cte * (y[(size_t) channel] - inputValue);
    y[(size_t) channel] = result;

    // sqrt it because this is doing rms processing
    return std::sqrt(result);
}
} // vOTT