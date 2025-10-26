#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "setupBoard.h"
#include "state.h"
#include "bluetoothManager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterSingletonInstance("Gobblet", 1, 0, "GameState", State::instance());

    static BluetoothManager blueTooth;
    QObject::connect(&blueTooth, &BluetoothManager::connected, [](const QString &deviceName) {
        qDebug() << "Connected to:" << deviceName;
    });
    QObject::connect(&blueTooth, &BluetoothManager::showMessage, [](const QString &sender, const QString &message) {
        qDebug() << sender << "says:" << message;
    });
    QObject::connect(&blueTooth, &BluetoothManager::reactOnSocketError, [](const QString &error) {
        qWarning() << "Error:" << error;
    });
    qmlRegisterSingletonInstance("Gobblet", 1, 0, "BluetoothManager", &blueTooth);

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
