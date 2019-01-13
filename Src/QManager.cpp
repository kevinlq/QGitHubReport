#include "QManager.h"

/*include Qt file*/
#include <QApplication>
#include <QQmlApplicationEngine>

#include <QJsonDocument>

#include <QDateTime>

#include <QFile>

#include <QDebug>

#include "GitHubApiUtil.h"
#include "QInterface.h"

#define SAFE_DELETE(p) { if(p){delete(p); (p)=NULL;} }

class PrivateData
{
public:
    /**
      define data struct for Repositories info
    */
    struct DS_CommitBase
    {
        QString m_strCommitDate;    /// < your commit Data
        QString m_strCommitName;    /// < commit name

        DS_CommitBase():m_strCommitDate(""),m_strCommitName(""){}
        ~DS_CommitBase(){}
    };

    struct DS_ReposInfoBase
    {
        DS_ReposInfoBase();
        ~DS_ReposInfoBase();

        QVariantList convertToList();

        QString m_strReposName;                     /// < repositories name
        QVector<DS_CommitBase*> m_vtrpCommintInfo;
    };

    typedef QVector<DS_ReposInfoBase *> VTRP_ReposBaseInfo;
public:
    PrivateData():m_strFilePath(""),m_pEngine(Q_NULLPTR){}
    ~PrivateData();

    bool isUserExists(const QString &strUserName);
    bool isReposExists( const QString &strReposName);

    QString getUserReposInfo(const QString &strUserName);

    bool parseReposJsonInfo(const QString &strUserName,const QString &strJson);

public:
    QString m_strFilePath;
    QQmlApplicationEngine *m_pEngine;
    QMap<QString, VTRP_ReposBaseInfo > m_mapReposInfo;
};

PrivateData::~PrivateData()
{
    if (m_pEngine != Q_NULLPTR)
    {
        m_pEngine->deleteLater();
        m_pEngine = Q_NULLPTR;
    }

    foreach (auto it, m_mapReposInfo.values ())
    {
        qDeleteAll(it);
    }

    m_mapReposInfo.clear ();
}

bool PrivateData::isReposExists(const QString &strReposName)
{
    foreach (auto it, m_mapReposInfo.values ())
    {
        for ( int i = 0; i < it.size (); i++)
        {
            auto pItem = it.at (i);
            if (pItem == Q_NULLPTR)
            {
                continue;
            }

            if ( pItem->m_strReposName == strReposName)
            {
                return true;
            }
        }
    }

    return false;
}

bool PrivateData::isUserExists(const QString &strUserName)
{
    return m_mapReposInfo.contains (strUserName);
}

QString PrivateData::getUserReposInfo(const QString &strUserName)
{
    if ( !m_mapReposInfo.contains (strUserName))
    {
        qWarning()<<"error: not info:"<<strUserName;
        return "";
    }

    QVariantList lsRepos;
    foreach (auto it, m_mapReposInfo.values ())
    {
        for ( int i = 0; i < it.size (); i++)
        {
            auto pItem = it.at (i);
            if (pItem == Q_NULLPTR)
            {
                continue;
            }

            QVariantMap subMap;
            subMap.insert ("strReposName", pItem->m_strReposName);
            subMap.insert ("commitCount", pItem->m_vtrpCommintInfo.size ());
            lsRepos.push_back (subMap);
        }
    }

    QJsonDocument doc = QJsonDocument::fromVariant (lsRepos);

    return QString(doc.toJson ());
}

bool PrivateData::parseReposJsonInfo(const QString &strUserName, const QString &strJson)
{
    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(strJson.toLatin1 (), &error);
    if (jsonDocument.isNull () || error.error != QJsonParseError::NoError)
    {
        qWarning()<<"jsonDocument is invalid! |"<<error.errorString ();
        return false;
    }

    QVariantList lsData = jsonDocument.toVariant().toList ();
    for ( int i = 0; i < lsData.size (); i++)
    {
        QVariantMap map = lsData[i].toMap ();

        if (map.isEmpty ())
        {
            continue;
        }

        QString strReposName = map.value ("full_name").toString ();
        strReposName = strReposName.remove ( QString("%1/").arg (strUserName));
        if (isReposExists (strReposName))
        {
            continue;
        }

        DS_ReposInfoBase *pBase = new DS_ReposInfoBase;
        pBase->m_strReposName = strReposName;

        m_mapReposInfo[strUserName].push_back (pBase);
    }

    return true;
}

PrivateData::DS_ReposInfoBase::DS_ReposInfoBase()
{
    m_strReposName = "";
    m_vtrpCommintInfo.clear ();
}

PrivateData::DS_ReposInfoBase::~DS_ReposInfoBase()
{

   qDeleteAll(m_vtrpCommintInfo);
   m_vtrpCommintInfo.clear ();
}

QVariantList PrivateData::DS_ReposInfoBase::convertToList()
{
    QVariantList lsCommitInfo;
    for ( int i = 0; i < m_vtrpCommintInfo.size (); i++)
    {
        auto pItem = m_vtrpCommintInfo[i];
        QVariantMap subMap;
        subMap.insert ("strCommitData", pItem->m_strCommitDate);
        subMap.insert ("strCommitName", pItem->m_strCommitName);

        lsCommitInfo.push_back (subMap);
    }

    return lsCommitInfo;
}

///////////////////////////////////////////////////////////////////////////////////

QManager::QManager(QObject *parent)
    : QObject(parent),
      d(new PrivateData)
{
}

QManager *QManager::instance()
{
    static QManager _instance;
    return &_instance;
}

QManager::~QManager()
{
    SAFE_DELETE(d);
}

bool QManager::initManager()
{
    qmlRegisterType<QInterface>("com.kevinlq.demo", 1, 0, "QInterface");

    return true;
}

int QManager::start(int argc, char *argv[])
{
    QApplication app(argc,argv);

    if (!initManager())
    {
        return -1;
    }

    QString strResPath = QString("%1/Main.qml").arg(QApplication::applicationDirPath ()+"/../../../QML");

    if (d->m_pEngine == Q_NULLPTR)
    {
        d->m_pEngine = new QQmlApplicationEngine;
    }

    d->m_pEngine->load (strResPath);
    if (d->m_pEngine->rootObjects ().isEmpty ())
    {
        qWarning()<<"engine.rootObjects().isEmpty:"<<strResPath;
        return -1;
    }

    return app.exec ();
}

QString QManager::initReposInfo(const QString &strUserName)
{
    if (strUserName.isEmpty ())
    {
        return "";
    }

    if (d->isUserExists (strUserName))
    {
        return d->getUserReposInfo (strUserName);
    }

    /// init all repos info
    QString strRepos = "";
    GitHubApiUtil::instance ()->setUserName (strUserName);
    bool bResult = GitHubApiUtil::instance ()->getGitHubAllReposInfo (strRepos);
    if (!bResult || strRepos.isEmpty ())
    {
        qWarning()<<"get result is invalid";
        return "";
    }

    if ( !d->parseReposJsonInfo (strUserName, strRepos))
    {
        qWarning()<<"parse repos error!"<<endl;
        return "";
    }

    return d->getUserReposInfo (strUserName);
}
