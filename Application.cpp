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
        qDebug() << "Database error!";;
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

std::pair<QString, QString> Application::PaseCommand( QString command )
{
    QJsonDocument document = QJsonDocument::fromJson( command.toUtf8() );
    QJsonObject object = document.object();

    return std::make_pair( object["command"].toString(), object["params"].toString() );
}

void Application::ExecActions( QByteArray data )
{
    std::pair<QString, QString> tmp = this->PaseCommand( QString( data ) );

    switch ( this->ParseAction( tmp.first ) )
    {
        case Actions::LoginUser:
            this->LoginUserAction();
            break;

        case Actions::RegisterUser:
            this->RegisterUserAction();
            break;

        case Actions::GetConversations:
            this->GetConversationsAction();
            break;

        case Actions::GetMessages:
            this->GetMessagesAction();
            break;

        case Actions::CreateConversation:
            this->CreateConversationAction();
            break;

        case Actions::SendMessage:
            this->SendMessageAction();
            break;

        default:
            this->socket->write( ACTION_NOT_FOUND );
            break;
    }
}

void Application::LoginUserAction()
{

}

void Application::RegisterUserAction()
{

}

void Application::GetConversationsAction()
{

}

void Application::GetMessagesAction()
{

}

void Application::CreateConversationAction()
{

}

void Application::SendMessageAction()
{

}
