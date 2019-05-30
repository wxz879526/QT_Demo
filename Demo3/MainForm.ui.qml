import QtQuick 2.4

Item {
    width: 800
    height: 600
    property alias mouseArea: mouseArea

    Image {
        id: image
        anchors.fill: parent
        source: "login_bg.png"

        Rectangle {
            id: rectangle
            height: 100
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: "#805bcce9"
                }

                GradientStop {
                    position: 1
                    color: "#80000000"
                }
            }
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0

            Text {
                id: text1
                color: "#ffffff"
                text: qsTr("Monday, 26-10-2015 3:14 PM")
                anchors.left: parent.left
                anchors.leftMargin: 28
                anchors.top: parent.top
                anchors.topMargin: 29
                font.pixelSize: 22
            }

            Rectangle {
                id: rectangle1
                width: 50
                height: 50
                color: "#27a9e3"
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.top: parent.top
                anchors.topMargin: 10

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent

                    Image {
                        id: image1
                        anchors.fill: parent
                        source: "shutdown_icon.png"
                    }
                }
            }

            Rectangle {
                id: rectangle2
                width: 50
                height: 50
                color: "#27a9e3"
                anchors.right: rectangle1.right
                anchors.rightMargin: 80
                anchors.top: parent.top
                anchors.topMargin: 10

                MouseArea {
                    id: mouseArea1
                    anchors.fill: parent

                    Image {
                        id: image2
                        anchors.fill: parent
                        source: "restart_icon.png"
                    }
                }
            }
        }

        Rectangle {
            id: rectangle3
            width: 360
            height: 200
            color: "#80000000"
            radius: 15
            anchors.verticalCenterOffset: 100
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter

            Text {
                id: text2
                x: 44
                y: 47
                color: "#ffffff"
                text: qsTr("Username:")
                font.pixelSize: 12
            }

            Text {
                id: text3
                x: 44
                y: 108
                color: "#ffffff"
                text: qsTr("Username:")
                font.pixelSize: 12
            }

            TextInput {
                id: textInput
                x: 132
                y: 47
                width: 80
                height: 20
                text: qsTr("Text Input")
                font.pixelSize: 12

                Rectangle {
                    id: rectangle4
                    color: "#ffffff"
                    radius: 5
                    anchors.fill: parent
                }
            }

            TextInput {
                id: textInput1
                x: 132
                y: 108
                width: 80
                height: 20
                text: qsTr("Text Input")
                font.pixelSize: 12

                Rectangle {
                    id: rectangle5
                    color: "#ffffff"
                    radius: 5
                    anchors.fill: parent
                }
            }

            MouseArea {
                id: mouseArea2
                x: 68
                y: 134
                width: 244
                height: 43

                Rectangle {
                    id: rectangle6
                    color: "#27a9e3"
                    radius: 3
                    anchors.topMargin: 8
                    anchors.fill: parent
                }

                Text {
                    id: text4
                    color: "#ffffff"
                    text: qsTr("Login")
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                }
            }
        }

        Image {
            id: image3
            x: 164
            y: 106
            source: "logo.png"
        }
    }
}


/*##^## Designer {
    D{i:6;anchors_width:200;anchors_x:51;anchors_y:28}D{i:11;anchors_height:50;anchors_width:50;anchors_x:742;anchors_y:8}
D{i:10;anchors_x:28;anchors_y:29}D{i:5;anchors_x:62;anchors_y:101}D{i:14;anchors_x:0;anchors_y:0}
D{i:17;anchors_height:100;anchors_width:100}D{i:19;anchors_x:0;anchors_y:0}
}
 ##^##*/
