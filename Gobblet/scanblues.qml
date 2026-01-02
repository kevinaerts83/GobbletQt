import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Gobblet 1.0

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
        interval: 10000 // 10 second
        repeat: true
        running: true
        triggeredOnStart: true
        onTriggered: {
            bluetoothDevices = BluetoothManager.getDevices()
        }
    }

    RowLayout {
        Button {
            text: "Cancel"
            onClicked: {
                BluetoothManager.stopDiscovery()
                stackView.pop()
            }
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

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor

                onClicked: {
                    //console.log("Clicked on device:", modelData.name, modelData.address)

                    // Call C++ function to connect to the device
                    BluetoothManager.connectWithName(modelData.name)
                    stackView.pop()
                }

                onEntered: parent.color = "#e0e0ff"   // Highlight when hovered
                onExited: parent.color = "#f0f0f0"    // Reset color when mouse leaves
            }
        }
    }

}
