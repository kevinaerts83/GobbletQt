#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>
#include "mediator.h"
#include "gobbler.h"
#include <QPainter>

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



    // Ensure that the QML file was loaded successfully
    if (engine.rootObjects().isEmpty()) {
        qCritical("Failed to load QML file.");
        return -1;
    }

    // Get the root object (the ApplicationWindow)
    QObject *rootObject = engine.rootObjects().first();

    // Get the list of children from the root object
    const QObjectList &children = rootObject->children();

    // Find the Column object in QML
    // QObject *mediatorObject = rootObject->findChild<QObject*>("mediator");

    // Check if the child at index 2 exists
    if (children.size() > 2) {
        QObject *child = children.at(2);  // Get the child at index 2

        // Optionally, cast to the Mediator type if you know it should be a Mediator object
        Mediator *mediator = qobject_cast<Mediator*>(child);
        if (mediator) {
            // Successfully cast, now you can call methods on the Mediator object


            Shape *myShape = new Shape();
            QQuickItem *parentItem = qobject_cast<QQuickItem *>(children.at(0));

            if (parentItem) {

                for (int i = 0; i < 24; i++) {

                    Gobbler *gobblerItem = new Gobbler(parentItem, myShape, *new Gobbler3d(150 - 15 - ((i % 4) * 30)));

                    if (gobblerItem) {
                        gobblerItem->m_matrix = qobject_cast<Matrix*>(children.at(1));
                        //gobblerItem->model = *new Gobbler3d(150);

                        gobblerItem->setDepth(i % 4);
                        gobblerItem->setSize(i % 4);
                        gobblerItem->setX3d((i > 11) ? 375 : -375);
                        gobblerItem->setY3d(0);
                        gobblerItem->setZ3d(-150 + ((((i > 11) ? i - 12 : i)/ 4) * 150));

                        gobblerItem->setWhite(i < 12);
                        gobblerItem->setProperty("id", i);
                        gobblerItem->setProperty("name", i);

                        gobblerItem->setWidth(1024);
                        gobblerItem->setHeight(768);

                        mediator->addItem(gobblerItem);
                    }
                }
            }
        } else {
            qWarning("Failed to cast child at index 2 to Mediator*");
        }
    } else {
        qWarning("Not enough children in root object to access index 2");
    }

    /*
    const QObjectList &children = rootObject->children();
    for (int i = 0; i < children.size(); ++i) {
        qDebug() << "Child at index" << i << ":" << children.at(i)->objectName()
        << "Type:" << children.at(i)->metaObject()->className();
    }
    */

    return app.exec();
}
