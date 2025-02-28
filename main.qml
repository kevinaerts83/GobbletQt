import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 1500
    height: 800
    title: "Gobblet"

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: "menu.qml"
        visible: true
        objectName: "stackview"
    }

    Connections {
        target: setupMenu
        function onBlackCounterChanged() {
            stackView.push("menu.qml");
        }
        function onWhiteCounterChanged() {
            stackView.push("menu.qml");
        }
    }

}
