#pragma once

#include "Rule.h"

class IsFileRule  : public Rule
{
	Q_OBJECT
		QML_ELEMENT
		QML_IMPLEMENTS_INTERFACES(Rule)
public:
	IsFileRule(QObject *parent);
	~IsFileRule();

	// Inherited via Rule
	OperationResult validate(const QVariant& value) const override;
};

