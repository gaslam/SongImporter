#ifndef RULE_H
#define RULE_H
#include "OperationResult.h"
#include <QVariant>

class Rule
{
public:
	virtual OperationResult validate(const QVariant& value) const = 0;
};
#endif