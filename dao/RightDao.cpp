#include "RightDao.h"
#include "bean/RightBean.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

RightBean RightDao::findById(qlonglong id)
{
    RightBean rightBean;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
            return rightBean;
        }

        QSqlQuery query(db);
        query.prepare("SELECT * FROM TRight WHERE right_id = :id");
        query.bindValue(":id", id);
        query.exec();

        while(query.next())
        {
            rightBean.setID(query.value("right_id").toLongLong());
            rightBean.setGenUser(query.value("is_gen_user").toBool());
            rightBean.setManageUser(query.value("is_manage_user").toBool());
            rightBean.setDeleteUser(query.value("is_delete_user").toBool());

            rightBean.setGenRole(query.value("is_gen_role").toBool());
            rightBean.setManageRole(query.value("is_manage_role").toBool());
            rightBean.setDeleteRole(query.value("is_delete_role").toBool());

            rightBean.setUploadMaterial(query.value("is_upload_material").toBool());
            rightBean.setCheckMaterial(query.value("is_check_material").toBool());
            rightBean.setDeleteMaterial(query.value("is_delete_material").toBool());

            rightBean.setUploadProgram(query.value("is_upload_program").toBool());
            rightBean.setCheckProgram(query.value("is_check_program").toBool());
            rightBean.setDeleteProgram(query.value("is_delete_program").toBool());
            rightBean.setGenProgram(query.value("is_gen_program").toBool());
            rightBean.setReleaseProgram(query.value("is_release_program").toBool());
            rightBean.setManageProgram(query.value("is_manage_program").toBool());
        }
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    return rightBean;
}

int RightDao::insert(const RightBean &rightBean)
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
        query.prepare("INSERT INTO TRight ("
                      "is_gen_user, is_manage_user, is_delete_user,"
                      "is_gen_role, is_manage_role, is_delete_role,"
                      "is_upload_material,  is_check_material,  is_delete_material,"
                      "is_upload_program,   is_check_program,   is_delete_program,"
                      "is_gen_program,      is_release_program, is_manage_program)"
                      "VALUES(:is_gen_user, :is_manage_user, :is_delete_user,"
                      ":is_gen_role, :is_manage_role, :is_delete_role,"
                      ":is_upload_material, :is_check_material, :is_delete_material,"
                      ":is_upload_program, :is_check_program, :is_delete_program,"
                      ":is_gen_program, :is_release_program, :is_manage_program)");

        query.bindValue(":is_gen_user", rightBean.isGenUser());
        query.bindValue(":is_manage_user", rightBean.isManageUser());
        query.bindValue(":is_delete_user", rightBean.isDeleteUser());

        query.bindValue(":is_gen_role", rightBean.isGenRole());
        query.bindValue(":is_manage_role", rightBean.isManageRole());
        query.bindValue(":is_delete_role", rightBean.isDeleteRole());

        query.bindValue(":is_upload_material", rightBean.isUploadMaterial());
        query.bindValue(":is_check_material", rightBean.isCheckMaterial());
        query.bindValue(":is_delete_material", rightBean.isDeleteMaterial());

        query.bindValue(":is_upload_program", rightBean.isUploadProgram());
        query.bindValue(":is_check_program", rightBean.isCheckProgram());
        query.bindValue(":is_delete_program", rightBean.isDeleteProgram());
        query.bindValue(":is_gen_program", rightBean.isGenProgram());
        query.bindValue(":is_release_program", rightBean.isReleaseProgram());
        query.bindValue(":is_manage_program", rightBean.isManageProgram());

        query.exec();

        nLastRowID = query.lastInsertId().toInt();
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    return nLastRowID;
}

bool RightDao::updateBy(const RightBean &rightBean)
{
    bool bIsOK = false;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
            return -1;
        }

        qDebug() << "id" << rightBean.getID();
        QSqlQuery query(db);
        query.prepare("UPDATE TRight SET is_gen_user = :is_gen_user, is_manage_user = :is_manage_user, is_delete_user = :is_delete_user,"
                      "is_gen_role = :is_gen_role, is_manage_role = :is_manage_role, is_delete_role = :is_delete_role,"
                      "is_upload_material = :is_upload_material, is_check_material = :is_check_material, is_delete_material =:is_delete_material,"
                      "is_upload_program = :is_upload_program, is_check_program = :is_check_program, is_delete_program = :is_delete_program,"
                      "is_gen_program = :is_gen_program, is_release_program = :is_release_program, is_manage_program = :is_manage_program "
                      "WHERE right_id = :id");

        query.bindValue(":is_gen_user", rightBean.isGenUser());
        query.bindValue(":is_manage_user", rightBean.isManageUser());
        query.bindValue(":is_delete_user", rightBean.isDeleteUser());

        query.bindValue(":is_gen_role", rightBean.isGenRole());
        query.bindValue(":is_manage_role", rightBean.isManageRole());
        query.bindValue(":is_delete_role", rightBean.isDeleteRole());

        query.bindValue(":is_upload_material", rightBean.isUploadMaterial());
        query.bindValue(":is_check_material", rightBean.isCheckMaterial());
        query.bindValue(":is_delete_material", rightBean.isDeleteMaterial());

        query.bindValue(":is_upload_program", rightBean.isUploadProgram());
        query.bindValue(":is_check_program", rightBean.isCheckProgram());
        query.bindValue(":is_delete_program", rightBean.isDeleteProgram());
        query.bindValue(":is_gen_program", rightBean.isGenProgram());
        query.bindValue(":is_release_program", rightBean.isReleaseProgram());
        query.bindValue(":is_manage_program", rightBean.isManageProgram());
        query.bindValue(":id", rightBean.getID());
        bIsOK = query.exec();
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    return bIsOK;
}

bool RightDao::deleteById(qlonglong id)
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
        query.prepare("DELETE FROM TRight WHERE right_id = :id");
        query.bindValue(":id", id);
        bIsOK = query.exec();
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    return bIsOK;
}
