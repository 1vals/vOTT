add_library(common STATIC
        ApvtsWrapper.h

        dsp/dynamics/DynamicsBase.h
        dsp/dynamics/Envelope.cpp
        dsp/dynamics/Envelope.h
        dsp/dynamics/Compressor.h
        dsp/dynamics/Expander.h

        dsp/filters/SecondOrderButterworth.h
        dsp/filters/LinkwitzRileyCrossover.h

        dsp/utils/ValueSmoother.h
        dsp/utils/Utils.h
)

target_link_libraries(common PRIVATE
        juce::juce_dsp
        juce::juce_audio_utils
)

target_include_directories(common PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)