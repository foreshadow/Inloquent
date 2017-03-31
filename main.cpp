#include <QCoreApplication>

#include <QSqlDatabase>

#include "user.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL3");
    User u1 = User::find(2);
    User u2 = User::where("name", "infinity").first();
    return a.exec();
}
