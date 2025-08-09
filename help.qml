import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: help

    background: Rectangle {
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#5e5358" }
            GradientStop { position: 0.7; color: "#222" }
        }
    }

    Flickable {
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: content.height
        clip: true

        Column {
            id: content
            width: parent.width - 50
            anchors.centerIn: parent
            spacing: 12
            padding: 10

            Text {
                text: "Object of the Game"
                font.bold: true
                font.pixelSize: 22
                color: "white"
            }
            Text {
                text: "Be the first player to align 4 gobblets in a row (vertically, horizontally or diagonally)."
                wrapMode: Text.WordWrap
                width: parent.width
                font.pixelSize: 18
                color: "white"
            }

            Text {
                text: "Playing"
                font.bold: true
                font.pixelSize: 22
                color: "white"
            }
            Text {
                text: "In turns, players can either put a new gobblet on the board,\nmove one of their gobblets already on the board to any empty space,\nor gobble up any smaller size gobblet with a bigger gobblet already in play."
                font.pixelSize: 18
                wrapMode: Text.WordWrap
                width: parent.width
                color: "white"
            }

            Text {
                text: "Gobbling"
                font.bold: true
                font.pixelSize: 22
                color: "white"
            }
            Text {
                text: "You can gobble up your own pieces.\nYou can gobble up any smaller size gobblet; it does not have to be the next size down."
                font.pixelSize: 18
                wrapMode: Text.WordWrap
                width: parent.width
                color: "white"
            }

            Text {
                text: "If you put a new gobblet in play, you must place it on an empty square.\nHowever, there is one exception to this rule:\nIf your opponent already has 3 gobblets in a row on the board,\nyou may gobble up one of the 3 pieces in the line by using a gobblet taken directly from one of your external stacks."
                font.pixelSize: 18
                wrapMode: Text.WordWrap
                width: parent.width
                color: "white"
            }

            Text {
                text: "Menu"
                font.bold: true
                font.pixelSize: 22
                color: "white"
            }

            Text {
                text: "You’re playing White!\nGive the right-side player a click to pick your challenger:\ngreen for a relaxed game, red for a real brain-burner, or black for a human rival.\nGo back to the menu by clicking the 3 dots in the left corner."
                font.pixelSize: 18
                wrapMode: Text.WordWrap
                width: parent.width
                color: "white"
            }
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
                stackView.pop();
            }
        }
    }
}
