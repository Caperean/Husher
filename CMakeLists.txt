cmake_minimum_required(VERSION 3.16)

project(Husher LANGUAGES C CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui)
set_property(SOURCE ${UI_FILES} PROPERTY SKIP_AUTOUIC OFF)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mavx2 -msse4.1 -msse2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -msse4.1 -msse2")

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core Concurrent)

file(GLOB_RECURSE SRC_FILES
    src/*.cpp
    algorithms/*.cpp
    algorithms/*.c
)

file(GLOB_RECURSE HEADER_FILES
    include/*.h
)

file(GLOB_RECURSE UI_FILES
    ui/*.ui
)

# 🔥 Crypto++ (statyczna biblioteka)
add_library(cryptopp STATIC IMPORTED)

set_target_properties(cryptopp PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/external/cryptopp/libcryptopp.a"
   INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/external"

)

# Główna aplikacja
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

# Linkowanie Qt + Crypto++
target_link_libraries(Husher PRIVATE
    Qt::Widgets
    Qt6::Core
    Qt6::Gui
    Qt6::Concurrent
    cryptopp
)
