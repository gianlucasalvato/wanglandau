cmake_minimum_required(VERSION 2.6)

project (wl)

set(DEBUG)
set(VERBOSE)
set(AUTHOR_NAME your_name)

set(DIMENSIONALITY 2)
set(MAX_SITES 0x40000000)
set(PRNG std::mt19937_64)

string(TIMESTAMP COMPILE_TIME "%Y-%m-%d %H:%M")

set (CXX_STANDARD "c++11")
set (CXX_STANDARD_LIB "libc++")

set (VERSION_MAJOR 0)
set (VERSION_MINOR 8)

set (CMAKE_CXX_COMPILER	"/usr/bin/clang++")

set (CMAKE_CXX_FLAGS				"-std=${CXX_STANDARD} -stdlib=${CXX_STANDARD_LIB}"	)
set (CMAKE_CXX_FLAGS_RELEASE		"-Ofast"											)
set (CMAKE_CXX_FLAGS_DEBUG          "-g -O0"											)
set (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG"										)
set (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG"										)
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g"											)

configure_file (
	"${PROJECT_SOURCE_DIR}/doc/config.h.in"
	"${PROJECT_SOURCE_DIR}/include/config.h"
)

if (NOT EXISTS "${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}.cpp")
	configure_file (
		"${PROJECT_SOURCE_DIR}/doc/project.cpp.in"
		"${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}.cpp"
	)
endif()

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/lib")
include_directories("${PROJECT_BINARY_DIR}")


if (APPLE)
	set (CMAKE_OSX_DEPLOYMENT_TARGET "10.7")
	set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "${CXX_STANDARD}")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "${CXX_STANDARD_LIB}")
elseif(UNIX)

elseif(Win32)

endif()


add_executable(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}.cpp")

set_target_properties( ${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
)

