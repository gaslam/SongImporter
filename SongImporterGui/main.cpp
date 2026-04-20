#include "IsFolderRule.h"
#include "IsFileRule.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
	IsFolderRule isFolderRule{ &app };
    IsFileRule isFileRule{ &app };
	engine.rootContext()->setContextProperty("isFolderRule", &isFolderRule);
    engine.rootContext()->setContextProperty("isFileRule", &isFileRule);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("SongImporterGui", "Main");



    return app.exec();
}
