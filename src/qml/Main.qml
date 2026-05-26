import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick.Dialogs

ApplicationWindow {
    id: root
    width: 1200
    height: 800
    title: (editorVM.isDirty ? "*" : "") + editorVM.title + " - Markdown Editor"
    visible: true

    Connections {
        target: editorVM
        function onCursorPositionChanged(newPos) {
            editorView.editor.cursorPosition = newPos
            editorView.editor.forceActiveFocus()
        }
    }

    Shortcut {
        sequences: ["Alt+Shift++", "Alt+Shift+="]
        onActivated: appSettings.increaseFontSize()
    }
    Shortcut {
        sequence: "Alt+Shift+-"
        onActivated: appSettings.decreaseFontSize()
    }

    palette: Palette {
        text: appSettings.editorText
        base: appSettings.editorBg
        highlight: appSettings.editorSelection
        highlightedText: appSettings.editorSelectionText
        button: appSettings.editorBg
        buttonText: appSettings.editorText
        window: appSettings.editorBg
        windowText: appSettings.editorText
    }

    font.family: appSettings.fontFamily
    font.pixelSize: appSettings.fontSize
    
    FileDialog {
        id: openDialog
        title: "Open Markdown File"
        nameFilters: ["Markdown files (*.md *.markdown)", "All files (*)"]
        onAccepted: editorVM.openFile(selectedFile)
    }
    FileDialog {
        id: saveDialog
        title: "Save Markdown File"
        fileMode: FileDialog.SaveFile
        nameFilters: ["Markdown (*.md)", "All files (*)"]
        onAccepted: {
            editorVM.saveFileAs(selectedFile)
            if (messageDialog.pendingCloseAfterSave) {
                messageDialog.pendingCloseAfterSave = false
                root.close()
            }
        }
        onRejected: {
            messageDialog.pendingCloseAfterSave = false
        }
    }
    FileDialog {
        id: exportDialog
        title: "Export HTML"
        fileMode: FileDialog.SaveFile
        defaultSuffix: "html"
        nameFilters: ["HTML (*.html)", "All files (*)"]
        onAccepted: editorVM.exportHtml(selectedFile)
    }

    menuBar: MenuBar {
        background: Rectangle {
            color: appSettings.editorBg
            border.color: appSettings.editorBorder
        }

        Menu {
            title: "File"
            Action { text: "New"; shortcut: "Ctrl+N"; onTriggered: editorVM.newFile() }
            Action { text: "Open..."; shortcut: "Ctrl+O"; onTriggered: openDialog.open() }
            Action { text: "Save"; shortcut: "Ctrl+S"; onTriggered: editorVM.saveFile() }
            Action { text: "Save As..."; shortcut: "Ctrl+Shift+S"; onTriggered: saveDialog.open() }
            MenuSeparator { }
            Action { text: "Export HTML..."; shortcut: "Ctrl+E"; onTriggered: exportDialog.open() }
            MenuSeparator { }
            Action { text: "Exit"; shortcut: "Ctrl+Q"; onTriggered: Qt.quit() }
        }
        Menu {
            title: "Edit"
            Action { text: "Undo"; shortcut: "Ctrl+Z"; onTriggered: editorView.editor.undo() }
            Action { text: "Redo"; shortcut: "Ctrl+Y"; onTriggered: editorView.editor.redo() }
            MenuSeparator { }
            Action { text: "Bold"; shortcut: "Ctrl+B"; onTriggered: editorView.wrapSelection("**", "**") }
            Action { text: "Italic"; shortcut: "Ctrl+I"; onTriggered: editorView.wrapSelection("*", "*") }
            Action { text: "Inline Code"; shortcut: "Ctrl+`"; onTriggered: editorView.wrapSelection("`", "`") }
            Action { text: "Link"; shortcut: "Ctrl+L"; onTriggered: editorView.wrapSelection("[", "](url)") }
            Action { text: "Image"; shortcut: "Ctrl+Shift+I"; onTriggered: editorView.wrapSelection("![", "](url)") }
        }
        Menu {
            title: "View"
            Action {
                text: "Dark Theme"
                checkable: true
                checked: appSettings.darkTheme
                onToggled: appSettings.darkTheme = checked
            }
            Action {
                id: splitAction
                text: "Split View (Vertical)"
                checkable: true
                checked: true
            }
        }
    }

    ToolBar {
        id: toolbar
        width: parent.width
        background: Rectangle {
            color: appSettings.editorBg
            border.color: appSettings.editorBorder
        }

        RowLayout {
            anchors.fill: parent
            ToolButton {
                implicitWidth: appSettings.fontSize * 1.2 + 20
                implicitHeight: appSettings.fontSize * 1.2 + 20
                focusPolicy: Qt.NoFocus
                text: "📄"
                onClicked: editorVM.newFile()
            }
            ToolButton {
                implicitWidth: appSettings.fontSize * 1.2 + 20
                implicitHeight: appSettings.fontSize * 1.2 + 20
                focusPolicy: Qt.NoFocus
                text: "📂"
                onClicked: openDialog.open()
            }
            ToolButton {
                implicitWidth: appSettings.fontSize * 1.2 + 20
                implicitHeight: appSettings.fontSize * 1.2 + 20
                focusPolicy: Qt.NoFocus
                text: "💾"
                onClicked: editorVM.saveFile()
            }
            ToolSeparator { }
            ToolButton {
                implicitWidth: appSettings.fontSize * 1.2 + 20
                implicitHeight: appSettings.fontSize * 1.2 + 20
                focusPolicy: Qt.NoFocus
                text: "B"
                font.bold: true
                onClicked: editorVM.applyFormatting(editorView.editor.selectionStart, editorView.editor.selectionEnd, "**", "**")
            }
            ToolButton {
                implicitWidth: appSettings.fontSize * 1.2 + 20
                implicitHeight: appSettings.fontSize * 1.2 + 20
                focusPolicy: Qt.NoFocus
                text: "I"
                font.italic: true
                onClicked: editorVM.applyFormatting(editorView.editor.selectionStart, editorView.editor.selectionEnd, "*", "*")
            }
            ToolButton {
                implicitWidth: appSettings.fontSize * 1.2 + 20
                implicitHeight: appSettings.fontSize * 1.2 + 20
                focusPolicy: Qt.NoFocus
                text: "`"
                onClicked: editorVM.applyFormatting(editorView.editor.selectionStart, editorView.editor.selectionEnd, "`", "`")
            }
            ToolButton {
                implicitWidth: appSettings.fontSize * 1.2 + 20
                implicitHeight: appSettings.fontSize * 1.2 + 20
                focusPolicy: Qt.NoFocus
                text: "🔗"
                onClicked: editorVM.applyFormatting(editorView.editor.selectionStart, editorView.editor.selectionEnd, "[", "](url)")
            }
            ToolButton {
                implicitWidth: appSettings.fontSize * 1.2 + 20
                implicitHeight: appSettings.fontSize * 1.2 + 20
                focusPolicy: Qt.NoFocus
                text: "🖼️"
                onClicked: editorVM.applyFormatting(editorView.editor.selectionStart, editorView.editor.selectionEnd, "![", "](url)")
            }
            Item { Layout.fillWidth: true }
            ToolButton {
                implicitWidth: appSettings.fontSize * 1.2 + 20
                implicitHeight: appSettings.fontSize * 1.2 + 20
                focusPolicy: Qt.NoFocus
                text: appSettings.darkTheme ? "☀️" : "🌙"
                onClicked: appSettings.darkTheme = !appSettings.darkTheme
            }
        }
    }

    SplitView {
        anchors.top: toolbar.bottom
        anchors.bottom: statusBar.top
        anchors.left: parent.left
        anchors.right: parent.right
        orientation: splitAction.checked ? Qt.Horizontal : Qt.Vertical

        EditorView {
            id: editorView
            SplitView.minimumWidth: 200
            SplitView.minimumHeight: 150
            SplitView.preferredWidth: splitAction.checked ? root.width / 2 : root.width
            SplitView.fillWidth: true
            text: editorVM.text
            onTextChanged: editorVM.setText(text)
        }

        PreviewView {
            id: previewView
            SplitView.minimumWidth: 200
            SplitView.minimumHeight: 150
            SplitView.preferredWidth: splitAction.checked ? root.width / 2 : root.width
            SplitView.fillWidth: true
        }
    }

    Rectangle {
        id: statusBar
        height: appSettings.fontSize * 1.2 + 8
        color: appSettings.editorBorder
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        RowLayout {
            anchors.fill: parent
            anchors.margins: 4
            Text {
                text: editorVM.title + (editorVM.isDirty ? " [modified]" : "")
                color: appSettings.editorText
                font.family: appSettings.fontFamily
                font.pixelSize: appSettings.fontSize
            }
            Item { Layout.fillWidth: true }
            Text {
                text: {
                    var pos = editorView.editor.cursorPosition;
                    var txt = editorView.editor.text;
                    var line = 0, col = pos;
                    for (var i = 0; i < pos; i++) {
                        if (txt[i] === '\n') line++;
                    }
                    for (var j = pos - 1; j >= 0; j--) {
                        if (txt[j] === '\n') {
                            col = pos - j - 1;
                            break;
                        }
                    }
                    return "Line: " + (line + 1) + ", Col: " + (col + 1);
                }
                color: appSettings.editorText
                font.family: appSettings.fontFamily
                font.pixelSize: appSettings.fontSize
            }
        }
    }

    onClosing: (close) => {
        if (messageDialog.discarded) {
            close.accepted = true
            return
        }
        if (editorVM.isDirty) {
            close.accepted = false
            messageDialog.open()
        } else {
            close.accepted = true
        }
    }

    Dialog {
        id: messageDialog
        modal: true
        closePolicy: Popup.NoAutoClose
        title: "Unsaved changes"
        x: (root.width - width) / 2
        y: (root.height - height) / 2

        property bool pendingCloseAfterSave: false
        property bool discarded: false

        ColumnLayout {
            Label {
                text: "Do you want to save changes?"
                Layout.bottomMargin: 10
            }
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Button {
                    text: "Save"
                    onClicked: {
                        if (editorVM.filePath === "") {
                            messageDialog.pendingCloseAfterSave = true
                            saveDialog.open()
                        } else {
                            editorVM.saveFile()
                            root.close()
                        }
                    }
                }
                Button {
                    text: "Discard"
                    onClicked: {
                        messageDialog.discarded = true
                        messageDialog.close()
                        root.close()
                    }
                }
                Button {
                    text: "Cancel"
                    onClicked: messageDialog.close()
                }
            }
        }
    }

    Dialog {
        id: errorDialog
        modal: true
        closePolicy: Popup.NoAutoClose
        title: "Error"
        width: 300
        height: 120
        x: (root.width - width) / 2
        y: (root.height - height) / 2
        standardButtons: Dialog.Ok
        property string text: ""
        Label {
            text: errorDialog.text
            wrapMode: Text.WordWrap
            width: parent.width - 40
        }
    }

    Connections {
        target: editorVM
        function onErrorOccurred(message) {
            errorDialog.text = message
            errorDialog.open()
        }
    }
}
