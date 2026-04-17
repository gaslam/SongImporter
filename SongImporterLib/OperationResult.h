#ifndef OPERATIONRESULT_H
#define OPERATIONRESULT_H
#include <QMetaType>

struct OperationResult {
	Q_GADGET
	Q_PROPERTY(bool isSuccessful MEMBER bIsSuccessful)
	Q_PROPERTY(QString errorMessage MEMBER errorMessage)

public:
	OperationResult(const bool succesfull = true, const QString& error = "") : 
		bIsSuccessful(succesfull), 
		errorMessage(error)
	{

	}
	const bool bIsSuccessful;
	const QString errorMessage;
};
Q_DECLARE_METATYPE(OperationResult)

#endif // !OPERATIONRESULT_H

