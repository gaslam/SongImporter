#ifndef ISFOLDERRULE_H
#define ISFOLDERRULE_H

#include "Rule.h"

class IsFolderRule  : public Rule
{
	Q_OBJECT
	QML_ELEMENT
	QML_IMPLEMENTS_INTERFACES(Rule)
public:
	explicit IsFolderRule(QObject *parent);

	// Inherited via Rule
	OperationResult validate(const QVariant& value) const override;
};

#endif