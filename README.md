# Inloquent

Inloquent is a C++ Qt version of PHP Laravel Eloquent ORM.

It provides a easy access to the database.

```
for (User user : User::where("name", "Infinity")) {
    for (Article article : user.hasMany<Article>()) {
        for (Comment comment : article.hasMany<Comment>()) {    
            QString time = comment.get("updated_at");
            article.set("last_activate_at", time);
            article.save();
            if (User author = comment.belongsTo<User>()) {
                author.set("activate", true);
                author.save();
            }
        }
    }
}
```

## Prerequisite
- Qt 5.x
  - QT += core sql
  - Corresponding database's driver compiled
- C++11 enabled
- MySQL-like syntax
  - Syntax other than MySQL is not guaranteeded

## Documents
Unavailable, early development.

## Example
```
#include "model.h"
#include "db.h"

class User : public Model
{
    INLOQUENT_MODEL(User, users, id);
};

int main(int, char *[])
{
    if (DB::initialize("QMYSQL", "localhost", 3306, "test", "root", "secret") == false)
        qDebug() << DB::lastErrorMessage();

    bool ok = DB::createIfNotExists("users", [](BluePrint *table) {
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

```

## Debug output
```
[SqlQuery]  0 ms, create table if not exists users (id int primary key auto_increment not null, name char(255) unique not null, created_at timestamp not null, updated_at timestamp not null)
[SqlQuery] 10 ms, insert into `users` (`users`.`created_at`, `users`.`name`, `users`.`updated_at`) values ('2017-08-09 20:38:51', 'Infinity', '2017-08-09 20:38:51')
[SqlQuery]  0 ms, select * from users where `users`.`id` = 1 limit 1
[SqlQuery]  0 ms, select * from users where `users`.`name` = 'Infinity' limit 1
```
