import QtQuick
import QtQuick.Controls.Basic

Item {
    property string htmlContent: ""

    TextArea {
        id: preview
        anchors.fill: parent
        font.family: "Courier New"
        font.pointSize: 12
        wrapMode: TextArea.Wrap
        color: "#000000"
        text: "Preview will be here"
        enabled: false

        background: Rectangle {
            color: "#ffffff"
        }
    }
}