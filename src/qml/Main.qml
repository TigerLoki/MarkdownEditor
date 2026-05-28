import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick.Dialogs

ApplicationWindow {
    id: root
    minimumWidth: 1200
    minimumHeight: 800
    title: (editorVM.isDirty ? "*" : "") + editorVM.title + " - Markdown Editor"
    visible: true

    // ------------------------------------------------------------------------
    // Theme
    // ------------------------------------------------------------------------
    font.family: appSettings.fontFamily
    font.pixelSize: appSettings.fontSize
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

    // ------------------------------------------------------------------------
    // Properties & helpers
    // ------------------------------------------------------------------------
    property string pendingAction: "" // "new", "open", "close"

    function executePendingAction() {
        if (pendingAction === "new")
            editorVM.newFile()
        else if (pendingAction === "open")
            openDialog.open()
        else if (pendingAction === "close")
            root.close()
        pendingAction = ""
    }

    onClosing: (close) => {
        if (unsavedDialog.discarded) {
            close.accepted = true
            return
        }
        if (editorVM.isDirty) {
            close.accepted = false
            pendingAction = "close"
            unsavedDialog.open()
        } else {
            close.accepted = true
        }
    }

    // ------------------------------------------------------------------------
    // Global shortcuts
    // ------------------------------------------------------------------------
    Shortcut {
        sequences: ["Alt+Shift++", "Alt+Shift+="]
        onActivated: appSettings.increaseFontSize()
    }
    Shortcut {
        sequence: "Alt+Shift+-"
        onActivated: appSettings.decreaseFontSize()
    }

    // ------------------------------------------------------------------------
    // Connections
    // ------------------------------------------------------------------------
    Connections {
        target: editorVM
        function onCursorPositionChanged(newPos) {
            editorView.editor.cursorPosition = newPos
            editorView.editor.forceActiveFocus()
        }
        function onErrorOccurred(message) {
            errorDialog.text = message
            errorDialog.open()
        }
    }

    // ------------------------------------------------------------------------
    // Reusable Actions (used both in menu and toolbar)
    // ------------------------------------------------------------------------
    Action {
        id: newAction
        text: "New"
        shortcut: "Ctrl+N"
        onTriggered: {
            if (editorVM.isDirty) {
                pendingAction = "new"
                unsavedDialog.open()
            } else {
                editorVM.newFile()
            }
        }
    }
    Action {
        id: openAction
        text: "Open..."
        shortcut: "Ctrl+O"
        onTriggered: {
            if (editorVM.isDirty) {
                pendingAction = "open"
                unsavedDialog.open()
            } else {
                openDialog.open()
            }
        }
    }
    Action {
        id: saveAction
        text: "Save"
        shortcut: "Ctrl+S"
        onTriggered: editorVM.filePath === "" ? saveDialog.open() : editorVM.saveFile()
    }
    Action {
        id: saveAsAction
        text: "Save As..."
        shortcut: "Ctrl+Shift+S"
        onTriggered: saveDialog.open()
    }
    Action {
        id: exportAction
        text: "Export HTML..."
        shortcut: "Ctrl+E"
        onTriggered: exportDialog.open()
    }
    Action {
        id: exitAction
        text: "Exit"
        shortcut: "Ctrl+Q"
        onTriggered: Qt.quit()
    }

    Action {
        id: undoAction
        text: "Undo"
        shortcut: "Ctrl+Z"
        onTriggered: editorView.editor.undo()
    }
    Action {
        id: redoAction
        text: "Redo"
        shortcut: "Ctrl+Y"
        onTriggered: editorView.editor.redo()
    }
    Action {
        id: boldAction
        text: "Bold"
        shortcut: "Ctrl+B"
        onTriggered: editorView.wrapSelection("**", "**")
    }
    Action {
        id: italicAction
        text: "Italic"
        shortcut: "Ctrl+I"
        onTriggered: editorView.wrapSelection("*", "*")
    }
    Action {
        id: inlineCodeAction
        text: "Inline Code"
        shortcut: "Ctrl+`"
        onTriggered: editorView.wrapSelection("`", "`")
    }
    Action {
        id: linkAction
        text: "Link"
        shortcut: "Ctrl+L"
        onTriggered: editorView.wrapSelection("[", "](url)")
    }
    Action {
        id: imageAction
        text: "Image"
        shortcut: "Ctrl+Shift+I"
        onTriggered: editorView.wrapSelection("![", "](url)")
    }

    Action {
        id: darkThemeAction
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

    // ------------------------------------------------------------------------
    // Menu bar
    // ------------------------------------------------------------------------
    menuBar: MenuBar {
        background: Rectangle {
            color: appSettings.editorBg
            border.color: appSettings.editorBorder
        }

        Menu {
            title: "File"
            width: appSettings.fontSize * 10 + 20
            Action { text: newAction.text; shortcut: newAction.shortcut; onTriggered: newAction.trigger() }
            Action { text: openAction.text; shortcut: openAction.shortcut; onTriggered: openAction.trigger() }
            Action { text: saveAction.text; shortcut: saveAction.shortcut; onTriggered: saveAction.trigger() }
            Action { text: saveAsAction.text; shortcut: saveAsAction.shortcut; onTriggered: saveAsAction.trigger() }
            MenuSeparator {}
            Action { text: exportAction.text; shortcut: exportAction.shortcut; onTriggered: exportAction.trigger() }
            MenuSeparator {}
            Action { text: exitAction.text; shortcut: exitAction.shortcut; onTriggered: exitAction.trigger() }
        }
        Menu {
            title: "Edit"
            width: appSettings.fontSize * 10 + 20
            Action { text: undoAction.text; shortcut: undoAction.shortcut; onTriggered: undoAction.trigger() }
            Action { text: redoAction.text; shortcut: redoAction.shortcut; onTriggered: redoAction.trigger() }
            MenuSeparator {}
            Action { text: boldAction.text; shortcut: boldAction.shortcut; onTriggered: boldAction.trigger() }
            Action { text: italicAction.text; shortcut: italicAction.shortcut; onTriggered: italicAction.trigger() }
            Action { text: inlineCodeAction.text; shortcut: inlineCodeAction.shortcut; onTriggered: inlineCodeAction.trigger() }
            Action { text: linkAction.text; shortcut: linkAction.shortcut; onTriggered: linkAction.trigger() }
            Action { text: imageAction.text; shortcut: imageAction.shortcut; onTriggered: imageAction.trigger() }
        }
        Menu {
            title: "View"
            width: appSettings.fontSize * 10 + 20
            Action {
                text: darkThemeAction.text
                checkable: darkThemeAction.checkable
                checked: darkThemeAction.checked
                onToggled: darkThemeAction.toggled(checked)
            }
            Action {
                text: splitAction.text
                checkable: splitAction.checkable
                checked: splitAction.checked
                onToggled: splitAction.toggled(checked)
            }
        }
    }

    // ------------------------------------------------------------------------
    // Tool bar
    // ------------------------------------------------------------------------
    ToolBar {
        id: toolbar
        width: parent.width
        background: Rectangle {
            color: appSettings.editorBg
            border.color: appSettings.editorBorder
        }

        RowLayout {
            anchors.fill: parent

            IconToolButton { text: "📄"; action: newAction }
            IconToolButton { text: "📂"; action: openAction }
            IconToolButton { text: "💾"; action: saveAction }

            ToolSeparator {}

            IconToolButton { text: "B"; font.bold: true; action: boldAction }
            IconToolButton { text: "I"; font.italic: true; action: italicAction }
            IconToolButton { text: "`"; action: inlineCodeAction }
            IconToolButton { text: "🔗"; action: linkAction }
            IconToolButton { text: "🖼️"; action: imageAction }

            Item { Layout.fillWidth: true }

            IconToolButton {
                text: appSettings.darkTheme ? "☀️" : "🌙"
                onClicked: appSettings.darkTheme = !appSettings.darkTheme
            }
        }
    }

    // ------------------------------------------------------------------------
    // Main content: split view (editor + preview)
    // ------------------------------------------------------------------------
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

    // ------------------------------------------------------------------------
    // Status bar
    // ------------------------------------------------------------------------
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
                function cursorInfo() {
                    let pos = editorView.editor.cursorPosition
                    let txt = editorView.editor.text
                    let line = 0, col = pos
                    for (let i = 0; i < pos; ++i)
                        if (txt[i] === '\n') line++
                    for (let j = pos-1; j >= 0; --j)
                        if (txt[j] === '\n') { col = pos - j - 1; break }
                    return `Line: ${line+1}, Col: ${col+1}`
                }
                text: cursorInfo()
                color: appSettings.editorText
                font.family: appSettings.fontFamily
                font.pixelSize: appSettings.fontSize
            }
        }
    }

    // ------------------------------------------------------------------------
    // Dialogs
    // ------------------------------------------------------------------------
    FileDialog {
        id: openDialog
        title: "Open Markdown File"
        nameFilters: ["Markdown files (*.md *.markdown)", "All files (*)"]
        onAccepted: editorVM.openFile(selectedFile)
    }

    FileDialog {
        id: saveDialog
        property string pendingAction: ""  // stores the action to do after save
        title: "Save Markdown File"
        fileMode: FileDialog.SaveFile
        nameFilters: ["Markdown (*.md)", "All files (*)"]
        onAccepted: {
            editorVM.saveFileAs(selectedFile)
            if (saveDialog.pendingAction !== "") {
                unsavedDialog.close()
                root.pendingAction = saveDialog.pendingAction
                saveDialog.pendingAction = ""
                root.executePendingAction()
            }
        }
        onRejected: {
            if (saveDialog.pendingAction !== "") {
                unsavedDialog.close()
                saveDialog.pendingAction = ""
                root.pendingAction = ""
            }
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

    Dialog {
        id: unsavedDialog
        modal: true
        closePolicy: Popup.NoAutoClose
        title: "Unsaved changes"
        x: (root.width - width) / 2
        y: (root.height - height) / 2
        property bool discarded: false

        ColumnLayout {
            Label { text: "Do you want to save changes?"; Layout.bottomMargin: 10 }
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Button {
                    text: "Save"
                    onClicked: {
                        if (editorVM.filePath === "") {
                            saveDialog.pendingAction = root.pendingAction
                            saveDialog.open()
                        } else {
                            editorVM.saveFile()
                            unsavedDialog.close()
                            root.executePendingAction()
                        }
                    }
                }
                Button {
                    text: "Discard"
                    onClicked: {
                        unsavedDialog.discarded = true
                        unsavedDialog.close()
                        root.executePendingAction()
                    }
                }
                Button {
                    text: "Cancel"
                    onClicked: {
                        root.pendingAction = ""
                        unsavedDialog.close()
                    }
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
}