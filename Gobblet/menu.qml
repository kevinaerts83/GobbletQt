import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Gobblet 1.0
import "."

Page {
    id: menu

    background: Rectangle {
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#5e5358" }
            GradientStop { position: 0.7; color: "#222" }
        }
    }

    Text {
        text: "Gobblet"
        font.pixelSize: Math.min(menu.height / 8, Math.min(menu.width * 0.24, 130))
        color: "#FFDF00"
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
                Layout.preferredHeight: 250
                color: "transparent"

                // Canvas for additional details
                Canvas {
                    anchors.fill: parent

                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.clearRect(0, 0, width, height);
                        // Draw additional body and head shape details
                        drawFigure(ctx, "#9F9F9F", width / 2 - 30, height / 2 - 68, 1.3 + width / 4000);
                    }

                    // Simplified drawFigure function focusing on details
                    function drawFigure(ctx, color, ox, oy, zoom) {
                        var gradient1 = ctx.createRadialGradient(37.7 * zoom + ox, 55.6 * zoom + oy, 0.0, 37.7 * zoom + ox, 55.6 * zoom + oy, 46.1);
                        gradient1.addColorStop(0.00, "white");
                        gradient1.addColorStop(1.00, color);

                        var gradient2 = ctx.createRadialGradient(37.7 * zoom + ox, 15.3 * zoom + oy, 0.0, 37.6 * zoom + ox, 15.3 * zoom + oy, 31.1);
                        gradient2.addColorStop(0.00, "white");
                        gradient2.addColorStop(1.00, color);

                        // Draw the main body shape
                        ctx.beginPath();
                        ctx.moveTo(73.1 * zoom + ox, 83.6 * zoom + oy);
                        ctx.bezierCurveTo(71.7 * zoom + ox, 102.1 * zoom + oy, 52.2 * zoom + ox, 105.2 * zoom + oy, 37.4 * zoom + ox, 105.2 * zoom + oy);
                        ctx.bezierCurveTo(22.5 * zoom + ox, 105.2 * zoom + oy, 3.0 * zoom + ox, 102.1 * zoom + oy, 1.6 * zoom + ox, 83.6 * zoom + oy);
                        ctx.bezierCurveTo(0.1 * zoom + ox, 62.7 * zoom + oy, 14.0 * zoom + ox, 35.3 * zoom + oy, 37.4 * zoom + ox, 35.3 * zoom + oy);
                        ctx.bezierCurveTo(60.8 * zoom + ox, 35.3 * zoom + oy, 74.7 * zoom + ox, 62.7 * zoom + oy, 73.1 * zoom + ox, 83.6 * zoom + oy);
                        ctx.closePath();
                        ctx.fillStyle = gradient1;
                        ctx.fill();

                        // Draw the head shape
                        ctx.beginPath();
                        ctx.moveTo(61.2 * zoom + ox, 25.3 * zoom + oy);
                        ctx.bezierCurveTo(61.2 * zoom + ox, 38.4 * zoom + oy, 50.5 * zoom + ox, 49.1 * zoom + oy, 37.4 * zoom + ox, 49.1 * zoom + oy);
                        ctx.bezierCurveTo(24.2 * zoom + ox, 49.1 * zoom + oy, 13.6 * zoom + ox, 38.4 * zoom + oy, 13.6 * zoom + ox, 25.3 * zoom + oy);
                        ctx.bezierCurveTo(13.6 * zoom + ox, 12.1 * zoom + oy, 24.2 * zoom + ox, 1.5 * zoom + oy, 37.4 * zoom + ox, 1.5 * zoom + oy);
                        ctx.bezierCurveTo(50.5 * zoom + ox, 1.5 * zoom + oy, 61.2 * zoom + ox, 12.1 * zoom + oy, 61.2 * zoom + ox, 25.3 * zoom + oy);
                        ctx.closePath();
                        ctx.fillStyle = gradient2;
                        ctx.fill();
                    }
                }
            }

            Text {
                text: BluetoothManager.serverName;
                font.pixelSize: 11
                Layout.alignment: Qt.AlignHCenter
                color: "#D0D0D0"
            }

            Text {
                id: whiteScore
                text: GameState.whiteCounter
                font.pixelSize: Math.min(menu.height / 8, Math.min(menu.width * 0.24, 130))
                Layout.alignment: Qt.AlignHCenter
                color: "#D0D0D0"
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
                        GameState.lock = false;
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
                        stackView.pop();
                    }
                }
            }

            Rectangle {
                Layout.preferredWidth: Math.max(Math.min(menu.width, menu.height) / 8, 50)
                Layout.preferredHeight: Math.max(Math.min(menu.width, menu.height) / 8, 50)
                radius: Math.min(menu.width, menu.height) / 16  // Make it circular
                color: "#4CAF50"  // Green background color
                visible: GameState.mode === 0 && !BluetoothManager.serverName
                id: serverBt

                BluetoothIcon {
                    Layout.alignment: Qt.AlignCenter
                    width: Math.max(Math.min(menu.width, menu.height) / 8, 50)
                    height: Math.max(Math.min(menu.width, menu.height) / 8, 50)
                    btColor: "white"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        BluetoothManager.startServer();
                    }
                }
            }

            Rectangle {
                Layout.preferredWidth: Math.max(Math.min(menu.width, menu.height) / 8, 50)
                Layout.preferredHeight: Math.max(Math.min(menu.width, menu.height) / 8, 50)
                radius: Math.min(menu.width, menu.height) / 16  // Make it circular
                color: "#4CAF50"  // Green background color
                visible: GameState.mode === 0
                id: clientBt

                BluetoothIcon {
                    Layout.alignment: Qt.AlignCenter
                    width: Math.max(Math.min(menu.width, menu.height) / 8, 50)
                    height: Math.max(Math.min(menu.width, menu.height) / 8, 50)
                    btColor: "black"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        stackView.push("scanblues.qml");
                    }
                }
            }

            Rectangle {
                Layout.preferredWidth: Math.max(Math.min(menu.width, menu.height) / 8, 50)
                Layout.preferredHeight: Math.max(Math.min(menu.width, menu.height) / 8, 50)
                radius: Math.min(menu.width, menu.height) / 16  // Make it circular
                color: "#4CAF50"  // Green background color
                id: helpButton
                // Eyes
                Text {
                    id: qmark
                    anchors.centerIn: parent
                    text: "?"
                    color: "#FFF"
                    // choose a scale factor; tweak 0.55 to taste
                    font.pixelSize: Math.max(12, Math.floor(Math.min(helpButton.width, helpButton.height) * 0.55))
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        stackView.push("help.qml");
                    }
                }
            }
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignCenter

            Rectangle {
                Layout.preferredWidth: menu.width * 0.45
                Layout.preferredHeight: 250
                color: "transparent"

                // Canvas for additional details
                Canvas {
                    anchors.fill: parent
                    id: rightButton

                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.clearRect(0, 0, width, height);

                        var zoom = width / 4000;
                        // Draw additional body and head shape details
                        if (GameState.mode === 0) {
                            drawFigure(ctx, "#ACAFA0", width / 2 - 60, height / 2 - 68, 1.3 + zoom);
                        } else if (GameState.mode === 1) {
                            drawComputer(ctx, "#00bb00", "#009900", width / 2 - 10, height / 2 - 25, 0.16 + zoom);
                        } else {
                            drawComputer(ctx, "#bb0000", "#990000", width / 2 - 10, height / 2 - 25, 0.16 + zoom);
                        }
                    }

                    // Simplified drawFigure function focusing on details
                    function drawFigure(ctx, color, ox, oy, zoom) {
                        var gradient1 = ctx.createRadialGradient(37.7 * zoom + ox, 55.6 * zoom + oy, 0.0, 37.7 * zoom + ox, 55.6 * zoom + oy, 46.1);
                        gradient1.addColorStop(0.00, 'black');
                        gradient1.addColorStop(1.00, color);

                        var gradient2 = ctx.createRadialGradient(37.7 * zoom + ox, 15.3 * zoom + oy, 0.0, 37.6 * zoom + ox, 15.3 * zoom + oy, 31.1);
                        gradient2.addColorStop(0.00, 'black');
                        gradient2.addColorStop(1.00, color);

                        // Draw the main body shape
                        ctx.beginPath();
                        ctx.moveTo(73.1 * zoom + ox, 83.6 * zoom + oy);
                        ctx.bezierCurveTo(71.7 * zoom + ox, 102.1 * zoom + oy, 52.2 * zoom + ox, 105.2 * zoom + oy, 37.4 * zoom + ox, 105.2 * zoom + oy);
                        ctx.bezierCurveTo(22.5 * zoom + ox, 105.2 * zoom + oy, 3.0 * zoom + ox, 102.1 * zoom + oy, 1.6 * zoom + ox, 83.6 * zoom + oy);
                        ctx.bezierCurveTo(0.1 * zoom + ox, 62.7 * zoom + oy, 14.0 * zoom + ox, 35.3 * zoom + oy, 37.4 * zoom + ox, 35.3 * zoom + oy);
                        ctx.bezierCurveTo(60.8 * zoom + ox, 35.3 * zoom + oy, 74.7 * zoom + ox, 62.7 * zoom + oy, 73.1 * zoom + ox, 83.6 * zoom + oy);
                        ctx.closePath();
                        ctx.fillStyle = gradient1;
                        ctx.fill();

                        // Draw the head shape
                        ctx.beginPath();
                        ctx.moveTo(61.2 * zoom + ox, 25.3 * zoom + oy);
                        ctx.bezierCurveTo(61.2 * zoom + ox, 38.4 * zoom + oy, 50.5 * zoom + ox, 49.1 * zoom + oy, 37.4 * zoom + ox, 49.1 * zoom + oy);
                        ctx.bezierCurveTo(24.2 * zoom + ox, 49.1 * zoom + oy, 13.6 * zoom + ox, 38.4 * zoom + oy, 13.6 * zoom + ox, 25.3 * zoom + oy);
                        ctx.bezierCurveTo(13.6 * zoom + ox, 12.1 * zoom + oy, 24.2 * zoom + ox, 1.5 * zoom + oy, 37.4 * zoom + ox, 1.5 * zoom + oy);
                        ctx.bezierCurveTo(50.5 * zoom + ox, 1.5 * zoom + oy, 61.2 * zoom + ox, 12.1 * zoom + oy, 61.2 * zoom + ox, 25.3 * zoom + oy);
                        ctx.closePath();
                        ctx.fillStyle = gradient2;
                        ctx.fill();
                    }

                    function drawComputer(ctx, lightcolor, color, ox, oy, zoom) {
                        ctx.strokeStyle = 'black';

                        // body
                        ctx.beginPath();
                        ctx.moveTo(ox, oy - 10 * zoom);
                        ctx.lineTo(ox, oy + 400 * zoom);
                        ctx.bezierCurveTo(ox - 80 * zoom, oy + 390 * zoom, ox - 150 * zoom, oy + 250 * zoom, ox - 200 * zoom, oy + 10 * zoom);
                        ctx.bezierCurveTo(ox - 150 * zoom, oy - 5 * zoom, ox - 50 * zoom, oy - 8 * zoom, ox, oy - 10 * zoom);
                        ctx.closePath();
                        ctx.fillStyle = lightcolor;
                        ctx.fill();
                        ctx.stroke();

                        ctx.beginPath();
                        ctx.moveTo(ox, oy - 10 * zoom);
                        ctx.lineTo(ox, oy + 400 * zoom);
                        ctx.bezierCurveTo(ox + 80 * zoom, oy + 390 * zoom, ox + 150 * zoom, oy + 250 * zoom, ox + 200 * zoom, oy + 10 * zoom);
                        ctx.bezierCurveTo(ox + 150 * zoom, oy - 5 * zoom, ox + 50 * zoom, oy - 8 * zoom, ox, oy - 10 * zoom);
                        ctx.closePath();
                        ctx.fillStyle = color;
                        ctx.fill();
                        ctx.stroke();

                        ctx.fillStyle = color;
                        ctx.fillRect(ox - 1, oy - 10 * zoom, 2, 410 * zoom);

                        // face
                        ctx.fillStyle = 'black';
                        ctx.fillRect(ox - 160 * zoom, oy - 160 * zoom, 320 * zoom, 100 * zoom);

                        ctx.beginPath();
                        ctx.arc(ox - 80 * zoom, oy - 120 * zoom, 20 * zoom, 0, Math.PI * 2);
                        ctx.fillStyle = 'white';
                        ctx.fill();

                        ctx.beginPath();
                        ctx.arc(ox + 80 * zoom, oy - 120 * zoom, 20 * zoom, 0, Math.PI * 2);
                        ctx.fillStyle = 'white';
                        ctx.fill();

                        ctx.fillStyle = 'white';
                        ctx.fillRect(ox - 20 * zoom, oy - 90 * zoom, 40 * zoom, 15 * zoom);

                        // Border face
                        ctx.beginPath();
                        ctx.moveTo(ox, oy - 10 * zoom);
                        ctx.lineTo(ox - 100 * zoom, oy - 10 * zoom);
                        ctx.arc(ox - 100 * zoom, oy - 110 * zoom, 100 * zoom, 0.5 * Math.PI, 1.5 * Math.PI);
                        ctx.lineTo(ox, oy - 210 * zoom);
                        ctx.moveTo(ox, oy - 160 * zoom);
                        ctx.lineTo(ox - 100 * zoom, oy - 160 * zoom);
                        ctx.arc(ox - 100 * zoom, oy - 110 * zoom, 50 * zoom, 1.5 * Math.PI, 0.5 * Math.PI, true);
                        ctx.lineTo(ox, oy - 60 * zoom);
                        ctx.moveTo(ox, oy - 10 * zoom);
                        ctx.fillStyle = lightcolor;
                        ctx.fill();
                        ctx.stroke();

                        ctx.beginPath();
                        ctx.moveTo(ox, oy - 10 * zoom);
                        ctx.lineTo(ox + 100 * zoom, oy - 10 * zoom);
                        //arc(x, y, radius, startAngle, endAngle, anticlockwise);
                        ctx.arc(ox + 100 * zoom, oy - 110 * zoom, 100 * zoom, 0.5 * Math.PI, 1.5 * Math.PI, true);
                        ctx.lineTo(ox, oy - 210 * zoom);
                        ctx.moveTo(ox, oy - 160 * zoom);
                        ctx.lineTo(ox + 100 * zoom, oy - 160 * zoom);
                        ctx.arc(ox + 100 * zoom, oy - 110 * zoom, 50 * zoom, 1.5 * Math.PI, 0.5 * Math.PI);
                        ctx.lineTo(ox, oy - 60 * zoom);
                        ctx.moveTo(ox, oy - 10 * zoom);
                        ctx.fillStyle = color;
                        ctx.fill();
                        ctx.stroke();

                    }
                }

                // MouseArea for button clicks
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        onClicked: {
                            GameState.mode = GameState.mode + 1;
                            if (GameState.mode === 3) GameState.mode = 0;
                            rightButton.requestPaint();
                            blackScore.text = GameState.blackCounter;
                            whiteScore.text = GameState.whiteCounter;
                        }
                    }
                }
            }

            Text {
                text: BluetoothManager.clientName;
                Layout.alignment: Qt.AlignHCenter
                font.pixelSize: 11
                color: "#D0D0D0"
            }

            Text {
                id: blackScore
                text: GameState.blackCounter
                Layout.alignment: Qt.AlignHCenter
                font.pixelSize: Math.min(menu.height / 8, Math.min(menu.width * 0.24, 130))
                color: "#D0D0D0"
            }
        }
    }
}
