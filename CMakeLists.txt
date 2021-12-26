cmake_minimum_required(VERSION 3.14...3.18)

project(
  QGrypho
  VERSION 0.2
  DESCRIPTION "Qt based visualizer for Grypho"
  LANGUAGES CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  option(BUILD_DEMO "Build the demo" ON)
  option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

add_subdirectory(src)
add_subdirectory(grypho)
add_subdirectory(nodesoup)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR QGrypho_BUILD_DEMO)
   AND BUILD_DEMO)
  add_subdirectory(demo)
else()
  target_compile_definitions(QGrypho PRIVATE QGRYPHO_LIBRARY)
endif()
