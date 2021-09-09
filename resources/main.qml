import QtQuick 2.2
import QtQuick.Controls.Material 2.12
import Qt5Compat.GraphicalEffects

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
            id: title
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

        Rectangle {
            anchors {
                top: title.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            color: "white"

            Gerber{
                id: gerber
                anchors.fill: parent
            }
        }

        Item {
            anchors.fill: parent
            visible: gerber.hasProject ? false : true

            Button {
                id: controlBt
                text: qsTr("Abrir")
                font.pixelSize: 24

                anchors.bottom: parent.bottom
                anchors.bottomMargin: controlBt.down ? 8 : 10
                anchors.right: parent.right
                anchors.rightMargin: controlBt.down ? 8 : 10
                contentItem: Text {
                    text: controlBt.text
                    font: controlBt.font
                    opacity: enabled ? 1.0 : 0.3
                    color: "black"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
    
                background: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 100
                    opacity: enabled ? 1 : 0.3
                    border.color: "black"
                    border.width: 1
                    radius: 50
                    color: controlBt.down ? "#e0e0e0" : "#f0f0f0"
                }

                onClicked: {
                    gerber.openProject("/home/cage/bottom.gbr");
                }
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
    }
}
