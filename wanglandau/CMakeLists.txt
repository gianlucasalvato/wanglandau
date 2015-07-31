cmake_minimum_required(VERSION 2.6)

project (wl)

set(DIMENSIONALITY	2)
set(MAX_SITES 		1073741824)
set(PRNG 			std::mt19937_64)

set (wl_VERSION_MAJOR 0)
set (wl_VERSION_MINOR 4)

set (CMAKE_CXX_FLAGS				"-std=c++11 -Ofast")
set (CMAKE_CXX_FLAGS_RELEASE		"-Ofast")
set (CMAKE_CXX_FLAGS_DEBUG          "-g")
set (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

set (CMAKE_CXX_COMPILER	"/usr/bin/clang++")

configure_file (
	"${PROJECT_SOURCE_DIR}/wlConfig.h.in"
	"${PROJECT_SOURCE_DIR}/wlConfig.h"
)

include_directories("${PROJECT_SOURCE_DIR}/include")

include_directories("${PROJECT_BINARY_DIR}")

add_executable(wl wl.cpp)


