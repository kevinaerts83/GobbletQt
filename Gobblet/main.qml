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

    // Auto-start the game once both players are connected over Bluetooth.
    // BluetoothManager.connected turns true when the real BLE link is
    // established (server: a central connected to our peripheral; client:
    // the remote service was discovered and notifications are enabled).
    Connections {
        target: BluetoothManager
        function onConnectedChanged() {
            if (BluetoothManager.connected && GameState.mode === 0) {
                GameState.lock = false;
                // server plays white (1), client plays black (2)
                GameState.user = BluetoothManager.serverName ? 1
                               : (BluetoothManager.clientName ? 2 : 0);
                while (stackView.depth > 1) {
                    stackView.pop();
                }
                stackView.push("game.qml");
            }
        }
    }

}
