import QtQuick

Item {
    id: root

    Item {
        id: designLayer
        anchors.centerIn: parent
        scale: Math.min(parent.width, parent.height) / 100

        // Left eye
        Rectangle {
            width: 30
            height: 30
            radius: 15
            color: "black"
            x: -40
            y: -30
        }

        Rectangle {
            width: 12
            height: 12
            radius: 6
            color: "white"
            x: -27
            y: -17
        }

        // Right eye
        Rectangle {
            width: 30
            height: 30
            radius: 15
            color: "black"
            x: 5
            y: -30
        }

        Rectangle {
            width: 12
            height: 12
            radius: 6
            color: "white"
            x: 18
            y: -17
        }
    }
}
