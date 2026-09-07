#include "BandStrip.h"

namespace vOTT {

void BandStrip::prepare(int sampleRate, int numChannels) {
    compressor.prepare(sampleRate);
    upwardsCompressor.prepare(sampleRate);

    switch (bandType) {
        xovers.resize(2);
    case high:
        for (auto& x : xovers) {
            x.prepare(sampleRate, numChannels);
            x.setFilterType(Filters::FilterType::highpass);
        }
        break;
    case low:
        for (auto& x : xovers) {
            x.prepare(sampleRate, numChannels);
            x.setFilterType(Filters::FilterType::lowpass);
        }
    case mid:
        xovers.resize(4);
        for (int x = 2; x < 4; ++x) {
            xovers[x].prepare(sampleRate, numChannels);
            xovers[x].setFilterType(Filters::FilterType::lowpass);
        }
    }
}

void BandStrip::process(const juce::dsp::ProcessContextReplacing<float>& context) {
    // signalflow:
    // xover 1 -> xover 2 -> (if mid band) xover 3 -> xover 4 -> upwards comp -> compression -> out

    

}

} // vOTT