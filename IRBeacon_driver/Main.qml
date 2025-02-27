import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 480
    height: 680
    visible: true
    title: qsTr("IR Beacon Driver")

    Rectangle {
        id: mainpageBorder
        width: 480; height: 680
        y: 0; x: 0
        color: "lightgray"

        Rectangle {
            id: mainPage
            width: 470; height: 670
            y: 5; x: 5
            color: "white"

            Text {
                id: titleText
                text: "Set parameters and control IR beacon"
                y: 10; x: 10
                font.pointSize: 14
            }

            //setup form
            Rectangle {
                id: setupFormBox
                width: 450; height: 300
                anchors.top: titleText.bottom;
                anchors.left: parent.left;
                anchors.margins: 10;
                color: "lightgray"

                Text {
                    id: setupTitle
                    text: "Setup Device"
                    y: 10; x: 10
                    font.pointSize: 11
                }

                ColumnLayout {
                    spacing: 10
                    anchors.top: setupTitle.bottom
                    anchors.left: parent.left
                    anchors.margins: 10

                    RowLayout {
                        spacing: 10

                        Button {
                            text: "Initialize Serial Communication"
                            onClicked: controller.initCommsClicked()
                        }

                        TextField {
                            id: comportInput
                            placeholderText: qsTr("COM port name")
                            text: controller.comport
                            onTextChanged: controller.setComportText(text)
                        }
                    }

                    RowLayout {
                        spacing: 10

                        Button {
                            text: "Set frametime (microseconds)"
                            onClicked: controller.setFrameTimeClicked()
                        }

                        TextField {
                            id: fpsInput
                            placeholderText: qsTr("10000")
                            text: controller.frametime
                            onTextChanged: controller.setFrameTimeText(text)
                        }
                    }

                    RowLayout {
                        spacing: 10

                        Button {
                            text: "Set Dead Time Between Frames"
                            onClicked: controller.setDeadTimeClicked()
                        }

                        TextField {
                            id: deadTimeInput
                            placeholderText: qsTr("2000")
                            text: controller.deadtime
                            onTextChanged: controller.setDeadTimeText(text)
                        }
                    }


                    RowLayout {
                        spacing: 10

                        Button {
                            id: runButton
                            text: "RUN"
                            width: 120
                            height: 50
                            font.pixelSize: 18
                            font.bold: true
                            background: Rectangle {
                                id: bg
                                radius: 25
                                color: runButton.pressed ? "#0056b3" : (runButton.hovered ? "#0077cc" : "#0088ff")
                                gradient: Gradient {
                                    GradientStop { position: 0.0; color: "#0099ff" }
                                    GradientStop { position: 1.0; color: "#0055aa" }
                                }
                                border.color: "#004488"
                                border.width: 2
                            }
                            onClicked: controller.runClicked()




                        }
                    }
                }

            }

            //setup form
            Rectangle {
                id: controlFormBox
                width: 450; height: 300
                anchors.top: setupFormBox.bottom;
                anchors.left: parent.left;
                anchors.margins: 10;
                color: "lightgray"

                Text {
                    id: controlTitle
                    text: "Control 3D Glasses (Runtime)"
                    y: 10; x: 10
                    font.pointSize: 11
                }

                ColumnLayout {
                    spacing: 10
                    anchors.top: controlTitle.bottom
                    anchors.left: parent.left
                    anchors.margins: 10

                    RowLayout {
                        spacing: 10

                        Text {
                            id: setDelayTitle
                            text: "One time delay"
                            font.pointSize: 11
                        }

                        Button {
                            text: "100 us"
                            onClicked: controller.initCommsClicked()
                        }

                        Button {
                            text: "500 us"
                            onClicked: controller.initCommsClicked()
                        }

                        Button {
                            text: "1 ms"
                            onClicked: controller.initCommsClicked()
                        }

                    }
                    RowLayout {
                        spacing: 10
                        Text {
                            id: swapSidesTitle
                            text: "Swap Sides of Glasses"
                            font.pointSize: 11
                        }

                        Button {
                            text: "Swap"
                            onClicked: controller.initCommsClicked()
                        }
                    }


                    RowLayout {
                        spacing: 10
                        Button {
                            text: "Back To Setup"
                            onClicked: controller.initCommsClicked()
                        }
                    }
                }

            }
        }







    }
}






