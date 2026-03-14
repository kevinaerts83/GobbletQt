import QtQuick
import QtQuick.Shapes

Item {
    id: root
    property color btColor: "#0A64D6"

    property real minSide: Math.min(width, height)
    property real symScale: ((minSide / 2) - minSide * 0.08) * 1.05
    property real barWidth: Math.max(2, symScale * 0.18)
    property real symHeight: symScale * 1.6

    property real cx: width / 2
    property real cy: height / 2

    Shape {
        anchors.fill: parent
        preferredRendererType: Shape.CurveRenderer

        // Vertical bar
        ShapePath {
            strokeWidth: 0
            fillColor: root.btColor

            startX: root.cx - root.barWidth/2
            startY: root.cy - root.symHeight/2

            PathLine { x: root.cx + root.barWidth/2; y: root.cy - root.symHeight/2 }
            PathLine { x: root.cx + root.barWidth/2; y: root.cy + root.symHeight/2 }
            PathLine { x: root.cx - root.barWidth/2; y: root.cy + root.symHeight/2 }
            PathLine { x: root.cx - root.barWidth/2; y: root.cy - root.symHeight/2 }
        }

        // Upper rune
        ShapePath {
            strokeColor: root.btColor
            strokeWidth: root.barWidth - 1
            fillColor: "transparent"
            capStyle: ShapePath.RoundCap
            joinStyle: ShapePath.RoundJoin

            startX: root.cx
            startY: root.cy - root.symHeight * 0.47 - 2

            PathLine {
                x: root.cx + root.symScale * 0.42
                y: root.cy - root.symHeight * 0.25
            }

            PathLine {
                x: root.cx - root.symScale * 0.42
                y: root.cy + root.symHeight * 0.25
            }
        }

        // Lower rune
        ShapePath {
            strokeColor: root.btColor
            strokeWidth: root.barWidth - 1
            fillColor: "transparent"
            capStyle: ShapePath.RoundCap
            joinStyle: ShapePath.RoundJoin

            startX: root.cx
            startY: root.cy + root.symHeight * 0.47 + 2

            PathLine {
                x: root.cx + root.symScale * 0.42
                y: root.cy + root.symHeight * 0.25
            }

            PathLine {
                x: root.cx - root.symScale * 0.42
                y: root.cy - root.symHeight * 0.25
            }
        }
    }
}
