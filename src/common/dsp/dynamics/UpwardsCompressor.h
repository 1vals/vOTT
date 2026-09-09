#pragma once

#include "DynamicsBase.h"

/**
 * roadmap:
 *
 * figure out how to make the inverted ratio work
 */
namespace vOTT {

class UpwardsCompressor : public DynamicsBase {
public:
    ~UpwardsCompressor() override = default;

    void forceUpdateAllParams(const Dynamics::ParamPtrs& params) override {
        // make attack = release and vice versa to have the parameters resemble their effect on the sound more
        // realistically;
        attack = params.release->get();
        release = params.attack->get();
        envelope.setAttackTime(attack);
        envelope.setReleaseTime(release);

        ratio = params.ratio->get();
        thresholdDb = params.threshold->get();
        smoothedRatio.reset(ratio);
        smoothedThreshold.reset(juce::Decibels::decibelsToGain(thresholdDb, -100.f));
    }

    void setAttack(float newAttack) override {
        if (attack != newAttack) {
            attack = newAttack;
            envelope.setReleaseTime(attack); // set release instead of attack
        }
    }
    void setRelease(float newRelease) override {
        if (release != newRelease) {
            release = newRelease;
            envelope.setAttackTime(release); // set attack instead of release
        }
    }
    void process(const juce::dsp::ProcessContextReplacing<float>& context) override {
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
    float processSample(int channel, float inputValue, float threshold_, float ratio_) override {
        auto env = envelope.processSample(channel, inputValue);

        constexpr float floor = 1.0e-5f;
        env = std::max(env, floor);
        threshold_ = std::max(threshold_, floor);

        // todo: small micro optimization, 1 / threshold can be stored & only calculated when threshold changes
        auto gain = (env > threshold_) ? 1.f
                                            : std::pow(env / threshold_, (1 / ratio_) - 1.f);

        float output = gain * inputValue;
        // DBG("env = " << env << ", threshold = " << threshold_ << ", gain = " << gain <<
        //     ", output value = " << output);

        return gain * inputValue;
    }
};

} // vOTT