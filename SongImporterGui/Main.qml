import QtQuick
import "./Forms"

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    SongExportForm{
    anchors.fill: parent
    }
}
