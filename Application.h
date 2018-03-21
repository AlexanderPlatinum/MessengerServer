#pragma once

#include <QSql>
#include <QSqlDatabase>
#include <QTcpServer>
#include <QTcpSocket>
#include <iostream>

#include "SQLCommands.h"

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

private slots:

    void NewConnectionHandler();
    void ReadClientDataHandler();
    void DisconnectHandler();

};
