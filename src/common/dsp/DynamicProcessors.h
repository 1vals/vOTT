#pragma once

#include <juce_dsp/juce_dsp.h>
#include "Envelope.h"

/**
 * im debating whether to create these as two independent classes or have them both derive from a base class.
 * since theyre both so similar, but need to process in serial, creating an abstract base class seems better
 *
 * planning:
 *
 * what we need:
 * functions that update each parameter
 * a function which processes audio
 *
 */
namespace vOTT {

class DynamicProcessor {
public:
    DynamicProcessor() = default;

    void setAttack(float newAttack) {
        attack = newAttack;
        envelope.setAttackTime(attack);
    }
    void setRelease(float newRelease) {
        release = newRelease;
        envelope.setReleaseTime(release);
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