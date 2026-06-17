import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import SongImporterLib.Utils
import SongImporterLib.Rules
import SongImporterGui.SongListModel
import "../Tables"
import "../Components"

Page{
	id:root
	anchors.margins: 20
	background: Rectangle{
		color: "white"
	}

    FileDialog {
        id: fileDialog
        title: "Select a music file"
        nameFilters: [
            "XML files (*.xml)"
        ]
        onAccepted: {

            fileFieldButton.valueText = FileUtils.toLocalFile(selectedFile)
        }
    }

	FileDialog {
		id: audioFileDialog
		title: "Select the audio files"
		fileMode: FileDialog.OpenFiles
		nameFilters: {

			let result = SoftwareUtils.supportedAudioFormatsString(SoftwareUtils.Rekordbox);
			let nameFilters = [];
			let fileRegex = " *."

			nameFilters.push("All files (" + fileRegex + result.join(fileRegex) + ")");

			for (let i=0;i<result.length;i++)
				nameFilters.push(result[i].toUpperCase() + " files (" + fileRegex + result[i] + ")")

			return nameFilters;

		}
		onAccepted: {
			songTable.addSongsFromFileToList(selectedFiles,folderFieldButton.valueText)
		}
	}

    FolderDialog {
		id: exportFolderDialog
		title: "Select the destination"
        onAccepted: {
            folderFieldButton.valueText = FileUtils.toLocalFile(selectedFolder)
        }
    }

	GridLayout{
		columns: 2
		rows: 4
		anchors.fill: parent
		columnSpacing: 50
		Text{
			text:"Song Importer for Rekordbox"
			font.bold: true
			font.pointSize: 15
			Layout.row: 0
			Layout.column: 0
			Layout.columnSpan: 2
			Layout.fillWidth: true
			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter
		}
		InputFieldButton{
			Layout.alignment: Qt.AlignTop
			id:fileFieldButton
			Layout.row: 1
			Layout.column: 0
			headerText: "Export file location: (default = software location)"
			valueText: SoftwareUtils.softwareDataLocation(SoftwareUtils.Rekordbox)
			buttonText: "Select"
			Layout.preferredWidth: 200
			onButtonClicked: {
				fileDialog.open()
			}
			inputValidator : IsFileValidator{
			}
		}

		InputFieldButton{
			id: folderFieldButton
			Layout.alignment: Qt.AlignTop
			Layout.row: 1
			Layout.column: 1
			headerText: "Export location:"
			buttonText: "Select"
			valueText: FileUtils.getUserMusicFolder()
			Layout.preferredWidth: 200
			onButtonClicked: {
				exportFolderDialog.open()
			}
			inputValidator: IsFolderValidator{
			}
		}


		ImporterButton{
			id: audioFileFieldButton
			Layout.row: 2
			Layout.column: 0
			text: "Import music files"
			Layout.preferredWidth: 200
			onButtonClicked: {
				audioFileDialog.open()
			}
		}
		SongTable{
			id:songTable
			Layout.row: 3
			Layout.column: 0
			Layout.columnSpan: 2
			Layout.fillWidth: true
			Layout.fillHeight: true
			FileDropArea{
				anchors.fill: parent
				onDropped: function(dropEvent)
				{
					songTable.addSongsFromFileToList(dropEvent.urls,folderFieldButton.valueText)
				}
			}
		}

		Connections {
			target: SongListModel

			function onErrorReceived(error) {
				addError(error);
			}

			property var errors: []

			function addError(error) {
				let dialog = errorDialogComponent.createObject(root, { informativeText: error });
				if (dialog) dialog.open();
			}

		}
	}


	Component {
		id: errorDialogComponent

		MessageDialog {
			text: "An error has occurred"
			buttons: MessageDialog.Ok

			onAccepted: destroy()
			onRejected: destroy()
		}
	}
}
