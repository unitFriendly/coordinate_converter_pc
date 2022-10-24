#include "clientsocket.h"

const QString ClientSocket::___BINARY_IMAGE = QString("binary_image");
const QString ClientSocket::___X_COORDINATE = QString("x_coordinate");
const QString ClientSocket::___Y_COORDINATE = QString("y_coordinate");
const QString ClientSocket::___WIDTH_IMAGE = QString("width_image");
const QString ClientSocket::___HEIGHT_IMAGE = QString("height_image");
const QString ClientSocket::___CLOSE_APP = QString("close_app");
const QString ClientSocket::___TIME_MS = QString("time_ms");
const QString ClientSocket::___ERR_STRING = QString("err_string");

QPair<float, float> ClientSocket::FilterString(QString src)
{
    QPair<float, float> result(-1.0, -1.0);

    src = src.trimmed().replace(" ", "").replace("/", "").replace("-", "").replace(",", ".");

    for(int i = 0; i < src.length(); i++)
    {
        if(!validSymbol.intersects({src[i]}))
        {
            return result;
        }
    }

    int indexN = src.indexOf("N");

    if(indexN == -1)
    {
        return result;
    }

    int indexE = src.indexOf("E");

    if(indexE == -1)
    {
        return result;
    }

    QString valueNStr = src.mid(0, indexN);
    src.remove(0, indexN + 1);

    QString valueEStr = src.mid(0, src.length() - 1);

    float N;
    float E;

    try
    {
        N = valueNStr.toFloat();
        E = valueEStr.toFloat();
    }
    catch (QException exp)
    {
        return result;
    }

    if (N == 0.0)
    {
        return result;
    }

    if (E == 0.0)
    {
        return result;
    }

    result.first = N;
    result.second = E;

    return result;
}

QPair<float, float> ClientSocket::FindCoordinate()
{
    QImage imgSrc("fromServer.jpeg");
    QImage imgDst;
    Pix *pixImage;
    QString outText;
    QPair<float, float> resultCoordinat;

    static int cccount = 1;

    QString currentTime = QString::number(cccount);

    cccount += 1;

    QString tmpName;

    for(int step = 175; step <= 400; step += 25)
    {
        imgDst = FilterWhite(imgSrc, step);

        tmpName = currentTime + " - " + QString::number(step) + ".jpeg";

//        imgDst.save("fromServerAfterFilter.jpeg");
        imgDst.save(tmpName);

//        pixImage = pixRead("fromServerAfterFilter.jpeg");
        pixImage = pixRead(tmpName.toStdString().c_str());

        tessBaseApi->SetImage(pixImage);

        outText = QString::fromUtf8(tessBaseApi->GetUTF8Text()).trimmed().replace("\n", " ");

//        qDebug() << outText;

        pixDestroy(&pixImage);

        resultCoordinat = FilterString(outText);

        if(resultCoordinat.first != -1.0)
        {
//            QFile::remove("fromServerAfterFilter.jpeg");
            return resultCoordinat;
        }
    }

//    QFile::remove("fromServerAfterFilter.jpeg");
    return QPair<float, float>(-1.0, -1.0);
}

QImage ClientSocket::FilterWhite(QImage &src, int step)
{
    QImage dst(src.width(), src.height(), QImage::Format_RGB888);
    QColor colorPixel;

    for(int i = 0; i < src.width(); i++)
    {
        for(int j = 0; j < src.height(); j++)
        {
            colorPixel = src.pixelColor(i, j);

            int sum = colorPixel.red() + colorPixel.green() + colorPixel.blue();

            if(sum < step)
            {
                dst.setPixelColor(i, j, QColorConstants::White);
            }
            else
            {
                dst.setPixelColor(i, j, QColorConstants::Black);
            }
        }
    }

    return dst;
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

ClientSocket::ClientSocket(QUrl &url, QObject *parent) : QObject(parent)
{
    validSymbol = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'N', 'E', '.'};

    tessBaseApi = new tesseract::TessBaseAPI();

    if (tessBaseApi->Init("C:/Users/rota/Documents/ImageToCoordinate/tessdata", "eng")) {
        qDebug() << "Could not initialize tesseract\n";
        qApp->exit();
    }

    serverSocket = new QWebSocket();
    urlServer = url;

    connect(serverSocket, &QWebSocket::connected, this, &ClientSocket::onConnected);
    connect(serverSocket, &QWebSocket::disconnected, this, &ClientSocket::onClosed);

    serverSocket->open(QUrl(url));
}

void ClientSocket::onConnected()
{
    QTextStream(stdout) << "~~~ client ~~~ connected on port " << serverSocket->peerPort() << "\n";

    connect(serverSocket, &QWebSocket::textMessageReceived, this, &ClientSocket::onTextMessageReceived);
}

void ClientSocket::onTextMessageReceived(QString message)
{
    QJsonObject object = StringToJsonObject(message);

    if (!object.find(___CLOSE_APP)->isUndefined())
    {
        serverSocket->close();//1

        qApp->exit();

        return;
    }

    int timeMS = object[___TIME_MS].toInt();

    if(abs(QTime::currentTime().msecsSinceStartOfDay() - timeMS) > 1000)
    {
        return;
    }

//    int widthImage = object[___WIDTH_IMAGE].toInt();
//    int heightImage = object[___HEIGHT_IMAGE].toInt();
    QByteArray image = QByteArray::fromBase64(object[___BINARY_IMAGE].toString().toUtf8());

    QFile file("fromServer.jpeg");
    file.open(QIODevice::WriteOnly);
    file.write(image);
    file.close();

    QPair<float, float> coordinat = FindCoordinate();

    if(coordinat.first != -1.0)
    {
        qDebug() << "\t\t\t\t\t" << coordinat;
    }
    else
    {
        qDebug() << coordinat;
    }

    return;

    double xCoordinate = 0.0;
    double yCoordinate = 0.0;

    QJsonObject* answer = new QJsonObject({
                                            {___X_COORDINATE, QJsonValue(xCoordinate)},
                                            {___Y_COORDINATE, QJsonValue(yCoordinate)}
                                          });

    serverSocket->sendTextMessage(JsonObjectToString(answer));
}

void ClientSocket::onClosed()
{
    QTextStream(stdout) << "~~~ server ~~~ close!\n";

    disconnect(serverSocket, &QWebSocket::textMessageReceived, this, &ClientSocket::onTextMessageReceived);
    disconnect(serverSocket, &QWebSocket::connected, this, &ClientSocket::onConnected);
    disconnect(serverSocket, &QWebSocket::disconnected, this, &ClientSocket::onClosed);

    qApp->exit();
}
