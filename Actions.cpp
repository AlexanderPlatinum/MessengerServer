#include "Actions.h"

void Actions::LoginUserAction( QTcpSocket *socket, QJsonObject params )
{
    auto seqId = Utilities::CheckSeqId( socket, params );

    if ( seqId.first )
    {
        return;
    }

    User *user = new User();

    user->email      = params["email"].toString();
    user->password   = params["password"].toString();

    if ( user->email.size() == 0 || user->password.size() == 0 )
    {
        return;
    }

    QString user_id = Database::GetUserByEmailAndPass( *user );

    if ( user_id.size() == 0 )
    {
        Utilities::SendOk( socket, seqId.second );
        return;
    }

    User tmp = Database::GetUserById( user_id );

    QString token = Database::InsertSession( user_id );

    QJsonObject *obj = new QJsonObject();

    obj->insert( "seqId", seqId.second );
    obj->insert( "msg", "OK" );
    obj->insert( "token", token );
    obj->insert( "user_id", QString::number(tmp.id) );
    obj->insert( "first_name", tmp.first_name );
    obj->insert( "last_name", tmp.last_name );

    QJsonDocument *doc = new QJsonDocument( *obj );
    socket->write( doc->toJson() );
}

void Actions::RegisterUserAction( QTcpSocket *socket, QJsonObject params )
{
    auto seqId = Utilities::CheckSeqId( socket, params );
    if ( seqId.first )
    {
        return;
    }

    User *user = new User();

    user->first_name = params["first_name"].toString();
    user->last_name  = params["last_name"].toString();
    user->email      = params["email"].toString();
    user->password   = params["password"].toString();

    if ( user->email.size() < 5 )
    {
        socket->write( EMAIL_NOT_VALID );
        return;
    }

    if ( user->password.size() < 6 )
    {
        socket->write( PASSWORD_NOT_VALID );
        return;
    }

    Database::InsertUser( *user );
    Utilities::SendOk( socket, seqId.second );
}

void Actions::GetConversationsAction( QTcpSocket *socket, QJsonObject params )
{
    QString user_id = Database::GetUserIdByToken( params["token"].toString() );

    if ( user_id.size() == 0 )
    {
        socket->write( USER_NOT_AUTHORIZE );
        return;
    }

    auto seqId = Utilities::CheckSeqId( socket, params );

    if ( seqId.first )
    {
        return;
    }

    QJsonArray conversations = Database::GetConversations( user_id );

    QJsonObject *obj = new QJsonObject();

    obj->insert( "seqId", seqId.second );
    obj->insert( "msg", "OK" );
    obj->insert( "response", QJsonValue( conversations ) );

    QJsonDocument *doc = new QJsonDocument( *obj );
    socket->write( doc->toJson() );
}

void Actions::GetMessagesAction( QTcpSocket *socket, QJsonObject params )
{
    QString user_id = Database::GetUserIdByToken( params["token"].toString() );

    if ( user_id.size() == 0 )
    {
        socket->write( USER_NOT_AUTHORIZE );
        return;
    }

    auto seqId = Utilities::CheckSeqId( socket, params );

    if ( seqId.first )
    {
        return;
    }

    QString conversation_id = params["conversation_id"].toString();

    if ( conversation_id.size() == 0 )
    {
        socket->write( CONVERSATION_ID_IS_NULL );
        return;
    }

    QJsonArray messages = Database::GetMessages( conversation_id );

    QJsonObject *obj = new QJsonObject();

    obj->insert( "seqId", seqId.second );
    obj->insert( "msg", "OK" );
    obj->insert( "response", QJsonValue( messages ) );

    QJsonDocument *doc = new QJsonDocument( *obj );
    socket->write( doc->toJson() );
}

void Actions::CreateConversationAction( QTcpSocket *socket, QJsonObject params )
{
    QString user_id = Database::GetUserIdByToken( params["token"].toString() );

    if ( user_id.size() == 0 )
    {
        socket->write( USER_NOT_AUTHORIZE );
        return;
    }

    auto seqId = Utilities::CheckSeqId( socket, params );

    if ( seqId.first )
    {
        return;
    }

    QString user_two = params["friend_id"].toString();

    if ( user_two.size() == 0 )
    {
        socket->write( FRIEND_ID_IS_NULL );
        return;
    }

    Database::InsertConversation( user_id, user_two );
    Utilities::SendOk( socket, seqId.second );
}

void Actions::SendMessageAction( QTcpSocket *socket, QJsonObject params )
{

    QString user_id = Database::GetUserIdByToken( params["token"].toString() );
    if ( user_id.size() == 0 )
    {
        socket->write( USER_NOT_AUTHORIZE );
        return;
    }

    auto seqId = Utilities::CheckSeqId( socket, params );
    if ( seqId.first )
    {
        return;
    }

    QString conversation_id = params["conversation_id"].toString();
    if ( conversation_id.size() == 0 )
    {
        socket->write( CONVERSATION_ID_IS_NULL );
        return;
    }

    QString message = params["message"].toString();

    if ( message.size() == 0)
    {
        socket->write( MESSAGE_IS_NULL );
    }

    Database::InsertMessage(conversation_id, user_id, message );
    Utilities::SendOk( socket, seqId.second );
}

void Actions::GetUsersAction( QTcpSocket *socket, QJsonObject params )
{

    QString user_id = Database::GetUserIdByToken( params["token"].toString() );
    if ( user_id.size() == 0 )
    {
        socket->write( USER_NOT_AUTHORIZE );
        return;
    }

    auto seqId = Utilities::CheckSeqId( socket, params );
    if( seqId.first )
    {
        return;
    }

    QJsonArray users = Database::GetAllUsers();

    QJsonObject *obj = new QJsonObject();

    obj->insert( "seqId", seqId.second );
    obj->insert( "msg", "OK" );
    obj->insert( "response", QJsonValue( users ) );

    QJsonDocument *doc = new QJsonDocument( *obj );
    socket->write( doc->toJson() );
}
