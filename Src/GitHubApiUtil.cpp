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

    bool initApiInfo(const QString &strFilePath);

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

bool GitHubApiUtil::PrivateData::initApiInfo(const QString &strFilePath)
{
    QFile file(strFilePath);
    if (!file.exists ())
    {
        m_vtrApiInfo[GitHubApiUtil::GitHub_Type_UsersInfo] = "https://api.github.com/users/your_Name";
        m_vtrApiInfo[GitHubApiUtil::GitHub_Type_AllReposInfos] = "https://api.github.com/users/your_Name/repos";

        if (file.open (QFile::WriteOnly))
        {
            QVariantMap vrtMap;

            for ( int i = 0; i < m_vtrApiInfo.size (); i++)
            {
                vrtMap.insert ( convertApiTypeToStrInfo ( GitHubApiUtil::GITHUB_INFO_TYPE(i)), m_vtrApiInfo.at (i));
            }

            QJsonDocument doc = QJsonDocument::fromVariant (vrtMap);
            QByteArray array = doc.toJson ();
            file.write (array);
            file.close ();
        }
        else
        {
            qWarning()<<"write| open file error:"<<strFilePath<<file.errorString ();
        }

        return true;
    }

    if (!file.open (QFile::ReadOnly))
    {
        qWarning()<<"open file error:"<<strFilePath<<file.errorString ();
        return false;
    }

    QByteArray jsonArray = file.readAll ();
    file.close ();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson (jsonArray, &error);
    if (doc.isEmpty () || error.error != QJsonParseError::NoError)
    {
        qWarning()<<"initApiInfo file error:"<<strFilePath<<error.errorString ();
        return false;
    }

    m_vtrApiInfo.resize (GitHub_Type_Size);
    m_vtrApiInfo[GitHub_Type_UsersInfo] = doc[convertApiTypeToStrInfo(GitHub_Type_UsersInfo)].toString ();

    return true;
}

///////////////////////////////////////////////////////////////////////////////
GitHubApiUtil::GitHubApiUtil(QObject *parent)
    : QObject(parent),
      d(new GitHubApiUtil::PrivateData)
{
    d->initApiInfo (QString("%1/%2")
                    .arg (QCoreApplication::applicationDirPath ())
                    .arg (CONFIG_FILE_NAME));
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
    d->m_strUserName = strUserName;
}


bool GitHubApiUtil::getGitHubInfos(GitHubApiUtil::GITHUB_INFO_TYPE eType, QString &strJsonInfo)
{
    if ( !d->isValid (eType))
    {
        strJsonInfo = "";
        return false;
    }

    strJsonInfo = QString(d->m_pNewWorker->get (QString("%1/%2")
                                                .arg (d->m_vtrApiInfo.at (eType))
                                                .arg (d->m_strUserName)));

    return true;
}

