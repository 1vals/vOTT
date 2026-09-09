#pragma once

#include <dsp/dsp.h>
/**
 * roadmap:
 * get Linkwitz-Riley filter to work in processblock
 * refactor processblock to use bandstrip
 * add input gain
 */
namespace vOTT {

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
    void updateCompressorParams(const Dynamics::ParamPtrs& params) {
        compressor.updateParams(params);
    }
    void updateUpwardsCompParams(const Dynamics::ParamPtrs& params) {
        upwardsCompressor.updateParams(params);
    }
    void forceUpdateCompParams(const Dynamics::ParamPtrs& compParams,
                              const Dynamics::ParamPtrs& upwdCompParams //,
                              /*const float& frequency*/) {
        compressor.forceUpdateAllParams(compParams);
        upwardsCompressor.forceUpdateAllParams(upwdCompParams);
    }

    void process(const juce::dsp::ProcessContextReplacing<float>& context);
private:
    BandType bandType;
    std::vector<Filters::SecondOrderButterworth> filters;


    Compressor compressor;
    UpwardsCompressor upwardsCompressor;
};

}