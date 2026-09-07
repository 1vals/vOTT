add_library(dsp STATIC
        dsp.h

        dynamics/Compressor.h
        dynamics/DynamicsBase.h
        dynamics/Envelope.h
        dynamics/Envelope.cpp
        dynamics/UpwardsCompressor.h

        filters/SecondOrderButterworth.h

        utils/Utils.h
        utils/ValueSmoother.h
)

target_link_libraries(dsp PRIVATE
        juce::juce_dsp
        juce::juce_audio_utils
)

target_include_directories(dsp PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)