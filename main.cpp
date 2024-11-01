#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>
#include <QPainter>
#include "setupBoard.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    SetupBoard setup(&engine);
    engine.rootContext()->setContextProperty("setupBoard", &setup);

    // Ensure that the QML file was loaded successfully
    if (engine.rootObjects().isEmpty()) {
        qCritical("Failed to load QML file.");
        return -1;
    }

    return app.exec();
}
