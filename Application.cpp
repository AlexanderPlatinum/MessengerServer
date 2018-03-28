#include "Application.h"

Application::Application( QObject *parent ) :
     QTcpServer( parent )
{
    this->db = QSqlDatabase::addDatabase("QSQLITE");
}

void Application::run()
{

    this->SetDB();

    if ( this->listen( QHostAddress::Any, 7777 ) )
    {
        qDebug() << "Server strat on 7777 port";
    }
    else
    {
        qDebug() << "Server not started!!";
    }
}

void Application::SetDB()
{
    this->db.setDatabaseName( "database.sqlite" );

    if ( !db.open() )
    {
        qDebug() << "Database error!";
    }
}

void Application::incomingConnection( qintptr socketDescriptor )
{
    qDebug() << "New connection";

    SocketThread *thread = new SocketThread( socketDescriptor, this );

    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}
