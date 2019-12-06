#include "TreeMenuDao.h"
#include "bean/TreeMenuBean.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

TreeMenuBean TreeMenuDao::findById(qlonglong id)
{
    TreeMenuBean treeMenuBean;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
            return treeMenuBean;
        }

        QSqlQuery query(db);
        query.prepare("SELECT * FROM TTreeMenu WHERE id = :id");
        query.bindValue(":id", id);
        query.exec();

        while(query.next())
        {
            treeMenuBean.setId(query.value("id").toLongLong());
            treeMenuBean.setName(query.value("name").toString());
            treeMenuBean.setParentId(query.value("parent_id").toLongLong());
            treeMenuBean.setLevel(query.value("level").toInt());
        }
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    return treeMenuBean;
}

QList<TreeMenuBean> TreeMenuDao::findtAll()
{
    QList<TreeMenuBean> treeMenuBeanList;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
        }

        QSqlQuery query(db);
        query.prepare("SELECT * FROM TTreeMenu");
        query.exec();

        while(query.next())
        {
            TreeMenuBean treeMenuBean;
            treeMenuBean.setId(query.value("id").toLongLong());
            treeMenuBean.setName(query.value("name").toString());
            treeMenuBean.setParentId(query.value("parent_id").toLongLong());
            treeMenuBean.setLevel(query.value("level").toInt());
            treeMenuBeanList.append(treeMenuBean);
        }
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    return treeMenuBeanList;
}

qlonglong TreeMenuDao::insert(const TreeMenuBean &TreeMenuBean)
{
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
            return -1;
        }

        QSqlQuery query(db);
        query.prepare("INSERT INTO TTreeMenu(name, parent_id, level) VALUES(:name, :parent_id, :level)");
        query.bindValue(":name",    TreeMenuBean.getName());
        query.bindValue(":parent_id", TreeMenuBean.getParentId());
        query.bindValue(":level", TreeMenuBean.getLevel());
        query.exec();
    }
    QSqlDatabase::removeDatabase("Connection_Name");
}

bool TreeMenuDao::update(const TreeMenuBean &TreeMenuBean)
{

}

bool TreeMenuDao::deleteById(qlonglong id)
{

}
