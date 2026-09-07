#pragma once

#include <juce_dsp/juce_dsp.h>
#include "Envelope.h"

namespace vOTT {

class DynamicProcessor {
public:
    DynamicProcessor() = default;

    void setAttack(float newAttack) {
        if (attack != newAttack) {
            attack = newAttack;
            envelope.setAttackTime(attack);
        }
    }
    void setRelease(float newRelease) {
        if (release != newRelease) {
            release = newRelease;
            envelope.setReleaseTime(release);
        }
    }
    void setThreshold(float newThreshold) {
        smoothedThreshold.setTargetValue(newThreshold);
    }
    virtual void setRatio(float newRatio) {
        smoothedRatio.setTargetValue(newRatio);
    }

    void prepare(int sr) {
        sampleRate = sr;
        thresholdGain = juce::Decibels::decibelsToGain(thresholdDb, -100.f);

        envelope.prepare(sr);
        envelope.setAttackTime(attack);
        envelope.setReleaseTime(release);

        envelope.reset();
    }

    virtual void process(const juce::dsp::ProcessContextReplacing<float>& context) = 0;
    virtual float processSample(int channel, float inputValue) = 0;

protected:
    Envelope envelope;

    // UI parameters
    float attack;
    float release;
    float thresholdDb;
    float ratio;

    juce::SmoothedValue<float> smoothedRatio, smoothedThreshold;

    // the threshold used in dsp math
    float thresholdGain;

    // audio environment specs
    int sampleRate = 44100;

    JUCE_LEAK_DETECTOR(DynamicProcessor)
};

class Compressor : public DynamicProcessor {
public:
    virtual ~Compressor() = default;

    void process(const juce::dsp::ProcessContextReplacing<float>& context) override;
    float processSample(int channel, float inputValue) override;
};
} // vOTT