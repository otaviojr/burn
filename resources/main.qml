import QtQuick 2.8
import QtQuick.Controls.Material 2.12
import QtWebSockets 1.0
import Qt5Compat.GraphicalEffects
import QtQuick.VirtualKeyboard

import GerberRenderer 1.0
import GerberRendererMirror 1.0
import WifiModel 1.0

import "."
import "pages"

ApplicationWindow {
    id: mainWindow
    width: 800
    height: 480
    visible: true
    title: "Delta 3D - UV Exposure"

    property variant clients: []
    property variant clientCounter: 0

    property alias mainWindow : mainWindow

    function startPresentation(){
        if(gerber.hasProject){
            var fileNames = [];
            gerber.fileNames.forEach(name => fileNames.push(name));
            sendCommandTextMessage(JSON.stringify({
                action: "play",
                fileNames: fileNames,
                mirror: gerber.mirror,
                rotate: gerber.rotate,
                negative: gerber.negative,
                dpix: gerber.dpix,
                dpiy: gerber.dpiy
            }));
        }
    }

    function endPresentation(){
        if(gerber.hasProject){
            sendCommandTextMessage(JSON.stringify({
                action: "stop"
            }));
        }
    }

    function sendCommandTextMessage(message){
        clients.forEach(socket => {
            socket.sendTextMessage(message);
        });
    }

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
                id: titleCanvas
                anchors {
                        top: parent.top
                        left: parent.left
                }

                property real viewScale: parent.scale

                width: parent.width * viewScale
                height: parent.height * viewScale

                scale: 1.0/viewScale
                antialiasing: true
                transformOrigin: Item.TopLeft

                onPaint:{
                     var context = getContext("2d");

                     context.beginPath();
                     context.moveTo(0, 0);
                     context.lineTo(titleCanvas.width-76, 0);
                     context.lineTo(titleCanvas.width-109, titleCanvas.height);
                     context.lineTo(0, titleCanvas.height);
                     context.closePath();

                     context.fillStyle = "#909090";
                     context.fill();

                     context.beginPath();
                     context.moveTo(0, 0);
                     context.lineTo(titleCanvas.width-80, 0);
                     context.lineTo(titleCanvas.width-110, titleCanvas.height-3);
                     context.lineTo(0, titleCanvas.height-3);
                     context.closePath();

                     context.fillStyle = "#0000FF";
                     context.fill();
                 }

                 FontLoader { id: webFont; source: "qrc:/resources/fonts/Roboto-Bold.ttf" }

                 Text {
                     anchors {
                         top: parent.top
                         left: parent.left
                         right: parent.right
                         leftMargin: 10
                     }
                     height: parent.height - 3
                     text: "Delta 3D - UV Exposure - Mon: " + clientCounter
                     verticalAlignment: Text.AlignVCenter
                     font{
                         pointSize: 24
                         family: webFont.name
                     }
                     color: "white"
                }

                Image {
                    anchors {
                        right: parent.right
                        top: parent.top
                        topMargin: 15
                        rightMargin: 30
                    }

                    width: 30
                    height: 30

                    source: wifiModel.isConnected ? "images/v1/wifi.svg" : "images/v1/wifi-no-signal.svg"

                    MouseArea {
                        anchors.fill: parent
                        enabled: true
                        onClicked: {
                            wifiModel.startScan();
                            wifiList.open();
                        }
                    }
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
                    dpix: 316.50
                    dpiy: 320.10
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
                    id: gerber_settings_btn
                    visible: gerber.hasProject
                    anchors{
                        bottom: parent.bottom
                        right: parent.right
                        bottomMargin: gerber_settings_btn.down ? 248 : 250
                        rightMargin: gerber_settings_btn.down ? 8 : 10
                    }
                    text: "Config."
                    width: 90
                    height: 100
                    onClicked: {
                        gerberConfigDialog.open()
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
                        startDialog.open();
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
            BurnButton {
                id: global_settings_btn
                anchors{
                    bottom: parent.bottom
                    left: parent.left
                    bottomMargin: global_settings_btn.down ? 8 : 10
                    leftMargin: global_settings_btn.down ? 8 : 10
                }
                text: "Ajustes"
                width: 90
                height: 100
                onClicked: {
                    startPresentation();
                    configDialog.open()
                }
            }
        }
    }

    Drawer {
        id: wifiList
        edge: Qt.RightEdge

        width: 300
        height: parent.height

        ListView {
            anchors {
                top: parent.top
                left: parent.left
            }
            width: parent.width; height: parent.height

            WifiModel {
                id: wifiModel
            }

            Component {
                id: networkNode

                Item{
                    height: 75;
                    width: 300;

                    Rectangle {
                        anchors{
                            fill: parent
                            leftMargin: 5
                            rightMargin: 5
                            topMargin: 2.5
                            bottomMargin: 2.5
                        }
                        color: connected ? "#00FF00" : "#C0C0C0"

                        border {
                            width: 1
                            color: "black"
                        }

                        Image {
                            id: wifiImg
                            anchors {
                                left: parent.left
                                top: parent.top
                                topMargin: 16
                                leftMargin: 8
                            }

                            width: 30
                            height: 30

                            source: (strength != 0 ?
                                        (strength < 50 ? "images/v1/wifi-low.svg" :
                                            (strength < 80 ? "images/v1/wifi-medium.svg" : "images/v1/wifi.svg"))
                                    : "images/v1/wifi-no-signal.svg")
                        }

                        Text {
                            id: networkNameTxt
                            anchors {
                                left: wifiImg.right
                                top: parent.top
                                right: parent.right
                                topMargin: 3
                                leftMargin: 8
                            }

                            font{
                                pointSize: 18
                                family: webFont.name
                            }

                            elide: Text.ElideMiddle

                            verticalAlignment: Text.AlignVCenter
                            text: name
                            height: 30
                            color: "black"
                        }

                        Text {
                            anchors {
                                left: parent.left
                                top: networkNameTxt.bottom
                                right: parent.right
                                leftMargin: 46
                                topMargin: 0
                            }
                            font{
                                pointSize: 14
                                family: webFont.name
                            }
                            verticalAlignment: Text.AlignVCenter
                            height: 30
                            text: strength.toFixed(0) + " dbs"
                            color: "black"
                        }

                        Image {
                            id: wifiProtectedImg
                            anchors {
                                right: parent.right
                                bottom: parent.bottom
                                bottomMargin: 5
                                rightMargin: 5
                            }

                            width: 10
                            height: 15

                            source: type == "open" ?  "images/v1/lock-open.svg" : "images/v1/lock.svg"
                        }

                        Image {
                            id: wifiKnownImg
                            anchors {
                                right: wifiProtectedImg.left
                                bottom: parent.bottom
                                bottomMargin: 5
                                rightMargin: 5
                            }

                            width: 15
                            height: 15

                            visible: known

                            source: "images/v1/star.svg"
                        }

                        MouseArea {
                            anchors.fill: parent
                            enabled: true
                            onClicked: {
                                wifiOptionsDialog.networkId = id
                                wifiOptionsDialog.networkName = name
                                wifiOptionsDialog.connected = connected
                                wifiOptionsDialog.known = known
                                wifiOptionsDialog.type = type
                                wifiOptionsDialog.open();
                            }
                        }
                    }
                }
            }

            model: wifiModel
            delegate: networkNode
        }
    }

    InputPanel {
        id: inputPanel
        y: Qt.inputMethod.visible ? parent.height - inputPanel.height : parent.height
        anchors.left: parent.left
        anchors.right: parent.right
    }

    WifiOptionsDialog {
        id: wifiOptionsDialog
        known: true
        connected: false

        onRejected: {
        }

        onConnectNetwork: {
            if(wifiOptionsDialog.type == "psk" && !wifiOptionsDialog.known){
                wifiPasswordDialog.networkId = wifiOptionsDialog.networkId
                wifiPasswordDialog.networkName = wifiOptionsDialog.networkName
                wifiPasswordDialog.connected = wifiOptionsDialog.connected
                wifiPasswordDialog.known = wifiOptionsDialog.known
                wifiPasswordDialog.type = wifiOptionsDialog.type
                wifiPasswordDialog.password = "";
                wifiPasswordDialog.open()
                wifiPasswordDialog.focus = true
            } else {
                wifiList.close();
                wifiModel.connectNetwork(networkId);
            }
        }

        onDisconnectNetwork: {
            wifiList.close();
            wifiModel.disconnectNetwork(networkId);
        }

        onForgetNetwork: {
            wifiList.close();
            wifiModel.forgetNetwork(networkId);
        }
    }

    WifiPasswordDialog {
        id: wifiPasswordDialog
        focus: true
        onConnectNetwork: {
            wifiModel.connectNetworkWithPassphrase(wifiPasswordDialog.networkId, wifiPasswordDialog.password);
        }
    }

    StartDialog {
        id: startDialog
    }

    ConfigDialog {
        id: configDialog
    }

    GerberConfigDialog {
        id: gerberConfigDialog
    }

    ExecutingDialog {
        id: executingDialog
    }

    MouseArea {
        anchors.fill: parent
        enabled: false
        cursorShape: Qt.BlankCursor
    }

    WebSocketServer {
        id: server
        listen: true
        port: 6969
        onClientConnected: function(webSocket){
            clients.push(webSocket);
            clientCounter++;

            webSocket.onTextMessageReceived.connect(function(message) {
            });

            webSocket.onStatusChanged.connect(function(status){
                if(status !== WebSocket.Open){
                    var index = clients.indexOf(webSocket);
                    if (index > -1) {
                      clients.splice(webSocket, 1);
                      clientCounter--;
                    }
                }
            });
        }
        onErrorStringChanged: {
            appTitle = "Error";
        }
    }
}
