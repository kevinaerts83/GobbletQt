import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Shapes 1.12
import QtQuick.Controls 2.15

import Gobbler 1.0

Window {
    readonly property int w: 1024
    readonly property int h: 768

    visible: true
    width: w
    height: h
    //color: "#006400"
    title: qsTr("Gobblet")

    Matrix {
        id:matrix
    }

    Mediator {
        id:mediator
    }

    Rectangle
    {
        anchors.fill: parent
        id : clickArea

        MouseArea {
            anchors.fill: parent
            onClicked: {

            }
            onPressed: (mouse)=> {
                mediator.onClick(matrix, mouse.x, mouse.y);
            }
        }
    }

    Gobbler {
        id: b10
        x: 0
        y: 0
        matrix: matrix
        width: w
        height: h
        name: "10"
        color: "saddlebrown"
        visible: true
        x3d : -100
        y3d : -52
        z3d : 0

        Component.onCompleted: {
            mediator.addBlackItem(this);
        }

        /*MouseArea {
            anchors.fill: parent
            drag.target: gobblerBlack30
            drag.axis: Drag.XAndYAxis
            drag.minimumX: 0
            drag.minimumY: 0
            onPositionChanged: {
                console.log('Moved', mouseX, mouseY)
            }
        }*/
    }

    Gobbler {
        id: b20 // 21 22 23 (smallest)
        x: 0
        y: 0
        matrix: matrix
        width: w
        height: h
        name: "20"
        color: "saddlebrown"
        visible: true
        x3d : -100
        y3d : -52
        z3d : 100

        Component.onCompleted: {
            mediator.addBlackItem(this);
        }

        /*MouseArea {
            anchors.fill: parent
            drag.target: gobblerBlack30
            drag.axis: Drag.XAndYAxis
            drag.minimumX: 0
            drag.minimumY: 0
            onPositionChanged: {
                console.log('Moved', mouseX, mouseY)
            }
        }*/
    }

    Gobbler {
        id: b30
        x: 0
        y: 0
        matrix: matrix
        width: w
        height: h
        name: "30"
        color: "saddlebrown"
        visible: true
        x3d : -100
        y3d : -52
        z3d : -100

        Component.onCompleted: {
            mediator.addBlackItem(this);
        }

        /*MouseArea {
            anchors.fill: parent
            drag.target: gobblerBlack30
            drag.axis: Drag.XAndYAxis
            drag.minimumX: 0
            drag.minimumY: 0
            onPositionChanged: {
                console.log('Moved', mouseX, mouseY)
            }
        }*/
    }

    Gobbler {
        id: gobblerWhite10
        matrix: matrix
        width: w
        height: h
        name: "pawn2"
        color: "ivory"
        visible: true
        x3d : 0
        y3d : 0
        z3d : 0

        Component.onCompleted: {
            mediator.addWhiteItem(this);
        }
    }

    Board {
        id: board
        x: 0
        y: 0
        matrix: matrix
        width: w
        height: h

        Component.onCompleted: {
            mediator.setBoard(this);
        }
    }

    /*Shape {
        id: tri2
        anchors.fill: parent
        ShapePath {
            strokeColor: "red"
            strokeWidth: 4
            fillColor: "red"

            startX: 10; startY: 10
            PathLine { x: tri2.width - 10; y: tri2.height - 10 }
            PathLine { x: 10; y: tri2.height - 10 }
            PathLine { x: 10; y: 10 }
        }

        SequentialAnimation on scale {
            running: tri2.visible
            NumberAnimation { from: 1; to: 4; duration: 2000; easing.type: Easing.InOutBounce }
            NumberAnimation { from: 4; to: 1; duration: 2000; easing.type: Easing.OutBack }
        }
    }*/

    // turns around y-axis
    Slider {
        x : 30
        y : 0
        value: 0
        from: -90
        to: 90
        stepSize: 1
        onValueChanged: {
            matrix.yangle = value;
            mediator.repaint();
        }
    }

    // turns around x-axis
    Slider {
        x: 0
        y : 30
        value: 90
        from: 90
        to: 35
        stepSize: 1
        orientation: "Vertical"
        onValueChanged: {
            matrix.xangle = value;
            mediator.repaint();
        }
    }

    Slider {
        x : w-50
        y : 20
        from: 1.5
        value: 1
        to: 0.2
        stepSize: 0.1
        orientation: "Vertical"
        onValueChanged: {
            matrix.zoom = value;
            mediator.repaint();
        }
    }

    Button {
        x : w-80
        y : 200
        text: "right"
        onClicked: {
            matrix.xangle = 45;
            mediator.repaint();
        }
    }

    Button {
        x : w-180
        y : 200
        text: "left"
        onClicked: {
            matrix.xangle += 10;
            mediator.repaint();
        }
    }

    /*Label {
        x: 0
        y: 200
        id: labelxaxis
    }

    Label {
        x : 200
        y: 0
        id: labelyaxis
    }*/
}
