import QtQuick 2.3
import SongImporterLib.Utils


DropArea{
    id:root

    property color dragColor : Theme.pastelBlue
    property double rectRadius : 3

    Rectangle {
        id:background
        radius: root.rectRadius
        color: root.dragColor
        anchors.fill: parent

        Behavior on opacity{
            NumberAnimation{
            duration: 200
            easing.type: Easing.InOutQuad
            }
        }
        Image {
            source: "qrc:/icons/logo-icon.svg"
            anchors.centerIn: parent
            width: root.height / 100 * 40
            fillMode: Image.PreserveAspectFit
        }
        opacity: 0
    }

onEntered: function(drag)
{
    background.opacity = 0.5
}

onPositionChanged: function(drag) {
    let isValid = true

    for (let i = 0; i < drag.urls.length; i++) {
        if (!SoftwareUtils.supportsAudioFormatFromUrl(
                SoftwareUtils.rekordBox,
                drag.urls[i])) {
            isValid = false
        }
    }

    drag.accepted = isValid
}

    onExited:
    {
        background.opacity = 0
    }

    onDropped:
    {
        background.opacity = 0
    }
}
