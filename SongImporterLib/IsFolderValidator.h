#ifndef ISFOLDERRULE_H
#define ISFOLDERRULE_H

#include "SongValidator.h"

class IsFolderValidator  : public SongValidator
{
	Q_OBJECT
public:
	explicit IsFolderValidator(QObject *parent = nullptr);


	// Inherited via QValidator
	State validate(QString&, int&) const override;

};

#endif