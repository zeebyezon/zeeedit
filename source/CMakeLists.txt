add_library(SharedCode INTERFACE)

# Enable fast math, C++20 and a few other target defaults
include(SharedCodeDefaults)

target_sources(SharedCode INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/AudioStub.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/AudioStub.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/PluginProcessorBase.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/PluginProcessorBase.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/ThreadSafeQueue.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/ZeeEdit.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ZeeEdit.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/GUI/WidgetPanel.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/GUI/WidgetPanel.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/GUI/ZeeEditGui.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/GUI/ZeeEditGui.h"
)

# Adds a BinaryData target for embedding assets into the binary
#include(Assets)

# MacOS only: Cleans up folder and target organization on Xcode.
#include(XcodePrettify)

# This is where you can set preprocessor definitions for JUCE and your plugin
target_compile_definitions(SharedCode
    INTERFACE

    # JUCE_WEB_BROWSER and JUCE_USE_CURL off by default
    JUCE_WEB_BROWSER=0  # If you set this to 1, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
    JUCE_USE_CURL=0     # If you set this to 1, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
    JUCE_VST3_CAN_REPLACE_VST2=0

    JucePlugin_IsMidiEffect=1
    JucePlugin_WantsMidiInput=1
    JucePlugin_ProducesMidiOutput=1

    # Uncomment if you are paying for a an Indie/Pro license or releasing under GPLv3
    # JUCE_DISPLAY_SPLASH_SCREEN=0

    # lets the app known if we're Debug or Release
    CMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}"
    VERSION="${CURRENT_VERSION}"

    # JucePlugin_Name is for some reason doesn't use the nicer PRODUCT_NAME
    PRODUCT_NAME_WITHOUT_VERSION="ZeeEdit-Prophet5"
)

# Link to any other modules you added (with juce_add_module) here!
# Usually JUCE modules must have PRIVATE visibility
# See https://github.com/juce-framework/JUCE/blob/master/docs/CMake%20API.md#juce_add_module
# However, with Pamplejuce, you'll link modules to SharedCode with INTERFACE visibility
# This allows the JUCE plugin targets and the Tests target to link against it
target_link_libraries(SharedCode
    INTERFACE
    #    Assets
    melatonin_inspector
    juce_audio_utils
    juce_audio_processors
    juce_gui_basics
    juce_gui_extra
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
