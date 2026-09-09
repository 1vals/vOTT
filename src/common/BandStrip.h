#pragma once

#include <dsp/dsp.h>
/**
 * roadmap:
 * get Linkwitz-Riley filter to work in processblock
 * refactor processblock to use bandstrip
 * add input gain
 */
namespace vOTT {

namespace State {
    struct BandState {
        CompressorParams compParams;
        CompressorParams upwdCompParams;
        juce::AudioProcessorParameter* frequency;
    };
} // state

    enum BandType {
        low,
        mid,
        high
    };

class BandStrip {
public:
    BandStrip(BandType type) {
        bandType = type;
    }

    void prepare(int sampleRate, int numChannels);

    void setFilterFrequency(float newFreq) {
        for (auto& x : filters)
            x.setFilterFrequency(newFreq);
    }
    void updateCompressorParams(const State::CompressorParams& params) {
        compressor.updateParams(params);
    }
    void updateUpwardsCompParams(const State::CompressorParams& params) {
        upwardsCompressor.updateParams(params);
    }
    void forceUpdateCompParams(const State::CompressorParams& compParams,
                              const State::CompressorParams& upwdCompParams //,
                              /*const float& frequency*/) {
        compressor.forceUpdateAllParams(compParams);
        upwardsCompressor.forceUpdateAllParams(upwdCompParams);
    }

    void updateParamsFromState(const State::BandState& state) {
        for (auto& filter : filters)
            filter.setFilterFrequency(state.frequency);
        upwardsCompressor.updateParams(state.upwdCompParams);
        compressor.updateParams(state.compParams);
    }

    void process(const juce::dsp::ProcessContextReplacing<float>& context);
private:
    BandType bandType;
    std::vector<Filters::SecondOrderButterworth> filters;


    Compressor compressor;
    UpwardsCompressor upwardsCompressor;
};

}