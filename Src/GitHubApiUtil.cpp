#include "GitHubApiUtil.h"

#include <QCoreApplication>

#include <QUrl>
#include <QJsonDocument>
#include <QFile>
#include <QMap>
#include <QDebug>

#include "QNetWorker.h"

#define CONFIG_FILE_NAME    "apiConfig.json"

class GitHubApiUtil::PrivateData
{
public:
    PrivateData();

    bool initApiInfo(const QString &strUserName);

    bool isValid(int nType);

private:
    QString convertApiTypeToStrInfo(GitHubApiUtil::GITHUB_INFO_TYPE eType);

public:
    QString     m_strUserName;
    QNetWorker *m_pNewWorker;
    QVector<QString> m_vtrApiInfo;
};

GitHubApiUtil::PrivateData::PrivateData()
    :m_pNewWorker(QNetWorker::instance())
{
    m_strUserName.clear ();
    m_vtrApiInfo.clear ();
    m_vtrApiInfo.resize (GitHub_Type_Size);
}

bool GitHubApiUtil::PrivateData::isValid(int nType)
{
    if ( nType < 0 || nType >= GitHubApiUtil::GitHub_Type_Size)
    {
        return false;
    }

    return true;
}

QString GitHubApiUtil::PrivateData::convertApiTypeToStrInfo(GitHubApiUtil::GITHUB_INFO_TYPE eType)
{
    if( GitHubApiUtil::GitHub_Type_UsersInfo == eType){return "apiUsersInfo";}
    else if (GitHubApiUtil::GitHub_Type_AllReposInfos == eType) {return "apiAllResponses";}

    return "";
}

bool GitHubApiUtil::PrivateData::initApiInfo(const QString &strUserName)
{
    if ( strUserName.isEmpty ())
    {
        qWarning()<<"error strUserName is empty!";
        return false;
    }

    m_strUserName = strUserName;

    m_vtrApiInfo[GitHubApiUtil::GitHub_Type_UsersInfo] = QString("https://api.github.com/users/%1").arg (m_strUserName);
    m_vtrApiInfo[GitHubApiUtil::GitHub_Type_AllReposInfos] =  QString("https://api.github.com/users/%1/repos").arg (m_strUserName);
    m_vtrApiInfo[GitHubApiUtil::GitHub_Type_SingleReposInfo] =  QString("https://api.github.com/repos/users/%1").arg (m_strUserName);

    return true;
}

///////////////////////////////////////////////////////////////////////////////
GitHubApiUtil::GitHubApiUtil(QObject *parent)
    : QObject(parent),
      d(new GitHubApiUtil::PrivateData)
{
}

GitHubApiUtil::~GitHubApiUtil()
{
    if (d)
    {
        delete d;
        d = Q_NULLPTR;
    }
}

void GitHubApiUtil::setUserName(const QString &strUserName)
{
    d->initApiInfo (strUserName);
}


bool GitHubApiUtil::getGitHubInfos(GitHubApiUtil::GITHUB_INFO_TYPE eType, QString &strJsonInfo)
{
    if ( !d->isValid (eType))
    {
        strJsonInfo = "";
        return false;
    }

    strJsonInfo = QString(d->m_pNewWorker->get (d->m_vtrApiInfo.at (eType)));

    return true;
}

bool GitHubApiUtil::getGitHubUserInfo( QString &strJsonInfo)
{
    if ( !d->isValid (GitHubApiUtil::GitHub_Type_UsersInfo))
    {
        strJsonInfo = "";
        return false;
    }

    strJsonInfo = QString(d->m_pNewWorker->get (d->m_vtrApiInfo.at (GitHubApiUtil::GitHub_Type_UsersInfo)));

    return true;
}

bool GitHubApiUtil::getGitHubAllReposInfo(QString &strJsonInfo)
{
    if ( !d->isValid (GitHubApiUtil::GitHub_Type_AllReposInfos))
    {
        strJsonInfo = "";
        return false;
    }

    strJsonInfo = QString(d->m_pNewWorker->get (d->m_vtrApiInfo.at (GitHubApiUtil::GitHub_Type_AllReposInfos)));

    return true;
}

bool GitHubApiUtil::getGitHubSingleReposInfo(const QString &strReosName, QString &strJsonInfo)
{
    if ( !d->isValid (GitHubApiUtil::GitHub_Type_SingleReposInfo))
    {
        strJsonInfo = "";
        return false;
    }

    d->m_vtrApiInfo[GitHubApiUtil::GitHub_Type_SingleReposInfo] =  QString("%1/%2")
            .arg (GitHubApiUtil::GitHub_Type_SingleReposInfo).arg (strReosName);

    strJsonInfo = QString(d->m_pNewWorker->get (d->m_vtrApiInfo.at (GitHubApiUtil::GitHub_Type_SingleReposInfo)));

    return true;
}

