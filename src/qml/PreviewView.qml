import QtQuick
import QtQuick.Controls.Basic
import MarkdownEditor

Item {
    Rectangle {
        anchors.fill: parent
        color: "transparent"
        border.color: appSettings.previewBorder
        border.width: 1
        radius: 4

        Flickable {
            id: flickable
            anchors.fill: parent
            contentWidth: parent.width - 16
            contentHeight: preview.contentHeight
            clip: true
            boundsBehavior: Flickable.StopAtBounds

            MarkdownPreview {
                id: preview
                width: flickable.width
                height: Math.max(contentHeight, flickable.height)
                markdown: editorVM.text
                theme: appSettings.currentTheme
            }
        }
    }
}