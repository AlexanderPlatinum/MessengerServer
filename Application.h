#pragma once

#include <QSql>
#include <QSqlDatabase>

#include "SQLCommands.h"

class Application
{
public:
    Application();

    void Run ();

private:
    QSqlDatabase db;

    void SetUpDB ();
};
