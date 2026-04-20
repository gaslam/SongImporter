#ifndef RULE_H
#define RULE_H
#include "OperationResult.h"
#include <QVariant>
#include <QObject>
#include <QtQml/qqmlregistration.h>


class Rule : public QObject
{
	Q_OBJECT
	QML_INTERFACE
public:
	Rule(QObject* parent = nullptr) : QObject(parent) {}
	virtual ~Rule() = default;
	virtual OperationResult validate(const QVariant& value) const = 0;
};

Q_DECLARE_INTERFACE(Rule, "Rule")
#endif