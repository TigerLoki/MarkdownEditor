import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Dialogs

ApplicationWindow {
    id: root
    width: 1024
    height: 768
    visible: true

    title: editorVM.isDirty
        ? editorVM.title + " * — Markdown Editor"
        : editorVM.title + " — Markdown Editor"

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")

            Action {
                text:     qsTr("New")
                shortcut: StandardKey.New
                onTriggered: editorVM.newFile()
            }
            Action {
                text:     qsTr("Open...")
                shortcut: StandardKey.Open
                onTriggered: openDialog.open()
            }
            Action {
                text:     qsTr("Save")
                shortcut: StandardKey.Save
                onTriggered: {
                    if (editorVM.filePath === "") {
                        saveAsDialog.open()
                        return
                    }
                    editorVM.saveFile()
                }
            }
            Action {
                text:     qsTr("Save As...")
                shortcut: StandardKey.SaveAs
                onTriggered: saveAsDialog.open()
            }
            MenuSeparator {}
            Action {
                text:     qsTr("Export HTML...")
                shortcut: "Ctrl+Shift+E"
                onTriggered: exportDialog.open()
            }
        }
    }

    FileDialog {
        id: openDialog
        title: qsTr("Open File")
        nameFilters: ["Markdown files (*.md)", "Text files (*.txt)"]
        onAccepted: editorVM.openFile(selectedFile)
    }

    FileDialog {
        id: saveAsDialog
        title: qsTr("Save As")
        fileMode: FileDialog.SaveFile
        nameFilters: ["Markdown files (*.md)"]
        onAccepted: editorVM.saveFileAs(selectedFile)
    }

    FileDialog {
        id: exportDialog
        title: qsTr("Export HTML")
        fileMode: FileDialog.SaveFile
        nameFilters: ["HTML files (*.html)"]
        onAccepted: editorVM.exportHtml(selectedFile)
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        EditorView {
            id: editorView
            Layout.fillWidth: true
            Layout.fillHeight: true

            onEditorTextChanged: (text) => editorVM.setText(text)
        }

        ToolSeparator {}

        PreviewView {
            id: previewView
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    Connections {
        target: editorVM
        function onTextChanged() {
            editorView.text = editorVM.text
        }
    }
}