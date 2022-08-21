#ifndef EVENTSLISTENER_H
#define EVENTSLISTENER_H

#include <QEvent>
#include <QObject>
#include <QDebug>
#include <QDropEvent>

class EventsListener : public QObject
{
    Q_OBJECT

public:
    EventsListener(QObject * ptr) : QObject (ptr) {
    }

    Q_INVOKABLE void handleEventsOf(QObject *object) {
        if (object)
            object->installEventFilter(this);
    }

    bool eventFilter(QObject *object, QEvent *event) override {
        if(event->type() == QEvent::DragMove) {
            QDragMoveEvent *mouseEvent = static_cast<QDragMoveEvent*>(event);

            qDebug() << "Mouse position dragging (x, y): (" << mouseEvent->pos().x() << ", " << mouseEvent->pos().y() << ")";
            return false; //this is must return false or drop event will be handled by this method and drag&drop won't work correctly
        }

        return false;
    }
};

#endif // EVENTSLISTENER_H
