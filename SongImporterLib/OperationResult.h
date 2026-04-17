#ifndef OPERATIONRESULT_H
#define OPERATIONRESULT_H
#include <QMetaType>

struct OperationResult {
	Q_GADGET
	Q_PROPERTY(bool isSuccessful MEMBER bIsSuccessful READ)
	Q_PROPERTY(QString errorMessage MEMBER errorMessage)

public:
	OperationResult(const bool succesfull = true, const QString& error = "") : 
		bIsSuccessful(succesfull), 
		errorMessage(error)
	{

	}
	bool bIsSuccessful;
	QString errorMessage;

	static inline OperationResult succeed() { return OperationResult(); }
	static inline OperationResult fail(const QString& error) { 
		constexpr bool bIsSuccessful{ false };

		return OperationResult(bIsSuccessful,error);
	}
};
Q_DECLARE_METATYPE(OperationResult)

#endif // !OPERATIONRESULT_H

