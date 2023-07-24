import QtQuick 2.0

Image {
    id: flag
    source: "flag.png"
    anchors.fill: parent
    MouseArea{
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked:  backend.setData(backend.index(row, column), "unchecked", 1) //flag.destroy()
    }
}
