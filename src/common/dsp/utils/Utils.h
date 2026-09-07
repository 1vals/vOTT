#pragma once

#include <cmath>

namespace vOTT::Utils {

// this overload merges the calculation of the exponential factor & the constant
[[nodiscard]] inline float calculateConstant(float ms, int sampleRate) noexcept {
    /*float expFactor = -1000.f / (float) sampleRate;
    return ms < 1.0e-3f ? 0 : std::exp(expFactor / ms);*/
    return std::exp(-1000.f / (ms * (float)sampleRate));
}

// this overload takes a pre-calculated exponential factor
// if the time in ms is less than 0.001, it simply returns zero
[[nodiscard]] inline float calculateConstant(float ms, float expFactor) noexcept {
    return ms < static_cast<float>(1.0e-3) ? 0
                                           : (std::exp(expFactor / ms));
}
}