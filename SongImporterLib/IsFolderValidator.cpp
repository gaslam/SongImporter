#include "IsFolderValidator.h"
#include <QDir>
#include <QValidator>

using State = QValidator::State;

IsFolderValidator::IsFolderValidator(QObject *parent)
	: SongValidator(parent)
{}

State IsFolderValidator::validate(QString& value, int&) const
{
    if (value.isNull() || value.isEmpty() || value.trimmed().isEmpty())
    {
        setErrorMessage("Directory is empty. Enter a valid directory!");
		return State::Intermediate;
    }

    QDir actualDirectory{ value };

    if (!actualDirectory.exists())
    {
        setErrorMessage("Directory does not exist. Enter a valid directory!");
        return State::Intermediate;
    }

    if (!actualDirectory.isReadable())
    {
        setErrorMessage("Directory is not readable. Enter a valid directory!");
        return State::Intermediate;
    }

    if (actualDirectory.isEmpty())
    {
        setErrorMessage("Directory is empty. Enter a valid directory!");
        return State::Intermediate;
    }
    clearErrorMessage();
    return State::Acceptable;
}

