#include "ircontroller.h"
#include <QDebug>
#include <QString>
#include "serialwriter.h"
#include <QTimer>
#include <QThread>

IRController::IRController(QObject *parent): QObject{parent}
{

    commThread = new QThread();
    s = new SerialWriter();
    s->moveToThread(commThread);
    commThread->start();
    readtimer = new QTimer(this);
    connect(readtimer, &QTimer::timeout, this, &IRController::readTimerTick);

    connect(commThread, &QThread::started, s, &SerialWriter::ReadSerial);

    connect(s, &SerialWriter::serialReceived, this, &IRController::receivedFromSerial);
    //QObject::connect(commThread, &QThread::started, s, &SerialWriter::WriteCommand);
}

Q_INVOKABLE void IRController::testButtonClicked()
{
    qDebug("Button was clicked");
}

Q_INVOKABLE void IRController::initCommsClicked()
{
    //qDebug() << "com port name: " << comport;
    s->Init(comport);
    readtimer->start(1000);
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


Q_INVOKABLE void IRController::setComportText(const QString &text)
{
    if (text != comport)
    {
        comport = text;
        emit comportTextChanged();
    }
}

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
    if (str != outputText)
    {
        outputText = str;
        emit outputTextChanged();
    }
    qDebug() << "read from serial " << outputText;

}


void IRController::readTimerTick()
{
    //s->ReadSerial();
    invokeRead();
}

void IRController::receivedFromSerial(QString str)
{
    setOutputText(str);
}



