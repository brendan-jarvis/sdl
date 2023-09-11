cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 17)

project(Breakout)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})

include_directories("include")

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
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
target_link_libraries(Breakout ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})
