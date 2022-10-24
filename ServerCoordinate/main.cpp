#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "serversocket.h"
#include "childprocess.h"
#include "basefunction.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    qmlRegisterType<ServerSocket>("ConvertPZ90.ServerSocket", 0, 1, "ServerSocket");
    qmlRegisterType<ChildProcess>("ConvertPZ90.ChildProcess", 0, 1, "ChildProcess");
    qmlRegisterType<BaseFunction>("ConvertPZ90.BaseFunction", 0, 1, "BaseFunction");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    ServerSocket server;
    engine.rootContext()->setContextProperty("server", &server);

    BaseFunction baseFunction;
    engine.rootContext()->setContextProperty("baseFunction", &baseFunction);

    return app.exec();
}
