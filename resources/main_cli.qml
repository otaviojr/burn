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

    Rectangle {
        id: backgroundObject
        anchors.fill: parent
        color: "white"
        Gerber {
            id: gerber
            anchors.fill: parent
            realSize: true
            property alias gerber : gerber
        }
    }
    MouseArea {
        anchors.fill: parent
        enabled: false
        cursorShape: Qt.BlankCursor
    }

    WebSocket {
        id: socket
        url: "ws://localhost:6969"
        active: false
        onTextMessageReceived: {
            var projectInfo = JSON.parse(message);
            gerber.newProject();
            projectInfo.fileNames.forEach(name => gerber.addFileToProject(name));
            gerber.mirror = projectInfo.mirror;
            gerber.rotate = projectInfo.rotate;
            gerber.negative = projectInfo.negative;

        }
        onStatusChanged: {
        }
    }

    Timer {
        interval: 1000; running: true; repeat: true
        onTriggered: {
            if(!socket.active){
                socket.active = true;
            } else {
                if(socket.status === WebSocket.Error || socket.status === WebSocket.Closed){
                    socket.active = false;
                }
            }
        }
    }

}
