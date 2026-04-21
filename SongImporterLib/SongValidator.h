#ifndef RULE_H
#define RULE_H
#include "OperationResult.h"
#include <QVariant>
#include <QValidator>
#include <QtQml/qqmlregistration.h>
#include <QObject>


class SongValidator : public QValidator
{
	Q_OBJECT
	Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)

public:
	using State = QValidator::State;
	SongValidator(QObject* parent = nullptr) : QValidator(parent) {}
	virtual ~SongValidator() = default;

	// Inherited via QValidator
	virtual State validate(QString&, int&) const override = 0;
	[[nodiscard]] QString errorMessage() const { return m_ErrorMessage; }

protected:

	void setErrorMessage(const QString& message) const {
		if (m_ErrorMessage == message)
		{
			return;
		}
		m_ErrorMessage = message;
		emit errorMessageChanged(m_ErrorMessage);
	}

	void clearErrorMessage() const {
		if (m_ErrorMessage.isEmpty())
		{
			return;
		}
		m_ErrorMessage.clear();
		emit errorMessageChanged(m_ErrorMessage);
	}

signals:
	void errorMessageChanged(const QString& errorMessage) const;

private:
	//This doesn't change the logical state of the object. Of course, 
	// we must try to avoid changing it in const functions, so any other suggestions are welcome.
	mutable QString m_ErrorMessage;
};
#endif