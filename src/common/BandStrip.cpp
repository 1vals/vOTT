#include "BandStrip.h"

namespace vOTT {

void BandStrip::prepare(int sampleRate, int numChannels) {
    compressor.prepare(sampleRate);
    upwardsCompressor.prepare(sampleRate);
    //
    // switch (bandType) {
    //     xovers.resize(2);
    // case high:
    //     for (auto& x : xovers) {
    //         x.prepare(sampleRate, numChannels);
    //         x.setFilterType(Filters::FilterType::highpass);
    //     }
    //     break;
    // case low:
    //     for (auto& x : xovers) {
    //         x.prepare(sampleRate, numChannels);
    //         x.setFilterType(Filters::FilterType::lowpass);
    //     }
    // case mid:
    //     xovers.resize(4);
    //     for (int x = 2; x < 4; ++x) {
    //         xovers[x].prepare(sampleRate, numChannels);
    //         xovers[x].setFilterType(Filters::FilterType::lowpass);
    //     }
    // }
}

void BandStrip::process(const juce::dsp::ProcessContextReplacing<float>& context) {
    /** Roadmap:
     * get compressor to work DONE
     * get upwards comp to work
     * get xover to work
     * test xover in all 3 states
     * create bypass params for each 
     * create new xover instances so that the whole crossover thing can be done
     */

    // for (auto& x : xovers)
    //     x.process(context);

    upwardsCompressor.process(context);

    compressor.process(context);
}

} // vOTT