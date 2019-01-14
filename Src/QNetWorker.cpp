#include "QNetWorker.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QUrl>

#include <QEventLoop>
#include <QTimer>

class QNetWorker::PrivateData
{
public:
    PrivateData(QNetWorker *q);
    ~PrivateData();

public:
    QNetworkAccessManager *m_pManager;
};

QNetWorker::PrivateData::PrivateData(QNetWorker *q)
    :m_pManager(new QNetworkAccessManager(q))
{
}

QNetWorker::PrivateData::~PrivateData()
{
    if (m_pManager)
    {
        m_pManager->deleteLater ();

        m_pManager = Q_NULLPTR;
    }
}

QNetWorker::QNetWorker(QObject *parent)
    : QObject(parent),
      d(new QNetWorker::PrivateData(this))
{
}

QNetWorker::~QNetWorker()
{
    delete d;
    d = Q_NULLPTR;
}

QByteArray QNetWorker::get(const QString &strUrl, const int nMsec)
{
    QTimer timer;
    timer.setSingleShot (true);

    QEventLoop eventLoop;

    QByteArray arrayResult = "";
    QNetworkReply *pReply = d->m_pManager->get (QNetworkRequest(QUrl(strUrl)));

    connect(&timer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);
    connect (d->m_pManager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);

    connect(d->m_pManager, &QNetworkAccessManager::finished,
            [&arrayResult, &pReply]()
    {
        arrayResult=  pReply->readAll ();
    });

    timer.start (nMsec); /// <5 secs . timeout
    eventLoop.exec ();

    if(!timer.isActive ())
    {
        timer.stop ();
    }

    pReply->deleteLater (); /// < delete pReply

    return arrayResult;
}

QNetWorker *QNetWorker::instance()
{
    static QNetWorker instance;
    return &instance;
}

