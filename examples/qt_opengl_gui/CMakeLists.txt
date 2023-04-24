# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(qt_opengl_gui LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL)

qt_standard_project_setup()

qt_add_executable(qt_opengl_gui
    logo.cpp logo.h
    glwindow.cpp glwindow.h
    main.cpp
)

set_target_properties(qt_opengl_gui PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(qt_opengl_gui PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::OpenGL
)

# Resources:
set(hellogles3_resource_files
    "qtlogo.png"
)

qt_add_resources(qt_opengl_gui "hellogles3"
    PREFIX
        "/"
    FILES
        ${hellogles3_resource_files}
)
