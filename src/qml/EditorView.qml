import QtQuick
import QtQuick.Controls.Basic

Rectangle {
    id: root
    color: appSettings.editorBg
    border.color: appSettings.editorBorder
    border.width: 1

    property alias text: textArea.text
    property alias editor: textArea

    Connections {
        target: editorVM
        function onCursorPositionChanged(newPos) {
            textArea.cursorPosition = newPos
            textArea.forceActiveFocus()
        }
    }

    Connections {
        target: appSettings
        function onFontSizeChanged() {
            lineNumberModel.rebuild()
        }
        function onFontFamilyChanged() {
            lineNumberModel.rebuild()
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
            font.family: appSettings.fontFamily
            font.pixelSize: appSettings.fontSize
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
        property int lastFontSize: 0

        function rebuild() {
            if (!textArea) return

            measuringText.font = textArea.font
            measuringText.width = textArea.width - textArea.leftPadding - textArea.rightPadding

            var lines = textArea.text.split('\n')
            var newLineCount = lines.length
            var fontSizeChanged = (appSettings.fontSize !== lastFontSize)
            var widthChanged = (width !== lastWidth)

            if (!fontSizeChanged && !widthChanged && newLineCount === lastLineCount) return

            lastLineCount = newLineCount
            lastWidth = width
            lastFontSize = appSettings.fontSize

            clear()
            for (var i = 0; i < lines.length; ++i) {
                measuringText.text = lines[i]
                measuringText.implicitHeight
                var lineHeight = measuringText.implicitHeight
                if (lineHeight <= 0) lineHeight = textArea.font.pixelSize * 1.2
                append({ number: i + 1, lineHeight: lineHeight })
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