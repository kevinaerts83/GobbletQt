import QtQuick
import QtQuick.Controls
import Gobblet 1.0

ApplicationWindow {
    visible: true
    title: "Gobblet"
    visibility: Window.FullScreen

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: "menu.qml"
        visible: true
        objectName: "stackview"
    }

    Connections {
        target: GameState
        function onBlackCounterChanged() {
            stackView.push("menu.qml");
        }
        function onWhiteCounterChanged() {
            stackView.push("menu.qml");
        }
    }

}
