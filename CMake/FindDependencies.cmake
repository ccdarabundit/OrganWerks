# Add in CPM, necessary for submodule management
include(${CMAKE_CURRENT_LIST_DIR}/CPM.cmake)

# Include JUCE and PGM
set(sdkDir ${CMAKE_CURRENT_LIST_DIR}/../sdk/)
CPMAddPackage(
        NAME JUCE
        GITHUB_REPOSITORY juce-framework/JUCE
        GIT_TAG origin/develop
        SOURCE_DIR ${sdkDir}/JUCE)

CPMAddPackage(
        NAME FoleysGUIMagic
        GITHUB_REPOSITORY ffAudio/foleys_gui_magic
        GIT_TAG origin/develop
        SOURCE_DIR ${sdkDir}/foleys_gui_magic)

# Include Faust
CPMAddPackage(
        NAME Faust
        GITHUB_REPOSITORY grame-cncm/faust
        GIT_TAG origin/master
        GIT_SHALLOW TRUE
        SOURCE_DIR ${sdkDir}/faust)

# Options for building faust
option(LIBMICROHTTPD_CFLAGS "Manual C flags for libmicrohttpd" ON)
option(LIBMICROHTTPD_LDFLAGS "Manual link flags for libmicrohttpd" ON)
option(INCLUDE_WASM_GLUE "" OFF)
option(INCLUDE_OSC "" OFF)
option(INCLUDE_HTTP "" OFF)
option(INCLUDE_ITP "" OFF)
option(INCLUDE_EMCC "" OFF)

set(CPP_BACKEND COMPILER STATIC DYNAMIC CACHE STRING "Include CPP backend" FORCE)

# Add the faust build so we can use command line 
add_subdirectory(${Faust_SOURCE_DIR}/build)

# Add in PGM to JUCE
juce_add_modules(${sdkDir}/foleys_gui_magic)