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
    DynamicProcessor();

    void setAttack(float newAttack) {
        attack = newAttack;
        update();
    }
    void setRelease(float newRelease) {
        release = newRelease;
        update();
    }
    void setThreshold(float newThreshold) {
        thresholdDb = newThreshold;
        update();
    }
    virtual void setRatio(float newRatio) {
        ratio = newRatio;
        update(); // does this even need to call this
    }

    void prepare(int sr) {
        sampleRate = sr;
        envelope.prepare(sr);
        update();
        envelope.reset();
    }

    virtual void process(const juce::dsp::ProcessContextReplacing<float>& context) = 0;
    virtual float processSample(int channel, float inputValue) = 0;

protected:
    virtual void update() {
        thresholdGain = juce::Decibels::decibelsToGain(thresholdDb, -100.f);

        envelope.setAttackTime(attack);
        envelope.setReleaseTime(release);
    }

    Envelope envelope;

    // UI parameters
    float attack;
    float release;
    float thresholdDb;
    float ratio;

    // the threshold used in dsp math
    float thresholdGain;

    // audio environment specs
    int sampleRate = 44100;

    JUCE_LEAK_DETECTOR(DynamicProcessor)
};

class Compressor : public DynamicProcessor {
public:
    ~Compressor() = default;

    void process(const juce::dsp::ProcessContextReplacing<float>& context) override;
    float processSample(int channel, float inputValue) override;
};
} // vOTT