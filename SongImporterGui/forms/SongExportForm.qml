import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import SongImporterLib.Utils
import SongImporterLib.Rules

Page{
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

    FolderDialog {
        id: folderDialog
        title: "Select an audio file"
        onAccepted: {
            folderFieldButton.valueText = FileUtils.toLocalFile(selectedFolder)
        }
    }

	GridLayout{
		columns: 2
		rows: 2
		anchors.fill: parent
		columnSpacing: 50
		InputFieldButton{
		    Layout.alignment: Qt.AlignTop 
			id:fileFieldButton
			Layout.row: 0
			Layout.column: 0
			headerText: "DJ software export file location:"
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
			Layout.row: 0
			Layout.column: 1
			headerText: "Export location:"
			buttonText: "Select"
			valueText: FileUtils.getUserMusicFolder()
			Layout.preferredWidth: 200
			onButtonClicked: {
				folderDialog.open()
			}
			inputValidator: IsFolderValidator{
			}
		}

		FileDropArea{
			Layout.column: 0
			Layout.columnSpan: 2
			Layout.row:1
			Layout.fillHeight: true
			Layout.fillWidth: true
		}
	}
}
