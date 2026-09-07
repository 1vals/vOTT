cmake_minimum_required(VERSION 3.22)

project(vOTT VERSION 0.0.1)

juce_add_plugin(V_OTT
    VERSION 0.0.1
    COMPANY_NAME Val(x86)
    PLUGIN_MANUFACTURER_CODE Vx86
    PRODUCT_NAME "vOTT"
    PLUGIN_CODE v0TT

    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    NEEDS_WEB_BROWSER FALSE
    NEEDS_CURL FALSE
    FORMATS AU VST3 Standalone
#    FORMATS ${JUCE_PLUGIN_FORMATS}
)

target_sources(V_OTT
    PRIVATE
        PluginEditor.cpp
        PluginEditor.h
        PluginProcessor.cpp
        PluginProcessor.h
        PluginParameters.h)

target_compile_definitions(V_OTT
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(V_OTT
    PRIVATE
        common
        dsp
        juce::juce_dsp
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
