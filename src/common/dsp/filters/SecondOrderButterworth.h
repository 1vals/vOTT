#pragma once

#include <juce_dsp/juce_dsp.h>

namespace vOTT::Filters {

enum FilterType {
    lowpass,
    highpass
};

struct SecondOrderButterworth {
    SecondOrderButterworth() = default;
    SecondOrderButterworth(FilterType type) {
        filterType = type;
        // probably dont call calculate coefficients since its not prepared yet
    }

    void setFilterType(FilterType newType) {
        filterType = newType;
        updateCoefficients();
    }

    void setFilterFrequency(float newFreq) {
        centerFreq = newFreq;
        updateCoefficients();
    }

    void prepare(int sr, int numChannels) {
        sampleRate = sr;
        state.assign(numChannels, ChannelState{});
        updateCoefficients();
    }

    void process(const juce::dsp::ProcessContextReplacing<float>& context) {
        const auto& inputBlock = context.getInputBlock();
        auto& outputBlock = context.getOutputBlock();

        if (context.isBypassed) {
            outputBlock.copyFrom(inputBlock);
            return;
        }

        for (size_t channel = 0; channel < outputBlock.getNumChannels(); ++channel) {
            auto inputSamples = inputBlock.getChannelPointer(channel);
            auto outputSamples = outputBlock.getChannelPointer(channel);

            for (size_t i = 0; i < outputBlock.getNumSamples(); ++i)
                outputSamples[i] = processSample((int) channel, inputSamples[i]);
        }
    }

    float processSample(int channel, float inputValue) {
        auto& s = state[static_cast<size_t>(channel)];

        // direct form 1
        float y0 = b0 * inputValue + b1 * s.x1 + b2 * s.x2
                   - a1 * s.y1 - a2 * s.y2;

        // move the state for next sample process
        s.x2 = s.x1;
        s.x1 = inputValue;
        s.y2 = s.y1;
        s.y1 = y0;

        return y0;
    }

    void reset() {
        // todo: make sure this is called whenever audio is scrubbed, samplerate changes, etc
        for (auto& s : state)
            s.reset();
    }
private:
    // todo: create some way of calculating filter coefficients on the UI thread

    struct ChannelState {
        float x1 = 0.f, x2 = 0.f;
        float y1 = 0.f, y2 = 0.f;

        void reset() {
            x1 = 0.f;
            x2 = 0.f;
            y1 = 0.f;
            y2 = 0.f;
        }
    };

    void updateCoefficients() {
        omega0 = (juce::MathConstants<float>::twoPi * centerFreq) / (float)sampleRate;
        alpha = (std::sin(omega0)) / (2.f * Q);

        switch (filterType) {
        case lowpass:
            calculateLPCoeffs();
            break;
        case highpass:
            calculateHPCoeffs();
            break;
        }
    }

    void calculateLPCoeffs() {
        float cosW0 = std::cos(omega0);

        float rawB1 = 1 - cosW0;
        float rawB0 = rawB1 / 2.f;
        float rawB2 = rawB0;

        float rawA0 = 1.f + alpha;
        float rawA1 = -2.f * std::cos(omega0);
        float rawA2 = 1.f - alpha;

        normalize(rawB0, rawB1, rawB2, rawA0, rawA1, rawA2);
    }

    void calculateHPCoeffs() {
        float cosW0 = std::cos(omega0);

        float rawB0 = (1 + cosW0) / 2;
        float rawB1 = 0 - (1 + cosW0);
        float rawB2 = rawB0;

        float rawA0 = 1 + alpha;
        float rawA1 = -2 * cosW0;
        float rawA2 = 1 - alpha;

        normalize(rawB0, rawB1, rawB2, rawA0, rawA1, rawA2);
    }

    void normalize(float rb0, float rb1, float rb2, float ra0, float ra1, float ra2) {
        // normalize a0 to be 1, and "merge" it into the other coefficients
        b0 = rb0 / ra0;
        b1 = rb1 / ra0;
        b2 = rb2 / ra0;
        a1 = ra1 / ra0;
        a2 = ra2 / ra0;
    }

    float b0, b1, b2;
    float a1, a2; // a0 is discarded after normalization

    float centerFreq;
    float omega0, alpha;
    float Q = 1 / std::sqrt(2.f); // can make this a constant
    int sampleRate = 44100;

    FilterType filterType = lowpass;
    std::vector<ChannelState> state;
};

} // vOTT::Filters