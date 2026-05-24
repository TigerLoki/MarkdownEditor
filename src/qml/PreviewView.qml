import QtQuick
import QtQuick.Controls.Basic
import MarkdownEditor

Item {
    property string markdownContent: ""

    Flickable {
        id: flickable
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: preview.contentHeight
        clip: true
        boundsBehavior: Flickable.StopAtBounds

        MarkdownPreview {
            id: preview
            width: flickable.width
            height: Math.max(contentHeight, flickable.height)
            markdown: markdownContent
        }
    }
}