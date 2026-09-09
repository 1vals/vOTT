#include "BandStrip.h"

namespace vOTT {

void BandStrip::prepare(int sampleRate, int numChannels) {
    compressor.prepare(sampleRate);
    upwardsCompressor.prepare(sampleRate);

    // prepare & set filter types for all filters needed by the crossover
    switch (bandType) {
    case low:
        filters.resize(2);
        for (auto& filter : filters) {
            filter.prepare(sampleRate, numChannels);
            filter.setFilterType(Filters::FilterType::lowpass);
        }
        break;
    case mid:
        filters.resize(4);
        for (int x = 0; x < 4; ++x) {
            filters[x].prepare(sampleRate, numChannels);
            filters[x].setFilterType((x < 2) ? Filters::FilterType::lowpass
                : Filters::FilterType::highpass);
        }
        break;
    case high:
        filters.resize(2);
        for (auto& filter : filters) {
            filter.prepare(sampleRate, numChannels);
            filter.setFilterType(Filters::FilterType::highpass);
        }
        break;
    }
}

void BandStrip::process(const juce::dsp::ProcessContextReplacing<float>& context) {
    for (auto& filter : filters)
        filter.process(context);

    upwardsCompressor.process(context);

    compressor.process(context);
}

} // vOTT