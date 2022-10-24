#include <QCoreApplication>
#include <QUrl>
#include <QDebug>
#include <QException>
#include "clientsocket.h"

#include <QImage>
#include <QBitmap>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int port = -1;

    if ((argc > 1) && (!QString::compare(argv[1], "-port")))
    {
        try
        {
            port = QString(argv[2]).toInt();
        }
        catch (QException ex)
        {
            qDebug() << ex.what();
            a.exit();
        }
    }
    else
    {
        qDebug() << "Error: cmd string\n" << argv << "\n";

        a.exit();
    }

    QUrl url = QUrl(QStringLiteral("ws://localhost:") + QString::number(port));

    ClientSocket client(url);

    return a.exec();
}
