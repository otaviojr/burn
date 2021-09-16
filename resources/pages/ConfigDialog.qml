import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.12

import ".."

Dialog {
    id: configDialog
    modal: true
    focus: true
    title: "Global Settings"
    x: (mainWindow.width - width) / 2
    y: 0
    width: mainWindow.width/1.5

    contentHeight: configContent.height

    standardButtons: Dialog.Ok

    onAccepted: {
        configDialog.close()
    }

    RowLayout {
        id: configContent
        spacing: 10
        Layout.fillWidth: true

        Row{
            anchors {
                top: parent.top
                bottom: parent.bottom
                topMargin: 10
            }
            ColumnLayout {
                spacing: 2
                width: ((configDialog.width-80)/2)-2

                Label {
                    text: "DPI X:"
                }

                DoubleSpinBox {
                    id: c_dpiX
                    anchors.horizontalCenter: parent.horizontalCenter
                    value: gerber.dpix
                    onChanged: value => {
                        mainWindow.sendCommandTextMessage(JSON.stringify({
                            action: "config",
                            dpix: value
                        }));
                    }
                }
            }
        }

        Rectangle{
            anchors {
                top: configContent.top
                bottom: configContent.bottom
            }
            width: 2
            height: configContent.height
            color: "black"
        }

        Row{
            anchors {
                top: parent.top
                topMargin: 10
            }
            ColumnLayout {
                spacing: 2
                width: (configDialog.width-80)/3
                Label {
                    text: "DPI Y:"
                }
                DoubleSpinBox {
                    anchors.horizontalCenter: parent.horizontalCenter
                    id: c_dpiY
                    value: gerber.dpiy
                    onChanged: value => {
                        mainWindow.sendCommandTextMessage(JSON.stringify({
                            action: "config",
                            dpiy: value
                        }));
                    }
                }
            }
        }
    }
}
