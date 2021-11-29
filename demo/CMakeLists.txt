add_executable(gv gv.cpp)

target_link_libraries(gv PRIVATE Qt${QT_VERSION_MAJOR}::Widgets QGrypho)
