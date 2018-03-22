#pragma once

#include <QSql>
#include <QSqlDatabase>
#include <QTcpServer>
#include <QTcpSocket>
#include <iostream>

#include "SQLCommands.h"
#include "ErrorMessages.h"

enum Actions {

    LoginUser,
    RegisterUser,

    GetConversations,
    GetMessages,

    CreateConversation,
    SendMessage

};

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

    Actions ParseAction( QString command );
    void ExecActions( QByteArray data );

    void LoginUserAction ();
    void RegisterUserAction();
    void GetConversationsAction();
    void GetMessagesAction();
    void CreateConversationAction();
    void SendMessageAction();

private slots:

    void NewConnectionHandler();
    void ReadClientDataHandler();
    void DisconnectHandler();

};
