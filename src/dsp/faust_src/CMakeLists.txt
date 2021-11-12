project(FaustBuilder) # Name of executable to build dsp code

# Add dummpy cpp file for executable
add_executable(FaustBuilder null.cpp)

# Add dsp files to build
faust_add_file(FaustBuilder flue.dsp flueEngine)
faust_add_file(FaustBuilder gedeckt.dsp gedecktEngine)