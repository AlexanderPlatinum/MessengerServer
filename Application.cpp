#include "Application.h"

Application::Application( QObject *parent ) : QObject(parent)
{
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->server = new QTcpServer( this );
}

void Application::Run()
{
    this->SetUpDB();
    this->SetUpTCPServer();
}

void Application::SetUpDB()
{
    this->db.setDatabaseName( "database.sqlite" );

    if ( !db.open() )
    {
        qDebug() << "Database error!";
    }
}

void Application::SetUpTCPServer()
{
    connect( this->server, SIGNAL(newConnection()), this, SLOT(NewConnectionHandler()) );

    if ( this->server->listen( QHostAddress::Any, 7777 ) )
    {
        qDebug() << "Server strat on 7777 port";
    }
    else
    {
        qDebug() << "Server not started!!";
    }
}

// Slots

void Application::NewConnectionHandler()
{
    qDebug() << "New connection";

    this->socket = this->server->nextPendingConnection();

    connect( this->socket, SIGNAL(readyRead()), this, SLOT(ReadClientDataHandler()) );
    connect( this->socket, SIGNAL(disconnected()), this, SLOT(DisconnectHandler()) );
}

void Application::ReadClientDataHandler()
{
    while ( this->socket->bytesAvailable() > 0 )
    {
        QByteArray buffer = this->socket->readAll();
        qDebug() << buffer;
        this->ExecActions( buffer );
    }
}

void Application::DisconnectHandler()
{
    this->socket->close();
}

// Parsers

Actions Application::ParseAction( QString command )
{
    command = command.toUpper();

    if ( command == "LOGIN_USER" )
        return Actions::LoginUser;

    if ( command == "REGISTER_USER" )
        return Actions::RegisterUser;

    if ( command == "GET_CONVERSATIONS" )
        return Actions::GetConversations;

    if ( command == "GET_MESSAGES" )
        return Actions::GetMessages;

    if ( command == "CREATE_CONVERSATION" )
        return Actions::CreateConversation;

    if ( command == "SEND_MESSAGE" )
        return Actions::SendMessage;

    return Actions::NotFound;
}

std::pair<QString, QJsonObject> Application::ParseCommand( QString command )
{
    QJsonDocument document = QJsonDocument::fromJson( command.toUtf8() );
    QJsonObject object = document.object();

    return std::make_pair( object["command"].toString(), object["params"].toObject() );
}

void Application::ExecActions( QByteArray data )
{
    std::pair<QString, QJsonObject> tmp = this->ParseCommand( QString( data ) );

    switch ( this->ParseAction( tmp.first ) )
    {
        case Actions::LoginUser:
            this->LoginUserAction( tmp.second );
            break;

        case Actions::RegisterUser:
            this->RegisterUserAction( tmp.second );
            break;

        case Actions::GetConversations:
            this->GetConversationsAction( tmp.second );
            break;

        case Actions::GetMessages:
            this->GetMessagesAction( tmp.second );
            break;

        case Actions::CreateConversation:
            this->CreateConversationAction( tmp.second );
            break;

        case Actions::SendMessage:
            this->SendMessageAction( tmp.second );
            break;

        default:
            this->socket->write( ACTION_NOT_FOUND );
            break;
    }
}

// Actions

void Application::LoginUserAction( QJsonObject params )
{
    auto seqId = this->CheckSeqId( params );

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

    QString user_id = this->GetUserByEmailAndPass( *user );

    if ( user_id.size() == 0 )
    {
        return;
    }

    QString token = this->InsertSession( user_id );

    QJsonObject *obj = new QJsonObject();

    obj->insert( "seqId", seqId.second );
    obj->insert( "msg", "OK" );
    obj->insert( "token", token );

    QJsonDocument *doc = new QJsonDocument( *obj );
    this->socket->write( doc->toJson() );
}

void Application::RegisterUserAction( QJsonObject params )
{
    auto seqId = this->CheckSeqId( params );
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
        this->socket->write( EMAIL_NOT_VALID );
        return;
    }

    if ( user->password.size() < 6 )
    {
        this->socket->write( PASSWORD_NOT_VALID );
        return;
    }

    this->InsertUser( *user );
    this->SendOk( seqId.second );
}

void Application::GetConversationsAction( QJsonObject params )
{
    QString user_id = this->GetUserIdByToken( params["token"].toString() );

    if ( user_id.size() == 0 )
    {
        this->socket->write( USER_NOT_AUTHORIZE );
        return;
    }

    auto seqId = this->CheckSeqId( params );

    if ( seqId.first )
    {
        return;
    }

    QJsonArray conversations = this->GetConversations( user_id );

    QJsonObject *obj = new QJsonObject();

    obj->insert( "seqId", seqId.second );
    obj->insert( "msg", "OK" );
    obj->insert( "response", QJsonValue( conversations ) );

    QJsonDocument *doc = new QJsonDocument( *obj );
    this->socket->write( doc->toJson() );
}

void Application::GetMessagesAction( QJsonObject params )
{
    QString user_id = this->GetUserIdByToken( params["token"].toString() );

    if ( user_id.size() == 0 )
    {
        this->socket->write( USER_NOT_AUTHORIZE );
        return;
    }

    auto seqId = this->CheckSeqId( params );

    if ( seqId.first )
    {
        return;
    }

    QString conversation_id = params["conversation_id"].toString();

    if ( conversation_id.size() == 0 )
    {
        this->socket->write( CONVERSATION_ID_IS_NULL );
        return;
    }

    QJsonArray messages = this->GetMessages( conversation_id );

    QJsonObject *obj = new QJsonObject();

    obj->insert( "seqId", seqId.second );
    obj->insert( "msg", "OK" );
    obj->insert( "response", QJsonValue( messages ) );

    QJsonDocument *doc = new QJsonDocument( *obj );
    this->socket->write( doc->toJson() );
}

void Application::CreateConversationAction( QJsonObject params )
{
    QString user_id = this->GetUserIdByToken( params["token"].toString() );

    if ( user_id.size() == 0 )
    {
        this->socket->write( USER_NOT_AUTHORIZE );
        return;
    }

    auto seqId = this->CheckSeqId( params );

    if ( seqId.first )
    {
        return;
    }

    QString user_two = params["friend_id"].toString();

    if ( user_two.size() == 0 )
    {
        this->socket->write( FRIEND_ID_IS_NULL );
        return;
    }

    this->InsertConversation( user_id, user_two );
    this->SendOk( seqId.second );
}

void Application::SendMessageAction( QJsonObject params )
{
    QString user_id = this->GetUserIdByToken( params["token"].toString() );

    if ( user_id.size() == 0 )
    {
        this->socket->write( USER_NOT_AUTHORIZE );
        return;
    }

    auto seqId = this->CheckSeqId( params );

    if ( seqId.first )
    {
        return;
    }

    QString conversation_id = params["conversation_id"].toString();
    if ( conversation_id.size() == 0 )
    {
        this->socket->write( CONVERSATION_ID_IS_NULL );
        return;
    }

    QString message = params["message"].toString();

    if ( message.size() == 0)
    {
        this->socket->write( MESSAGE_IS_NULL );
    }

    this->InsertMessage(conversation_id, user_id, message );
    this->SendOk( seqId.second );
}

// Execute sql

void Application::InsertUser( User user )
{
    QSqlQuery query;

    query.prepare( INSERT_USER_SQL );

    query.bindValue( ":first_name", user.first_name );
    query.bindValue( ":last_name", user.last_name );
    query.bindValue( ":email", user.email );
    query.bindValue( ":password", user.password );

    query.exec();
}

QString Application::GetUserByEmailAndPass( User user )
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

QString Application::InsertSession( QString user_id )
{

    QString token =  this->randString( 45 );
    QSqlQuery query;

    query.prepare( INSERT_SESSION_SQL );

    query.bindValue( ":token", token );
    query.bindValue( ":user_id", user_id );

    query.exec();

    return token;
}

QString Application::GetUserIdByToken( QString token )
{
    QSqlQuery query;

    query.prepare( GET_USER_BY_TOKEN_SQL );
    query.bindValue( ":token", token );

    query.exec();

    while ( query.next() )
    {
        return query.value( "user_id" ).toString();
    }

    return "";
}

void Application::InsertConversation( QString user_one, QString user_two )
{
    QSqlQuery query;

    query.prepare( CREATE_CONVERSATION_SQL );

    query.bindValue( ":user_one", user_one );
    query.bindValue( ":user_two", user_two );

    query.exec();
}

void Application::InsertMessage( QString conversation_id, QString author_id, QString message )
{
    QSqlQuery query;

    query.prepare( CREATE_MESSAGE_SQL );

    query.bindValue( ":conversation_id", conversation_id );
    query.bindValue( ":author_id", author_id );
    query.bindValue( ":msg_text", message );

    query.exec();
}

QJsonArray Application::GetConversations( QString user_id )
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

QJsonArray Application::GetMessages( QString conversation_id )
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

// System

std::pair<bool, QString> Application::CheckSeqId ( QJsonObject params )
{
    QString seqId = params["seqId"].toString();

    if ( seqId.size() == 0 )
    {
        this->socket->write( SEQ_ID_IS_NULL );
        return std::make_pair( true, "NULL" );
    }

    return std::make_pair( false, seqId );
}

QString Application::randString(int len)
{
    QString str;

    str.resize(len);

    for (int s = 0; s < len ; ++s)
        str[s] = QChar('A' + char(qrand() % ('Z' - 'A')));

    return str;
}

void Application::SendOk( QString seqId )
{
    this->socket->write( QString("{ \"seqId\" : \"" + seqId + "\", \"msg\": \"OK\" }").toUtf8() );
}
