#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>

#include <QImage>
#include <QFile>

class ServerSocket : public QObject
{
    Q_OBJECT
public:
    explicit ServerSocket(QObject *parent = nullptr);

private slots:
    void onNewConnection();
    void processMessage(const QString &message);
    void socketDisconnected();

private:
    QWebSocketServer *socketServer;
    QWebSocket *socketClient;

    const static QString ___BINARY_IMAGE;
    const static QString ___X_COORDINATE;
    const static QString ___Y_COORDINATE;
    const static QString ___WIDTH_IMAGE;
    const static QString ___HEIGHT_IMAGE;
    const static QString ___CLOSE_APP;
    const static QString ___TIME_MS;
    const static QString ___ERR_STRING;

public:
    void CloseClientProcess();
    void SendImage(int width, int height, QByteArray& image);
    quint16 GetServerPort();

    Q_INVOKABLE void sendImageFromPath(QString path);
    Q_INVOKABLE bool clientIsOnline();

signals:

};

#endif // SERVERSOCKET_H
