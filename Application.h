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

#include "SQLCommands.h"
#include "ErrorMessages.h"

enum Actions {

    LoginUser,
    RegisterUser,

    GetConversations,
    GetMessages,

    CreateConversation,
    SendMessage,

    NotFound

};

struct User {
    int id;
    QString first_name;
    QString last_name;
    QString email;
    QString password;
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

    // Actions
    void LoginUserAction ( QJsonObject params );
    void RegisterUserAction( QJsonObject params );
    void GetConversationsAction( QJsonObject params );
    void GetMessagesAction( QJsonObject params );
    void CreateConversationAction( QJsonObject params );
    void SendMessageAction( QJsonObject params );

    std::pair<QString, QJsonObject> ParseCommand ( QString command );

    // Execute sql
    void InsertUser( User user );

    std::pair<bool, QString> CheckSeqId ( QJsonObject params );

    void SendOk ( QString seqId );

private slots:

    void NewConnectionHandler();
    void ReadClientDataHandler();
    void DisconnectHandler();

};
