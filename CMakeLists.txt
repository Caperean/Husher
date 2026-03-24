cmake_minimum_required(VERSION 3.16)

project(Hcarer LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui)
set_property(SOURCE ${UI_FILES} PROPERTY SKIP_AUTOUIC OFF)

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core)

# Zbieranie plików źródłowych i nagłówków
file(GLOB_RECURSE SRC_FILES
    src/*.cpp
)

file(GLOB_RECURSE HEADER_FILES
    include/*.h
)

file(GLOB_RECURSE UI_FILES
    ui/*.ui
)

# Główna aplikacja
add_executable(Hcarer WIN32
    ${SRC_FILES}
    ${HEADER_FILES}
    ${UI_FILES}
    resources/resources.qrc
)

# Ikona EXE na Windows
if(WIN32)
    target_sources(Hcarer PRIVATE resources/app.rc)
endif()

# Include
target_include_directories(Hcarer PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/ui
)

# Linkowanie Qt
target_link_libraries(Hcarer PRIVATE Qt::Widgets Qt6::Core Qt6::Gui)
