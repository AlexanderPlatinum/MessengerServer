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
        this->ExecActions( buffer );
    }
}

void Application::DisconnectHandler()
{
    this->socket->close();
}

// Parsers

ActionsEnum Application::ParseAction( QString command )
{
    command = command.toUpper();

    if ( command == "LOGIN_USER" )
        return ActionsEnum::LoginUser;

    if ( command == "REGISTER_USER" )
        return ActionsEnum::RegisterUser;

    if ( command == "GET_CONVERSATIONS" )
        return ActionsEnum::GetConversations;

    if ( command == "GET_MESSAGES" )
        return ActionsEnum::GetMessages;

    if ( command == "CREATE_CONVERSATION" )
        return ActionsEnum::CreateConversation;

    if ( command == "SEND_MESSAGE" )
        return ActionsEnum::SendMessage;

    if ( command == "GET_USERS" )
        return ActionsEnum::GetUsers;

    return ActionsEnum::NotFound;
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
        case ActionsEnum::LoginUser:
            Actions::LoginUserAction( this->socket, tmp.second );
            break;

        case ActionsEnum::RegisterUser:
            Actions::RegisterUserAction( this->socket, tmp.second );
            break;

        case ActionsEnum::GetConversations:
            Actions::GetConversationsAction( this->socket, tmp.second );
            break;

        case ActionsEnum::GetMessages:
            Actions::GetMessagesAction( this->socket, tmp.second );
            break;

        case ActionsEnum::CreateConversation:
            Actions::CreateConversationAction( this->socket, tmp.second );
            break;

        case ActionsEnum::SendMessage:
            Actions::SendMessageAction( this->socket, tmp.second );
            break;

        case ActionsEnum::GetUsers:
            Actions::GetUsersAction( this->socket, tmp.second );
            break;

        default:
            this->socket->write( ACTION_NOT_FOUND );
            break;
    }
}
