import QtQuick 2.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.12

import ".."

Popup {
    id: executingDialog
    modal: true
    focus: true
    x: (mainWindow.width - width) / 2
    y: (mainWindow.height - height) / 2
    width: 300
    height: 300
    closePolicy: Popup.NoAutoClose

    contentHeight: executingContent.height

    property var counter: 0
    property var targetCounter: 0

    onTargetCounterChanged: {
        if(targetCounter > 0){
            counter = 0;
            mainCounter.running = true;
            mainWindow.startPresentation();
        }
    }

    RowLayout {
        id: executingContent
        Layout.fillWidth: true

        Row{
            anchors {
                top: parent.top
                bottom: parent.bottom
            }

            ColumnLayout {
                id: colContent
                spacing: 2
                width: executingDialog.width - executingDialog.rightPadding - executingDialog.leftPadding

                Canvas{
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        top: parent.top
                    }
                    id: counterCanvas
                    width: executingDialog.width - executingDialog.leftPadding - executingDialog.rightPadding
                    height: executingDialog.height - executingDialog.topPadding - executingDialog.bottomPadding
                    onPaint:{
                         var context = getContext("2d");

                         if(counter == 0){
                             context.clearRect(0, 0, counterCanvas.width, counterCanvas.height);
                         }
                         context.beginPath();
                         context.arc(counterCanvas.width/2, counterCanvas.height/2, counterCanvas.width/4, 0, 2 * Math.PI);
                         context.strokeStyle = '#A0A0A0';
                         context.lineWidth = 1;
                         context.stroke();
                         if(targetCounter > 0){
                             context.beginPath();
                             context.arc(counterCanvas.width/2, counterCanvas.height/2, counterCanvas.width/4, - Math.PI/2, ((2 * Math.PI*counter)/targetCounter) - Math.PI/2);
                             context.strokeStyle = '#0000FF';
                             context.lineWidth = 3;
                             context.stroke();
                         }
                     }
                 }
            }
        }
    }

    Timer {
        id: mainCounter
        interval: 1000; running: false; repeat: true
        onTriggered: {
            if(counter < targetCounter){
                counter ++;
            } else if(counter == targetCounter) {
                mainCounter.running = false;
                targetCounter = 0;
                counter = 0;
                mainWindow.endPresentation();
            }
            counterCanvas.requestPaint();
        }
    }

}
