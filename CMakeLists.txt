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

# Zbieranie plików źródłowych i nagłówków
file(GLOB_RECURSE SRC_FILES
    src/*.cpp
    algorithms/*.cpp
    algorithms/*.c #jednorazowo potrzebny
)

file(GLOB_RECURSE HEADER_FILES
    include/*.h
)

file(GLOB_RECURSE UI_FILES
    ui/*.ui
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


# Ikona EXE na Windows
if(WIN32)
    target_sources(Husher PRIVATE resources/app.rc)
endif()

# Include
target_include_directories(Husher PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/ui
)

# Linkowanie Qt
target_link_libraries(Husher PRIVATE Qt::Widgets Qt6::Core Qt6::Gui Qt6::Concurrent)
