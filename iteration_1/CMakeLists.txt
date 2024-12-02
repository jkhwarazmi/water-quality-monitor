cmake_minimum_required(VERSION 3.16)

project(WaterQualityMonitor VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(watermonitor
    main.cpp
    sample.cpp
    dataset.cpp
    model.cpp
    window.cpp
    tab1.cpp
    tab2.cpp
    tab3.cpp
    loaddatawidget.cpp
)

target_link_libraries(watermonitor PRIVATE Qt6::Widgets)

set_target_properties(watermonitor PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE OFF
)
