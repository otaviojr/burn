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

    Item {
        id: firstView
        anchors.fill: parent

        Rectangle{
            anchors.fill: parent
            color: "green"

            Gerber{
                id: gerber
                anchors.fill: parent
            }
        }
    }
}
