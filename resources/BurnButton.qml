import QtQuick 2.2
import QtQuick.Controls.Material 2.12
import Qt5Compat.GraphicalEffects

Item {
    id: burnButton

    property alias text: buttonText.text
    property alias down: controlBt.down
    signal clicked

    Button {
        id: controlBt
        anchors.fill: parent

        contentItem: Text {
            id: buttonText
            text: controlBt.text
            font.pixelSize: 18
            opacity: enabled ? 1.0 : 0.3
            color: "black"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            implicitWidth: parent.width
            implicitHeight: parent.height
            opacity: enabled ? 1 : 0.3
            border.color: "black"
            border.width: 1
            radius: (parent.width+parent.height)/4
            color: controlBt.down ? "#e0e0e0" : "#f0f0f0"
        }

        onClicked: parent.clicked()
    }

    DropShadow {
        anchors.fill: controlBt
        horizontalOffset: controlBt.down ? 0 : 3
        verticalOffset: controlBt.down ? 0 : 4
        radius: 9.0
        color: "#80000000"
        source: controlBt
    }
}
