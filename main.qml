import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "Gobblet"

    //property int currentPage: 0 // 0 for Menu, 1 for Game

    /*
    Loader {
        id: menuLoader
        source: currentPage === 0 ? "menu.qml" : ""
        anchors.fill: parent
        visible: currentPage === 0
        onLoaded: {
            menuLoader.item.showGame.connect(function() {
                currentPage = 1;
            });
        }
    }

    Loader {
        id: gameLoader
        source: currentPage === 1 ? "game.qml" : ""
        anchors.fill: parent
        visible: currentPage === 1
        onLoaded: {
            if (gameLoader.item) {
                setupBoard.parent = gameLoader.item;
            }
            gameLoader.item.showMenu.connect(function() {
                currentPage = 0;
            });
        }
    }*/

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: "menu.qml"
        visible: true
        objectName: "stackview"
    }

}
