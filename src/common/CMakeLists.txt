add_library(common STATIC
        ApvtsWrapper.h
)

target_link_libraries(common PRIVATE
        dsp
        juce::juce_dsp
        juce::juce_audio_utils
)

target_include_directories(common PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)