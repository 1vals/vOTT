#pragma once

#include "dsp/DynamicProcessors.h"
#include "dsp/filters/SecondOrderButterworth.h"

/**
 * roadmap:
 * get Linkwitz-Riley filter to work in processblock
 * refactor processblock to use bandstrip
 * add input gain
 */
namespace vOTT {

class BandStrip {
public:
    Bandstrip() = default;

    void prepare(int sampleRate, int numChannels) {
        xover1.prepare(sampleRate, numChannels);
        xover2.prepare(sampleRate, numChannels);
        comp.prepare(sampleRate);
    }

    void setFilterType(Filters::SecondOrderButterworth::FilterType type) {
        xover1.setFilterType(type);
        xover2.setFilterType(type);
    }

    void setFilterFrequency(float newFreq) {
        if (newFreq == freq)
            return;

        freq = newFreq;
        xover1.setFilterFrequency(newFreq);
        xover2.setFilterFrequency(newFreq);
    }

    void process(const juce::dsp::ProcessContextReplacing<float>& context) {
        xover1.process(context);
        xover2.process(context);

        // comp.process(context);
    }
private:
    Filters::SecondOrderButterworth xover1, xover2;
    float freq;

    struct CompExpParams {
        float attack,
        release,
        ratio,
        threshold;
    };
    Compressor comp;
    CompExpParams compParams;
};

}