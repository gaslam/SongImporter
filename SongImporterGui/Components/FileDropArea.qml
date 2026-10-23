import QtQuick 2.3
import SongImporterLib.Utils


DropArea{
    id:root

    property color dragColor : Theme.pastelBlue
    property double rectRadius : 3
    property bool isAcceptedFileInArea : false

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

    isAcceptedFileInArea = SoftwareUtils.isFileListValid(SoftwareUtils.Rekordbox,drag.urls);

}

onPositionChanged: function(drag) {
    drag.accepted = isAcceptedFileInArea;
}

    onExited:
    {
        background.opacity = 0;
        isAcceptedFileInArea = false;
    }

    onDropped:
    {
        background.opacity = 0;
        isAcceptedFileInArea = false;
    }
}
