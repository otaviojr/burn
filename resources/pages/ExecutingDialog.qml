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
                    id: counterCanvas

                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        top: parent.top
                    }

                    property real viewScale: executingDialog.scale

                    scale: 1.0/viewScale
                    antialiasing: true
                    transformOrigin: Item.TopLeft

                    width: (executingDialog.width - executingDialog.leftPadding - executingDialog.rightPadding) * viewScale
                    height: (executingDialog.height - executingDialog.topPadding - executingDialog.bottomPadding - 50) * viewScale

                    FontLoader { id: webFont; source: "qrc:/resources/fonts/Roboto-Bold.ttf" }

                    onPaint:{
                         var context = getContext("2d");

                         context.clearRect(0, 0, counterCanvas.width, counterCanvas.height);

                         context.beginPath();
                         context.arc(counterCanvas.width/2, (counterCanvas.height/2), (Math.min(counterCanvas.height, counterCanvas.width)/2)-10, 0, 2 * Math.PI);
                         context.strokeStyle = '#A0A0A0';
                         context.lineWidth = 2 * viewScale;
                         context.stroke();
                         if(targetCounter > 0){
                             context.beginPath();

                             context.arc(counterCanvas.width/2, (counterCanvas.height/2),
                                (Math.min(counterCanvas.height, counterCanvas.width)/2)-10, - Math.PI/2,
                                ((2 * Math.PI*counter)/targetCounter) - Math.PI/2);

                             context.strokeStyle = '#0000FF';
                             context.lineWidth = 4 * viewScale;
                             context.stroke();
                         }

                         context.fillStyle = "#0000FF";
                         //context.font = '20px ' + webFont.name;
                         context.font='bold 40px "%1"'.arg(webFont.name);
                         var minutes = Math.floor((targetCounter - counter) / 60);
                         var seconds = Math.ceil((targetCounter - counter) % 60);

                         var textString = String(minutes).padStart(2, '0') + ":" + String(seconds).padStart(2, '0'),
                         textMeasure = context.measureText(textString);
                         context.textBaseline = "middle"
                         context.fillText(textString , (counterCanvas.width/2) - (textMeasure.width / 2),
                                            counterCanvas.height/2);
                     }
                 }

                 Button {
                     anchors {
                         top: counterCanvas.bottom
                         bottom: parent.bottom
                         horizontalCenter: parent.horizontalCenter
                         topMargin: 10
                     }

                     text: "Cancelar"

                     onClicked: {
                         mainWindow.endPresentation();
                         executingDialog.close();
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
                executingDialog.close();
            }
            counterCanvas.requestPaint();
        }
    }

}
