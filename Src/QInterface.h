#ifndef QINTERFACE_H
#define QINTERFACE_H

#include <QObject>

#ifdef OS_WIN
#ifdef RHDINTERFACE_LIBRARY
#define QGITHUB_EXPORT    __declspec(dllexport)
#else
#define QGITHUB_EXPORT    __declspec(dllimport)
#endif
#else
#define QGITHUB_EXPORT    __attribute__((visibility("default")))
#endif

class QGITHUB_EXPORT QInterface : public QObject
{
    Q_OBJECT
public:
    explicit QInterface(QObject *parent = nullptr);
    ~QInterface();

public Q_SLOTS:
    QString initReposInfo(const QString &strUserName);
};

#endif // QINTERFACE_H
