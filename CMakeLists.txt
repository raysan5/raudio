cmake_minimum_required(VERSION 3.11)
project(raudio
    DESCRIPTION "raudio: A simple and easy-to-use audio library based on miniaudio"
    HOMEPAGE_URL "https://github.com/raylib5/raudio"
    LANGUAGES C
)
set(PROJECT_VERSION 1.1.0)

# Directory Variables
set(RAUDIO_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/)
set(RAUDIO_SRC ${RAUDIO_ROOT}/src)


option(LIBTYPE_SHARED FALSE)

# raudio
if (NOT LIBTYPE_SHARED)
add_library(raudio SHARED ${RAUDIO_SRC}/raudio.c)
endif()

if (LIBTYPE_SHARED)
add_library(raudio STATIC ${RAUDIO_SRC}/raudio.c)
endif()


# Includes
target_include_directories(raudio INTERFACE ${RAUDIO_SRC})

# Defines
target_compile_definitions(raudio PUBLIC
    RAUDIO_STANDALONE
    SUPPORT_MODULE_RAUDIO
)

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

# Dependenices
set_target_properties(raudio PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION 0)

target_link_libraries(raudio m pthread z gcc)
