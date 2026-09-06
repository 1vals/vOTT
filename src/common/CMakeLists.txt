add_library(common STATIC
    Parameters.h
    BandStrip.cpp
    BandStrip.h

    dsp/DynamicProcessors.cpp
    dsp/DynamicProcessors.h
    dsp/Envelope.cpp
    dsp/Envelope.h

    dsp/filters/SecondOrderButterworth.h
    dsp/filters/LinkwitzRileyCrossover.h
)

target_link_libraries(common PRIVATE
    juce::juce_dsp
    juce::juce_audio_utils
)

target_include_directories(common PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)