#include "IsFolderRule.h"
#include <QDir>

IsFolderRule::IsFolderRule(QObject *parent)
	: Rule(parent)
{}

OperationResult IsFolderRule::validate(const QVariant & value) const
{
    if (value.isNull() || !value.canConvert<QString>())
    {
        return OperationResult::fail("Directory is empty. Enter a valid directory!");
    }

    const QString input{ value.toString() };

    QDir actualDirectory{ input };

    if (!actualDirectory.exists())
    {
        return OperationResult::fail("Directory does not exist. Enter a valid directory!");
    }

    if (!actualDirectory.isReadable())
    {
        return OperationResult::fail("Directory is not readable. Enter a valid directory!");
    }

    if (actualDirectory.isEmpty())
    {
        return OperationResult::fail("Directory is empty. Enter a valid directory!");
    }
	return OperationResult::succeed();
}

