import QtQuick 2.8
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
            dpix: 1920/6.0
            dpiy: 1080/3.4
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
            var obj = JSON.parse(message);
            if(obj.action == "play"){
                gerber.newProject();
                obj.fileNames.forEach(name => gerber.addFileToProject(name));
                gerber.mirror = obj.mirror;
                gerber.rotate = obj.rotate;
                gerber.negative = obj.negative;
            } else if (obj.action == "config"){
                if(obj.dpix){
                    gerber.dpix = obj.dpix;
                }
                if(obj.dpiy){
                    gerber.dpiy = obj.dpix;
                }
            }
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
