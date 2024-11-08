#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>
#include <QPainter>

#include "setupBoard.h"
#include "setupMenu.h"
#include "bridge.h"

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

    Bridge comm;
    SetupMenu setupMenu;
    SetupBoard setupBoard(&comm, &engine);

    // Connect the button's increment signal to update the counter in the label
    QObject::connect(&comm, &Bridge::incrementWhite, [&]() {
        setupMenu.setWhiteCounter(setupMenu.whiteCounter() + 1);
    });
    QObject::connect(&comm, &Bridge::incrementBlack, [&]() {
        setupMenu.setBlackCounter(setupMenu.blackCounter() + 1);
    });

    engine.rootContext()->setContextProperty("setupMenu", &setupMenu);
    engine.rootContext()->setContextProperty("comm", &comm);
    engine.rootContext()->setContextProperty("setupBoard", &setupBoard);

    // Ensure that the QML file was loaded successfully
    if (engine.rootObjects().isEmpty()) {
        qCritical("Failed to load QML file.");
        return -1;
    }

    return app.exec();
}
