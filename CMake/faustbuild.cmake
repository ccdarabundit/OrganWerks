# Function to add faust.dsp files and process to h files
# Target is the faust build executable
# File is the .dsp file
# Class is the output class name/h file name
function(faust_add_file TARGET FILE	CLASS)
	set(genDir ${CMAKE_CURRENT_LIST_DIR}/gen)				# Files output in gen file where function is called
	set(outFileName ${genDir}/${CLASS}.h)					# h. file name based on Class Name
	set(inFileName ${CMAKE_CURRENT_LIST_DIR}/${FILE})		# Get full filepath  
	set(archDir ${CMAKE_CURRENT_LIST_DIR}/faustMinimal.h)	# Currently using this architecture file, maybe a parameter?

	# Make the directory
	file(MAKE_DIRECTORY ${genDir})
	# Add the directory to the executable
	target_include_directories(${TARGET} PRIVATE ${genDir})
    target_include_directories(${TARGET} PRIVATE ${Faust_SOURCE_DIR}/architecture)

    # Command to build faust code
	add_custom_command(
		OUTPUT ${outFileName}
		COMMAND faust ${inFileName}
		-i -a ${archDir}	#inline
		-I ${Faust_SOURCE_DIR}/libraries
        -o ${outFileName}
        -cn ${CLASS}
        DEPENDS ${fileName}
    )

	# Add the output file to the target
	target_sources(${TARGET} PRIVATE ${outFileName})
endfunction()