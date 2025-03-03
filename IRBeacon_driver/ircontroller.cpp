#include "ircontroller.h"
#include <QDebug>
#include <QString>
#include "serialwriter.h"
#include <QTimer>

IRController::IRController(QObject *parent): QObject{parent}
{
    s = new SerialWriter();
    //QString str = "COM3";

    readtimer = new QTimer(this);
    connect(readtimer, &QTimer::timeout, this, &IRController::readTimerTick);
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
    s->WriteCommand('S', frametime);
    //qDebug() << "Frametime: " << frametime;
}

Q_INVOKABLE void IRController::setDeadTimeClicked()
{
    s->WriteCommand('D', deadtime);
    //qDebug() << "Deadtime: " << deadtime;
}


Q_INVOKABLE void IRController::runClicked()
{
    s->WriteCommand('R');
    //qDebug() << "RUN!";
}

//runtime buttons

Q_INVOKABLE void IRController::delay1Clicked()
{
    s->WriteCommand('I');
    //qDebug() << "RUN!";
}

Q_INVOKABLE void IRController::delay2Clicked()
{
    s->WriteCommand('J');
    //qDebug() << "RUN!";
}

Q_INVOKABLE void IRController::delay3Clicked()
{
    s->WriteCommand('K');
    //qDebug() << "RUN!";
}

Q_INVOKABLE void IRController::swapClicked()
{
    s->WriteCommand('W');
    //qDebug() << "RUN!";
}

Q_INVOKABLE void IRController::setupClicked()
{
    s->WriteCommand('E');
    //qDebug() << "RUN!";
}


Q_INVOKABLE void IRController::setComportText(const QString &text)
{
    if (text != comport)
    {
        comport = text;
        emit comportTextChanged();
    }
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


void IRController::readTimerTick()
{
    s->ReadSerial();
}
