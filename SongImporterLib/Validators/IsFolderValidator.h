#ifndef ISFOLDERVALIDATOR_H
#define ISFOLDERVALIDATOR_H

#include "SongValidator.h"

class SONGIMPORTERLIB_EXPORT IsFolderValidator  : public SongValidator
{
	Q_OBJECT
public:
	explicit IsFolderValidator(QObject *parent = nullptr);


	// Inherited via QValidator
	State validate(QString&, int&) const override;

};

#endif