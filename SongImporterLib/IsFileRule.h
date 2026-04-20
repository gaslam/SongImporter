#pragma once

#include "Rule.h"

class IsFileRule  : public Rule
{
	Q_OBJECT

public:
	IsFileRule(QObject *parent);
	~IsFileRule();

	// Inherited via Rule
	OperationResult validate(const QVariant& value) const override;
};

