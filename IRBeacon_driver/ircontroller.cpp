#include "ircontroller.h"
#include <QDebug>
#include <QString>
#include "serialwriter.h"
#include <QTimer>
#include <QThread>

IRController::IRController(QObject *parent): QObject{parent}
{
    beacon_state = 0;
    command_in_queue = 0;
    commThread = new QThread();
    s = new SerialWriter();
    s->moveToThread(commThread);
    commThread->start();
    readtimer = new QTimer(this);
    setuptimer = new QTimer(this);

    connect(readtimer, &QTimer::timeout, this, &IRController::readTimerTick);

    connect(setuptimer, &QTimer::timeout, this, &IRController::setupClicked);

    connect(commThread, &QThread::started, s, &SerialWriter::ReadSerial);

    //signals from serial writer
    connect(s, &SerialWriter::serialReceived, this, &IRController::receivedFromSerial);
    connect(s, &SerialWriter::writeToOutputBox, this, &IRController::writeInfoToOutputBox);


    //QObject::connect(commThread, &QThread::started, s, &SerialWriter::WriteCommand);
}

Q_INVOKABLE void IRController::testButtonClicked()
{
    qDebug("Button was clicked");
}

Q_INVOKABLE void IRController::initCommsClicked()
{
    //qDebug() << "com port name: " << comport;
    bool init = s->Init(comport);
    if (init)
    {
        beacon_state = 1;
        readtimer->start(1000);
        setuptimer->start(2000);
    }


}

Q_INVOKABLE void IRController::setFrameTimeClicked()
{
    invokeCommThreadWrite('S', frametime);
    //qDebug() << "Frametime: " << frametime;
}

Q_INVOKABLE void IRController::setDeadTimeClicked()
{
    invokeCommThreadWrite('D', deadtime);
    //qDebug() << "Deadtime: " << deadtime;
}


Q_INVOKABLE void IRController::runClicked()
{
    invokeCommThreadWrite('R');
}

//runtime buttons

Q_INVOKABLE void IRController::delay1Clicked()
{
    invokeCommThreadWrite('I');
}

Q_INVOKABLE void IRController::delay2Clicked()
{
    invokeCommThreadWrite('J');
}

Q_INVOKABLE void IRController::delay3Clicked()
{
    invokeCommThreadWrite('K');
}

Q_INVOKABLE void IRController::swapClicked()
{
    invokeCommThreadWrite('W');
}

Q_INVOKABLE void IRController::setupClicked()
{
    invokeCommThreadWrite('E');
}





//invoke methods on the commthread
void IRController::invokeCommThreadWrite(char command, QString str)
{
    qDebug() << "invoking write at" << commThread <<"from " << QThread::currentThread();
    QMetaObject::invokeMethod(s, "WriteCommand", Qt::QueuedConnection, Q_ARG(char, command), Q_ARG(QString, str));
}

void IRController::invokeRead()
{
    //qDebug() << "invoking read at" << commThread <<"from " << QThread::currentThread();
    QString ret;
    QMetaObject::invokeMethod(s, "ReadSerial", Qt::QueuedConnection);
    //qDebug() << "read from serial" << ret;
}


//qml setter methods
Q_INVOKABLE void IRController::setDeadTimeText(const QString &text)
{
    if (text != deadtime)
    {
        deadtime = text;
        emit deadtimeTextChanged();
    }
}

Q_INVOKABLE void IRController::setFrameTimeText(const QString &text)
{
    if (text != frametime)
    {
        frametime = text;
        emit frametimeTextChanged();
    }
}

Q_INVOKABLE void IRController::setOutputText(QString str)
{
    //format incoming text for the output text box
    str.replace("\n", " ");
    str.replace("\r", " | ");
    str.prepend(">");
    outputText.append(str).append("\n");
    emit outputTextChanged();

}

Q_INVOKABLE void IRController::setComportText(const QString &text)
{
    if (text != comport)
    {
        comport = text;
        emit comportTextChanged();
    }
}

//write any string to the output box
Q_INVOKABLE void IRController::writeInfoToOutputBox(QString str)
{
    setOutputText(str);
}


void IRController::readTimerTick()
{
    //s->ReadSerial();
    invokeRead();
}

void IRController::receivedFromSerial(QString str)
{
    parseBeaconState(str);
}

void IRController::parseBeaconState(QString str)
{
    //parses beacon state from the received serial comms

    //got the connected -message from beacon
    if (str.toLower().contains("connected") || str.toLower().contains("setupmode"))
    {
        beacon_state = 2;
        setOutputText("Beacon in setup mode\n");
        if (setuptimer->isActive()) {
            qDebug() << "stopped setuptimer";
            setuptimer->stop();
        }
    }

    else if (str.toLower().contains("could not write"))
    {
        beacon_state = 0;
        setOutputText("Connection dead\n");

    }


    //run-command needs two integers in response, first is period, second is the dead time
    else if (str.toLower().contains("run") || command_in_queue == 3)
    {
        beacon_state = 3;
        QString tmp = str.replace("\n", " ").replace("\r", " ");//remove characters
        QStringList ls = tmp.split(" ");

        bool intCheck = false;


        //if command in queue, check whether time values have already been received
        int frametime_tmp = -1;
        int deadtime_tmp = -1;

        if (command_in_queue == 3)
        {
            if (run_comm_frametimestorage > 0)
            {
                frametime_tmp = run_comm_frametimestorage;
            }
            if (run_comm_deadtimestorage > 0)
            {
                deadtime_tmp = run_comm_deadtimestorage;
            }
        }

        //parse input
        foreach(QString element, ls)
        {
            int tmpint = element.toInt(&intCheck);
            if (intCheck) //integer was found
            {
                if (frametime_tmp < 0)
                {
                    frametime_tmp = tmpint;
                }
                else
                {
                    deadtime_tmp = tmpint;
                }
            }
        }


        //set command in queue & storages depending on current state
        if (deadtime_tmp > 0 && frametime_tmp > 0)
        {
            command_in_queue = 0;
            run_comm_deadtimestorage = -1;
            run_comm_frametimestorage = -1;
            QString qs = "Beacon running with frametime";
            qs.append(QString::number(frametime_tmp)).append(", dead time: ").append(QString::number(deadtime_tmp)).append("\n");
            setOutputText(qs);
        }

        //only frametime was received
        else if (frametime_tmp > 0 && deadtime_tmp < 0)
        {
            command_in_queue = 3;
            run_comm_frametimestorage = frametime_tmp;
            run_comm_deadtimestorage = -1;
             qDebug() << "waiting to receive dead time from run command";
        }

        //neither
        else if (frametime_tmp < 0 && deadtime_tmp < 0)
        {
            command_in_queue = 3;
            run_comm_frametimestorage = -1;
            run_comm_deadtimestorage = -1;
            qDebug() << "waiting to receive frame time and dead time from run command";
        }



    }

    //either new resonse to frametime -command, or the command was already partially received
    else if (str.toLower().contains("frametime") || command_in_queue == 1)
    {
        QString tmp = str.replace("\n", " ").replace("\r", " ");//remove characters
        QStringList ls = tmp.split(" ");

        //try to find an integer from the received contents -> if not found, buffer this and wait for it to arrive
        bool intCheck = false;
        foreach(QString element, ls)
        {
            int frametime = element.toInt(&intCheck);
            if (intCheck)
            {
                break;
            }
        }

        //succesfully parsed an integer from the response
        if (intCheck)
        {
            QString qs = "Succesfully set frametime to " + frametime + "\n";
            setOutputText(qs);
            command_in_queue = 0;
        }

        //wait to receive the full response before printing anything
        else
        {
            qDebug() << "waiting to receive frametime";
            command_in_queue = 1;
        }

    }

    else if (str.toLower().contains("deadtime") || command_in_queue == 2)
    {
        QString tmp = str.replace("\n", " ").replace("\r", " ");//remove characters
        QStringList ls = tmp.split(" ");

        bool intCheck = false;
        foreach(QString element, ls)
        {
            int deadtime = element.toInt(&intCheck);
            if (intCheck)
            {
                break;
            }
        }

        //succesfully parsed an integer from the response
        if (intCheck)
        {
            QString s = "Succesfully set deadtime to " + deadtime + "\n";
            setOutputText(s);
            command_in_queue = 0;
        }

        //wait to receive the full response before printing anything
        else
        {
            qDebug() << "waiting to receive frametime";
            command_in_queue = 2;
        }
    }

}



