import QtQuick
import QtQuick.Window
import QtQuick.Shapes
import QtQuick.Controls

Page {
    id: game
    objectName: "game"

    background: Rectangle {
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#333" }
            GradientStop { position: 1.0; color: "#191919" }
        }
    }

    Component.onCompleted: {
        setupBoard.onGameVisible(game)
        Screen.orientationChanged.connect(onOrientationChanged)
    }

    function onOrientationChanged(newOrientation) {
        matrix.setCenter(parent.width, parent.height)
        mediator.resetItems(parent.width, parent.height)
        mediator.repaint()
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
                mediator.onClick(mouse.x, mouse.y)
            }
        }

        PinchHandler {
            id: pinchHandler

            // Update the scale value based on pinch
            onScaleChanged: {
                gameArea.scaleValue = scale
                gameArea.scaleValue = gameArea.scaleValue > 1.3 ? 1.3 : gameArea.scaleValue < 0.5 ? 0.5 : gameArea.scaleValue
                matrix.zoom = gameArea.scaleValue
                mediator.repaint()
            }
        }

        DragHandler {
            id: dragHandler
            target: null

            onActiveChanged: {
                if (!dragHandler.active) {
                    gameArea.lastCrossedThresholdX = 0
                    gameArea.lastCrossedThresholdY = 0
                }
            }

            onTranslationChanged: {
                const oldX = Math.floor(dragHandler.centroid.position.x / 30)
                if (oldX != gameArea.lastCrossedThresholdX) {
                    matrix.yangle += (oldX < gameArea.lastCrossedThresholdX) ? 10 : -10
                    mediator.repaint()
                    gameArea.lastCrossedThresholdX = oldX
                }

                const oldY = Math.floor(dragHandler.centroid.position.y / 30)
                if (oldY != gameArea.lastCrossedThresholdY) {

                    const newAngle = (oldY > gameArea.lastCrossedThresholdY) ? 10 : -10
                    if ((matrix.xangle + newAngle) < 5) {
                        matrix.xangle += 3
                    } else if ((matrix.xangle + newAngle) > 90) {
                        matrix.xangle -= 10
                    } else {
                        matrix.xangle += newAngle
                    }

                    mediator.repaint()
                    gameArea.lastCrossedThresholdY = oldY
                }
            }
        }
    }

    Board {
        id: board
        matrix: matrix
        width: parent.width
        height: parent.height

        Component.onCompleted: {
            mediator.setBoard(this)
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

                ctx.fillStyle = "white";

                // Define circle properties
                var circleRadius = 3;
                var centerX = width / 2;
                var circleSpacing = 7; // Space between circles

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
                stackView.push("menu.qml")
            }
        }
    }

}
