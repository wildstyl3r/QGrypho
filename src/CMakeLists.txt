add_library(QGrypho QGrypho.cpp QGrypho.ui)
target_include_directories(
  QGrypho
  PUBLIC
    $<BUILD_INTERFACE:${QGrypho_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${QGrypho_SOURCE_DIR}/grypho/include>
  PRIVATE
    $<BUILD_INTERFACE:${QGrypho_SOURCE_DIR}/src>)
target_link_libraries(QGrypho PRIVATE Qt${QT_VERSION_MAJOR}::Widgets grypho)

target_compile_definitions(QGrypho PRIVATE QGRYPHO_LIBRARY)

set_target_properties(QGrypho PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    PUBLIC_HEADER ${QGrypho_SOURCE_DIR}/QGrypho.h)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(QGrypho PRIVATE -Wall -Wextra)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(QGrypho PRIVATE -Wall -Wextra)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(QGrypho PRIVATE /W4)
  target_compile_definitions(QGrypho PUBLIC _USE_MATH_DEFINES)
endif()
