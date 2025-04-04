cmake_minimum_required(VERSION 3.16)
project(CPPWord VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt 6.8.2 packages
find_package(Qt6 6.8.2 COMPONENTS Widgets Core Gui PrintSupport REQUIRED)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/texteditor.cpp
    src/formatbar.cpp
)

set(HEADERS
    src/mainwindow.h
    src/texteditor.h
    src/formatbar.h
)

set(RESOURCES
    icons.qrc
)

add_executable(CPPWord ${SOURCES} ${HEADERS} ${RESOURCES})
target_link_libraries(CPPWord PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui Qt6::PrintSupport) 