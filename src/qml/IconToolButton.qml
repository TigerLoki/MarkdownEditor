import QtQuick
import QtQuick.Controls

ToolButton {
    implicitWidth: appSettings.fontSize * 1.2 + 20
    implicitHeight: appSettings.fontSize * 1.2 + 20
    focusPolicy: Qt.NoFocus
    font.pixelSize: appSettings.fontSize
    font.family: appSettings.fontFamily
}