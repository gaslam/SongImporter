import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion


GridLayout {
    id: root
    rows: 3
    //Allows for buttons to be added
    columns:2

    Layout.fillWidth: true
    anchors.top: parent.top

    property string headerText: "header"
    property string valueText: "value"
    property alias inputValidator: input.validator
    property alias inputVerticalCenter: input.verticalCenter

signal inputTextChanged(string text)

    clip: true
    Layout.minimumWidth: 100

    Text {
        Layout.row: 0
        text: headerText
        color: Theme.black
        font.pointSize: 8
        Layout.leftMargin: 4

        Layout.fillWidth: true
        wrapMode: Text.NoWrap
        elide: Text.ElideRight
        Layout.preferredHeight: 16
    }

    TextField {
        Layout.row: 1
        id: input
        Layout.preferredHeight: 28
        text: root.valueText
        activeFocusOnTab: true
        font.pointSize: 10
        cursorVisible: activeFocus

        Layout.fillWidth: true
        Layout.minimumWidth: 0      // allow shrinking!
        Layout.maximumWidth: root.width

        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter

        wrapMode: Text.NoWrap
        clip: true
        padding: 4
        color: Theme.black
        onTextEdited:{
            root.valueText = text;
            inputTextChanged(text)
        }

        background: Rectangle {
            id: rectangle
            border.color: Theme.black
            border.width: 1.5
            radius: 3
            color: "transparent"

            Behavior on color {
                ColorAnimation {
                    duration: 150
                    easing.type: Easing.InOutQuad
                }
            }
        }

        onActiveFocusChanged: {
            var color = Theme.mutedBlueGray
            rectangle.color = activeFocus
                ? Qt.rgba(color.r, color.g, color.b, 0.5)
                : "transparent"
        }
    }

    // --- Bottom long text (wraps) ---
    Text {
        Layout.row: 2
        text: inputValidator !== null ? inputValidator.errorMessage : ""
        color: "red"
        font.pointSize: 8

        Layout.fillWidth: true
        wrapMode: Text.WordWrap   // adapts to narrow width

        Layout.leftMargin: 4
        elide: Text.ElideRight
        maximumLineCount: 3
        Layout.preferredHeight: 36
    }
}
