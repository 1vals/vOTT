#pragma once

#include <juce_dsp/juce_dsp.h>
#include "Envelope.h"

namespace vOTT {

class DynamicProcessor {
public:
    DynamicProcessor() = default;

    void updateParams(float threshold_, float ratio_, float attack_, float release_) {
        /**
         * literally does all the same stuff as the update functions just in one function
         */
        if (attack != attack_) {
            attack = attack_;
            envelope.setAttackTime(attack);
        }

        if (release != release_) {
            release = release_;
            envelope.setReleaseTime(release);
        }

        // DBG("current threshold in db " << thresholdDb);
        if (thresholdDb != threshold_) {
            thresholdDb = threshold_;
            thresholdGain = juce::Decibels::decibelsToGain(thresholdDb, -100.f);
            smoothedThreshold.setTargetValue(thresholdGain);
        }

        if (ratio != ratio_) {
            ratio = ratio_;
            smoothedRatio.setTargetValue(ratio_);
        }
    }

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
        thresholdDb = newThreshold;
        thresholdGain = juce::Decibels::decibelsToGain(thresholdDb, -100.f);
        smoothedThreshold.setTargetValue(thresholdGain);
    }
    virtual void setRatio(float newRatio) {
        ratio = newRatio;
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

    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedRatio;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> smoothedThreshold;

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