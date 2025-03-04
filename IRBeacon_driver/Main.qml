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

            Text {
                id: titleText
                text: "Set parameters and control IR beacon"
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

            //runtime form
            Rectangle {
                id: controlFormBox
                width: 450; height: childrenRect.height + 20
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
                color: "lightgray";

                Text {
                    id: outputBoxTitle
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
                            anchors.top: scrollView.top
                            anchors.bottom: scrollView.bottom;
                            anchors.left: scrollView.left;
                            anchors.right: scrollView.right;
                            wrapMode: Text.Wrap
                            readOnly: true  // Prevents user input (output-only)
                            placeholderText: qsTr("Output from beacon")
                            text: controller.outputText
                            font.pixelSize: 16
                            color: "black"
                            background: Rectangle {
                                color: "#f0f0f0"
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






