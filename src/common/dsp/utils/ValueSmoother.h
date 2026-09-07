#pragma once

#include "Utils.h"
#include "juce_core/juce_core.h"

namespace vOTT::Utils {

class ValueSmoother {
public:
    ValueSmoother() = default;

    void prepare(int sr) {
        sampleRate = sr;
        coeff = calculateConstant(timeMs, sampleRate);
    }

    void setTime(float ms) {
        timeMs = ms;
        coeff = calculateConstant(timeMs, sampleRate);
    }

    void reset(float initValue) {
        current = initValue;
        target = initValue;
    }

    void setTarget(float target_) {
        target = target_;
    }

    float getNextValue() {
        current = target + coeff * (current - target);
        return current;
    }

    [[nodiscard]] bool isSmoothing() const {
        return std::abs(current - target) > 0;
    }

private:
    float timeMs = 30.f;
    float coeff = 0.f;
    float current = 0.f, target = 0.f;
    int sampleRate = 44100;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueSmoother)
};
}
