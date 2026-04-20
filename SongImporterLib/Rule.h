#ifndef RULE_H
#define RULE_H
#include "OperationResult.h"
#include <QVariant>

class Rule : public QObject
{
public:
	Rule(QObject* parent = nullptr) : QObject(parent) {}
	virtual ~Rule() = default;
	virtual OperationResult validate(const QVariant& value) const = 0;
};
#endif