#ifndef IRCONTROLLER_H
#define IRCONTROLLER_H

#include <QObject>
#include <QQmlEngine>
#include <QString>
#include "serialwriter.h"
#include <QTimer>
#include <QThread>
class IRController : public QObject
{
    Q_OBJECT;
    QML_ELEMENT;

    //Properties accessible from qml
    Q_PROPERTY(QString outputText READ getOutputText WRITE setOutputText NOTIFY outputTextChanged)
    Q_PROPERTY(QString comport READ getComport WRITE setComportText NOTIFY comportTextChanged)
    Q_PROPERTY(QString deadtime READ getDeadTime WRITE setDeadTimeText NOTIFY deadtimeTextChanged)
    Q_PROPERTY(QString frametime READ getFrameTime WRITE setFrameTimeText NOTIFY frametimeTextChanged)
    Q_PROPERTY(int beaconstate READ getBeaconState NOTIFY beaconStateChanged)
public:
    explicit IRController(QObject *parent = nullptr);
    Q_INVOKABLE void testButtonClicked();
    Q_INVOKABLE void initCommsClicked(); // init com button clicked
    Q_INVOKABLE void setFrameTimeClicked();
    Q_INVOKABLE void setDeadTimeClicked();
    Q_INVOKABLE void runClicked();

    //runtime buttons
    Q_INVOKABLE void delay1Clicked();
    Q_INVOKABLE void delay2Clicked();
    Q_INVOKABLE void delay3Clicked();
    Q_INVOKABLE void swapClicked();
    Q_INVOKABLE void setupClicked();

    //qml methods for updating
    Q_INVOKABLE void setComportText(const QString &text); //text changed
    Q_INVOKABLE void setFrameTimeText(const QString &text);
    Q_INVOKABLE void setDeadTimeText(const QString &text);
    Q_INVOKABLE void setOutputText(QString text);

    //qml methods for fetching
    QString getOutputText()
    {
        return outputText;
    }

    QString getComport()
    {
        return comport;
    }

    QString getDeadTime()
    {
        return deadtime;
    }
    QString getFrameTime()
    {
        return frametime;
    }

    int getBeaconState()
    {
        return beacon_state;
    }


    void readTimerTick();

    void invokeCommThreadWrite(char command, QString str = ""); //invokes writecommand in another thread
    void invokeRead(); //invokes serial read in another thread

    QString comport = "";
    QString frametime = "";
    QString deadtime = "";
    QString outputText = "";

public slots:
    void receivedFromSerial(QString str);
    Q_INVOKABLE void writeInfoToOutputBox(QString str);

private:

    SerialWriter *s;
    QTimer *readtimer;
    QTimer *setuptimer; // once comms initialized, try to periodically send a command for the beacon to go to setupmode
    QThread *commThread;
    int beacon_state; // 0=not connected, 1=initialized, 2=in setup, 3=running
    int command_in_queue; //0 = nothing, 1 = frametime, 2 = dead time, 3 = run

    //run command response needs two integers -> might need to buffer
    int run_comm_frametimestorage; // -1 if nothing stored
    int run_comm_deadtimestorage;
    void parseBeaconState(QString str);

signals:
    void comportTextChanged();
    void frametimeTextChanged();
    void deadtimeTextChanged();
    void outputTextChanged();
    void beaconStateChanged();
};

#endif // IRCONTROLLER_H
