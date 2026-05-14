cmake_minimum_required(VERSION 4.1)

project(MarkdownEditor
        VERSION 0.1
        DESCRIPTION "Markdown Editor with Qt6 and C++20"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_compile_options(-Wall -Wextra -Wpedantic)

find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Qml
        Quick
        Widgets
)

qt_standard_project_setup(REQUIRES 6.10)

qt_add_executable(appMarkdownEditor
        src/main.cpp
        app.rc
        resources.qrc
)

qt_add_qml_module(appMarkdownEditor
        URI MarkdownEditor
        QML_FILES
        src/qml/Main.qml
)

set_target_properties(appMarkdownEditor PROPERTIES
        WIN32_EXECUTABLE TRUE
        MACOSX_BUNDLE TRUE
)

target_link_libraries(appMarkdownEditor PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::Widgets
)