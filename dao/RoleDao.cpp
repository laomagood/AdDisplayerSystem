#include "RoleDao.h"
#include "bean/RoleBean.h"
#include "RightDao.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

RoleBean RoleDao::findById(qlonglong id)
{
    RoleBean roleBean;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
            return roleBean;
        }

        QSqlQuery query(db);
        query.prepare("SELECT * FROM TRole WHERE role_id = :id");
        query.bindValue(":id", id);
        query.exec();

        while(query.next())
        {
            roleBean.setId(query.value("role_id").toLongLong());
            roleBean.setName(query.value("role_name").toString());
            roleBean.setGenerateTime(query.value("gen_time").toLongLong());
            roleBean.setUpdateTime(query.value("update_time").toLongLong());
            roleBean.setDescription(query.value("description").toString());
            roleBean.setRightId(query.value("right_id").toLongLong());
        }
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    return roleBean;
}

QList<RoleBean> RoleDao::findAll()
{
    QList<RoleBean> roleBeanList;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
        }

        QSqlQuery query(db);
        query.prepare("SELECT * FROM TRole");
        query.exec();

        while(query.next())
        {
            RoleBean roleBean;
            roleBean.setId(query.value("role_id").toLongLong());
            roleBean.setName(query.value("role_name").toString());
            roleBean.setGenerateTime(query.value("gen_time").toLongLong());
            roleBean.setUpdateTime(query.value("update_time").toLongLong());
            roleBean.setDescription(query.value("description").toString());
            roleBean.setRightId(query.value("right_id").toLongLong());

            roleBeanList.append(roleBean);
        }
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    return roleBeanList;
}

QList<RoleBean> RoleDao::findBy(const QString &name)
{
    QList<RoleBean> roleBeanList;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
        }
        QString roleName = "%" + name + "%";
        qDebug() << roleName;
        QSqlQuery query(db);
        query.prepare("SELECT * FROM TRole WHERE role_name like :name");
        query.bindValue(":name", roleName);
        query.exec();

        while(query.next())
        {
            RoleBean roleBean;
            roleBean.setId(query.value("role_id").toLongLong());
            roleBean.setName(query.value("role_name").toString());
            roleBean.setGenerateTime(query.value("gen_time").toLongLong());
            roleBean.setUpdateTime(query.value("update_time").toLongLong());
            roleBean.setDescription(query.value("description").toString());
            roleBean.setRightId(query.value("right_id").toLongLong());

            roleBeanList.append(roleBean);
        }
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    return roleBeanList;
}

bool RoleDao::isExistName(QString name)
{
    RoleBean roleBean;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
        }

        QSqlQuery query(db);
        query.prepare("SELECT * FROM TRole WHERE role_name = :role_name");
        query.bindValue(":role_name", name );
        query.exec();

        while(query.next())
        {
            roleBean.setId(query.value("role_id").toLongLong());
        }
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    return roleBean.getID() == -1 ? false : true;
}

int RoleDao::insert(const RoleBean &roleBean)
{
    int nLastRowID = -1;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
            return -1;
        }

        QSqlQuery query(db);
        query.prepare("INSERT INTO TRole(role_name, gen_time, description, right_id)"
                      "VALUES(:name, :gen_time, :description, :right_id)");
        query.bindValue(":name", roleBean.getName());
        query.bindValue(":gen_time", roleBean.getGenerateTime());
        query.bindValue(":description", roleBean.getDescription());
        query.bindValue(":right_id", roleBean.getRightId());

        query.exec();
        nLastRowID = query.lastInsertId().toInt();
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    return nLastRowID;
}

bool RoleDao::updateBy(const RoleBean &roleBean)
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
        query.prepare("UPDATE TRole SET role_name = :role_name, description = :description "
                      "WHERE role_id = :id");

        query.bindValue(":role_name", roleBean.getName());
        query.bindValue(":description", roleBean.getDescription());
        query.bindValue(":id", roleBean.getID());
        bIsOK = query.exec();
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    return bIsOK;
}

bool RoleDao::deleteById(qlonglong id)
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
        query.prepare("DELETE FROM TRole WHERE role_id = :id");
        query.bindValue(":id", id);
        bIsOK = query.exec();
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    return bIsOK;
}
