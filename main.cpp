#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>
#include <QPainter>

#include "setupBoard.h"
#include "state.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterSingletonType<State>("Gobblet", 1, 0, "State", State::instance);

    SetupBoard setupBoard;
    engine.rootContext()->setContextProperty("setupBoard", &setupBoard);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    // Ensure that the QML file was loaded successfully
    if (engine.rootObjects().isEmpty()) {
        qCritical("Failed to load QML file.");
        return -1;
    }

    return app.exec();
}
