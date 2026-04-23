#pragma once

#include <QObject>

class SoftwareUtils  : public QObject
{
	Q_OBJECT

public:
	SoftwareUtils(QObject *parent);
	~SoftwareUtils();
};

