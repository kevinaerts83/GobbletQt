import QtQuick
import QtQuick.Controls
import Gobblet 1.0

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
        target: State
        function onBlackCounterChanged() {
            stackView.push("menu.qml");
        }
        function onWhiteCounterChanged() {
            stackView.push("menu.qml");
        }
    }

}
