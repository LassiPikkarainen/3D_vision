#ifndef SERIALWRITER_H
#define SERIALWRITER_H

#include <windows.h>
#include <stdio.h>
#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
#include<thread>
#include <chrono>
#include <thread>
#include <QString>
#include <QObject>
#include <QQmlEngine>

class SerialWriter : public QObject
{
    Q_OBJECT

public:
    explicit SerialWriter(QObject *parent = nullptr);
    bool Init(QString name);

public slots:
    Q_INVOKABLE bool WriteCommand(char byte, QString param = "");
    //bool SetPort(QString name);
    Q_INVOKABLE void ReadSerial();

signals:
    void serialReceived(QString str);


private:
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};

    LPCWSTR wComport = L"COM3"; //serial port to be used
    //char responsebuffer[40];
};

#endif // SERIALWRITER_H
