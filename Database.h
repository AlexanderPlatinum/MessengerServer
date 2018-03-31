#pragma once

//#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QVariant>

#include "SQLCommands.h"
#include "Utilities.h"

class Database
{

public:

    static void InsertUser( User user );
    static void InsertConversation ( QString user_one, QString user_two );
    static void InsertMessage( QString conversation_id, QString author_id, QString message );

    static QString GetUserByEmailAndPass ( User user );
    static User GetUserById( QString user_id );
    static QString InsertSession( QString user_id );
    static QString GetUserIdByToken( QString token );
    static QJsonArray GetConversations( QString user_id );
    static QJsonArray GetMessages( QString conversation_id );
    static QJsonArray GetAllUsers();

};
