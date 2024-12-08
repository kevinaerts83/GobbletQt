import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Shapes 1.12
import QtQuick.Controls 2.15

import Gobbler 1.0

Page {
    width: 1024
    height: 768
    id: game
    objectName: "game"

    Component.onCompleted: {
        setupBoard.onGameVisible(game);
    }

    Matrix {
        id: matrix
        objectName: "matrix"
    }

    Mediator {
        id: mediator
        objectName: "mediator"
    }

    Rectangle
    {
        anchors.fill: parent
        id: clickArea
        color: "transparent"

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
        matrix: matrix
        width: 1024
        height: 768

        Component.onCompleted: {
            mediator.setBoard(this);
        }
    }

    // turns around y-axis
    Slider {
        x : 200
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
        y : 50
        value: 65
        from: 90
        to: 10
        stepSize: 1
        orientation: "Vertical"
        onValueChanged: {
            matrix.xangle = value;
            mediator.repaint(matrix);
        }
    }

    Slider {
        x : 1024-50
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


    Rectangle {
        width: 40
        height: 80
        color: "transparent"
        objectName: "meuButton"

        // Canvas for additional details
        Canvas {
            anchors.fill: parent

            onPaint: {
                // Draw additional body and head shape details
                var ctx = getContext("2d");
                ctx.clearRect(0, 0, width, height); // Clear previous drawings

                ctx.fillStyle = "black";

                // Define circle properties
                var circleRadius = 4;
                var centerX = width / 2;
                var circleSpacing = 10; // Space between circles

                // Draw three circles
                for (var i = 0; i < 3; i++) {
                    var centerY = 10 + i * circleSpacing;
                    ctx.beginPath();
                    ctx.arc(centerX, centerY, circleRadius, 0, 2 * Math.PI);
                    ctx.fill();
                }
            }
        }

        // MouseArea for button clicks
        MouseArea {
            anchors.fill: parent
            onClicked: {
                onClicked: {
                    stackView.push("menu.qml");
                }
            }
        }
    }

}
