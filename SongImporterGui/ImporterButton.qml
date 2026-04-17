import QtQuick

Rectangle {
    id: control
    radius: 3
    border.width: 1.5

    property string text: ""
    property color backgroundColor: Theme.black
    property color hoverColor: Theme.lightGray
    property color pressedColor: Theme.lightBlack

    signal buttonClicked()
    signal buttonDown()
    signal buttonUp()

    color: mouseArea.pressed
           ? pressedColor
           : mouseArea.containsMouse
             ? hoverColor
             : backgroundColor

    implicitWidth: label.implicitWidth + label.font.pointSize
    implicitHeight: label.implicitHeight + label.font.pointSize

    Behavior on color{
        ColorAnimation {
            duration: 200
            easing: Easing.InOutQuad
        }
    }

    Text {
        id: label
        anchors.centerIn: parent
        color: "white"
        font.bold: true
        font.pointSize: 8
        text: control.text
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            buttonDown()
            buttonClicked()
        }
        onReleased: {
            buttonUp()
        }
    }
}