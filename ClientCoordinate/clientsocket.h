#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QtWebSockets>
#include <QJsonDocument>
#include <QJsonObject>
#include <QImage>

#include "tesseract/baseapi.h"
#include "leptonica/allheaders.h"

class ClientSocket : public QObject
{
    Q_OBJECT
public:
    explicit ClientSocket(QUrl &url, QObject *parent = nullptr);

private slots:
    void onConnected();
    void onTextMessageReceived(QString message);
    void onClosed();

private:
    QWebSocket* serverSocket;
    QUrl urlServer;
    tesseract::TessBaseAPI* tessBaseApi;

    QImage FilterWhite(QImage &src, int step);
    QPair<float, float> FilterString(QString src);
    QPair<float, float> FindCoordinate();

    QSet<QChar> validSymbol;

    const static QString ___BINARY_IMAGE;
    const static QString ___X_COORDINATE;
    const static QString ___Y_COORDINATE;
    const static QString ___WIDTH_IMAGE;
    const static QString ___HEIGHT_IMAGE;
    const static QString ___CLOSE_APP;
    const static QString ___TIME_MS;
    const static QString ___ERR_STRING;

};

#endif // CLIENTSOCKET_H
