
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <Validators/SongValidator.h>
#include <Validators/IsFolderValidator.h>
#include <Validators/IsFileValidator.h>
#include <Song/Song.h>
#include <Utils/SoftwareUtils.h>
#include <Utils/FileUtils.h>
#include <QThread>
#include <Providers/AlbumCoverProvider.h>
#include "Models/SongModel.h"


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

    QThread* providerThread{new QThread{}};
    providerThread->setObjectName("albumProvider");
    QImage image{":/icons/logo-icon.png"};
    QString defaultId{"default"};
    AlbumCoverProvider* albumCoverProvider{new AlbumCoverProvider{defaultId,image}};
    engine.addImageProvider("albumProvider",albumCoverProvider);
    albumCoverProvider->moveToThread(providerThread);
    // ensure provider is deleted when thread finishes
    QObject::connect(providerThread, &QThread::finished, albumCoverProvider, &QObject::deleteLater);
    // delete thread when app destroyed
    QObject::connect(&app, &QCoreApplication::aboutToQuit, providerThread, &QThread::quit);
    QObject::connect(providerThread, &QThread::finished, providerThread, &QObject::deleteLater);

    SongModel* model{new SongModel{albumCoverProvider,&app}};
    qmlRegisterSingletonInstance("SongImporterGui.SongListModel",1,0,"SongListModel",model);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("SongImporterGui", "Main");

    return app.exec();
}
