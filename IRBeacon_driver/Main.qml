import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: qtWindow
    width: 480
    height: 680
    visible: true
    title: qsTr("IR Beacon Driver")
    maximumHeight: height
    maximumWidth: width
    minimumHeight: height
    minimumWidth: width

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
            radius: 10  // Rounded corners
            border.color: "#b3b3b3"  // Dark gray border
            border.width: 2

            Text {
                id: titleText
                text: "Set parameters and control IR beacon"
                font.bold: true
                y: 10; x: 10
                font.pointSize: 14
            }

            //setup form
            Rectangle {
                id: setupFormBox
                width: 450; height: childrenRect.height + 20
                anchors.top: titleText.bottom;
                anchors.left: parent.left;
                anchors.margins: 10;
                color: "#f5f5f5"  // Light gray background
                radius: 10  // Rounded corners
                border.color: "#b3b3b3"  // Dark gray border
                border.width: 2

                Text {
                    id: setupTitle
                    text: "Setup Device"
                    font.bold: true
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
                            text: "Run Beacon"
                            font.pixelSize: 14
                            font.bold: true
                            highlighted: true
                            flat: true
                            background: Rectangle {
                                id: bg
                                radius: 5
                                color: parent.down ? "#DBEAF1" : (parent.hovered ? "#DBEAF1": "#DBEAF1")

                                border.color: "#55595C"
                                border.width: 2
                            }
                            onClicked: controller.runClicked()




                        }
                    }
                }

            }

            //runtime form
            Rectangle {
                id: controlFormBox
                width: 450; height: childrenRect.height + 20
                anchors.top: setupFormBox.bottom;
                anchors.left: parent.left;
                anchors.margins: 10;
                color: "#f5f5f5"  // Light gray background
                radius: 10  // Rounded corners
                border.color: "#b3b3b3"  // Dark gray border
                border.width: 2

                Text {
                    id: controlTitle
                    text: "Control 3D Glasses (Runtime)"
                    font.bold: true
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
                            onClicked: controller.delay1Clicked()
                        }

                        Button {
                            text: "500 us"
                            onClicked: controller.delay2Clicked()
                        }

                        Button {
                            text: "1 ms"
                            onClicked: controller.delay3Clicked()
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
                            onClicked: controller.swapClicked()
                        }
                    }


                    RowLayout {
                        spacing: 10
                        Button {
                            text: "Back To Setup"
                            font.pixelSize: 14
                            font.bold: true
                            highlighted: true
                            flat: true
                            background: Rectangle {
                                id: bg2
                                radius: 5
                                color: parent.down ? "#DBEAF1" : (parent.hovered ? "#DBEAF1": "#DBEAF1")

                                border.color: "#55595C"
                                border.width: 2
                            }
                            onClicked: controller.setupClicked()
                        }
                    }
                }

            }

            //output textbox
            Rectangle {
                id: outputBox
                width: 450;
                anchors.top: controlFormBox.bottom;
                anchors.bottom: mainPage.bottom;
                anchors.left: parent.left;
                anchors.right: parent.right;
                anchors.margins: 10;
                color: "#f5f5f5"  // Light gray background
                radius: 10  // Rounded corners
                border.color: "#b3b3b3"  // Dark gray border
                border.width: 2
                Text {
                    id: outputBoxTitle
                    font.bold: true
                    text: "Output From Beacon"
                    y: 10; x: 10
                    font.pointSize: 11
                }
                ScrollView {
                    id:scrollView
                    anchors.top: outputBoxTitle.bottom;
                    anchors.bottom: outputBox.bottom;
                    anchors.left: outputBox.left;
                    anchors.right: outputBox.right;
                    anchors.margins: 10;
                    TextArea {
                            id: outputTextWindow
                            anchors.top: outputBox.top
                            anchors.bottom: outputBox.bottom;
                            anchors.left: outputBox.left;
                            anchors.right: outputBox.right;
                            wrapMode: Text.Wrap
                            readOnly: true  // Prevents user input (output-only)
                            placeholderText: qsTr("Output from beacon")
                            text: controller.outputText
                            font.pixelSize: 12
                            color: "grey"
                            background: Rectangle {
                                color: "#FAFAFA"
                                border.color: "#888"
                                border.width: 1
                                radius: 5
                            }
                        }
                }





            }





        }







    }
}






