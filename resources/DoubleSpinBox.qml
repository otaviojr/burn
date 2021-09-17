import QtQuick 2.8
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.1

Item {
    id: doubleSpinBox
    width: 120
    height: 40

    property var value
    signal changed(var value)

    FontLoader { id: webFont; source: "qrc:/resources/fonts/Roboto-Regular.ttf" }

    RowLayout {
        anchors.fill: parent
        height: parent.height
        Row{
            id: buttonMinusRow
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left;
            }
            width: 30

            Button {
                anchors.fill: parent
                autoRepeat: true
                text: "-"

                font {
                    pointSize: 22
                    family: webFont.name
                }

                onClicked: {
                    value = parseFloat((parseFloat(spinContent.text) - 0.05).toFixed(2));
                    //spinContent.text = value.toString();
                    doubleSpinBox.changed(value);
                }
            }
        }

        Row{
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: buttonMinusRow.right
                right: buttonPlusRow.left
            }
            width: 60
            height: parent.height

            TextField {
                id: spinContent
                anchors {
                    fill: parent
                    leftMargin: 5
                    rightMargin: 5
                }

                width: parent.width
                height: parent.height

                verticalAlignment: TextInput.AlignVCenter
                horizontalAlignment: TextInput.AlignHCenter
                inputMethodHints: Qt.ImhFormattedNumbersOnly

                text: value.toFixed(2).toString()
            }
        }

        Row{
            id: buttonPlusRow
            anchors {
                top: parent.top
                bottom: parent.bottom
                right: parent.right;
            }
            width: 30

            Button{
                anchors.fill: parent
                autoRepeat: true
                text: "+"

                font {
                    pointSize: 22
                    family: webFont.name
                }

                onClicked: {
                    value = parseFloat((parseFloat(spinContent.text) + 0.05).toFixed(2));
                    //spinContent.text = value.toString();
                    doubleSpinBox.changed(value);
                }
            }
        }
    }
}
