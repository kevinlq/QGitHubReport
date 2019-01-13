#include "QInterface.h"

#include "QManager.h"

QInterface::QInterface(QObject *parent) : QObject(parent)
{
}

QInterface::~QInterface()
{
}

QString QInterface::initReposInfo(const QString &strUserName)
{
    return QManager::instance ()->initReposInfo (strUserName);
}
