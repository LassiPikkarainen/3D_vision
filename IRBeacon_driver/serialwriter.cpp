#include "serialwriter.h"
#include <windows.h>
#include <stdio.h>
#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
#include<thread>
#include <chrono>
#include <thread>
#include<QDebug>
#include <QString>
#include <QThread>

SerialWriter::SerialWriter(QObject *parent): QObject{parent}
{

}

//setup the connection
Q_INVOKABLE bool SerialWriter::Init(QString name)
{

    emit writeToOutputBox(QString("Setting up serial comms [%1]").arg(name));
    wComport = (const wchar_t*) name.utf16(); //filename must be in a wchar_t format ??
    //qDebug() << std::string(wComport);
    //hSerial = CreateFileW( L"COM3", GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL );
    hSerial = CreateFileW( wComport, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL );

    if (hSerial == INVALID_HANDLE_VALUE)
    {
        emit writeToOutputBox("Error while creating file handle for serial communication");
        qInfo("Error while creating file handle for serial communication");
        return false;
    }
    else
    {
        emit writeToOutputBox("Succesfully created file handle");
        qInfo("Succesfully created file handle");
    }

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (GetCommState(hSerial, &dcbSerialParams) == 0)
    {
        qInfo("Error getting device state");
        emit writeToOutputBox("Error getting device state");
        CloseHandle(hSerial);
        return false;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if(SetCommState(hSerial, &dcbSerialParams) == 0)
    {
        emit writeToOutputBox("Error setting device parameters");
        qInfo("Error setting device parameters");
        CloseHandle(hSerial);
        return false;
    }

    // Set COM port timeout settings
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if(SetCommTimeouts(hSerial, &timeouts) == 0)
    {
        emit writeToOutputBox("Error setting timeouts");
        qInfo("Error setting timeouts");
        CloseHandle(hSerial);
        return false;
    }
    emit writeToOutputBox("COM port setup succesful");
    qInfo("COM port setup succesful");


    return true;
}


//writes a char to the COM port
Q_INVOKABLE bool SerialWriter::WriteCommand(char byte, QString param)
{
    qDebug() << "Running in thread:" << QThread::currentThread();
    if (param.isEmpty()) {
        if(!WriteFile(hSerial, &byte, 1, NULL, NULL))
        {
            emit writeToOutputBox("Could not write to serial, re-initialize comms");
            CloseHandle(hSerial);
            return false;
        }
        return true;
    }

    //create line to be written by adding param after command byte separated by a space
    const int commandlen = 1 + 1 + param.size();
    char* command = new char[commandlen];
    command[0] = byte;
    command[commandlen-1] = '\n';
    strcpy(command + 1, param.toStdString().c_str());
    qDebug() << command;
    if(!WriteFile(hSerial, command, commandlen, NULL, NULL))
    {
        emit serialReceived("Could not write to serial, re-initialize comms");
        CloseHandle(hSerial);
        return false;
    }
    return true;
}

Q_INVOKABLE void SerialWriter::ReadSerial()
{
    //qDebug() << "Running in thread:" << QThread::currentThread();
    unsigned long bytesRead;
    char responsebuffer[40];
    memset(responsebuffer, 0, sizeof(responsebuffer));
    ReadFile(hSerial, &responsebuffer, 39, &bytesRead, NULL);

    std::string readstr(responsebuffer);

    if (bytesRead > 0)
    {
        qDebug() << readstr;
        emit serialReceived(QString::fromStdString(readstr)); //emit signal that the main thread can catch
    }

}














