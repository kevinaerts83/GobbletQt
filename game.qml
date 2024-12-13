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
        id: gameArea
        anchors.fill: parent
        color: "transparent"

        property real scaleValue: 1.0
        property int lastCrossedThresholdX: 0
        property int lastCrossedThresholdY: 0

        MouseArea {
            anchors.fill: parent
            onClicked: {

            }
            onPressed: (mouse)=> {
                mediator.onClick(matrix, mouse.x, mouse.y);
            }
        }

        PinchHandler {
            id: pinchHandler

            // Update the scale value based on pinch
            onScaleChanged: {
                gameArea.scaleValue *= scale;
                gameArea.scaleValue = gameArea.scaleValue > 2 ? 2 : gameArea.scaleValue < 0.5 ? 0.5 : gameArea.scaleValue;
                matrix.zoom = gameArea.scaleValue
                mediator.repaint(matrix);
            }
        }

        WheelHandler {
            property: "rotation"
            onWheel: (event)=> console.log("rotation", event.angleDelta.y,
                                                  "scaled", rotation, "@", point.position,
                                                  "=>", parent.rotation)
            /*onWheel: {
                matrix.zoom *= rotation; // rotationScale;
                mediator.repaint(matrix);
            }*/
        }

        DragHandler {
            id: dragHandler
            target: null

            onActiveChanged: {
                if (!dragHandler.active) {
                    gameArea.lastCrossedThresholdX = 0;
                    gameArea.lastCrossedThresholdY = 0;
                }
            }

            onTranslationChanged: {
                const oldX = Math.floor(dragHandler.centroid.position.x / 50);
                if (oldX != gameArea.lastCrossedThresholdX) {
                    matrix.yangle += (oldX < gameArea.lastCrossedThresholdX) ? 10 : -10;
                    mediator.repaint(matrix);
                    gameArea.lastCrossedThresholdX = oldX;
                }

                const oldY = Math.floor(dragHandler.centroid.position.y / 50);
                if (oldY != gameArea.lastCrossedThresholdY) {

                    const newAngle = (oldY > gameArea.lastCrossedThresholdY) ? 10 : -10;
                    if ((matrix.xangle + newAngle) < 5) {
                        matrix.xangle += 1;
                    } else if ((matrix.xangle + newAngle) > 90) {
                        matrix.xangle -= 5;
                    } else {
                        matrix.xangle += newAngle;
                    }

                    mediator.repaint(matrix);
                    gameArea.lastCrossedThresholdY = oldY;
                }
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
