import QtQuick 2.0
import QtQuick.Window 2.0
import QtLocation 5.6
import QtPositioning 5.6
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.3

Window {
    id: mainWindow
    objectName: "mainWindow"
    width: 1024
    height: 800
    color: "#f4ba22"
    title: "Kitti Dataset EKF-INS Viewer"
    visible: true

    Plugin {
        id: mapPlugin
        name: "osm"
    }

    property var initialPosition: QtPositioning.coordinate(32.7767828,35.0253158) // Technion
    Map {
        anchors.topMargin: 70
        anchors.fill: parent
        plugin: mapPlugin
        center: initialPosition
        zoomLevel: 20
        objectName: "osmMap"

        MapCircle {
            objectName: "mapCircleStart"
            center: initialPosition
            color: "red"
            opacity: 0.7
            radius: 2.5
        }

        MapCircle {
            objectName: "mapCircleRoute"
            center: initialPosition
            color: "green"
            opacity: 0.7
            radius: 1
            visible: false
        }
    }

    Item {
        id: loadButton
        x: parent.width/2  - width - 50
        y: parent.y + 2
        width: 150
        height: 42

        Button {
            x: 532
            y: 10
            text: "Load"
            objectName: "loadButton"
        }

    }

    Item {
        id: startButton
        x: parent.width/2 + 50
        y: parent.y + 2
        width: 150
        height: 42

        Button {
            x: 374
            y: 10
            text: "Start"
            objectName: "startButton"
        }
    }

    FileDialog {
        id: folderDialog
        objectName: "folderDialog"
        selectFolder: true
        selectExisting: true
        title: "Select Kitti Dataset Folder"
        onAccepted: textInput.text = folderDialog.fileUrl
    }

    Item {
        id: textEntry
        x: 8
        y: 8
        width: 320
        height: 32

        Button {
            id: browseButton
            objectName: "browseButton"
            x: 679
            y: 5
            text: qsTr("Browse")
            onClicked: folderDialog.open()
        }
    }

    Rectangle {
        id: rectangle
        x: 8
        y: 8
        width: 668
        height: 36
        color: "#e0f98b"
        border.width: 1

        TextInput {
            id: textInput
            objectName: "folderPath"
            x: 8
            y: 8
            width: parent.width
            height: parent.height
            text: qsTr("Please browse and select an oxts folder...")
            font.pixelSize: 12
        }
    }
}
