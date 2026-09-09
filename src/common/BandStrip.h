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
        juce::AudioParameterFloat* frequency { nullptr };
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
    void forceUpdateCompParams(const State::BandState& state) {
        compressor.forceUpdateAllParams(state.compParams);
        upwardsCompressor.forceUpdateAllParams(state.upwdCompParams);
    }

    void updateParamsFromState(const State::BandState& state) {
        float freq = state.frequency->get();
        for (auto& filter : filters)
            filter.setFilterFrequency(freq);
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