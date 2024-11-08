import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "Gobblet"

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: "menu.qml"
        visible: true
        objectName: "stackview"

        onCurrentItemChanged: {
            if (stackView.currentItem.objectName == "game") { // Only trigger on returning to Page1
                setupBoard.createBoard()  // Call the C++ function
            }
        }
    }

}
