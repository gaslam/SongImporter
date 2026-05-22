#include "songAnalyzer.h"

SongAnalyzer::SongAnalyzer(const QString& file, QObject *parent)
    : QObject{parent},
    m_FileToProcess{file}
{}
