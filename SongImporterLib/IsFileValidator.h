#ifndef ISFILEVALIDATOR_H
#define ISFILEVALIDATOR_H

#include "SongValidator.h"

class IsFileValidator  : public SongValidator
{
	Q_OBJECT

public:
	explicit IsFileValidator(QObject *parent = nullptr);

	// Inherited via QValidator
	State validate(QString&, int&) const override;
};
#endif // !ISFILEVALIDATOR_H


