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

        onCurrentItemChanged: {
            //if (stackView.currentItem === 1) { // Only trigger on returning to Page1
                setupBoard.createRectangle()  // Call the C++ function
            //}
        }
    }

}
