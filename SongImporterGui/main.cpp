#include "IsFolderValidator.h"
#include "IsFileValidator.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterUncreatableType<SongValidator>(
        "SongImporterGui", 1, 0, "SongValidator",
        "Abstract base class for custom SongValidators"
    );
    qmlRegisterType<IsFolderValidator>("SongImporterLib.Rules", 1, 0, "IsFolderValidator");
    qmlRegisterType<IsFileValidator>("SongImporterLib.Rules", 1, 0, "IsFileValidator");
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("SongImporterGui", "Main");



    return app.exec();
}
