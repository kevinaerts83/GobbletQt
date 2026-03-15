import QtQuick
import QtQuick.Shapes

Item {
    id: root
    property real s: Math.min(width, height)

    Shape {
        anchors.fill: parent

        ShapePath {
            fillColor: "white"
            strokeWidth: 0

            startX: root.s/3 + 5
            startY: root.s/4

            PathLine {
                x: root.s/3 + 5
                y: root.s*3/4
            }

            PathLine {
                x: root.s*2/3 + 5
                y: root.s/2
            }

            PathLine {
                x: root.s/3 + 5
                y: root.s/4
            }
        }
    }
}
