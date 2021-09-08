import QtQuick 2.2
import QtQuick.Controls.Material 2.12

import GerberRenderer 1.0

ApplicationWindow {
    id: mainWindow
    width: 360
    height: 520
    visible: true
    title: "Delta3D - Burn"

    StackView {
        id: mainView
        initialItem: firstView
        anchors.fill: parent
    }

    Component{
        id: firstView

        Rectangle {
            anchors.fill: parent
            color: "#00B000"
            width: 80; height: 80

            Gerber {
                anchors.centerIn: parent
                width: 100; height: 100
            }
        }
    }
}
