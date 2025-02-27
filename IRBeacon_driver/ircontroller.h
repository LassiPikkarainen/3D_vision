#ifndef IRCONTROLLER_H
#define IRCONTROLLER_H

#include <QObject>
#include <QQmlEngine>
#include <QString>
#include "serialwriter.h"

class IRController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit IRController(QObject *parent = nullptr);
    Q_INVOKABLE void testButtonClicked();
    Q_INVOKABLE void initCommsClicked(); // init com button clicked
    Q_INVOKABLE void setFrameTimeClicked();
    Q_INVOKABLE void setDeadTimeClicked();
    Q_INVOKABLE void runClicked();


    Q_INVOKABLE void setComportText(const QString &text); //text changed
    Q_INVOKABLE void setFrameTimeText(const QString &text);
    Q_INVOKABLE void setDeadTimeText(const QString &text);

private:
    QString comport = "";
    QString frametime = "";
    QString deadtime = "";
    SerialWriter *s;

signals:
    void comportTextChanged();
    void frametimeTextChanged();
    void deadtimeTextChanged();
};

#endif // IRCONTROLLER_H
