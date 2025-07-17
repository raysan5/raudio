cmake_minimum_required(VERSION 3.11)
project(raudio
    DESCRIPTION "raudio: A simple and easy-to-use audio library based on miniaudio"
    HOMEPAGE_URL "https://github.com/raylib5/raudio"
    LANGUAGES C
)

# Config options
option(BUILD_RAUDIO_EXAMPLES "Build the examples." OFF)

# Force building examples if building in the root as standalone.
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(BUILD_RAUDIO_EXAMPLES TRUE)
endif()

# Directory Variables
set(RAUDIO_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(RAUDIO_SRC ${RAUDIO_ROOT}/src)
set(RAUDIO_EXAMPLES ${RAUDIO_ROOT}/examples)

option(SUPPORT_FILEFORMAT_WAV "WAV Support" TRUE)
option(SUPPORT_FILEFORMAT_OGG "OGG Support" TRUE)
option(SUPPORT_FILEFORMAT_MP3 "MP3 Support" TRUE)
option(SUPPORT_FILEFORMAT_QOA "QOA Support" TRUE)
option(SUPPORT_FILEFORMAT_FLAC "FLAC Support" TRUE)
option(SUPPORT_FILEFORMAT_XM "XM Support" TRUE)
option(SUPPORT_FILEFORMAT_MOD "MOD Support" TRUE)

# raudio
add_library(raudio STATIC
    ${RAUDIO_SRC}/raudio.c
)

# Includes
target_include_directories(raudio INTERFACE ${RAUDIO_SRC})

# Defines
target_compile_definitions(raudio PUBLIC
    RAUDIO_STANDALONE
    SUPPORT_MODULE_RAUDIO
)

# Dependenices
if(NOT MSVC)
    target_link_libraries(raudio
        m # math
    )
endif()

# Audio file support options
if (SUPPORT_FILEFORMAT_WAV)
    target_compile_definitions(raudio PUBLIC SUPPORT_FILEFORMAT_WAV)
endif()
if (SUPPORT_FILEFORMAT_OGG)
    target_compile_definitions(raudio PUBLIC SUPPORT_FILEFORMAT_OGG)
endif()
if (SUPPORT_FILEFORMAT_MP3)
    target_compile_definitions(raudio PUBLIC SUPPORT_FILEFORMAT_MP3)
endif()
if (SUPPORT_FILEFORMAT_QOA)
    target_compile_definitions(raudio PUBLIC SUPPORT_FILEFORMAT_QOA)
endif()
if (SUPPORT_FILEFORMAT_FLAC)
    target_compile_definitions(raudio PUBLIC SUPPORT_FILEFORMAT_FLAC)
endif()
if (SUPPORT_FILEFORMAT_XM)
    target_compile_definitions(raudio PUBLIC SUPPORT_FILEFORMAT_XM)
endif()
if (SUPPORT_FILEFORMAT_MOD)
    target_compile_definitions(raudio PUBLIC SUPPORT_FILEFORMAT_MOD)
endif()

# Examples
if (BUILD_RAUDIO_EXAMPLES)
    # Create a list of all examples
    set(examples)
    file(GLOB sources ${RAUDIO_EXAMPLES}/*.c)
    list(APPEND examples ${sources})

    # Build each example
    foreach(example_source ${examples})
        # Create the basename for the example
        get_filename_component(example_name ${example_source} NAME)
        string(REPLACE ".c" "${OUTPUT_EXT}" example_name ${example_name})

        # Setup the example
        add_executable(${example_name} ${example_source})
        target_link_libraries(${example_name} PUBLIC raudio)
    endforeach()

    # Resources
    file(COPY ${RAUDIO_EXAMPLES}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()