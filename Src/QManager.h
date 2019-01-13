#ifndef QMANAGER_H
#define QMANAGER_H

#include <QObject>

class PrivateData;

class QManager : public QObject
{
    Q_OBJECT

private:
    explicit QManager(QObject *parent = nullptr);
public:
    static QManager * instance();

    ~QManager();

    bool initManager();

    int start(int argc, char *argv[]);

    QString initReposInfo(const QString &strUserName);
private:
    PrivateData *d;
};

#endif // QMANAGER_H
