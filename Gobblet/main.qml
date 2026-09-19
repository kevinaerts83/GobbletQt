import QtQuick
import QtQuick.Controls
import Gobblet 1.0

ApplicationWindow {
    visible: true
    title: "Gobblet"
    visibility: Window.FullScreen

    StackView {
        id: stackView
        objectName: "stackview"
        initialItem: "menu.qml"
        visible: true

        // Inset content by the system bars (status/nav) and display cutouts
        // so the app renders correctly edge-to-edge on Android 15+ (SDK 35+).
        anchors.fill: parent
        anchors.topMargin: SafeArea.margins.top
        anchors.bottomMargin: SafeArea.margins.bottom
        anchors.leftMargin: SafeArea.margins.left
        anchors.rightMargin: SafeArea.margins.right
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
