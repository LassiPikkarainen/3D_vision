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

class SerialWriter
{
public:
    SerialWriter();
    bool Init(QString name);
    bool WriteCommand(char byte);
    //bool SetPort(QString name);


private:
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};
    LPCWSTR wComport = L"COM3"; //serial port to be used

};

#endif // SERIALWRITER_H
