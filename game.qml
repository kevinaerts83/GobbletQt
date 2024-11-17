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

    Button {
        text: "Go to Page 1"
        objectName: "menuButton"
        onClicked: {
            stackView.push("menu.qml");
            //showMenu();
        }
    }

}
