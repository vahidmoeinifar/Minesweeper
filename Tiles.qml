import QtQuick 2.15
import QtQml 2.12
import QtQuick.Controls 2.15
import Qt.labs.qmlmodels 1.0

TableView {
    id: table
    anchors.fill: parent
    model: backend
    enabled: !gameEnded

    Rectangle{anchors.fill: parent}
    rowSpacing: 1
    columnSpacing: 1
    delegate: Rectangle {
        property bool mineV: false
        id: delegateItem
        color: model.status === "checked" ? "steelblue" : "yellowgreen"
        implicitWidth: 26
        implicitHeight: 26

        Text {
            id: mineCount
            text: model.mineCount !== 0 ? model.mineCount.toString() : ""
            anchors.centerIn: parent
            color: "white"
            font.bold: true
            visible: model.status === "checked"
        }

        MouseArea{
            id: mouseLeft
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            onClicked: {
                function getRowAndColumn(index) {
                    var row = Math.floor(index / gridCols);
                    var column = index % gridCols;
                    return {
                        row: row,
                        column: column
                    }
                }
                var result = getRowAndColumn(index)

                if (model.type === "mine") {
                    statusBar.color = "red"
                    statusBar.sTitle = "Game Over!"
                    for (var i=0; i<table.columns; i++){
                        for (var j=0; j<table.rows; j++){
                            table.itemAtCell(i,j).mineV =  true
                        }

                    }
                    gameEnded = true


                }

                backend.unCheckingCells(result.column, result.row, gridRows, gridCols)



            }
        }

        MouseArea{
            id: mouseRight
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            visible: model.status === "unchecked"
            onClicked: {
                backend.setData(backend.index(row, column), "flag", 1)
                if (backend.checkWin(gridRows, gridCols)) {
                    statusBar.color = "blue"
                    statusBar.sTitle = "Win... All mines cleaned!"
                    gameEnded = true
                }
            }
        }

        Flag{
            visible: model.status === "flag" ? true: false
        }

        //           Image {
        //               id: mine
        //               source: "mine.png"
        //               anchors.fill: parent
        //               visible: mineV && model.type === "mine" ? true: false

        //           }


        AnimatedImage{
            id: mineEx
            source: "explosion.gif"
            anchors.fill: parent
            visible: mineV && model.type === "mine" ? true: false

        }

    }




}
