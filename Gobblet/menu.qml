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

        ColumnLayout {
            Layout.alignment: Qt.AlignCenter

            Rectangle {
                Layout.preferredWidth: menu.width * 0.40
                Layout.preferredHeight: menu.height * 0.5
                color: "transparent"

                HumanIcon {
                    anchors.centerIn: parent
                    width: parent.width
                    height: parent.height
                    startColor: "white"
                    stopColor: "#939393"
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

            Rectangle {
                Layout.preferredHeight: (2 * menu.height - 60) / 13
                visible: { stackView.depth > 1 && GameState.mode === 0 }
            }

            Rectangle {
                id: playButton
                Layout.preferredWidth: (2 * menu.height - 60) / 13
                Layout.preferredHeight: width
                radius: width / 2  // Make it circular
                color: "#4CAF50"  // Green background color

                PlayIcon {
                    anchors.centerIn: parent
                    width: parent.width
                    height: parent.height
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        GameState.lock = false;
                        GameState.user = BluetoothManager.serverName ? 1 : (BluetoothManager.clientName ? 2 : 0);
                        while (stackView.depth > 1) {
                            stackView.pop();
                        }
                        stackView.push("game.qml");
                    }
                }
            }

            Rectangle {
                Layout.preferredWidth: (2 * menu.height - 60) / 13
                Layout.preferredHeight: width
                radius: width / 2  // Make it circular
                color: "#4CAF50"  // Green background color
                visible: { stackView.depth > 1 }

                EyesIcon {
                    anchors.centerIn: parent
                    width: parent.width
                    height: parent.height
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        stackView.pop();
                    }
                }
            }

            Rectangle {
                Layout.preferredWidth: (2 * menu.height - 60) / 13
                Layout.preferredHeight: width
                radius: width / 2  // Make it circular
                color: "#4CAF50"  // Green background color
                visible: GameState.mode === 0
                id: serverBt

                BluetoothIcon {
                    Layout.alignment: Qt.AlignCenter
                    width: parent.width
                    height: parent.height
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
                Layout.preferredWidth: (2 * menu.height - 60) / 13
                Layout.preferredHeight: width
                radius: width / 2  // Make it circular
                color: "#4CAF50"  // Green background color
                visible: GameState.mode === 0
                id: clientBt

                BluetoothIcon {
                    Layout.alignment: Qt.AlignCenter
                    width: parent.width
                    height: parent.height
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
                Layout.preferredWidth: (2 * menu.height - 60) / 13
                Layout.preferredHeight: width
                radius: width / 2  // Make it circular
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
                Layout.preferredWidth: menu.width * 0.40
                Layout.preferredHeight: menu.height * 0.5
                color: "transparent"

                HumanIcon {
                    anchors.centerIn: parent
                    width: parent.width
                    height: parent.height
                    startColor: "black"
                    stopColor: "#ACAFA0"
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
