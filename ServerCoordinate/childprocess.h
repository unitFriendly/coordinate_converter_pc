#ifndef CHILDPROCESS_H
#define CHILDPROCESS_H

#include <QObject>
#include <QProcess>
#include <QDebug>

class ChildProcess : public QObject
{
    Q_OBJECT
public:
    explicit ChildProcess(QString path, QStringList arguments, QObject *parent = nullptr);
    explicit ChildProcess(QObject *parent = nullptr);

private slots:
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QProcess childProcess;

signals:

};

#endif // CHILDPROCESS_H
