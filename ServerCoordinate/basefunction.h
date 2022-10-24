#ifndef BASEFUNCTION_H
#define BASEFUNCTION_H

#include <QObject>
#include <QFile>

class BaseFunction : public QObject
{
    Q_OBJECT
public:
    explicit BaseFunction(QObject *parent = nullptr);

    Q_INVOKABLE bool deleteFile(QString path);

signals:

};

#endif // BASEFUNCTION_H
