#include "IsFileValidator.h"
#include <QFileInfo>
#include <QUrl>

using State = QValidator::State;

IsFileValidator::IsFileValidator(QObject* parent)
	: SongValidator(parent)
{}

State IsFileValidator::validate(QString& value, int&) const
{
	if (value.isNull() || value.isEmpty() || value.trimmed().isEmpty())
	{
		setErrorMessage("The path is empty");
		return State::Intermediate;
	}
	const QFileInfo fileInfo{ value};

	if (fileInfo.isDir())
	{
		setErrorMessage("The path does not point to a file");
		return State::Intermediate;
	}

	if (!fileInfo.exists())
	{
		setErrorMessage("The file does not exist");
		return State::Intermediate;
	}

	if (!fileInfo.isReadable())
	{
		setErrorMessage("The file is not readable");
		return State::Invalid;
	}
	clearErrorMessage();
	return State::Acceptable;
}

