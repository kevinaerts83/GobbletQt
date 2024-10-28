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
        id: mediator
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

    // turns around y-axis
    Slider {
        x : 30
        y : 0
        value: 0
        from: -180
        to: 180
        stepSize: 1
        onValueChanged: {
            matrix.yangle = value;
            mediator.repaint(matrix);
        }
    }

    // turns around x-axis
    Slider {
        x: 0
        y : 30
        value: 65
        from: 90
        to: 35
        stepSize: 1
        orientation: "Vertical"
        onValueChanged: {
            matrix.xangle = value;
            mediator.repaint(matrix);
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
            mediator.repaint(matrix);
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
