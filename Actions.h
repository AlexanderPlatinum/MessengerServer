#pragma once

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>

#include "SQLCommands.h"
#include "ErrorMessages.h"
#include "Database.h"

enum ActionsEnum {

    LoginUser,
    RegisterUser,
    GetConversations,
    GetMessages,
    CreateConversation,
    SendMessage,
    GetUsers,
    NotFound

};

class Actions
{

public:

    static void LoginUserAction ( QTcpSocket *socket, QJsonObject params );
    static void RegisterUserAction( QTcpSocket *socket, QJsonObject params );
    static void GetConversationsAction( QTcpSocket *socket, QJsonObject params );
    static void GetMessagesAction( QTcpSocket *socket, QJsonObject params );
    static void CreateConversationAction( QTcpSocket *socket, QJsonObject params );
    static void SendMessageAction( QTcpSocket *socket, QJsonObject params );
    static void GetUsersAction( QTcpSocket *socket, QJsonObject params );

};
