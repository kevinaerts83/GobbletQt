#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "setupBoard.h"
#include "state.h"
#include "../blues/bluetoothManager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

#ifdef Q_OS_ANDROID
    QNativeInterface::QAndroidApplication::runOnAndroidMainThread([] {
        QJniObject activity = QNativeInterface::QAndroidApplication::context();
        QJniObject window = activity.callObjectMethod(
            "getWindow", "()Landroid/view/Window;");
        window.callMethod<void>("setDecorFitsSystemWindows", "(Z)V", false);
    });
#endif

    QQmlApplicationEngine engine;

    qmlRegisterSingletonInstance("Gobblet", 1, 0, "GameState", State::instance());

    static BluetoothManager blueTooth;
    qmlRegisterSingletonInstance("Gobblet", 1, 0, "BluetoothManager", &blueTooth);

    static Mediator mediator(&blueTooth);
    qmlRegisterSingletonInstance("Gobblet", 1, 0, "Mediator", &mediator);

    SetupBoard setupBoard(&mediator);
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
