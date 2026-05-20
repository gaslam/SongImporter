#include "IsFolderValidator.h"
#include "IsFileValidator.h"
#include "SoftwareUtils.h"
#include "FileUtils.h"
#include "Song.h"
#include "BytesTracker.h"

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
    qRegisterMetaType<Song>("song");
    qmlRegisterSingletonType<SoftwareUtils>("SongImporterLib.Utils", 1, 0, "SoftwareUtils",
        [&app](QQmlEngine*, QJSEngine*) -> QObject* {
            return new SoftwareUtils(&app);
        });
    qmlRegisterSingletonType<FileUtils>("SongImporterLib.Utils", 1, 0, "FileUtils",
        [&app](QQmlEngine*, QJSEngine*) -> QObject* {
            return new FileUtils(&app);
        });
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("SongImporterGui", "Main");

    BytesTracker tracker{};
    if(tracker.checkSpaceAvailable(QList<QUrl>{QUrl{ "E:/testfill.txt" }}, "E:/"))
    {
        qInfo() << "yey";
    }

    return app.exec();
}
