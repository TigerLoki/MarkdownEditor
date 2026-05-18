import QtQuick
import QtQuick.Controls.Basic

Item {
    signal editorTextChanged(string text)
    property alias text: editor.text

    TextArea {
        id: editor
        anchors.fill: parent
        font.family: "Courier New"
        font.pointSize: 12
        wrapMode: TextArea.Wrap
        color: "#000000"

        background: Rectangle {
            color: "#ffffff"
        }

        onTextChanged: parent.editorTextChanged(text)
    }
}