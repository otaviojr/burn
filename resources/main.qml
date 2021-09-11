import QtQuick 2.2
import QtQuick.Controls.Material 2.12
import Qt5Compat.GraphicalEffects

import GerberRenderer 1.0
import GerberRendererMirror 1.0

import "."
import "pages"

ApplicationWindow {
    id: mainWindow
    width: 800
    height: 480
    visible: true
    title: "Delta3D - Burn"

    property alias mainWindow : mainWindow

    Item {
        id: frameView
        anchors.fill: parent

        Rectangle{
            id: titleView
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
            height: 60

            Canvas{
                anchors.fill: parent
                onPaint:{
                     var context = getContext("2d");

                     context.beginPath();
                     context.moveTo(0, 0);
                     context.lineTo(parent.width-76, 0);
                     context.lineTo(parent.width-109, parent.height);
                     context.lineTo(0, parent.height);
                     context.closePath();

                     context.fillStyle = "#909090";
                     context.fill();

                     context.beginPath();
                     context.moveTo(0, 0);
                     context.lineTo(parent.width-80, 0);
                     context.lineTo(parent.width-110, parent.height-3);
                     context.lineTo(0, parent.height-3);
                     context.closePath();

                     context.fillStyle = "#0000FF";
                     context.fill();
                 }

                 Text {
                     anchors {
                         top: parent.top
                         left: parent.left
                         right: parent.right
                         leftMargin: 10
                     }
                     height: parent.height - 3
                     text: "Delta3D - Burn"
                     verticalAlignment: Text.AlignVCenter
                     font{
                         pointSize: 24
                     }
                     color: "white"
                }
            }
        }

        StackView {
            id: mainView
            initialItem: firstView
            anchors {
                top: titleView.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            property alias mainView : mainView
        }

        Item {
            id: firstView
            anchors.fill: parent

            Rectangle {
                anchors.fill: parent
                color: "white"

                Gerber{
                    id: gerber
                    anchors {
                        fill: parent
                        rightMargin: 110
                    }
                    property alias gerber : gerber
                }
            }

            Item {
                id: buttons_area

                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    right: parent.right
                }

                width: 90

                BurnButton {
                    id: settings_btn
                    visible: gerber.hasProject
                    anchors{
                        bottom: parent.bottom
                        right: parent.right
                        bottomMargin: settings_btn.down ? 248 : 250
                        rightMargin: settings_btn.down ? 8 : 10
                    }
                    text: "Config."
                    width: 90
                    height: 100
                    onClicked: {
                        configDialog.open()
                    }
                }

                BurnButton {
                    id: start_btn
                    visible: gerber.hasProject
                    anchors{
                        bottom: parent.bottom
                        right: parent.right
                        bottomMargin: start_btn.down ? 128 : 130
                        rightMargin: start_btn.down ? 8 : 10
                    }
                    text: "Iniciar"
                    width: 90
                    height: 100
                    onClicked: {
                    }
                }

                BurnButton {
                    id: open_close_btn
                    anchors{
                        bottom: parent.bottom
                        right: parent.right
                        bottomMargin: open_close_btn.down ? 8 : 10
                        rightMargin: open_close_btn.down ? 8 : 10
                    }
                    text: gerber.hasProject ? "Fechar" : "Abrir"
                    width: 90
                    height: 100
                    onClicked: {
                        if(gerber.hasProject){
                            gerber.closeProject();
                        } else {
                            mainView.push("qrc:/resources/pages/FolderSelection.qml");
                        }
                    }
                }
            }
        }
    }

    ConfigDialog {
        id: configDialog
    }

    MouseArea {
        anchors.fill: parent
        enabled: false
        cursorShape: Qt.BlankCursor
    }
}
