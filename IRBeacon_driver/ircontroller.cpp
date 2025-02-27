#include "ircontroller.h"
#include <QDebug>
#include <QString>
#include "serialwriter.h"

IRController::IRController(QObject *parent): QObject{parent}
{
    s = new SerialWriter();
    //QString str = "COM3";
}

Q_INVOKABLE void IRController::testButtonClicked()
{
    qDebug("Button was clicked");
}

Q_INVOKABLE void IRController::initCommsClicked()
{
    qDebug() << "com port name: " << comport;
    s->Init(comport);
}

Q_INVOKABLE void IRController::setFrameTimeClicked()
{
    qDebug() << "Frametime: " << frametime;
}

Q_INVOKABLE void IRController::setDeadTimeClicked()
{
    qDebug() << "Deadtime: " << deadtime;
}


Q_INVOKABLE void IRController::runClicked()
{
    qDebug() << "RUN!";
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

