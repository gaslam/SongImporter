import QtQuick 2.3
import QtQuick.Layouts

RowLayout {
	id: root
    width:parent.width

	property string headerText: "header"
	property string inputText: "value"
	property string buttonText: "button"
	property alias inputValidator: inputField.inputValidator

	signal buttonClicked()

	InputField {
		id: inputField
		headerText: root.headerText

		/* Bind the internal value to the root so external assignments update the visible text,
		   and keep the two sides in sync when the user edits the field. */
		inputText: root.inputText

		onInputTextChanged: {
			// propagate user edits back to the component property and emit signal
			root.inputText = inputField.inputText
		}
	}

	ImporterButton {
		id: button
		text: root.buttonText
		onButtonClicked: root.buttonClicked()
	}

	Component.onCompleted: {
		// ensure initial sync (optional)
		inputField.inputText = root.inputText
	}

	//Somehow this works
	function setInputText(selectedFolder)
	{
		inputField.inputText = selectedFolder
	}
}
