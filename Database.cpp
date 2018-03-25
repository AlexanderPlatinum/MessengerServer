#include "Database.h"

void Database::InsertUser( User user )
{
    QSqlQuery query;

    query.prepare( INSERT_USER_SQL );

    query.bindValue( ":first_name", user.first_name );
    query.bindValue( ":last_name", user.last_name );
    query.bindValue( ":email", user.email );
    query.bindValue( ":password", user.password );

    query.exec();
}

QString Database::GetUserByEmailAndPass( User user )
{
    QSqlQuery query;

    query.prepare( GET_USER_ID_BY_EMAIL_PASS );

    query.bindValue( ":email", user.email );
    query.bindValue( ":password", user.password );

    query.exec();

    while ( query.next() )
    {
        return query.value("id").toString();
    }

    return "";
}

QString Database::InsertSession( QString user_id )
{

    QString token = Utilities::randString( 45 );
    QSqlQuery query;

    query.prepare( INSERT_SESSION_SQL );

    query.bindValue( ":token", token );
    query.bindValue( ":user_id", user_id );

    query.exec();

    return token;
}

QString Database::GetUserIdByToken( QString token )
{
    QSqlQuery query;

    query.prepare( GET_USER_BY_TOKEN_SQL );
    query.bindValue( ":token",token );

    query.exec();

    while ( query.next() )
    {
        return query.value( "user_id" ).toString();
    }

    return "";
}

void Database::InsertConversation( QString user_one, QString user_two )
{
    QSqlQuery query;

    query.prepare( CREATE_CONVERSATION_SQL );

    query.bindValue( ":user_one", user_one );
    query.bindValue( ":user_two", user_two );

    query.exec();
}

void Database::InsertMessage( QString conversation_id, QString author_id, QString message )
{
    QSqlQuery query;

    query.prepare( CREATE_MESSAGE_SQL );

    query.bindValue( ":conversation_id", conversation_id );
    query.bindValue( ":author_id", author_id );
    query.bindValue( ":msg_text", message );

    query.exec();
}

QJsonArray Database::GetConversations( QString user_id )
{
    QSqlQuery query;

    query.prepare( GET_CONVERSATIONS_SQL );
    query.bindValue( ":user_id", user_id );

    query.exec();

    QJsonArray array;

    while ( query.next() )
    {
        QJsonObject obj;

        obj.insert( "conversation_id",  query.value( "id" ).toString() );
        obj.insert( "u_one_first_name", query.value( "u_one_first_name" ).toString() );
        obj.insert( "u_one_last_name",  query.value( "u_one_last_name" ).toString() );
        obj.insert( "u_two_first_name", query.value( "u_two_first_name" ).toString() );
        obj.insert( "u_two_last_name",  query.value( "u_two_last_name" ).toString() );

        array.push_back( QJsonValue( obj ) );
    }

    return array;
}

QJsonArray Database::GetMessages( QString conversation_id )
{
    QSqlQuery query;

    query.prepare( GET_MESSAGES_SQL );
    query.bindValue( ":conversation_id", conversation_id );

    query.exec();

    QJsonArray array;

    while ( query.next() )
    {
        QJsonObject obj;

        obj.insert( "msg_id", query.value( "id" ).toString() );
        obj.insert( "author_id", query.value( "author_id" ).toString() );
        obj.insert( "msg_text", query.value( "msg_text" ).toString() );

        array.push_back( QJsonValue( obj ) );
    }

    return array;
}

QJsonArray Database::GetAllUsers()
{
    QSqlQuery query;

    query.prepare( GET_USERS_SQL );

    query.exec();

    QJsonArray array;

    while ( query.next() )
    {
        QJsonObject obj;

        obj.insert( "id", query.value( "id" ).toString() );
        obj.insert( "first_name", query.value( "first_name" ).toString() );
        obj.insert( "last_name", query.value( "last_name" ).toString() );

        array.push_back( QJsonValue( obj ) );
    }

    return array;
}
