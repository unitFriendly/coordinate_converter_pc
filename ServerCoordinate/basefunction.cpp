#include "basefunction.h"

BaseFunction::BaseFunction(QObject *parent) : QObject(parent)
{

}

bool BaseFunction::deleteFile(QString path)
{
    return QFile::remove(path);
}
