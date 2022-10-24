#include "serversocket.h"

const QString ServerSocket::___BINARY_IMAGE = QString("binary_image");
const QString ServerSocket::___X_COORDINATE = QString("x_coordinate");
const QString ServerSocket::___Y_COORDINATE = QString("y_coordinate");
const QString ServerSocket::___WIDTH_IMAGE = QString("width_image");
const QString ServerSocket::___HEIGHT_IMAGE = QString("height_image");
const QString ServerSocket::___CLOSE_APP = QString("close_app");
const QString ServerSocket::___TIME_MS = QString("time_ms");
const QString ServerSocket::___ERR_STRING = QString("err_string");

QString getIdentifier(QWebSocket *peer)
{
    return QStringLiteral("%1:%2").arg(peer->peerAddress().toString(), QString::number(peer->peerPort()));
}

QJsonObject StringToJsonObject(QString str)
{
    static QJsonDocument jDoc;
    jDoc = QJsonDocument::fromJson(str.toUtf8());
    return jDoc.object();
}

QString JsonObjectToString(QJsonObject *object)
{
    static QJsonDocument jDoc;
    jDoc.setObject(*object);
    return jDoc.toJson();
}

ServerSocket::ServerSocket(QObject *parent) : QObject(parent)
{
    socketClient = nullptr;
    socketServer = new QWebSocketServer(QStringLiteral("~~~ server ~~~"), QWebSocketServer::NonSecureMode, this);

    if (socketServer->listen(QHostAddress::Any, 0))
    {
        QTextStream(stdout) << "~~~ server ~~~ listening on port " << socketServer->serverPort() << '\n';
        connect(socketServer, &QWebSocketServer::newConnection, this, &ServerSocket::onNewConnection);
    }
}

void ServerSocket::onNewConnection()
{
    socketClient = socketServer->nextPendingConnection();
    QTextStream(stdout) << getIdentifier(socketClient) << " connected!\n";
    socketClient->setParent(this);

    connect(socketClient, &QWebSocket::textMessageReceived, this, &ServerSocket::processMessage);
    connect(socketClient, &QWebSocket::disconnected, this, &ServerSocket::socketDisconnected);

    disconnect(socketServer, &QWebSocketServer::newConnection, this, &ServerSocket::onNewConnection);
}

void ServerSocket::processMessage(const QString &message)
{
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    qDebug() << message;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    QJsonObject object = StringToJsonObject(message);

    if (!object.find(___X_COORDINATE)->isUndefined())
    {
        // Демонстрация ошибки
        return;
    }

    if ((object.find(___X_COORDINATE)->isUndefined()) || (object.find(___Y_COORDINATE)->isUndefined()))
    {
        return;
    }

    double xCoordinate = object[___X_COORDINATE].toDouble();
    double yCoordinate = object[___Y_COORDINATE].toDouble();

    // TODO emit получение новых координат
}

void ServerSocket::socketDisconnected()
{
    QTextStream(stdout) << getIdentifier(socketClient) << " disconnected!\n";

    disconnect(socketClient, &QWebSocket::textMessageReceived, this, &ServerSocket::processMessage);
    disconnect(socketClient, &QWebSocket::disconnected, this, &ServerSocket::socketDisconnected);
}

void ServerSocket::CloseClientProcess()
{
    QJsonObject* object = new QJsonObject({
                                              {___CLOSE_APP, QJsonValue(___CLOSE_APP)}
                                          });

    socketClient->sendTextMessage(JsonObjectToString(object));
}

void ServerSocket::SendImage(int width, int height, QByteArray &image)
{
    QJsonObject* object = new QJsonObject({
                                              {___WIDTH_IMAGE, QJsonValue(width)},
                                              {___HEIGHT_IMAGE, QJsonValue(height)},
                                              {___TIME_MS, QJsonValue(QTime::currentTime().msecsSinceStartOfDay())},
                                              {___BINARY_IMAGE, QJsonValue(QLatin1String(image.toBase64()))}
                                          });

    socketClient->sendTextMessage(JsonObjectToString(object));
}

quint16 ServerSocket::GetServerPort()
{
    return socketServer->serverPort();
}

void ServerSocket::sendImageFromPath(QString path)
{
    QFile imageFile(path);
    imageFile.open(QIODevice::ReadOnly);
    QByteArray image = imageFile.readAll();
    imageFile.close();

    QJsonObject* object = new QJsonObject({
                                              {___TIME_MS, QJsonValue(QTime::currentTime().msecsSinceStartOfDay())},
                                              {___BINARY_IMAGE, QJsonValue(QLatin1String(image.toBase64()))}
                                          });

    socketClient->sendTextMessage(JsonObjectToString(object));
}

bool ServerSocket::clientIsOnline()
{
    if((socketClient != nullptr) && (socketClient->state() == QAbstractSocket::ConnectedState))
    {
        return true;
    }

    return false;
}
