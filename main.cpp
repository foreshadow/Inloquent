#include <bits/stdc++.h>
#include "model.h"
#include "user.h"

#include <QSqlDatabase>

using namespace std;

int main(int, char *[])
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL3");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("homestead");
    db.setUserName("root");
    db.setPassword("secret");

    User user;
    cout << user.User::tableName().toStdString() << endl;
    cout << user.find(1).getInt("id") << endl;
    return 0;
}
