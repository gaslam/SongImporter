import QtQuick
import "./Forms"

Window {
    width: 700
    height: 525
    visible: true
    title: qsTr("Hello World")

    SongExportForm{
    anchors.fill: parent
    }
}
