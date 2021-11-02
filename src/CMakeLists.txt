cmake_minimum_required(VERSION 3.15)

set(TEST_FILE "log.txt")

# add_custom_command does not create a new target. You have to define targets explicitly
# by add_executable, add_library or add_custom_target in order to make them visible to make
add_custom_command(OUTPUT ${TEST_FILE}
    COMMAND touch ${TEST_FILE}

    # Display the given message before the commands are executed at build time
    COMMENT "Creating ${TEST_FILE}"
)

# target zoo is always built
add_custom_target(zoo ALL
    COMMAND echo "This is ALL target 'zoo', and it depends on ${TEST_FILE}"
    # If the file exists, then commands related to that file won't be executed
    # DONOT let other target depends on the same OUTPUT as current target,
    #   or it may be bad when doing parallel make
    DEPENDS ${TEST_FILE}

    # to make quotes printable,for example
    VERBATIM
)


project(WaveOrgan VERSION 0.9)

juce_add_plugin(WaveOrgan
	COMPANY_NAME Champion Sound Systems
	PLUGIN_MANUFACTURER_CODE CPSS
	PLUGIN_CODE orgw
	FORMATS AU VST3 Standalone
	ProductName "Wave Organ")

juce_generate_juce_header(WaveOrgan)

target_compile_definitions(WaveOrgan PUBLIC
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(WaveOrgan PRIVATE 
		juce_plugin_modules)

target_sources(WaveOrgan PRIVATE
	WaveOrgan.cpp

	# DSP Includes
)


