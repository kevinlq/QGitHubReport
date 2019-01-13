include ($$PWD/QGitHubReport.pri)

QT  +=core gui widgets qml quick network

CONFIG += c++11

TARGET = QGitHubReport
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += \
    main.cpp \
    GitHubApiUtil.cpp \
    QNetWorker.cpp \
    QManager.cpp \
    QInterface.cpp

HEADERS += \
    GitHubApiUtil.h \
    QNetWorker.h \
    QManager.h \
    QInterface.h
