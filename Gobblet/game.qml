import QtQuick
import QtQuick.Window
import QtQuick.Shapes
import QtQuick.Controls
import Gobblet 1.0

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
        Mediator.resetItems(parent.width, parent.height)
        Mediator.repaint()
    }

    Matrix {
        id: matrix
        objectName: "matrix"
    }

    Board {
        id: board
        matrix: matrix
        width: parent.width
        height: parent.height

        Component.onCompleted: {
            Mediator.setBoard(this)
        }
    }

    Label {
        id: errorLabel
        width: parent.width
        color: "white"
        horizontalAlignment: "AlignHCenter"
    }

    Rectangle {
        id: gameArea
        anchors.fill: parent
        color: "transparent"

        QtObject {
            id: camera

            property real zoom: 1
            property real xAngle: 0
            property real yAngle: 0

            Component.onCompleted: {
                zoom = matrix.zoom
                xAngle = matrix.xangle
                yAngle = matrix.yangle
            }

            function apply() {
                matrix.zoom = zoom
                matrix.xangle = xAngle
                matrix.yangle = yAngle
                Mediator.repaint()
            }
        }

        MouseArea {
            anchors.fill: parent

            onPressed: (mouse)=> {
                if (!Mediator.onClick(mouse.x, mouse.y))
                    errorLabel.text = "Place on an empty square or block a possible win!"
                else
                    errorLabel.text = ""
            }
        }

        PinchHandler {
            id: pinch
            target: null

            property real startZoom: 1.0

            onActiveChanged: {
                if (active)
                    startZoom = camera.zoom
            }

            onScaleChanged: {
                let newZoom = startZoom * scale
                camera.zoom = Math.max(0.5, Math.min(1.3, newZoom))
                camera.apply()
            }
        }

        DragHandler {
            id: drag
            target: null

            property real sensitivity: 0.05

            onTranslationChanged: {

                camera.yAngle -= translation.x * sensitivity

                let newAngle = camera.xAngle + translation.y * sensitivity
                camera.xAngle = Math.max(5, Math.min(90, newAngle))

                camera.apply()
            }
        }
    }

    Rectangle {
        width: 40
        height: 80
        color: "transparent"
        objectName: "meuButton"

        Column {
            anchors.centerIn: parent
            spacing: 4

            Repeater {
                model: 3
                delegate: Rectangle {
                    width: 6
                    height: 6
                    radius: 3
                    color: "white"
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
