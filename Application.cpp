#include "Application.h"

Application::Application()
{
    this->db = QSqlDatabase::addDatabase("QSQLITE");
}

void Application::Run()
{
    this->SetUpDB();
}

void Application::SetUpDB()
{

    this->db.setDatabaseName( "database.sqlite" );

    if ( !db.open() )
    {
        qDebug() << "Database error!";
        return 1;
    }
}
