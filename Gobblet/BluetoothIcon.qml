import QtQuick
import QtQuick.Shapes

Item {
    id: root
    property color btColor: "#0A64D6"

    property real symScale: 44
    property real barWidth: Math.max(2, symScale * 0.18)
    property real symHeight: symScale * 1.6

    Item {
        id: designLayer
        anchors.centerIn: parent
        scale: Math.min(parent.width, parent.height) / 100

        Shape {
            anchors.fill: parent
            preferredRendererType: Shape.CurveRenderer

            // Vertical bar
            ShapePath {
                strokeWidth: 0
                fillColor: root.btColor

                startX: -root.barWidth/2
                startY: -root.symHeight/2

                PathLine { x: root.barWidth/2; y: -root.symHeight/2 }
                PathLine { x: root.barWidth/2; y: root.symHeight/2 }
                PathLine { x: -root.barWidth/2; y: root.symHeight/2 }
                PathLine { x: -root.barWidth/2; y: -root.symHeight/2 }
            }

            // Upper rune
            ShapePath {
                strokeColor: root.btColor
                strokeWidth: root.barWidth - 1
                fillColor: "transparent"
                capStyle: ShapePath.RoundCap
                joinStyle: ShapePath.RoundJoin

                startX: 0
                startY: -root.symHeight * 0.47 - 2

                PathLine {
                    x: root.symScale * 0.42
                    y: -root.symHeight * 0.25
                }

                PathLine {
                    x: -root.symScale * 0.42
                    y: root.symHeight * 0.25
                }
            }

            // Lower rune
            ShapePath {
                strokeColor: root.btColor
                strokeWidth: root.barWidth - 1
                fillColor: "transparent"
                capStyle: ShapePath.RoundCap
                joinStyle: ShapePath.RoundJoin

                startX: 0
                startY: root.symHeight * 0.47 + 2

                PathLine {
                    x: root.symScale * 0.42
                    y: root.symHeight * 0.25
                }

                PathLine {
                    x: -root.symScale * 0.42
                    y: -root.symHeight * 0.25
                }
            }
        }
    }
}
