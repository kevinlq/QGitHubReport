#ifndef NETWORKER_H
#define NETWORKER_H

#include <QObject>

class QNetworkReply;

class QNetWorker : public QObject
{
    Q_OBJECT
public:
    static QNetWorker*instance();

    ~QNetWorker();

    QByteArray get( const QString &strUrl);

Q_SIGNALS:
//    void finished(QNetworkReply *reply);
    void finished(QByteArray replyArray);

private:
    explicit QNetWorker(QObject *parent = nullptr);
    QNetWorker(const QNetWorker &) Q_DECL_EQ_DELETE;
    QNetWorker & operator=(QNetWorker others) Q_DECL_EQ_DELETE;

private:
    class PrivateData;
    friend class PrivateData;
    PrivateData *d;
};

#endif // NETWORKER_H
