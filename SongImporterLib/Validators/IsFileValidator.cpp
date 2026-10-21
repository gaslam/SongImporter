#include "IsFileValidator.h"
#include <QFileInfo>

using State = QValidator::State;

IsFileValidator::IsFileValidator(QObject* parent)
	: SongValidator(parent)
{}

State IsFileValidator::validate(QString& value, int&) const
{
        /// Check if the value is null or empty. Even with whitespaces.
	if (value.isNull() || value.isEmpty() || value.trimmed().isEmpty())
	{
		setErrorMessage("The path is empty");
		return State::Intermediate;
	}


	const QFileInfo fileInfo{ value};

    /// Is the value a directory?
	if (fileInfo.isDir())
	{
		setErrorMessage("The path does not point to a file");
		return State::Intermediate;
	}


    /// Does the file exist?
	if (!fileInfo.exists())
	{
		setErrorMessage("The file does not exist");
		return State::Intermediate;
	}

    /// Is the file readable?
	if (!fileInfo.isReadable())
	{
		setErrorMessage("The file is not readable");
		return State::Invalid;
	}

    /// If none of the conditions are met, clear the error and return an acceptable state.
	clearErrorMessage();
	return State::Acceptable;
}

