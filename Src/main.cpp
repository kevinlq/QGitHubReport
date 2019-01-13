#include <QCoreApplication>

#include <QDebug>
#include <QJsonDocument>

#include "GitHubApiUtil.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString strResult = "";
    bool bResult = false;

    GitHubApiUtil util;
    util.setUserName ("kevinlq");

    bResult = util.getGitHubInfos (GitHubApiUtil::GitHub_Type_UsersInfo, strResult);

    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(strResult.toLatin1 (), &error);
    if (error.error == QJsonParseError::NoError)
    {
        qWarning()<<"--->>>>:"<<bResult<<strResult;
        QVariantMap data = jsonDocument.toVariant().toMap();
    }

    return a.exec();
}
