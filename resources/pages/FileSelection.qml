import QtQuick 2.2
import QtQuick.Controls.Material 2.12
import Qt5Compat.GraphicalEffects
import Qt.labs.folderlistmodel 2.15

Item{
    property var basePath: ""

    Text {
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: 60
        id: titleView
        font {
            pointSize: 20
        }
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: "Selecione uma arquivo para abrir"
    }

    GridView {
        id: filesListView

        cellWidth: 120
        cellHeight: 120

        clip: true

        anchors {
            top: titleView.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            leftMargin: 30
            rightMargin: 30
            bottomMargin: 30
        }

        FolderListModel {
            id: filesModel
            showFiles: true
            showDirs: true
            showDirsFirst: true
            folder: basePath
        }

        Component {
            id: fileDelegate
            Column {
                width: 120; height: 120
                Image {
                    id: itemImage
                    anchors {
                        top: parent.top
                        left: parent.left
                        right: parent.right
                    }
                    height: parent.height-itemText.height
                    width: parent.width
                    source: "../images/v1/folder.png"
                    fillMode: Image.PreserveAspectFit
                    horizontalAlignment: Image.AlignHCenter
                }
                Text {
                    id: itemText
                    anchors {
                        top: itemImage.bottom
                        left: parent.left
                        right: parent.right
                    }
                    height:30
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: fileName
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if(fileIsDir){
                            mainView.push("qrc:/resources/pages/FileSelection.qml", {basePath: fileUrl});
                        } else {
                            var fileToOpen = fileUrl.toString();
                            fileToOpen = fileToOpen.replace(/^(file:\/{3})/,"");
                            fileToOpen = decodeURIComponent(fileToOpen);

                            mainView.pop(null);

                            gerber.openProject(fileToOpen);
                        }
                    }
                }
            }
        }

        model: filesModel
        delegate: fileDelegate
    }
    Item {
        anchors.fill: parent

        Button {
            id: backBt
            text: qsTr("Voltar")
            font.pixelSize: 24

            anchors.bottom: parent.bottom
            anchors.bottomMargin: backBt.down ? 8 : 10
            anchors.right: parent.right
            anchors.rightMargin: backBt.down ? 8 : 10

            contentItem: Text {
                text: backBt.text
                font: backBt.font
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
                color: backBt.down ? "#e0e0e0" : "#f0f0f0"
            }

            onClicked: {
                mainView.pop();
            }
        }

        DropShadow {
            anchors.fill: backBt
            horizontalOffset: backBt.down ? 0 : 3
            verticalOffset: backBt.down ? 0 : 4
            radius: 9.0
            color: "#80000000"
            source: backBt
        }
    }
}
