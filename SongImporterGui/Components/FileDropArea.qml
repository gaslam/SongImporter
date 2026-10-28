import QtQuick 2.3
import SongImporterLib.Utils

DropArea {
    id: root

    property color dragColor: Theme.pastelBlue
    property double rectRadius: 3
    property bool isAcceptedFileInArea: false
    property bool isBusy: false

    property var currentDrag: null

    enabled: !isBusy

    function validateDrag(drag) {
        isAcceptedFileInArea = !isBusy
                && SoftwareUtils.isFileListValid(
                    SoftwareUtils.Rekordbox,
                    drag.urls
                )

        drag.accepted = isAcceptedFileInArea
    }

    onEntered: function(drag) {
        background.opacity = 0.5
        currentDrag = drag
        validateDrag(drag)
    }

    onPositionChanged: function(drag) {
        currentDrag = drag
        validateDrag(drag)
    }

    onIsBusyChanged: {
        if (currentDrag !== null) {
            validateDrag(currentDrag)
        }
    }

    onExited: {
        background.opacity = 0
        isAcceptedFileInArea = false
        currentDrag = null
    }

    onDropped: {
        background.opacity = 0
        isAcceptedFileInArea = false
        currentDrag = null
    }

    Rectangle {
        id: background
        anchors.fill: parent
        radius: root.rectRadius
        color: root.dragColor
        enabled: !isBusy
        opacity: 0

        Behavior on opacity {
            NumberAnimation {
                duration: 200
                easing.type: Easing.InOutQuad
            }
        }

        Image {
            source: "qrc:/icons/logo-icon.svg"
            anchors.centerIn: parent
            width: root.height * 0.4
            fillMode: Image.PreserveAspectFit
        }
    }
}
