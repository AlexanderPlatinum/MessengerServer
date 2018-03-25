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

    // Common
    Actions ParseAction( QString command );
    std::pair<QString, QJsonObject> ParseCommand ( QString command );
    void ExecActions( QByteArray data );

    // Actions
    void LoginUserAction ( QJsonObject params );
    void RegisterUserAction( QJsonObject params );
    void GetConversationsAction( QJsonObject params );
    void GetMessagesAction( QJsonObject params );
    void CreateConversationAction( QJsonObject params );
    void SendMessageAction( QJsonObject params );

    // Execute sql
    void InsertUser( User user );
    void InsertConversation ( QString user_one, QString user_two );
    void InsertMessage( QString conversation_id, QString author_id, QString message );

    QString GetUserByEmailAndPass ( User user );
    QString InsertSession( QString user_id );
    QString GetUserIdByToken( QString token );
    QJsonArray GetConversations( QString user_id );
    QJsonArray GetMessages( QString conversation_id );

    // Utilities
    std::pair<bool, QString> CheckSeqId ( QJsonObject params );
    void SendOk ( QString seqId );
    QString randString(int len);

private slots:

    void NewConnectionHandler();
    void ReadClientDataHandler();
    void DisconnectHandler();

};
