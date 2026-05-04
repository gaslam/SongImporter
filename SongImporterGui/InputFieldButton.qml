import QtQuick 2.3
import QtQuick.Layouts

RowLayout {
	id: root
    width:parent.width

    property string headerText: "header"
    property string valueText: "value"
	property string buttonText : "button"
    property alias inputValidator: inputField.inputValidator

	signal buttonClicked()
	signal onInputTextChanged(string text)

	InputField{
		id: inputField
		onInputTextChanged: root.inputTextChanged(text)
		headerText: root.headerText
		valueText: root.valueText
	   ImporterButton{
			id: button
			Layout.column: 1
			Layout.row:1
			text: root.buttonText
			onButtonClicked:{
			root.buttonClicked()
			}
        }
	}
}
