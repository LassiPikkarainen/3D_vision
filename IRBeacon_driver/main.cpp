#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "serialwriter.h"
#include "ircontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;


    //IR controller
    IRController *controller = new IRController();
    engine.rootContext()->setContextProperty("controller", controller);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("IRBeacon_driver", "Main");





    //SerialWriter *s = new SerialWriter();
    //s->Init(L"COM3");
    return app.exec();
}
