import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.12

import ".."

Dialog {
    id: wifiOptionsDialog
    modal: true
    focus: true
    x: (mainWindow.width - width) / 2
    y: 0
    width: mainWindow.width/3

    contentHeight: wifiOptionsContent.height

    property variant networkId
    property variant networkName
    property variant connected
    property variant known
    property variant type

    signal rejected
    signal connectNetwork
    signal forgetNetwork
    signal disconnectNetwork

    RowLayout {
        id: wifiOptionsContent
        Layout.fillWidth: true

        Row{
            anchors {
                top: parent.top
                bottom: parent.bottom
            }

            ColumnLayout {
                id: colContent
                spacing: 2
                width: wifiOptionsDialog.width - wifiOptionsDialog.rightPadding -  wifiOptionsDialog.leftPadding

                Text {
                    id: titleText
                    anchors.horizontalCenter: parent.horizontalCenter
                    font {
                        pointSize: 16
                    }
                    text: networkName
                }

                Button {
                    id: wifiConnect
                    anchors{
                        left: parent.left
                        right: parent.right
                        horizontalCenter: parent.horizontalCenter
                        topMargin: 30
                    }
                    text: connected ? "Desconectar" : "Conectar"
                    onClicked: {
                        wifiOptionsDialog.close()
                        if(connected){
                            wifiOptionsDialog.disconnectNetwork()
                        } else {
                            wifiOptionsDialog.connectNetwork()
                        }
                    }
                }

                Button {
                    id: wifiForget
                    visible: known
                    anchors{
                        left: parent.left
                        right: parent.right
                        horizontalCenter: parent.horizontalCenter
                        topMargin: 10
                    }
                    text: "Esquecer"
                    onClicked: {
                        wifiOptionsDialog.close()
                        wifiOptionsDialog.forgetNetwork()
                    }
                }

                Button {
                    id: wifiCancel
                    anchors{
                        left: parent.left
                        right: parent.right
                        horizontalCenter: parent.horizontalCenter
                        topMargin: 10
                    }
                    text: "Cancelar"
                    onClicked: {
                        wifiOptionsDialog.close()
                        wifiOptionsDialog.rejected()
                    }
                }
            }
        }
    }
}
