import QtQuick
import QtQuick.Shapes

Item {
    id: root
    property color figureColor: "#A3A3A3"

    Item {
        id: designLayer
        width: 120
        height: 120
        anchors.centerIn: parent
        scale: Math.min(parent.width, parent.height) / 200

        Shape {
            anchors.fill: parent
            layer.enabled: true // cache shape
            preferredRendererType: Shape.CurveRenderer

            scale: Math.min(width, height) / 120

            // BODY
            ShapePath {
                strokeWidth: 0

                fillGradient: RadialGradient {
                    centerX: 60
                    centerY: 70
                    centerRadius: 50

                    focalX: 60
                    focalY: 42

                    GradientStop { position: 0.0; color: "white" }
                    GradientStop { position: 0.8; color: "#939393" }
                }

                startX: 93.1
                startY: 83.6

                PathCubic {
                    x: 57.4; y: 105.2
                    control1X: 91.7; control1Y: 102.1
                    control2X: 72.2; control2Y: 105.2
                }

                PathCubic {
                    x: 21.6; y: 83.6
                    control1X: 42.5; control1Y: 105.2
                    control2X: 23.0; control2Y: 102.1
                }

                PathCubic {
                    x: 57.4; y: 35.3
                    control1X: 20.1; control1Y: 62.7
                    control2X: 34.0; control2Y: 35.3
                }

                PathCubic {
                    x: 93.1; y: 83.6
                    control1X: 80.8; control1Y: 35.3
                    control2X: 94.7; control2Y: 62.7
                }

                // Line back to start to close the path
                PathLine {
                    x: 93.1; y: 83.6
                }

            }

            // HEAD
            ShapePath {
                strokeWidth: 0

                fillGradient: RadialGradient {
                    centerX: 60
                    centerY: 5
                    centerRadius: 50

                    focalX: 60
                    focalY: 17

                    GradientStop { position: 0.0; color: "white" }
                    GradientStop { position: 0.7; color: "#939393" }
                }

                startX: 81.2
                startY: 25.3

                PathCubic {
                    x: 57.4; y: 49.1
                    control1X: 81.2; control1Y: 38.4
                    control2X: 70.5; control2Y: 49.1
                }

                PathCubic {
                    x: 33.6; y: 25.3
                    control1X: 44.2; control1Y: 49.1
                    control2X: 33.6; control2Y: 38.4
                }

                PathCubic {
                    x: 57.4; y: 1.5
                    control1X: 33.6; control1Y: 12.1
                    control2X: 44.2; control2Y: 1.5
                }

                PathCubic {
                    x: 81.2; y: 25.3
                    control1X: 70.5; control1Y: 1.5
                    control2X: 81.2; control2Y: 12.1
                }

                // Line back to start to close the path
                PathLine {
                    x: 81.2; y: 25.3
                }
            }
        }
    }
}
