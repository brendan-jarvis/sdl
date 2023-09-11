cmake_minimum_required(VERSION 3.27)
set(CMAKE_CXX_STANDARD 17)

project(Breakout)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Manually specify the path to SDL2 and SDL2_ttf libraries
set(SDL2_LIBRARY /Library/Frameworks/sdl2)  
set(SDL2_TTF_LIBRARY /Library/Frameworks/sdl2_ttf)
set(SDL2_INCLUDE_DIRS /Library/Frameworks/sdl2.framework/Headers)
set(SDL2_TTF_INCLUDE_DIRS /Library/Frameworks/sdl2_ttf.framework/Headers)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Make sure SDL2 was found
message(STATUS "SDL2_LIBRARY: ${SDL2_LIBRARY}")
message(STATUS "SDL2_TTF_LIBRARY: ${SDL2_TTF_LIBRARY}")
message(STATUS "SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2_TTF_INCLUDE_DIRS: ${SDL2_TTF_INCLUDE_DIRS}")

# Add the include directories for SDL2 and SDL2_ttf
include_directories("include" ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# List your source files explicitly or using wildcards
file(GLOB SOURCES "src/*.cpp")

# Make sure there are source files to compile
if(SOURCES)
    add_executable(Breakout ${SOURCES})
else()
    message(FATAL_ERROR "No source files found. Please add your source files to the 'src' directory.")
endif()

# Remove any leading or trailing whitespace from SDL2_LIBRARIES
string(STRIP ${SDL2_LIBRARY} SDL2_LIBRARY)
target_link_libraries(Breakout ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})
