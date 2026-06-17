import QtQuick 2.15
import SongImporterGui.SongListModel
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout
{
    Text{
        text: "Supported files: mp3, aiff, flac, zip"
    }

    Rectangle {
        color: Theme.darkGray
        Layout.fillWidth: true
        Layout.fillHeight: true
        Text {
            id: emptyTableText
            anchors.fill: parent
            text: "No songs were imported. Click Import by clicking the button above or drag a supported audio file into this field."
            anchors.centerIn: parent
            wrapMode: Text.Wrap
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

        }
        Component{
            id: contactDelegate
            Item {
                required property string title
                required property string artists
                required property url albumCover
                required property string album
                required property int year
                anchors.left: parent.left; anchors.right: parent.right
                Layout.fillWidth: true
                height: 50
                RowLayout {
                    anchors.fill: parent
                    spacing: 10

                    Image{
                        source: "image://albumProvider/" + albumCover

                        Layout.preferredWidth: 4
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        fillMode: Image.PreserveAspectFit
                        smooth: true

                    }

                    Text {
                        text: (!title.length !== 0 ? title :"unknown")
                        Layout.fillWidth: true
                        Layout.preferredWidth: 7
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        elide: Text.ElideRight
                        wrapMode: Text.NoWrap
                        clip: true
                    }

                    Text {
                        text: (album.length > 0 ? album : "unknown")
                        Layout.fillWidth: true
                        Layout.preferredWidth: 7
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        elide: Text.ElideRight
                        wrapMode: Text.NoWrap
                        clip: true
                    }
                    Text {
                        text: (artists.length > 0 ? artists : "unknown")
                        Layout.fillWidth: true
                        Layout.preferredWidth: 7
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        elide: Text.ElideRight
                        wrapMode: Text.NoWrap
                        clip: true
                    }
                    Text {
                        text: (year > 0 ? year : "unknown")
                        Layout.fillWidth: true
                        Layout.preferredWidth: 7
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        elide: Text.ElideRight
                        wrapMode: Text.NoWrap
                        clip: true
                    }
                }
            }
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 10

            Rectangle {
                color: Theme.black
                Layout.fillWidth: true
                Layout.preferredHeight: 20
                RowLayout
                {
                    anchors.fill: parent
                    Text{
                        text: "Cover"
                        color:"white"
                        Layout.fillWidth: true
                        Layout.preferredWidth: 3
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        clip: true
                    }
                    Text{
                        text: "Title"
                        color:"white"
                        Layout.fillWidth: true
                        Layout.preferredWidth: 5
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        wrapMode: Text.NoWrap
                        clip: true
                    }
                    Text{
                        text: "Album"
                        color:"white"
                        Layout.fillWidth: true
                        Layout.preferredWidth: 5
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        wrapMode: Text.NoWrap
                        clip: true
                    }
                    Text{
                        text: "Artist(s)"
                        color:"white"
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        Layout.preferredWidth: 5
                        wrapMode: Text.NoWrap
                        clip: true
                    }
                    Text{
                        text: "Year"
                        color:"white"
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        Layout.preferredWidth: 5
                        wrapMode: Text.NoWrap
                        clip: true
                    }
                }
            }

            ListView {
                id: listView
                model: SongListModel
                clip: true
                delegate: contactDelegate
                ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

                Layout.fillWidth: true
                Layout.fillHeight: true    // take the rest of the column under the red bar
            }
        }
    }

    function addSongsFromFileToList(files,destination)
    {
        SongListModel.addFiles(files,destination)
        emptyTableText.visible = false
    }
}
