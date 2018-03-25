#pragma once

#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>
#include <utility>

#include "Actions.h"

#include "SQLCommands.h"
#include "ErrorMessages.h"


class Application : public QObject
{
    Q_OBJECT;

public:

    explicit Application( QObject *parent = 0 );

    void Run ();

private:

    QSqlDatabase db;
    QTcpServer *server;
    QTcpSocket *socket;

    void SetUpDB ();
    void SetUpTCPServer();

    // Common
    ActionsEnum ParseAction( QString command );
    std::pair<QString, QJsonObject> ParseCommand ( QString command );
    void ExecActions( QByteArray data );

private slots:

    void NewConnectionHandler();
    void ReadClientDataHandler();
    void DisconnectHandler();

};
