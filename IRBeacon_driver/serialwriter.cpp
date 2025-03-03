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


SerialWriter::SerialWriter()
{

}

//setup the connection
bool SerialWriter::Init(QString name)
{


    //wComport = (const wchar_t*) name.utf16();
    //qDebug() << std::string(wComport);
    hSerial = CreateFileW( L"COM3", GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL );
    if (hSerial == INVALID_HANDLE_VALUE)
    {
        qInfo("Error\n");
        return false;
    }
    else qInfo("Succesfully created file handle\n");

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (GetCommState(hSerial, &dcbSerialParams) == 0)
    {
        qInfo("Error getting device state\n");
        CloseHandle(hSerial);
        return false;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if(SetCommState(hSerial, &dcbSerialParams) == 0)
    {
        qInfo("Error setting device parameters\n");
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
        qInfo("Error setting timeouts\n");
        CloseHandle(hSerial);
        return false;
    }
    qInfo("COM port setup succesful \n");


    return true;
}


//writes a char to the COM port
bool SerialWriter::WriteCommand(char byte, QString param)
{
    if (param.isEmpty()) {
        if(!WriteFile(hSerial, &byte, 1, NULL, NULL))
        {
            printf("Error\n");
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
        printf("Error\n");
        CloseHandle(hSerial);
        return false;
    }
    return true;
}

void SerialWriter::ReadSerial()
{
    unsigned long bytesRead;
    char responsebuffer[40];
    memset(responsebuffer, 0, sizeof(responsebuffer));
    ReadFile(hSerial, &responsebuffer, 39, &bytesRead, NULL);

    std::string readstr(responsebuffer);

    if (bytesRead > 0)
    {
        qDebug() << readstr;
    }

}














