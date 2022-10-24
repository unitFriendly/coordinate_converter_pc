import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12
import QtMultimedia 5.12
import QtQuick.Layouts 1.12

ApplicationWindow{
    id: appWindow
    width: 960
    height: 540

    visible: true
    title: qsTr("Hello World")
    color: "green"

    onWidthChanged: {
        if(appWindow.width < recBorder.x + recBorder.width) {
            recBorder.x = appWindow.width - recBorder.width
            recTouchPoint.x = recBorder.x + recBorder.width - recTouchPoint.width / 2
        }
    }

    onHeightChanged: {
        if(appWindow.height < recBorder.y + recBorder.height) {
            recBorder.y = appWindow.height - recBorder.height
            recTouchPoint.y = recBorder.y + recBorder.height - recTouchPoint.height / 2
        }
    }

    Rectangle {
        id: appRec
        anchors.fill: parent

        function saveGrabImage(img)
        {
            img.saveToFile("grab.jpeg")
        }

        Timer{
            interval: 1000
            repeat: true
            running: true

            onTriggered: {
                if(server.clientIsOnline())
                {
                    recImage.grabToImage(appRec.saveGrabImage)
                    server.sendImageFromPath("grab.jpeg")
                }
            }
        }

        Camera {
            id: camera
        }

        MediaPlayer {
            id: mediaPlayer
            source: "C:/Users/rota/Documents/ImageToCoordinate/videoTest.mp4"
            autoPlay: true
            loops: MediaPlayer.Infinite

        }

        ListModel {
            id: listModel
        }

        Rectangle {
            id: recImage
            width: recBorder.width
            height: recBorder.height
            x:recBorder.x
            y:recBorder.y
            color: "transparent"
            z:1

            VideoOutput {
                id: video
                width: appWindow.width
                height: appWindow.height
                x:-parent.x
                y:-parent.y
                focus: visible
                source: mediaPlayer
                z: -1

//                MouseArea {
//                    anchors.fill: parent

//                    onClicked: {
//                        mediaOPlayer.play()
//                    }
//                }
            }
        }

        Rectangle {
            id:recBorder
            width: 150
            height: 150
            z: 2
            border.width: 2
            border.color: "red"
            color: "transparent"
            anchors.centerIn: parent

            MouseArea {
                id: mouseAreaRecBorder
                anchors.fill: parent
                drag.target: parent

                onPositionChanged: {
                    recTouchPoint.x = parent.x + parent.width - recTouchPoint.width / 2
                    recTouchPoint.y = parent.y + parent.height - recTouchPoint.height / 2
                }
            }

            Component.onCompleted: {
                anchors.centerIn = undefined
            }
        }

        Rectangle {
            id:recTouchPoint
            width: 20
            height: width
            radius: height / 2
            z: 2
            anchors.horizontalCenter: recBorder.right
            anchors.verticalCenter: recBorder.bottom
            color: "red"

            MouseArea {
                anchors.fill: parent
                drag.target: parent

                onPositionChanged: {
                    var newWidth = parent.x - recBorder.x + parent.width / 2
                    var newHeight = parent.y - recBorder.y + parent.height / 2

                    if(newWidth < 25)
                    {
                        recBorder.width = 25
                        parent.x = recBorder.x + parent.radius
                    }
                    else
                    {
                        recBorder.width = newWidth
                    }

                    if(newHeight < 25)
                    {
                        recBorder.height = 25
                        parent.y = recBorder.y + parent.radius
                    }
                    else
                    {
                        recBorder.height = newHeight
                    }
                }
            }

            Component.onCompleted: {
                anchors.horizontalCenter = undefined
                anchors.verticalCenter = undefined
            }
        }
    }
}
