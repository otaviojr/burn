import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.12

import ".."

Dialog {
    id: wifiPasswordDialog
    modal: true
    focus: true
    x: (mainWindow.width - width) / 2
    y: 0
    width: mainWindow.width/1.5

    contentHeight: wifiPasswordContent.height
    standardButtons: DialogButtonBox.Ok | DialogButtonBox.Cancel

    property variant networkId
    property variant networkName
    property variant connected
    property variant known
    property variant type

    property alias password : wifiPasswordText.text


    signal connectNetwork

    onAccepted: {
        wifiPasswordDialog.connectNetwork()
    }

    onRejected: {
        wifiPasswordDialog.close();
    }

    RowLayout {
        id: wifiPasswordContent
        Layout.fillWidth: true

        Row{
            anchors {
                top: parent.top
                bottom: parent.bottom
            }

            ColumnLayout {
                id: colContent
                spacing: 2
                width: wifiPasswordDialog.width - wifiPasswordDialog.rightPadding - wifiPasswordDialog.leftPadding

                Text {
                    id: titleText
                    anchors.horizontalCenter: parent.horizontalCenter
                    font {
                        pointSize: 16
                    }
                    text: "Entre com a senha para a rede " + networkName
                }

                TextField {
                    id: wifiPasswordText
                    anchors{
                        left: parent.left
                        right: parent.right
                        top: titleText.bottom
                        topMargin: 10
                        leftMargin: 10
                        rightMargin: 10
                        horizontalCenter: parent.horizontalCenter
                    }
                }
            }
        }
    }
}
