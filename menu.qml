import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Page {
    id: menu

    background: Rectangle {
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#333" }
            GradientStop { position: 1.0; color: "#191919" }
        }
    }

    Text {
        text: "Gobblet"
        font.pixelSize: Math.min(menu.height / 8, Math.min(menu.width * 0.24, 130))
        color: "silver"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
    }

    RowLayout {
        anchors.centerIn: parent
        spacing: menu.width / 10

        ColumnLayout {
            Layout.alignment: Qt.AlignCenter

            Rectangle {
                Layout.preferredWidth: menu.width * 0.45
                Layout.preferredHeight: 150
                color: "transparent"

                // Canvas for additional details
                Canvas {
                    anchors.fill: parent

                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.clearRect(0, 0, width, height);

                        // Draw additional body and head shape details
                        drawFigure(ctx, "#9F9F9F", width / 2 - 30, 10);
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
                font.pixelSize: Math.min(menu.height / 8, Math.min(menu.width * 0.24, 130))
                Layout.alignment: Qt.AlignHCenter
                color: "silver"
            }
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignCenter
            Layout.preferredWidth: menu.width * 0.1

            Rectangle {
                Layout.preferredWidth: Math.max(Math.min(menu.width, menu.height) / 8, 50)
                Layout.preferredHeight: Math.max(Math.min(menu.width, menu.height) / 8, 50)
                radius: Math.min(menu.width, menu.height) / 16  // Make it circular
                color: "#4CAF50"  // Green background color

                // Play Icon (Triangle) in the middle
                Canvas {
                    anchors.fill: parent
                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.clearRect(0, 0, height, height);

                        // Draw triangle (white play symbol)
                        ctx.beginPath();
                        ctx.moveTo(height / 3 + 5, height / 4);
                        ctx.lineTo(height / 3 + 5, height * 3 / 4);
                        ctx.lineTo(height * 2 / 3 + 5, height / 2);
                        ctx.closePath();

                        ctx.fillStyle = "white";
                        ctx.fill();
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        setupMenu.setLock();
                        while (stackView.depth > 1) {
                            stackView.pop();
                        }
                        stackView.push("game.qml");
                    }
                }
            }

            Rectangle {
                Layout.preferredWidth: Math.max(Math.min(menu.width, menu.height) / 8, 50)
                Layout.preferredHeight: Math.max(Math.min(menu.width, menu.height) / 8, 50)
                radius: Math.min(menu.width, menu.height) / 16  // Make it circular
                color: "#4CAF50"  // Green background color
                visible: { stackView.depth > 1 }

                // Eyes
                Canvas {
                    anchors.fill: parent
                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.clearRect(0, 0, height, height);

                        var eyeLeft = [height / 3, height / 3 + 3, height * 2 / 3, height * 2 / 3 + 3];
                        var eyeTop = [height / 3, height / 3 + 3, height / 3, height / 3 + 3];
                        var eyeRadius = [height / 10, height / 20, height / 10, height / 20];

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
                        //setupMenu.setLock(true);
                        stackView.pop();
                    }
                }
            }

            /*Rectangle {
                Layout.preferredWidth: Math.max(Math.min(menu.width, menu.height) / 8, 50)
                Layout.preferredHeight: Math.max(Math.min(menu.width, menu.height) / 8, 50)
                radius: Math.min(menu.width, menu.height) / 16  // Make it circular
                color: "#4CAF50"  // Green background color

                // Phone or Tablet mode
                Canvas {
                    anchors.fill: parent
                    id: phoneAlignment

                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.clearRect(0, 0, height, height);
                        ctx.strokeStyle = "white"; // Border color
                        ctx.lineWidth = 3;
                        var x = height / (setupMenu.vertical ? 5 : 3);
                        var y = height / (setupMenu.vertical ? 3 : 5);
                        // x, y, width, height
                        ctx.strokeRect((height / 2) - x, (height / 2) - y, x * 2, y * 2);
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        setupMenu.setVertical(setupMenu.vertical);
                        phoneAlignment.requestPaint();
                    }
                }
            }*/

        }

        ColumnLayout {
            Layout.alignment: Qt.AlignCenter

            Rectangle {
                Layout.preferredWidth: menu.width * 0.45
                Layout.preferredHeight: 150
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
                            drawFigure(ctx, "#2C2F20", width / 2 - 33, 10);
                        } else if (setupMenu.mode === 1) {
                            drawComputer(ctx, "MediumSeaGreen", "darkgreen", width / 2.6, width > 300 ? 0 : 60, width > 300 ? 1 : 0.5);
                        } else{
                            drawComputer(ctx, "red", "maroon", width / 2.6, width > 300 ? 0 : 60, width > 300 ? 1 : 0.5);
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

                    function drawComputer(ctx, color, fillcolor, offsetX, offsetY, zoom) {
                        ctx.clearRect(0, 0, width, height);  // Clear the canvas

                        // Draw ellipse border
                        ctx.fillStyle = fillcolor;
                        ctx.beginPath();
                        ctx.ellipse((10 + offsetX) * zoom, (30 + offsetY) * zoom, 120 * zoom, 70 * zoom, 0, 0, Math.PI * 2);
                        ctx.fill();

                        // Draw ellipse border
                        ctx.lineWidth = 7 * zoom; // Border width
                        ctx.strokeStyle = color; // Black border color
                        ctx.beginPath();
                        ctx.ellipse((10 + offsetX) * zoom, (30 + offsetY) * zoom, 120 * zoom, 70 * zoom, 0, 0, Math.PI * 2);
                        ctx.stroke();

                        // Draw left circle
                        ctx.fillStyle = "white"; // White circle
                        ctx.beginPath();
                        ctx.arc((30 + offsetX) * zoom, (60 + offsetY) * zoom, 10 * zoom, 0, Math.PI * 2); // Circle on the left
                        ctx.fill();

                        // Draw right circle
                        ctx.beginPath();
                        ctx.arc((100 + offsetX) * zoom, (60 + offsetY) * zoom, 10 * zoom, 0, Math.PI * 2); // Circle on the right
                        ctx.fill();

                        ctx.fillStyle = "white";
                        ctx.beginPath();
                        ctx.rect((47 + offsetX) * zoom, (72 + offsetY) * zoom, 28 * zoom, 9 * zoom);
                        ctx.fill();

                        // Draw angled line
                        ctx.beginPath();
                        ctx.moveTo((10 + offsetX) * zoom, (20 + offsetY) * zoom);
                        ctx.lineTo((28 + offsetX) * zoom, (38 + offsetY) * zoom);
                        ctx.strokeStyle = "white";
                        ctx.lineWidth = 4 * zoom;
                        ctx.stroke();

                        // Draw angled line
                        ctx.beginPath();
                        ctx.moveTo((128 + offsetX) * zoom, (20 + offsetY) * zoom);
                        ctx.lineTo((102 + offsetX) * zoom, (36 + offsetY) * zoom);
                        ctx.strokeStyle = "white";
                        ctx.lineWidth = 4 * zoom;
                        ctx.stroke();

                        // Draw left circle
                        ctx.fillStyle = "white"; // White circle
                        ctx.beginPath();
                        ctx.arc((8 + offsetX) * zoom, (15 + offsetY) * zoom, 8 * zoom, 0, Math.PI * 2); // Circle on the left
                        ctx.fill();

                        // Draw right circle
                        ctx.beginPath();
                        ctx.arc((132 + offsetX) * zoom, (18 + offsetY) * zoom, 8 * zoom, 0, Math.PI * 2); // Circle on the right
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
                Layout.alignment: Qt.AlignHCenter
                font.pixelSize: Math.min(menu.height / 8, Math.min(menu.width * 0.24, 130))
                color: "silver"
            }
        }
    }
}
