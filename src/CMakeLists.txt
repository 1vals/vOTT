# option(BUILD_VST3 "Build vOTT as a VST3 plugin"  ON)
# option(BUILD_AU "Build vOTT as an AudioUnit" ON)
# option(BUILD_STANDALONE "Build vOTT as a standalone app" ON)

# if(${BUILD_VST3})
#     list(APPEND JUCE_PLUGIN_FORMATS VST3)
# endif()
# if(${BUILD_AU})
#     list(APPEND JUCE_PLUGIN_FORMATS AU)
# endif()
# if(${BUILD_STANDALONE})
#     list(APPEND JUCE_PLUGIN_FORMATS Standalone)
# endif()

# message(STATUS "Building vOTT with the following plugin formats: ${JUCE_PLUGIN_FORMATS}" )

add_subdirectory(common)
add_subdirectory(plugin)