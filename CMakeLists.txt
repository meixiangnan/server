cmake_minimum_required(VERSION 3.29)
project(mini_server CXX)

if(POLICY CMP0091)
  cmake_policy(SET CMP0091 NEW)
endif()

include(CheckIncludeFileCXX)

check_include_file_cxx(any HAS_ANY)
check_include_file_cxx(string_view HAS_STRING_VIEW)
check_include_file_cxx(coroutine HAS_COROUTINE)

set(CMAKE_CXX_STANDARD 20)

# set(CMAKE_BUILD_TYPE Release)
set(CMAKE_TOOLCHAIN_FILE "conan_toolchain.cmake")
set(CMAKE_POLICY_DEFAULT_CMP0091 NEW)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")
set(CMAKE_INSTALL_PREFIX "exe/bin")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(publish_bin_debug 			${CMAKE_CURRENT_SOURCE_DIR}/exe/bind)
set(publish_bin_release 		${CMAKE_CURRENT_SOURCE_DIR}/exe/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	${publish_bin_debug})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE 	${publish_bin_release})

if (CMAKE_CXX_STANDARD LESS 17)
    message(FATAL_ERROR "c++17 or higher is required")
elseif (CMAKE_CXX_STANDARD LESS 20)
    message(STATUS "use c++17")
else ()
    message(STATUS "use c++20")
endif ()

if(MSVC)
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} 
        "${CMAKE_CURRENT_SOURCE_DIR}/third_party/installed/drogon/win/release"        
    )
    add_compile_options(/utf-8)
endif()

set(CMAKE_PREFIX_PATH  ${CMAKE_PREFIX_PATH})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)

find_package(jsoncpp REQUIRED)
# Create alias for drogon's expected jsoncpp target name
if(NOT TARGET Jsoncpp_lib)
    if(TARGET JsonCpp::JsonCpp)
        add_library(Jsoncpp_lib ALIAS JsonCpp::JsonCpp)
    elseif(TARGET jsoncpp_lib)
        add_library(Jsoncpp_lib ALIAS jsoncpp_lib)
    elseif(TARGET jsoncpp_static)
        add_library(Jsoncpp_lib ALIAS jsoncpp_static)
    endif()
endif()
message("JSONCPP_INCLUDE_DIRS" ${jsoncpp_INCLUDE_DIR})
include_directories( ${jsoncpp_INCLUDE_DIR})


find_package(drogon REQUIRED)
message("DROGON_INCLUDE_DIRS" ${DROGON_INCLUDE_DIRS})
include_directories( ${DROGON_INCLUDE_DIRS})

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/third_party)

add_subdirectory(src)

