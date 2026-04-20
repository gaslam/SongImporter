#include "IsFileRule.h"
#include <QFileInfo>
#include <QUrl>

IsFileRule::IsFileRule(QObject* parent)
	: Rule(parent)
{}

IsFileRule::~IsFileRule()
{}

OperationResult IsFileRule::validate(const QVariant& value) const
{

	QFileInfo fileInfo{ };
	switch (value.typeId())
	{
	case QMetaType::QUrl:
	{
		const QUrl url = value.toUrl();
		if (!url.isLocalFile())
		{
			return OperationResult::fail("The path does not point to a local file");
		}
		fileInfo = QFileInfo{ url.toLocalFile() };
	}
	break;

	case QMetaType::QString:
		fileInfo = QFileInfo{ value.toString() };
		break;
	default:
		return OperationResult::fail("Cannot read any text from the input field");
	}

	if (fileInfo.isDir())
	{
		return OperationResult::fail("The path does not point to a file");
	}

	if (!fileInfo.exists())
	{
		return OperationResult::fail("The file does not exist");
	}

	if (!fileInfo.isReadable())
	{
		return OperationResult::fail("The file is not readable");
	}

	return OperationResult::succeed();
}

