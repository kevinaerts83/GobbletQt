import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {
    id: menu

    Text {
        text: "Gobblet"
        font.pixelSize: 120
        color: "yellow"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 20
    }

    RowLayout {
        anchors.centerIn: parent
        spacing: 100

        ColumnLayout {
            Layout.alignment: Qt.AlignCenter
            Layout.leftMargin: 50

            Rectangle {
                width: 150
                height: 200
                color: "transparent"

                // Canvas for additional details
                Canvas {
                    anchors.fill: parent

                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.clearRect(0, 0, width, height);

                        // Draw additional body and head shape details
                        drawFigure(ctx, "#9F9F9F", 10, 10);
                    }

                    // Simplified drawFigure function focusing on details
                    function drawFigure(ctx, color, offsetX, offsetY) {
                        // Define gradients
                        var gradient1 = ctx.createRadialGradient(37.7 + offsetX, 55.6 + offsetY, 0.0, 37.7 + offsetX, 55.6 + offsetY, 46.1);
                        gradient1.addColorStop(0.00, "#fff");
                        gradient1.addColorStop(1.00, color);

                        var gradient2 = ctx.createRadialGradient(37.7 + offsetX, 15.3 + offsetY, 0.0, 37.6 + offsetX, 15.3 + offsetY, 31.1);
                        gradient2.addColorStop(0.00, "#fff");
                        gradient2.addColorStop(1.00, color);

                        ctx.save();

                        // Draw the main body shape
                        ctx.beginPath();
                        ctx.moveTo(73.1 + offsetX, 83.6 + offsetY);
                        ctx.bezierCurveTo(71.7 + offsetX, 102.1 + offsetY, 52.2 + offsetX, 105.2 + offsetY, 37.4 + offsetX, 105.2 + offsetY);
                        ctx.bezierCurveTo(22.5 + offsetX, 105.2 + offsetY, 3.0 + offsetX, 102.1 + offsetY, 1.6 + offsetX, 83.6 + offsetY);
                        ctx.bezierCurveTo(0.1 + offsetX, 62.7 + offsetY, 14.0 + offsetX, 35.3 + offsetY, 37.4 + offsetX, 35.3 + offsetY);
                        ctx.bezierCurveTo(60.8 + offsetX, 35.3 + offsetY, 74.7 + offsetX, 62.7 + offsetY, 73.1 + offsetX, 83.6 + offsetY);
                        ctx.closePath();
                        ctx.fillStyle = gradient1;
                        ctx.fill();

                        // Draw the head shape
                        ctx.beginPath();
                        ctx.moveTo(61.2 + offsetX, 25.3 + offsetY);
                        ctx.bezierCurveTo(61.2 + offsetX, 38.4 + offsetY, 50.5 + offsetX, 49.1 + offsetY, 37.4 + offsetX, 49.1 + offsetY);
                        ctx.bezierCurveTo(24.2 + offsetX, 49.1 + offsetY, 13.6 + offsetX, 38.4 + offsetY, 13.6 + offsetX, 25.3 + offsetY);
                        ctx.bezierCurveTo(13.6 + offsetX, 12.1 + offsetY, 24.2 + offsetX, 1.5 + offsetY, 37.4 + offsetX, 1.5 + offsetY);
                        ctx.bezierCurveTo(50.5 + offsetX, 1.5 + offsetY, 61.2 + offsetX, 12.1 + offsetY, 61.2 + offsetX, 25.3 + offsetY);
                        ctx.closePath();
                        ctx.fillStyle = gradient2;
                        ctx.fill();
                        ctx.restore();
                    }
                }
            }


            Text {
                text: setupMenu ? setupMenu.whiteCounter : "Loading..."
                font.pixelSize: 120
                color: "black"
            }
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignCenter

            Rectangle {
                width: 100
                height: 100
                radius: 50  // Make it circular
                color: "#4CAF50"  // Green background color

                // Play Icon (Triangle) in the middle
                Canvas {
                    anchors.fill: parent
                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.clearRect(0, 0, width, height);

                        // Draw triangle (white play symbol)
                        ctx.beginPath();
                        ctx.moveTo(width / 3 + 5, height / 4);
                        ctx.lineTo(width / 3 + 5, height * 3 / 4);
                        ctx.lineTo(width * 2 / 3 + 5, height / 2);
                        ctx.closePath();

                        ctx.fillStyle = "white";
                        ctx.fill();
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        while (stackView.depth > 1) {
                            stackView.pop();
                        }
                        stackView.push("game.qml");
                    }

                }
            }

            Rectangle {
                width: 100
                height: 100
                radius: 50  // Make it circular
                color: "#4CAF50"  // Green background color

                // Play Icon (Triangle) in the middle
                Canvas {
                    anchors.fill: parent
                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.clearRect(0, 0, width, height);

                        var eyeLeft = [32, 35, 62, 65];
                        var eyeTop = [30, 33, 30, 33];
                        var eyeRadius = [10, 4, 10, 4];

                        for (var i = 0; i < 4; i++) {
                            ctx.beginPath();
                            ctx.arc(eyeLeft[i], eyeTop[i], eyeRadius[i], 0, 2 * Math.PI);
                            ctx.fillStyle = i % 2 === 0 ? 'black' : 'white';
                            ctx.fill();
                            ctx.closePath();
                        }

                        ctx.fillStyle = "white";
                        ctx.fill();
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        stackView.pop();
                    }
                }
            }

        }

        ColumnLayout {
            Layout.alignment: Qt.AlignCenter
            Layout.leftMargin: 50

            Rectangle {
                width: 150
                height: 200
                color: "transparent"

                // Canvas for additional details
                Canvas {
                    anchors.fill: parent
                    id: rightButton

                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.clearRect(0, 0, width, height);

                        // Draw additional body and head shape details
                        if (setupMenu.mode === 0) {
                            drawFigure(ctx, "#2C2F20", 10, 10);
                        } else if (setupMenu.mode === 1) {
                            drawComputer(ctx, "green");
                        } else{
                            drawComputer(ctx, "red");
                        }
                    }

                    // Simplified drawFigure function focusing on details
                    function drawFigure(ctx, color, offsetX, offsetY) {
                        // Define gradients
                        var gradient1 = ctx.createRadialGradient(37.7 + offsetX, 55.6 + offsetY, 0.0, 37.7 + offsetX, 55.6 + offsetY, 46.1);
                        gradient1.addColorStop(0.00, "#fff");
                        gradient1.addColorStop(1.00, color);

                        var gradient2 = ctx.createRadialGradient(37.7 + offsetX, 15.3 + offsetY, 0.0, 37.6 + offsetX, 15.3 + offsetY, 31.1);
                        gradient2.addColorStop(0.00, "#fff");
                        gradient2.addColorStop(1.00, color);

                        ctx.save();

                        // Draw the main body shape
                        ctx.beginPath();
                        ctx.moveTo(73.1 + offsetX, 83.6 + offsetY);
                        ctx.bezierCurveTo(71.7 + offsetX, 102.1 + offsetY, 52.2 + offsetX, 105.2 + offsetY, 37.4 + offsetX, 105.2 + offsetY);
                        ctx.bezierCurveTo(22.5 + offsetX, 105.2 + offsetY, 3.0 + offsetX, 102.1 + offsetY, 1.6 + offsetX, 83.6 + offsetY);
                        ctx.bezierCurveTo(0.1 + offsetX, 62.7 + offsetY, 14.0 + offsetX, 35.3 + offsetY, 37.4 + offsetX, 35.3 + offsetY);
                        ctx.bezierCurveTo(60.8 + offsetX, 35.3 + offsetY, 74.7 + offsetX, 62.7 + offsetY, 73.1 + offsetX, 83.6 + offsetY);
                        ctx.closePath();
                        ctx.fillStyle = gradient1;
                        ctx.fill();

                        // Draw the head shape
                        ctx.beginPath();
                        ctx.moveTo(61.2 + offsetX, 25.3 + offsetY);
                        ctx.bezierCurveTo(61.2 + offsetX, 38.4 + offsetY, 50.5 + offsetX, 49.1 + offsetY, 37.4 + offsetX, 49.1 + offsetY);
                        ctx.bezierCurveTo(24.2 + offsetX, 49.1 + offsetY, 13.6 + offsetX, 38.4 + offsetY, 13.6 + offsetX, 25.3 + offsetY);
                        ctx.bezierCurveTo(13.6 + offsetX, 12.1 + offsetY, 24.2 + offsetX, 1.5 + offsetY, 37.4 + offsetX, 1.5 + offsetY);
                        ctx.bezierCurveTo(50.5 + offsetX, 1.5 + offsetY, 61.2 + offsetX, 12.1 + offsetY, 61.2 + offsetX, 25.3 + offsetY);
                        ctx.closePath();
                        ctx.fillStyle = gradient2;
                        ctx.fill();
                        ctx.restore();
                    }

                    function drawComputer(ctx, color) {
                        ctx.clearRect(0, 0, width, height);  // Clear the canvas

                        // Draw ellipse border
                        ctx.fillStyle = "black";
                        ctx.beginPath();
                        ctx.ellipse(10, 30, 120, 70, 0, 0, Math.PI * 2);
                        ctx.fill();

                        // Draw ellipse border
                        ctx.lineWidth = 7; // Border width
                        ctx.strokeStyle = color; // Black border color
                        ctx.beginPath();
                        ctx.ellipse(10, 30, 120, 70, 0, 0, Math.PI * 2);
                        ctx.stroke();

                        // Draw left circle
                        ctx.fillStyle = "white"; // White circle
                        ctx.beginPath();
                        ctx.arc(30, 60, 10, 0, Math.PI * 2); // Circle on the left
                        ctx.fill();

                        // Draw right circle
                        ctx.beginPath();
                        ctx.arc(100, 60, 10, 0, Math.PI * 2); // Circle on the right
                        ctx.fill();

                        ctx.fillStyle = "white";
                        ctx.beginPath();
                        ctx.rect(47, 72, 28, 9);
                        ctx.fill();

                        // Draw angled line
                        ctx.beginPath();
                        ctx.moveTo(10, 20);
                        ctx.lineTo(28, 38);
                        ctx.strokeStyle = "white";
                        ctx.lineWidth = 4;
                        ctx.stroke();

                        // Draw angled line
                        ctx.beginPath();
                        ctx.moveTo(128, 20);
                        ctx.lineTo(102, 36);
                        ctx.strokeStyle = "white";
                        ctx.lineWidth = 4;
                        ctx.stroke();

                        // Draw left circle
                        ctx.fillStyle = "white"; // White circle
                        ctx.beginPath();
                        ctx.arc(8, 15, 8, 0, Math.PI * 2); // Circle on the left
                        ctx.fill();

                        // Draw right circle
                        ctx.beginPath();
                        ctx.arc(132, 18, 8, 0, Math.PI * 2); // Circle on the right
                        ctx.fill();

                    }
                }

                // MouseArea for button clicks
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        onClicked: {
                            setupMenu.setMode(setupMenu.mode + 1);
                            if (setupMenu.mode === 3) setupMenu.setMode(0);
                            rightButton.requestPaint();
                        }
                    }
                }
            }

            Text {
                text: setupMenu ? setupMenu.blackCounter : "Loading..."
                font.pixelSize: 120
                color: "black"
            }
        }
    }
}
