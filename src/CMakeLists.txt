project(WaveOrgan VERSION 0.0.1)

# Set your plugin variables here
set(COMPANY_NAME "Champion Sound Systems")
set (PRODUCT_NAME "Wave Organ")

# Target for the plugin
set (BASE_TARGET "${PROJECT_NAME}" CACHE INTERNAL "")

##############################
# INITIALIZE JUCE MODULES
##############################

add_library(juce_plugin_modules STATIC)
# Add juce modules
target_link_libraries(juce_plugin_modules
    PRIVATE
        BinaryData # Don't add me unless you have binary data!
        juce::juce_audio_utils
        foleys_gui_magic
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
)

# Plugin compile settings like you would in Projucer
target_compile_definitions(juce_plugin_modules
    PUBLIC
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_REPORT_APP_USAGE=0
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_JACK=1
        JUCE_ALSA=1
        JucePlugin_Manufacturer="${COMPANY_NAME}"
        JucePlugin_VersionString="${CMAKE_PROJECT_VERSION}"
        JucePlugin_Name="${PRODUCT_NAME}"
        FOLEYS_SHOW_GUI_EDITOR_PALLETTE=1
        FOLEYS_ENABLE_BINARY_DATA=1
    INTERFACE
        $<TARGET_PROPERTY:juce_plugin_modules,COMPILE_DEFINITIONS>
)

target_include_directories(juce_plugin_modules
    INTERFACE
        $<TARGET_PROPERTY:juce_plugin_modules,INCLUDE_DIRECTORIES>
)

set_target_properties(juce_plugin_modules PROPERTIES
    POSITION_INDEPENDENT_CODE TRUE
    VISIBILITY_INLINES_HIDDEN TRUE
    C_VISBILITY_PRESET hidden
    CXX_VISIBILITY_PRESET hidden
)

##############################
# INITIALIZE PLUGIN
##############################

juce_add_plugin(${BASE_TARGET}
        COMPANY_NAME "${COMPANY_NAME}"
        IS_SYNTH TRUE
        NEEDS_MIDI_INPUT TRUE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE Chss
        PLUGIN_CODE WvOg
        FORMATS AU VST3 Standalone
        PRODUCT_NAME "${PRODUCT_NAME}"
)

juce_generate_juce_header(${BASE_TARGET})

target_compile_definitions(${BASE_TARGET} PUBLIC
    JUCE_VST3_CAN_REPLACE_VST2=0
)

# Attach the juce modules to the plugin
target_link_libraries(${BASE_TARGET} PRIVATE juce_plugin_modules)

##############################
# SOURCE FILES
##############################

target_sources(${BASE_TARGET} PRIVATE
    # JUCE FILES
    WaveOrgan.cpp
    
    # DSP Files
    dsp/declicker.h 
    dsp/OrganSound.h
    dsp/OrganVoice.h
    dsp/lossyPipeCoefs.cpp

    # GUI
    gui/organLAF.cpp
    # Generated Faust Files
    dsp/faust_src/gen/flueEngine.h
)
target_precompile_headers(${BASE_TARGET} PRIVATE pch.h)
# Add in Faust directory
add_subdirectory(dsp)
