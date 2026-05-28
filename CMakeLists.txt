cmake_minimum_required(VERSION 3.20)

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
        QuickControls2
        QuickDialogs2
        Widgets
)

qt_standard_project_setup(REQUIRES 6.11)

qt_add_executable(appMarkdownEditor
        src/main.cpp
        app.rc
        resources.qrc
        src/core/Document.cpp
        include/core/Document.h
        src/core/FileManager.cpp
        include/core/FileManager.h
        src/viewmodels/EditorViewModel.cpp
        include/viewmodels/EditorViewModel.h
        include/parser/Token.h
        src/parser/Lexer.cpp
        include/parser/Lexer.h
        include/parser/AstNode.h
        include/parser/BlockNodes.h
        include/parser/InlineNodes.h
        src/parser/BlockParser.cpp
        include/parser/BlockParser.h
        src/parser/InlineParser.cpp
        include/parser/InlineParser.h
        src/parser/HtmlExporter.cpp
        include/parser/HtmlExporter.h
        src/parser/renderers/MarkdownPreviewItem.cpp
        include/parser/renderers/MarkdownPreviewItem.h
        src/parser/renderers/MarkdownTextDocument.cpp
        include/parser/renderers/MarkdownTextDocument.h
        src/core/AppSettings.cpp
        include/core/AppSettings.h
        include/parser/renderers/ThemeColors.h
)

qt_add_qml_module(appMarkdownEditor
        URI MarkdownEditor
        QML_FILES
        src/qml/Main.qml
        src/qml/EditorView.qml
        src/qml/PreviewView.qml
        src/qml/IconToolButton.qml
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
        Qt6::QuickControls2
        Qt6::QuickDialogs2
        Qt6::Widgets
)

include_directories(include)