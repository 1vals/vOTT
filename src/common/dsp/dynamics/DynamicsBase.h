#pragma once

#include <juce_dsp/juce_dsp.h>
#include "Envelope.h"
#include "../utils/ValueSmoother.h"

namespace vOTT {

namespace Dynamics {
    struct ParamPtrs {
        juce::AudioParameterFloat* attack { nullptr };
        juce::AudioParameterFloat* release { nullptr };
        juce::AudioParameterFloat* threshold { nullptr };
        juce::AudioParameterFloat* ratio { nullptr };
    };
}

class DynamicsBase {
public:
    DynamicsBase() = default;
    virtual ~DynamicsBase() = default;

    void forceUpdateAllParams(const Dynamics::ParamPtrs& params) {
        attack = params.attack->get();
        release = params.release->get();
        envelope.setAttackTime(attack);
        envelope.setReleaseTime(release);

        ratio = params.ratio->get();
        thresholdDb = params.threshold->get();
        smoothedRatio.reset(ratio);
        smoothedThreshold.reset(thresholdDb);
    }

    void updateParams(float threshold_, float ratio_, float attack_, float release_) {
        if (attack != attack_) {
            attack = attack_;
            envelope.setAttackTime(attack);
        }

        if (release != release_) {
            release = release_;
            envelope.setReleaseTime(release);
        }

        if (thresholdDb != threshold_) {
            thresholdDb = threshold_;
            smoothedThreshold.setTarget(juce::Decibels::decibelsToGain(threshold_, -100.f));
        }

        if (ratio != ratio_) {
            ratio = ratio_;
            smoothedRatio.setTarget(ratio_);
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
        if (thresholdDb == newThreshold)
            return;
        thresholdDb = newThreshold;
        smoothedThreshold.setTarget(juce::Decibels::decibelsToGain(thresholdDb, -100.f));
    }
    virtual void setRatio(float newRatio) {
        if (ratio == newRatio)
            return;
        ratio = newRatio;
        smoothedRatio.setTarget(newRatio);
    }

    void prepare(int sr) {
        sampleRate = sr;

        smoothedThreshold.prepare(sr);
        smoothedThreshold.reset(juce::Decibels::decibelsToGain(thresholdDb, -100.f));

        smoothedRatio.prepare(sr);
        smoothedRatio.reset(ratio);

        envelope.prepare(sr);
        envelope.setAttackTime(attack);
        envelope.setReleaseTime(release);

        envelope.reset();
    }

    virtual void process(const juce::dsp::ProcessContextReplacing<float>& context) = 0;
    virtual float processSample(int channel, float inputValue, float threshold_, float ratio_) = 0;

protected:
    Envelope envelope;

    // UI parameters
    float attack;
    float release;
    float thresholdDb;
    float ratio;
    int sampleRate = 44100;

    Utils::ValueSmoother smoothedRatio = Utils::ValueSmoother(5.f);
    Utils::ValueSmoother smoothedThreshold = Utils::ValueSmoother(5.f);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DynamicsBase)
};
} // vOTT