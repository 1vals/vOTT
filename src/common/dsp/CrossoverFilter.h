#pragma once

#include <juce_dsp/juce_dsp.h>

namespace vOTT {

namespace filters {
    struct SecondOrderButterworth {
        SecondOrderButterworth() {
            Q = (std::sqrt(2)) / 2; // leaving this for now, probably needs to change later
        }

        void setFilterFrequency(float newFreq) {
            centerFreq = newFreq;
            updateAlphaAndOmega();
        }

        void prepare(int sr) {
            if (sampleRate != sr) {
                sampleRate = sr;
                updateAlphaAndOmega();
            }
        }

        // void process(const juce::dsp::ProcessContextReplacing<float>& context) {
        //
        // }
        // float processSample(int channel, float inputValue) {
        //
        // }

    private:
        // todo: create some way of calculating filter coefficients on the UI thread

        void updateAlphaAndOmega() {
            omega0 = (juce::MathConstants<float>::twoPi * centerFreq) / (float)sampleRate;
            alpha = (std::sin(omega0)) / 2 * Q;
        }

        float calculateLPCoeffs() {
            /**
             * b0 = (1 - cos(omega0)) / 2
             * b1 = 1 - cos(omega0)
             * b2 = (1 - cos(omega0)) / 2
             * a0 = 1 + alpha
             * a1 = -2 * cos(omega0)
             * a2 = 1 - alpha
             */

            // technically could be more optimized (?) by calculating cos(omega0) and storing it
            // i calculate b1 first since b0 and b2 are equal to b1 / 2
            b1 = 1 - std::cos(omega0);
            b0 = b1 / 2;
            b2 = b0;

            a0 = 1 + alpha;
            a1 = -2 * std::cos(omega0);
            a2 = 1 - alpha;
        }
        float calculateHPCoeffs();

        float b0, b1, b2;
        float a0, a1, a2;

        float centerFreq;
        float omega0, alpha, Q;
        int sampleRate = 44100;
    };
} // filters

//==============================================================================
class CrossoverFilter {
public:
    CrossoverFilter() = default;

    void setFilterFrequency(float newFreq) {
        freq = newFreq;
    }

    void prepare(int sr) {
        sampleRate = sr;
    }
private:
    float freq;
    int sampleRate = 44100;
};

} // vOTT