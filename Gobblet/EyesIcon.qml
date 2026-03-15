import QtQuick

Item {
    id: root
    property real s: Math.min(width, height)

    // Left eye
    Rectangle {
        width: root.s/5
        height: root.s/5
        radius: width/2
        color: "black"
        x: root.s/3 - width/2
        y: root.s/3 - height/2
    }

    Rectangle {
        width: root.s/10
        height: root.s/10
        radius: width/2
        color: "white"
        x: root.s/3 + 3 - width/2
        y: root.s/3 + 3 - height/2
    }

    // Right eye
    Rectangle {
        width: root.s/5
        height: root.s/5
        radius: width/2
        color: "black"
        x: root.s*2/3 - width/2
        y: root.s/3 - height/2
    }

    Rectangle {
        width: root.s/10
        height: root.s/10
        radius: width/2
        color: "white"
        x: root.s*2/3 + 3 - width/2
        y: root.s/3 + 3 - height/2
    }
}
