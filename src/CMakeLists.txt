add_library(QGrypho ../include/QGrypho.hpp QGrypho.cpp QGVertex.cpp QGrypho.ui)
target_include_directories(
  QGrypho
  PUBLIC
    $<BUILD_INTERFACE:${QGrypho_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${QGrypho_SOURCE_DIR}/grypho/include>
    $<BUILD_INTERFACE:${QGrypho_SOURCE_DIR}/nodesoup/include>
  PRIVATE
    $<BUILD_INTERFACE:${QGrypho_SOURCE_DIR}/src>)
target_link_libraries(QGrypho PUBLIC grypho PRIVATE Qt${QT_VERSION_MAJOR}::Widgets nodesoup)

set_target_properties(QGrypho PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    PUBLIC_HEADER ${QGrypho_SOURCE_DIR}/include/QGrypho.hpp)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(QGrypho PRIVATE -Wall -Wextra)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(QGrypho PRIVATE -Wall -Wextra)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(QGrypho PRIVATE /W4)
  target_compile_definitions(QGrypho PUBLIC _USE_MATH_DEFINES)
endif()
