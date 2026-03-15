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
                Layout.preferredHeight: menu.height * 0.5
                color: "transparent"

                HumanIcon {
                    anchors.centerIn: parent
                    width: parent.width
                    height: parent.height
                    figureColor: "#0A64D6"
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

                PlayIcon {
                    width: 80
                    height: 80
                }
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
                //visible: { stackView.depth > 1 }

                EyesIcon {
                    width: 80
                    height: 80
                }

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
                visible: GameState.mode === 0
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
                        if (BluetoothManager.serverName) {
                            BluetoothManager.serverName = null;
                            BluetoothManager.stopServer();
                        } else {
                            BluetoothManager.startServer();
                        }
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
                        if (BluetoothManager.clientName) {
                            BluetoothManager.clientName = null;
                            BluetoothManager.stopClient();
                        } else {
                            stackView.push("scanblues.qml");
                        }
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
                Layout.preferredHeight: menu.height * 0.5
                color: "transparent"

                HumanIcon {
                    anchors.centerIn: parent
                    width: parent.width
                    height: parent.height
                    figureColor: "#ACAFA0"
                    visible: GameState.mode === 0
                }

                ComputerIcon {
                    anchors.centerIn: parent
                    width: parent.width
                    height: parent.height
                    lightcolor: GameState.mode === 1 ? "#00bb00" : "#bb0000"
                    color: GameState.mode === 1 ? "#009900" : "#990000"
                    visible: GameState.mode !== 0
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
