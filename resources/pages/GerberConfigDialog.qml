import QtQuick 2.8
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.12

import GerberRendererMirror 1.0

Dialog {
    id: gerberConfigDialog
    modal: true
    focus: true
    title: "Gerber Settings"
    x: (mainWindow.width - width) / 2
    y: 0
    width: mainWindow.width/1.5

    contentHeight: gerberConfigContent.height

    standardButtons: Dialog.Ok

    onAccepted: {
        gerberConfigDialog.close()
    }

    RowLayout {
        id: gerberConfigContent
        spacing: 10
        Layout.fillWidth: true

        Row{
            anchors {
                top: parent.top
                bottom: parent.bottom
                topMargin: 10
            }
            ColumnLayout {
                spacing: 2
                width: ((gerberConfigDialog.width-80)/3)-2

                Label {
                    text: "Inverter Eixo:"
                }

                RadioButton {
                    text: "Nenhum"
                    checked: gerber.mirror == GerberMirror.NONE ? true : false
                    onReleased: {
                        gerber.mirror = GerberMirror.NONE
                    }
                }
                RadioButton {
                    text: "Eixo X"
                    checked: gerber.mirror == GerberMirror.X ? true : false
                    onReleased: {
                        gerber.mirror = GerberMirror.X
                    }
                }
                RadioButton {
                    text: "Eixo Y"
                    checked: gerber.mirror == GerberMirror.Y ? true : false
                    onReleased: {
                        gerber.mirror = GerberMirror.Y
                    }
                }
                RadioButton {
                    text: "Eixo X&Y"
                    checked: gerber.mirror == GerberMirror.XY ? true : false
                    onReleased: {
                        gerber.mirror = GerberMirror.XY
                    }
                }
            }
        }

        Rectangle{
            anchors {
                top: gerberConfigContent.top
                bottom: gerberConfigContent.bottom
            }
            width: 2
            height: gerberConfigContent.height
            color: "black"
        }

        Row{
            anchors {
                top: parent.top
                topMargin: 10
            }
            ColumnLayout {
                spacing: 2
                width: (gerberConfigDialog.width-80)/3

                Label {
                    text: "Exibição:"
                }

                RadioButton {
                    text: "Imagem Normal"
                    checked: gerber.negative ? false : true
                    onReleased: {
                        gerber.negative = false
                    }
                }
                RadioButton {
                    text: "Imagem Negativa"
                    checked: gerber.negative ? true : false
                    onReleased: {
                        gerber.negative = true
                    }
                }
            }
        }

        Rectangle{
            anchors {
                top: gerberConfigContent.top
                bottom: gerberConfigContent.bottom
            }
            width: 2
            height: gerberConfigContent.height
            color: "black"
        }

        Row{
            anchors {
                top: parent.top
                topMargin: 10
            }
            ColumnLayout {
                spacing: 2
                width: ((gerberConfigDialog.width-80)/3)-2

                Label {
                    text: "Rotação:"
                }

                RadioButton {
                    text: "Nenhuma"
                    checked: gerber.rotate == 0 ? true : false
                    onReleased: {
                        gerber.rotate = 0
                    }
                }
                RadioButton {
                    text: "90º"
                    checked: gerber.rotate == 90 ? true : false
                    onReleased: {
                        gerber.rotate = 90
                    }
                }
                RadioButton {
                    text: "180º"
                    checked: gerber.rotate == 180 ? true : false
                    onReleased: {
                        gerber.rotate = 180
                    }
                }
                RadioButton {
                    text: "270º"
                    checked: gerber.rotate == 270 ? true : false
                    onReleased: {
                        gerber.rotate = 270
                    }
                }
            }
        }
    }
}
