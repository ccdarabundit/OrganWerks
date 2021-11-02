cmake_minimum_required(VERSION 3.15)
project(WaveOrgan VERSION 0.9)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-std=c++0x")

# Download CPM if we don't have it

set(CPM_DOWNLOAD_VERSION 0.32.0)

if(CPM_SOURCE_CACHE)
  set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
elseif(DEFINED ENV{CPM_SOURCE_CACHE})
  set(CPM_DOWNLOAD_LOCATION "$ENV{CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
else()
  set(CPM_DOWNLOAD_LOCATION "CMake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
endif()

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
  message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
  file(DOWNLOAD
       https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
       ${CPM_DOWNLOAD_LOCATION}
  )
endif()

include(${CPM_DOWNLOAD_LOCATION})


set_property(GLOBAL PROPERTY USE_FOLDERS YES)
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)

CPMAddPackage(
        NAME JUCE
        GITHUB_REPOSITORY juce-framework/JUCE
        GIT_TAG origin/develop
        SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/sdk/JUCE)

CPMAddPackage(
        NAME FoleysGUIMagic
        GITHUB_REPOSITORY ffAudio/foleys_gui_magic
        GIT_TAG origin/develop
        SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/sdk/foleys_gui_magic)

CPMAddPackage(
        NAME Faust
        GITHUB_REPOSITORY grame-cncm/faust
        GIT_TAG origin/master-dev
        SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/sdk/faust)


add_subdirectory(sdk)

juce_add_modules(sdk/foleys_gui_magic)

add_subdirectory(src)
add_subdirectory(res)




