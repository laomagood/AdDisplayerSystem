#include "UserDao.h"
#include "bean/UserBean.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

UserBean UserDao::findById(qlonglong id)
{
    UserBean userBean;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
            return userBean;
        }

        QSqlQuery query(db);
        query.prepare("SELECT * FROM TUser JOIN TRole ON TUser.role_id = TRole.role_id WHERE user_id = :id");
        query.bindValue(":id", id);
        query.exec();

        while(query.next())
        {
            userBean.setId(query.value("user_id").toLongLong());
            userBean.setName(query.value("login_name").toString());
            userBean.setGenerateTime(query.value("gen_time").toLongLong());
            userBean.setLoginTime(query.value("login_time").toLongLong());
            userBean.setLastLoginTime(query.value("last_login_time").toLongLong());
            userBean.setRoleId(query.value("TUser.role_id").toInt());
            userBean.setRoleName(query.value("TRole.role_name").toString());

            QSqlQuery queryRole(db);
            queryRole.prepare("SELECT * FROM TUser WHERE user_id = :id");
            queryRole.bindValue(":id", id);
            queryRole.exec();
        }
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    return userBean;
}

QList<UserBean> UserDao::findAll()
{
    QList<UserBean> userBeanList;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
        }

        QSqlQuery query(db);
        query.prepare("SELECT * FROM TUser JOIN TRole ON TUser.role_id = TRole.role_id");
        query.exec();

        while(query.next())
        {
            UserBean userBean;
            userBean.setId(query.value("user_id").toLongLong());
            userBean.setName(query.value("login_name").toString());
            userBean.setGenerateTime(query.value("gen_time").toLongLong());
            userBean.setLoginTime(query.value("login_time").toLongLong());
            userBean.setLastLoginTime(query.value("last_login_time").toLongLong());
            userBean.setRoleId(query.value("TUser.role_id").toInt());
            userBean.setRoleName(query.value("TRole.role_name").toString());
            userBeanList.append(userBean);
        }
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    return userBeanList;
}

QList<UserBean> UserDao::findBy(QString loginName)
{
    QList<UserBean> userBeanList;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
        }
        loginName = "%" + loginName + "%";
        QSqlQuery query(db);
        query.prepare("SELECT * FROM TUser WHERE login_name like :name");
        query.bindValue(":name",  loginName);
        query.exec();

        while(query.next())
        {
            UserBean userBean;
            userBean.setId(query.value("user_id").toLongLong());
            userBean.setName(query.value("login_name").toString());
            userBean.setGenerateTime(query.value("gen_time").toLongLong());
            userBean.setLoginTime(query.value("login_time").toLongLong());
            userBean.setLastLoginTime(query.value("last_login_time").toLongLong());
            userBean.setRoleId(query.value("role_id").toInt());
            userBeanList.append(userBean);
        }
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    return userBeanList;
}

bool UserDao::isExistLoginName(QString loginName)
{
    UserBean userBean;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
        }

        QSqlQuery query(db);
        query.prepare("SELECT * FROM TUser WHERE login_name = :name");
        query.bindValue(":name", loginName );
        query.exec();

        while(query.next())
        {
            qDebug() << "id:";
            userBean.setId(query.value("user_id").toLongLong());
        }
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    qDebug() << "userBean.getID:" << userBean.getID();
    return userBean.getID() == -1 ? false : true;
}

bool UserDao::insert(const UserBean &userBean)
{
    bool bIsOK = false;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
            return -1;
        }

        QSqlQuery query(db);
        query.prepare("INSERT INTO TUser(login_name, password, gen_time, role_id) VALUES(:login_name, :password, :gen_time, :role_id)");
        query.bindValue(":login_name", userBean.getName());
        query.bindValue(":password", userBean.getPassword());
        query.bindValue(":gen_time", userBean.getGenerateTime());
        query.bindValue(":role_id", userBean.getRoleId());
        bIsOK = query.exec();
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    qDebug() << "bIsOK:" << bIsOK;
    return bIsOK;
}

bool UserDao::updateBy(const UserBean &userBean)
{

}

bool UserDao::updateBy(const int &id, const QString &password)
{
    bool bIsOK = false;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
            return -1;
        }

        QSqlQuery query(db);
        query.prepare("UPDATE TUser SET password = :password WHERE user_id = :id");
        query.bindValue(":password", password);
        query.bindValue(":id", id);
        bIsOK = query.exec();
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    qDebug() << "bIsOK:" << bIsOK;
    return bIsOK;
}

bool UserDao::updateBy(const int &id, const int &role)
{
    bool bIsOK = false;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
            return -1;
        }

        QSqlQuery query(db);
        query.prepare("UPDATE TUser SET role_id = :role_id WHERE user_id = :id");
        query.bindValue(":role_id", role);
        query.bindValue(":id", id);
        bIsOK = query.exec();
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    qDebug() << "bIsOK:" << bIsOK;
    return bIsOK;
}

bool UserDao::deleteById(qlonglong id)
{
    bool bIsOK = false;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
            return -1;
        }

        QSqlQuery query(db);
        query.prepare("DELETE FROM TUser WHERE user_id = :id");
        query.bindValue(":id", id);
        bIsOK = query.exec();
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    qDebug() << "bIsOK:" << bIsOK;
    return bIsOK;
}
