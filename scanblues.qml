import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: scanblues

    property var bluetoothDevices: [] // This will be updated dynamically

    background: Rectangle {
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#5e5358" }
            GradientStop { position: 0.7; color: "#222" }
        }
    }

    Timer {
        id: refreshTimer
        interval: 1000    // 1 second
        repeat: true
        running: true
        triggeredOnStart: true
        onTriggered: {
            // Fetch latest Bluetooth device list
            // In a real app, call a C++ function or backend here
            // Example: bluetoothDevices = bluetoothManager.getDevices()
            console.log("Refreshing device list...")
            bluetoothDevices = generateDummyDevices() // For demo only
        }
    }

    function generateDummyDevices() {
        // Simulate random devices for UI testing
        var devices = []
        var count = Math.floor(Math.random() * 5) + 1
        for (var i = 0; i < count; i++) {
            devices.push({
                name: "Device " + (i + 1),
                address: "00:11:22:33:44:" + (10 + i)
            })
        }
        return devices
    }

    RowLayout {
        Button {
            text: "Connect"
            onClicked: stackView.pop();
        }
        Button {
            text: "Cancel"
            onClicked: stackView.pop();
        }
    }

    ListView {
        id: deviceList
        anchors.fill: parent
        anchors.margins: 30
        spacing: 8
        clip: true

        model: bluetoothDevices

        delegate: Rectangle {
            width: parent.width
            height: 60
            radius: 8
            color: "#f0f0f0"
            border.color: "#cccccc"
            border.width: 1
            anchors.horizontalCenter: parent.horizontalCenter

            Row {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                Column {
                    spacing: 2
                    Text {
                        text: modelData.name
                        font.bold: true
                        font.pixelSize: 16
                    }
                    Text {
                        text: modelData.address
                        font.pixelSize: 12
                        color: "#666666"
                    }
                }
            }
        }
    }

}
