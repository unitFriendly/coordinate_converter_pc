#include "childprocess.h"

ChildProcess::ChildProcess(QString path, QStringList arguments, QObject *parent) : QObject(parent)
{
    childProcess.setProgram(path);
    childProcess.setParent(this);
    childProcess.setProcessChannelMode(QProcess::ForwardedChannels);
    childProcess.setArguments(arguments);
    connect(&childProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &ChildProcess::onFinished);

    childProcess.start();
}

ChildProcess::ChildProcess(QObject *parent) : QObject(parent)
{

}

void ChildProcess::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "Child process finish with code " << exitCode << "\n" << exitStatus;
}
