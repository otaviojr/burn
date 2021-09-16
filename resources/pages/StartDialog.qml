import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.12

import ".."

Dialog {
    id: startDialog
    modal: true
    focus: true
    title: "Start Proccess"
    x: (mainWindow.width - width) / 2
    y: 0
    width: mainWindow.width/1.5

    contentHeight: startContent.height
    standardButtons: DialogButtonBox.Ok | DialogButtonBox.Cancel

    property var timer: 0

    onAccepted: {
        executingDialog.targetCounter = timer
        executingDialog.open()
        startDialog.close()
    }

    RowLayout {
        id: startContent
        Layout.fillWidth: true

        Row{
            anchors {
                top: parent.top
                bottom: parent.bottom
            }

            ColumnLayout {
                id: colContent
                spacing: 2
                width: startDialog.width - startDialog.rightPadding - startDialog.leftPadding

                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    font {
                        pointSize: 16
                    }

                    id: timerTile
                    text: "Time"
                }

                Text {
                    anchors{
                        top: timerTile.bottom
                        topMargin: 10
                        horizontalCenter: parent.horizontalCenter
                    }

                    font {
                        pointSize: 24
                    }

                    id: timerText
                    text: timer + "s"
                }

                Slider {
                    anchors {
                        top: timerText.bottom
                        left: parent.left
                        right: parent.right

                        leftMargin: 20
                        rightMargin: 20
                        topMargin: 10
                    }
                    id: slider
                    value: timer
                    to: 500
                    stepSize: 5

                    onMoved: {
                        timer = value
                    }
                }
            }
        }
    }
}
