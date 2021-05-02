import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Shapes 1.12

import Charts 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

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

    PieChart {
        id: pie
        x: 100; y: 100
        width: 100; height: 100
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
    }

    Shape {
        id: gobble
        x: 100; y: 100
        width: 100; height: 100
        name: "A simple pie chart"
        property bool isBlue: true
        color: isBlue ? "blue" : "red"

        MouseArea {
           anchors.fill: parent
           onClicked: {
               gobble.isBlue = !gobble.isBlue
           }
           drag.target: gobble
           drag.axis: Drag.XAndYAxis
           drag.minimumX: 0
           drag.minimumY: 0
       }
    }
}
