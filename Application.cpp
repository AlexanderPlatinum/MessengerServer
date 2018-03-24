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

}

void Application::GetMessagesAction( QJsonObject params )
{

}

void Application::CreateConversationAction( QJsonObject params )
{

}

void Application::SendMessageAction( QJsonObject params )
{

}

void Application::SendOk( QString seqId )
{
    this->socket->write( QString("{ \"seqId\" : \"" + seqId + "\", \"msg\": \"OK\" }").toUtf8() );
}

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

QString Application::randString(int len)
{
    QString str;

    str.resize(len);

    for (int s = 0; s < len ; ++s)
        str[s] = QChar('A' + char(qrand() % ('Z' - 'A')));

    return str;
}
