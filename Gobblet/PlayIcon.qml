import QtQuick
import QtQuick.Shapes

Item {
    id: root

    Item {
        id: designLayer
        anchors.centerIn: parent
        scale: Math.min(parent.width, parent.height) / 150

        Shape {
            anchors.fill: parent
            preferredRendererType: Shape.CurveRenderer

            ShapePath {
                fillColor: "white"
                strokeWidth: 0

                startX: -27
                startY: 45

                PathLine {
                    x: 43
                    y: 0
                }

                PathLine {
                    x: -27
                    y: -45
                }

                PathLine {
                    x: -27
                    y: 45
                }
            }
        }
    }
}
