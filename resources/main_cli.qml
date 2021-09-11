import QtQuick 2.2
import QtQuick.Controls.Material 2.12
import QtWebSockets 1.0
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

    Gerber {
        id: gerber
        anchors.fill: parent
        property alias gerber : gerber
    }

    WebSocketServer {
        id: server
        listen: true
        onClientConnected: {
            webSocket.onTextMessageReceived.connect(function(message) {
            });
        }
        onErrorStringChanged: {
        }
    }

    /*WebSocket {
        id: socket
        url: server.url
        onTextMessageReceived: appendMessage(qsTr("Client received message: %1").arg(message))
        onStatusChanged: {
            if (socket.status == WebSocket.Error) {
                appendMessage(qsTr("Client error: %1").arg(socket.errorString));
            } else if (socket.status == WebSocket.Closed) {
                appendMessage(qsTr("Client socket closed."));
            }
        }
    }*/

}
