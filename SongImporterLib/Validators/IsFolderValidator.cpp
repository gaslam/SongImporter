#include "IsFolderValidator.h"
#include <QDir>

using State = QValidator::State;

IsFolderValidator::IsFolderValidator(QObject *parent)
	: SongValidator(parent)
{}

State IsFolderValidator::validate(QString& value, int&) const
{
    /// Check if the value is null or empty. Even with whitespaces.
    if (value.isNull() || value.isEmpty() || value.trimmed().isEmpty())
    {
        setErrorMessage("Directory is empty. Enter a valid directory!");
		return State::Intermediate;
    }

    QDir actualDirectory{ value };

    /// Does the directory exist
    if (!actualDirectory.exists())
    {
        setErrorMessage("Directory does not exist. Enter a valid directory!");
        return State::Intermediate;
    }

    /// Is the directory readable?
    if (!actualDirectory.isReadable())
    {
        setErrorMessage("Directory is not readable. Enter a valid directory!");
        return State::Intermediate;
    }

    /// If the previous condiations are not met, clear the error message and return an acceptable state.
    clearErrorMessage();
    return State::Acceptable;
}

