cmake_minimum_required(VERSION 3.16)

# Project name and settings
project(WaterQualityMonitor VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${Qt6Widgets_INCLUDE_DIRS})
include_directories(${Qt6Charts_INCLUDE_DIRS})

# Find required Qt components
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts Gui) # Added Gui

# Setup Qt project
qt_standard_project_setup() # Add "I18N_TRANSLATED_LANGUAGES fr" back if needed

# Add executable
qt_add_executable(watermonitor
    main.cpp
    sample.cpp
    dataset.cpp
    model.cpp
    window.cpp
    tab1.cpp
    tab2.cpp
    tab3.cpp
    tab4.cpp
    tab5.cpp
    tab6.cpp
    tab7.cpp
    loaddatawidget.cpp
)

# Add translations (if needed)
# qt_add_translations(watermonitor)

# Link required libraries
target_link_libraries(watermonitor PRIVATE Qt6::Widgets Qt6::Charts Qt6::Gui)

# Set target properties
set_target_properties(watermonitor PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE OFF
)
