#include <bits/stdc++.h>
#include "model.h"
#include "db.h"

using namespace std;

class User : public Model
{
    INLOQUENT_MODEL(User, users, id);
};

int main(int, char *[])
{
    if (DB::initialize("QMYSQL", "localhost", 3306, "test", "root", "secret") == false)
        cout << DB::lastErrorMessage().toStdString() << endl;

    bool ok = DB::createIfNotExists("users", [](BluePrint *table){
        table->increment("id");
        table->string("name")->unique();
        table->timestamps();
    });
    if (ok == false)
        qDebug() << DB::lastErrorMessage();

    User user;
    user.set("name", "Infinity");
    if (user.save() == false)
        qDebug() << DB::lastErrorMessage();
    Q_ASSERT(User::find(1).get("name") == "Infinity");
    Q_ASSERT(User::where("name", "Infinity").first()["id"] == 1);
    return 0;
}
