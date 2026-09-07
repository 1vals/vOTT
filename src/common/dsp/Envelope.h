#pragma once

/**
 * common/dsp/Envelope.h
 * one-pole lowpass smoothing filter for dynamic processing
 *
 * eventually, will have to have it work in both peak & rms mode
 * for now, i think rms probably works best
 */

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "utils/Utils.h"

namespace vOTT {

class Envelope {
public:
    Envelope() {}

    void setAttackTime(float newMs) {
        attackMs = newMs;
        constantAtk = Utils::calculateConstant(newMs, expFactor);
    }
    void setReleaseTime(float newMs) {
        releaseMs = newMs;
        constantRls = Utils::calculateConstant(newMs, expFactor);
    }

    void prepare(int sr);

    void process(const juce::dsp::ProcessContextReplacing<float>& context);
    float processSample(int channel, float inputValue);

    void reset() {
        reset(0.f);
    }
    void reset(float initValue) {
        for (auto& old : y)
            old = initValue;
    }
private:
    [[nodiscard]] float calculateConstant(float ms) const noexcept;

    std::vector<float> y = std::vector<float>(2, 0.f);

    float attackMs, constantAtk;
    float releaseMs, constantRls;

    int sampleRate = 44100;

    // since this is set by envelope.prepare anyways, it should be uninitialized.
    float expFactor/* = -1000.f / (float)sampleRate*/;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Envelope)
};

} // vOTT
