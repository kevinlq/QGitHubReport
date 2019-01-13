/* This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * Copyright: kevinlq
 * Author: kevinlq
 */
#ifndef GITHUBAPIUTIL_H
#define GITHUBAPIUTIL_H

#include <QObject>

/*! get GitHub info
 * This class defines get GitHub info
 */
class GitHubApiUtil : public QObject
{
    Q_OBJECT
public:
    /**
     * GitHub info type set.
     */
    enum GITHUB_INFO_TYPE
    {
        GitHub_Type_UsersInfo,         /// < user info
        GitHub_Type_AllReposInfos,     /// < all response info
        GitHub_Type_Size
    };

    /// Create an GitHubApiUtil with the default intial values
    explicit GitHubApiUtil(QObject *parent = nullptr);

    ///  Destruct GitHubApiUtil
    ~GitHubApiUtil();

    /**
      * \brief set your GitHub user name.
      * This method must be implemented by first init
      * \param strUserName Value to be set.
    */
    void setUserName(const QString &strUserName);

    /**
      * \brief get your GitHub infos.
      * \param strJsonInfo. your GitHub infos, json format data
      * \return returns true if the request is ok; otherwise returns false.
    */
    bool getGitHubInfos(GITHUB_INFO_TYPE eType, QString &strJsonInfo);

private:
    class PrivateData;
    friend class PrivateData;
    PrivateData *d;         /// < d point
};

#endif // GITHUBAPIUTIL_H
