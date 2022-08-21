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

    //property alias dropEnabled: acceptDropCB.checked
    //color: dropArea.containsDrag ? "#CFC" : "#EEE"

    /*Rectangle
    {
        anchors.fill: parent
        gradient: Gradient
        {
            GradientStop {position: 0.500;color: Qt.rgba(0, 0.5, 0, 1);}
            GradientStop {position: 1.000;color: Qt.rgba(0, 0.2, 0, 1);}
        }
    }*/

    Matrix {
        id:matrix
    }

    Gobbler {
        id: gobbler
        x: 0
        y: 0
        matrix: matrix
        width: w
        height: h
        name: "pawn"
        color: "saddlebrown"
        tx : 400
        ty : 300
        tz : 150
    }

    Gobbler {
        id: gobbler2
        x: 0
        y: 0
        matrix: matrix
        width: w
        height: h
        name: "pawn2"
        color: "ivory"
        tx : 600
        ty : 300
        tz : 300
    }

    Board {
        id: board
        x: 0
        y: 0
        matrix: matrix
        width: w
        height: h
        tx : 450
        ty : 300
        tz : 300
    }

    Rectangle {
        id: rect
        width: 100
        height: 100
        property bool isBlue: true
        color: isBlue ? "blue" : "red"
        border.color: "black"
        border.width: 5
        radius: 10

        MouseArea {
           anchors.fill: parent
           onClicked: {
               rect.isBlue = !rect.isBlue
           }
           drag.target: rect
           drag.axis: Drag.XAndYAxis
           drag.minimumX: 0
           drag.minimumY: 0
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

    /*PieChart {
        id: pie
        x: 100
        y: 100
        width: 100
        height: 100
        name: "A simple pie chart"
        property bool isBlue: true
        color: isBlue ? "blue" : "red"

        MouseArea {
           anchors.fill: parent
           onClicked: {
               pie.isBlue = !pie.isBlue
           }
           drag.target: pie
           drag.axis: Drag.XAndYAxis
           drag.minimumX: 0
           drag.minimumY: 0
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
            board.update();
            gobbler.update();
            gobbler2.update();
        }
    }

    // turns around x-axis
    Slider {
        x: 0
        y : 30
        value: 45
        from: 90
        to: 0
        stepSize: 1
        orientation: "Vertical"
        onValueChanged: {
            matrix.xangle = value;
            board.update();
            gobbler.update();
            gobbler2.update();
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
            board.update();

            gobbler.update();
            gobbler2.update();
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
