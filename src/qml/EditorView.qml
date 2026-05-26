import QtQuick
import QtQuick.Controls.Basic

Rectangle {
    id: root
    color: appSettings.editorBg
    border.color: appSettings.editorBorder
    border.width: 1

    property alias text: textArea.text
    property alias editor: textArea

    function format(prefix, suffix) {
        editorVM.applyFormatting(textArea.selectionStart, textArea.selectionEnd, prefix, suffix)
    }

    function linePrefix(prefix) {
        editorVM.applyLinePrefix(textArea.cursorPosition, prefix)
    }

    Connections {
        target: editorVM
        function onCursorPositionChanged(newPos) {
            textArea.cursorPosition = newPos
            textArea.forceActiveFocus()
        }
    }

    Row {
        anchors.fill: parent
        spacing: 0

        Item {
            width: 44
            height: parent.height
            clip: true

            Rectangle {
                anchors.fill: parent
                color: appSettings.lineNumbersBg
                border.color: appSettings.editorBorder
            }

            ListView {
                id: lineNumbersList
                anchors.fill: parent
                model: lineNumberModel
                contentY: textArea ? textArea.contentY - textArea.topPadding : 0
                boundsBehavior: Flickable.StopAtBounds
                interactive: false
                header: Item { width: lineNumbersList.width; height: textArea ? textArea.topPadding : 0 }

                delegate: Text {
                    width: lineNumbersList.width
                    height: model.lineHeight
                    font: textArea.font
                    color: appSettings.lineNumbersText
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    rightPadding: 8
                    text: model.number
                }
            }
        }

        TextArea {
            id: textArea
            width: parent.width - 44
            height: parent.height
            font.family: "Courier New"
            font.pixelSize: 16
            renderType: Text.NativeRendering
            wrapMode: TextArea.Wrap
            color: appSettings.editorText
            selectionColor: appSettings.editorSelection
            selectedTextColor: appSettings.editorSelectionText
            leftPadding: 8
            rightPadding: 8
            topPadding: 4
            bottomPadding: 4

            background: Rectangle {
                color: "transparent"
            }

            onTextChanged: {
                lineNumberModel.rebuild()
            }
            onWidthChanged: lineNumberModel.rebuild()
        }
    }

    ListModel {
        id: lineNumberModel
        property int lastLineCount: 0

        function rebuild() {
            if (!textArea) return
            var lines = textArea.text.split('\n')
            var newLineCount = lines.length
            if (newLineCount === lastLineCount && width === lastWidth) return
            lastLineCount = newLineCount
            lastWidth = width
            clear()
            for (var i = 0; i < lines.length; ++i) {
                measuringText.text = lines[i]
                append({ number: i + 1, lineHeight: measuringText.implicitHeight })
            }
        }
        property real lastWidth: 0
    }

    Text {
        id: measuringText
        visible: false
        font: textArea.font
        width: textArea ? textArea.width - textArea.leftPadding - textArea.rightPadding : 0
        wrapMode: Text.Wrap
        text: ""
    }
}