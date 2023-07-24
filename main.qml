import QtQuick 2.15
import QtQml 2.12
import QtQuick.Controls 2.15
import Qt.labs.qmlmodels 1.0
import io.vahit.backend 1.0

ApplicationWindow {
    property int gridCols: gridRows
    property int gridRows: 15
    property bool gameEnded: false

    id: root
    width: 405
    height: 405 +50
    visible: true
    title: qsTr("Minesweeper")

    CellModel
    {
        id: backend
    }
       Component.onCompleted: {
           backend.addItem(gridRows, gridCols);
           backend.adjacentMine(gridRows, gridCols)
       }
    Rectangle{
        id: statusBar
        property string sTitle: "MineSweeper"
        width: parent.width
        height: 50
        z:2
        color: "lightslategray"
        border.color: "white"
        border.width: 2


        Text {
            id: name
            text: statusBar.sTitle
            anchors.centerIn: parent
            font.bold: true
            font.pixelSize: 23
            color: "white"
        }
    }




    Rectangle{
        id: panelrec
        color: "black"
        width: parent.width
        height: parent.height - statusBar.height
        anchors.top: statusBar.bottom
        border.color: "white"
        border.width: 2

        Tiles{}
    }

    Button{
        width: parent.width
        height: 60
        text: "Play agin"
        anchors.centerIn: parent
        visible: gameEnded

        onClicked: {
            backend.addItem(gridRows, gridCols);
            backend.adjacentMine(gridRows, gridCols)
            gameEnded = false
            statusBar.sTitle = "MineSweeper"
            statusBar.color = "lightslategray"
        }


    }



}
