import QtQuick
import QtQuick.Shapes

Item {
    id: root
    property color lightcolor: "#cfcfcf"
    property color color: "#7a7a7a"
    readonly property real designSize: 420

    Item {
        id: designLayer
        anchors.centerIn: parent
        scale: Math.min(parent.width, parent.height) / 1100

        Shape {
            anchors.fill: parent
            preferredRendererType: Shape.CurveRenderer

            // LEFT BODY
            ShapePath {
                strokeColor: "black"
                fillColor: root.lightcolor

                startX: 0; startY: -110

                PathLine { x: 0; y: 300 }

                PathCubic {
                    x: -200; y: -90
                    control1X: -80; control1Y: 290
                    control2X: -150; control2Y: 150
                }

                PathCubic {
                    x: 0; y: -110
                    control1X: -150; control1Y: -105
                    control2X: -50; control2Y: -108
                }

                PathLine { x: 0; y: -110 }
            }

            // RIGHT BODY
            ShapePath {
                strokeColor: "black"
                fillColor: root.color

                startX: 0; startY: -110

                PathLine { x: 0; y: 300 }

                PathCubic {
                    x: 200; y: -90
                    control1X: 80; control1Y: 290
                    control2X: 150; control2Y: 150
                }

                PathCubic {
                    x: 0; y: -110
                    control1X: 150; control1Y: -105
                    control2X: 50; control2Y: -108
                }

                PathLine { x: 0; y: -110 }
            }

            // CENTER LINE
            /*ShapePath {
                strokeWidth: 0
                fillColor: root.color

                startX: -1; startY: -10
                PathLine { x: 1; y: -10 }
                PathLine { x: 1; y: 400 }
                PathLine { x: -1; y: 400 }
                PathLine { x: -1; y: -10 }
            }*/

            // FACE RECTANGLE
            ShapePath {
                strokeWidth: 0
                fillColor: "black"

                startX: -160; startY: -260
                PathLine { x: 160; y: -260 }
                PathLine { x: 160; y: -160 }
                PathLine { x: -160; y: -160 }
                PathLine { x: -160; y: -260 }
            }

            // LEFT EYE
            ShapePath {
                strokeWidth: 0
                fillColor: "white"

                startX: -60; startY: -220

                PathAngleArc {
                    centerX: -80
                    centerY: -220
                    radiusX: 20
                    radiusY: 20
                    startAngle: 0
                    sweepAngle: 360
                }
            }

            // RIGHT EYE
            ShapePath {
                strokeWidth: 0
                fillColor: "white"

                startX: 100; startY: -220

                PathAngleArc {
                    centerX: 80
                    centerY: -220
                    radiusX: 20
                    radiusY: 20
                    startAngle: 0
                    sweepAngle: 360
                }
            }

            // MOUTH
            ShapePath {
                strokeWidth: 0
                fillColor: "white"

                startX: -20; startY: -190
                PathLine { x: 20; y: -190 }
                PathLine { x: 20; y: -175 }
                PathLine { x: -20; y: -175 }
                PathLine { x: -20; y: -190 }
            }

            // LEFT FACE
            ShapePath {
                strokeColor: "black"
                fillColor: root.lightcolor
                strokeWidth: 1

                startX: 0
                startY: -110

                PathLine { x: -100; y: -110 }

                PathArc {
                    x: -100
                    y: -310
                    radiusX: 100
                    radiusY: 100
                    direction: PathArc.Clockwise
                }

                PathLine { x: 0; y: -310 }

                PathMove { x: 0; y: -260 }

                PathLine { x: -100; y: -260 }

                PathArc {
                    x: -100
                    y: -160
                    radiusX: 50
                    radiusY: 50
                    direction: PathArc.Counterclockwise
                }

                PathLine { x: 0; y: -160 }

                PathLine { x: 0; y: -110 }
            }


            // RIGHT FACE
            ShapePath {
                strokeColor: "black"
                fillColor: root.color
                strokeWidth: 1

                startX: 0
                startY: -110

                PathLine { x: 100; y: -110 }

                PathArc {
                    x: 100
                    y: -310
                    radiusX: 100
                    radiusY: 100
                    direction: PathArc.Counterclockwise
                }

                PathLine { x: 0; y: -310 }

                PathMove { x: 0; y: -260 }

                PathLine { x: 100; y: -260 }

                PathArc {
                    x: 100
                    y: -160
                    radiusX: 50
                    radiusY: 50
                    direction: PathArc.Clockwise
                }

                PathLine { x: 0; y: -160 }

                PathLine { x: 0; y: -110 }
            }
        }
    }
}
