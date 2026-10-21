#ifndef OPERATIONRESULT_H
#define OPERATIONRESULT_H
#include <QMetaType>
#include "SongImporterLib_global.h"


/// This is a struct that helps certain parts of the code.
/// This way, the code doesn't have to rely on throwing an exception
/// Can also be expanded to log the messages.
/// TODO: Optional logging of messages.
struct SONGIMPORTERLIB_EXPORT OperationResult {
	Q_GADGET
	Q_PROPERTY(bool isSuccessful MEMBER bIsSuccessful)
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

	explicit operator bool() const { return bIsSuccessful; }
};
Q_DECLARE_METATYPE(OperationResult)

#endif // !OPERATIONRESULT_H

