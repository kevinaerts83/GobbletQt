import QtQuick
import QtQuick.Shapes

Item {
    id: root

    property color startColor: "black"
    property color stopColor: "#939393"

    Item {
        id: designLayer
        width: 1200
        height: 1200
        anchors.centerIn: parent
        scale: Math.min(parent.width, parent.height) / 1100

        Shape {
            anchors.fill: parent
            layer.enabled: true // cache shape
            preferredRendererType: Shape.CurveRenderer

            // BODY
            ShapePath {
                strokeWidth: 0

                fillGradient: RadialGradient {
                    centerX: 600
                    centerY: 700
                    centerRadius: 500

                    focalX: 600
                    focalY: 420

                    GradientStop { position: 0.0; color: root.startColor }
                    GradientStop { position: 0.7; color: root.stopColor }
                }

                startX: 931
                startY: 836

                PathCubic {
                    x: 574; y: 1052
                    control1X: 917; control1Y: 1021
                    control2X: 722; control2Y: 1052
                }

                PathCubic {
                    x: 216; y: 836
                    control1X: 425; control1Y: 1052
                    control2X: 230; control2Y: 1021
                }

                PathCubic {
                    x: 574; y: 353
                    control1X: 201; control1Y: 627
                    control2X: 340; control2Y: 353
                }

                PathCubic {
                    x: 931; y: 836
                    control1X: 808; control1Y: 353
                    control2X: 947; control2Y: 627
                }

                // Line back to start to close the path
                PathLine {
                    x: 931; y: 836
                }

            }

            // HEAD
            ShapePath {
                strokeWidth: 0

                fillGradient: RadialGradient {
                    centerX: 600
                    centerY: 50
                    centerRadius: 500

                    focalX: 600
                    focalY: 170

                    GradientStop { position: 0.0; color: root.startColor }
                    GradientStop { position: 0.7; color: root.stopColor }
                }

                startX: 812
                startY: 253

                PathCubic {
                    x: 574; y: 491
                    control1X: 812; control1Y: 384
                    control2X: 705; control2Y: 491
                }

                PathCubic {
                    x: 336; y: 253
                    control1X: 442; control1Y: 491
                    control2X: 336; control2Y: 384
                }

                PathCubic {
                    x: 574; y: 15
                    control1X: 336; control1Y: 121
                    control2X: 442; control2Y: 15
                }

                PathCubic {
                    x: 812; y: 253
                    control1X: 705; control1Y: 15
                    control2X: 812; control2Y: 121
                }

                // Line back to start to close the path
                PathLine {
                    x: 812; y: 253
                }
            }
        }
    }
}
