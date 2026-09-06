#pragma once

#include <juce_dsp/juce_dsp.h>
#include "SecondOrderButterworth.h"

namespace vOTT::Filters {

struct LinkwitzRileyCrossover {
    void setFilterType(Filters::SecondOrderButterworth::FilterType type) {
        stage1.setFilterType(type);
        stage2.setFilterType(type);
    }

    void setFilterFrequency(float newFreq) {
        if (newFreq == freq)
            return;

        freq = newFreq;
        stage1.setFilterFrequency(newFreq);
        stage2.setFilterFrequency(newFreq);
    }

    void prepare(int sampleRate, int numChannels) {
        stage1.prepare(sampleRate, numChannels);
        stage2.prepare(sampleRate, numChannels);
    }

    void process(const juce::dsp::ProcessContextReplacing<float>& context) {
        DBG("xover process running");
        stage1.process(context);
        stage2.process(context);
        DBG("xover process complete");
    }
private:
    SecondOrderButterworth stage1, stage2;
    float freq;
};

} // vOTT::Filters