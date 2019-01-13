#include "QManager.h"

int main(int argc, char *argv[])
{
    return QManager::instance ()->start (argc, argv);

#if 0
    QString strResult = "";
    bool bResult = false;

    GitHubApiUtil util;
    util.setUserName ("kevinlq");

//    bResult = util.getGitHubInfos (GitHubApiUtil::GitHub_Type_UsersInfo, strResult);

//    QJsonParseError error;
//    QJsonDocument jsonDocument = QJsonDocument::fromJson(strResult.toLatin1 (), &error);
//    if (error.error == QJsonParseError::NoError)
//    {
//        qWarning()<<"--->>>>:"<<bResult<<strResult<<endl;
//        QVariantMap data = jsonDocument.toVariant().toMap();
//    }


    QDateTime timeStamp = QDateTime::fromString ("2019-01-13T09:08:17Z");

    QDateTime date = QDateTime::fromString("2019-01-13T09:08:17Z", "yyyy-MM-ddTHH:mm:ssZ");
    date.setTimeSpec(Qt::UTC);
    QDateTime local = date.toLocalTime();

    qDebug() << "utc: " << date;
    qDebug() << "local: " << local.toString();
    qDebug() << "hax: " << local.toString(Qt::SystemLocaleLongDate);
#endif
}
