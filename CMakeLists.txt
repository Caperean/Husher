cmake_minimum_required(VERSION 3.16)

project(Husher LANGUAGES C CXX)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mavx2 -msse4.1 -msse2 -mpclmul -maes -msha")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -msse4.1 -msse2 -mpclmul -maes -msha")


find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core Concurrent)


# CONFIGURE_DEPENDS: CMake re-checks the globs on every build, so newly added
# files are picked up without having to re-run "cmake .." manually.
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    src/*.cpp
    algorithms/*.cpp
    algorithms/*.c
)

file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS
    include/*.h
)

file(GLOB_RECURSE UI_FILES CONFIGURE_DEPENDS
    ui/*.ui
)


# ---- Crypto++ ---------------------------------------------------------------
file(GLOB CRYPTOPP_SRC CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/external/cryptopp/*.cpp
)

# Crypto++'s own test/benchmark programs contain a main() (test.cpp).
# NOTE: dll.cpp must stay in the library - it holds the explicit template
# instantiations (AllocatorWithCleanup, StringSinkTemplate, AbstractRing<Integer>, ...).
# Linking them into the library made Husher.exe run Crypto++'s main() instead
# of Qt's, so they must not be part of the library.
list(FILTER CRYPTOPP_SRC EXCLUDE REGEX
    ".*/(test|bench[0-9]*|validat[0-9]*|datatest|regtest[0-9]*|dlltest|fipsalgt|fipstest|pch|simple)\\.cpp$")

add_library(cryptopp STATIC ${CRYPTOPP_SRC})

target_include_directories(cryptopp PUBLIC
    ${CMAKE_SOURCE_DIR}/external/cryptopp
)


# ---- xxHash -----------------------------------------------------------------
file(GLOB XXHASH_SRC CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/external/xxhash/*.c
)

add_library(xxhash STATIC ${XXHASH_SRC})

target_include_directories(xxhash PUBLIC
    ${CMAKE_SOURCE_DIR}/external/xxhash
)


# ---- Application ------------------------------------------------------------
add_executable(Husher WIN32
    ${SRC_FILES}
    ${HEADER_FILES}
    ${UI_FILES}
    resources/resources.qrc
)


add_definitions(
    -DBLAKE3_NO_SSE2
    -DBLAKE3_NO_SSE41
    -DBLAKE3_NO_AVX2
    -DBLAKE3_NO_AVX512
    -DBLAKE3_NO_NEON
)


if(WIN32)
    target_sources(Husher PRIVATE resources/app.rc)
endif()


target_include_directories(Husher PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/ui
    ${CMAKE_SOURCE_DIR}/external
)


target_link_libraries(Husher PRIVATE
    Qt::Widgets
    Qt6::Core
    Qt6::Gui
    Qt6::Concurrent
    cryptopp
    xxhash
)