#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "MapUpdater/MapUpdater.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QObject *root_object = engine.rootObjects().first();

    MapUpdater map_updater(root_object);

    return app.exec();
}

