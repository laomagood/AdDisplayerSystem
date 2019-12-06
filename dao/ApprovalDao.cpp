#include "ApprovalDao.h"
#include "bean/ApprovalBean.h"
#include "dao/MaterialDao.h"
#include "dao/UserDao.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

/*
qlonglong m_nId;                ✔
qlonglong m_nUserId;            ✔
qlonglong m_nMaterialId;        ✔
QDateTime m_nApprovalTime;      ✔
ApprovalType m_approvalType;    ✔
MaterialBean m_materialBean;    ✔
UserBean m_userBean;            ✔
*/

ApprovalBean ApprovalDao::findById(qlonglong id)
{
    ApprovalBean approvalBean;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
            return approvalBean;
        }

        QSqlQuery query(db);
        query.prepare("SELECT * FROM TMaterialApproval WHERE tma_id = :id");
        query.bindValue(":id", id);
        query.exec();

        while(query.next())
        {
            approvalBean.setId(query.value("tma_id").toLongLong());
            approvalBean.setUserId(query.value("user_id").toLongLong());
            approvalBean.setMaterialId(query.value("material_id").toLongLong());
            approvalBean.setApprovalTime(query.value("approval_time").toLongLong());
            approvalBean.setApprovalType(query.value("approval_type").toLongLong());
        }
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    return approvalBean;
}

ApprovalBean ApprovalDao::findByMaterialId(qlonglong id)
{
    ApprovalBean approvalBean;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
            return approvalBean;
        }

        QSqlQuery query(db);
        query.prepare("SELECT * FROM TMaterialApproval WHERE material_id = :id");
        query.bindValue(":id", id);
        query.exec();

        while(query.next())
        {
            approvalBean.setId(query.value("tma_id").toLongLong());
            approvalBean.setUserId(query.value("user_id").toLongLong());
            approvalBean.setMaterialId(query.value("material_id").toLongLong());
            approvalBean.setApprovalTime(query.value("approval_time").toLongLong());
            approvalBean.setApprovalType(query.value("approval_type").toLongLong());
        }
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    return approvalBean;
}

QList<ApprovalBean> ApprovalDao::findAll()
{
    QList<ApprovalBean> approvalBeanList;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
        }

        QSqlQuery query(db);
        query.prepare("SELECT * FROM TMaterialApproval");
        query.exec();

        while(query.next())
        {
            ApprovalBean approvalBean;
            approvalBean.setId(query.value("material_id").toLongLong());
            approvalBeanList.append(approvalBean);
        }
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    return approvalBeanList;
}


QList<ApprovalBean> ApprovalDao::findBy(QString name, MaterialActionBarWidget::ArrangeType arrangeType)
{
    // [1] 查询 QList<MaterialBean>
    MaterialDao materialDao;
    QList<MaterialBean> materialBeanList = materialDao.findBy(name, arrangeType);

    QList<ApprovalBean> approvalBeanList;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
        }

        for (int i=0; i<materialBeanList.size(); i++) {
            // [2] 查询 QList<ApprovalBean>
            qlonglong id = materialBeanList.at(i).getID();
            ApprovalBean approvalBean;
            approvalBean.setMaterialId(id);
            approvalBean.setMaterialBean(materialBeanList.at(i));

            QSqlQuery query(db);
            query.prepare("SELECT * FROM TMaterialApproval WHERE material_id = :material_id");
            query.bindValue(":material_id", id);
            query.exec();

            while(query.next())
            {
                approvalBean.setId(query.value("tma_id").toLongLong());
                approvalBean.setUserId(query.value("user_id").toLongLong());
                approvalBean.setMaterialId(query.value("material_id").toLongLong());
                approvalBean.setApprovalTime(query.value("approval_time").toLongLong());
                approvalBean.setApprovalType(query.value("approval_type").toLongLong());
            }
            approvalBeanList.append(approvalBean);

            // [3] 查询 QList<UserBean>
            UserDao userlDao;
            UserBean userBean = userlDao.findById(approvalBean.getUserID());
            approvalBean.setUserBean(userBean);
        }
    }

    QSqlDatabase::removeDatabase("Connection_Name");
    return approvalBeanList;
}

QList<ApprovalBean> ApprovalDao::findBy(FileType fileType, MaterialActionBarWidget::ArrangeType arrangeType,
                                        MaterialActionBarWidget::ApprovalType approvalType)
{
    // [1] 查询 QList<MaterialBean>
    MaterialDao materialDao;
    QList<MaterialBean> materialBeanList = materialDao.findBy(fileType, arrangeType);

    QList<ApprovalBean> approvalBeanList;
    {
        for (int i=0; i<materialBeanList.size(); i++) {
            // [2] 查询 QList<ApprovalBean>
            qlonglong id = materialBeanList.at(i).getID();

            ApprovalBean approvalBean = findByMaterialId(id);
            approvalBean.setMaterialBean(materialBeanList.at(i));

            // [3] 查询 QList<UserBean>
            UserDao userlDao;
            UserBean userBean = userlDao.findById(approvalBean.getUserID());
            approvalBean.setUserBean(userBean);
//            qDebug() << approvalType << approvalBean.getApprovalType();

            // 审核类型判断
            if (approvalType == MaterialActionBarWidget::ApprovalAll) {
                approvalBeanList.append(approvalBean);
            } else {
                if ((int)approvalBean.getApprovalType() == (int)approvalType){
                    approvalBeanList.append(approvalBean);
                }
            }
        }
    }

    return approvalBeanList;
}

bool ApprovalDao::isExistName(QString name)
{
    ApprovalBean approvalBean;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
        }

        QSqlQuery query(db);
        query.prepare("SELECT * FROM TMaterialApproval WHERE material_name = :name");
        query.bindValue(":name", name );
        query.exec();

        while(query.next())
        {
            approvalBean.setId(query.value("material_id").toLongLong());
        }
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    qDebug() << "approvalBean.getID:" << approvalBean.getID();
    return approvalBean.getID() == -1 ? false : true;
}

bool ApprovalDao::insert(const ApprovalBean &approvalBean)
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
        query.prepare("INSERT INTO TMaterialApproval(user_id, material_id, approval_time, approval_type) "
                      "VALUES(:user_id, :material_id, :approval_time, :approval_type)");
        query.bindValue(":user_id", approvalBean.getUserID());
        query.bindValue(":material_id", approvalBean.getMaterialID());
        query.bindValue(":approval_time", approvalBean.getApprovalTime());
        query.bindValue(":approval_type", approvalBean.getApprovalType());
        bIsOK = query.exec();
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    qDebug() << "bIsOK:" << bIsOK;
    return bIsOK;
}

bool ApprovalDao::updateBy(const ApprovalBean &approvalBean)
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
        query.prepare("UPDATE TMaterialApproval SET approval_time = :approval_time, approval_type = :approval_type WHERE tma_id = :tma_id");
        query.bindValue(":approval_time", approvalBean.getApprovalTime());
        query.bindValue(":approval_type", approvalBean.getApprovalType());
        query.bindValue(":tma_id", approvalBean.getID());
        bIsOK = query.exec();
        qDebug() << query.lastQuery();
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    qDebug() << "bIsOK:" << bIsOK;
    return bIsOK;
}

bool ApprovalDao::deleteById(qlonglong id)
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
        query.prepare("DELETE FROM TMaterialApproval WHERE material_id = :id");
        query.bindValue(":id", id);
        bIsOK = query.exec();
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    qDebug() << "bIsOK:" << bIsOK;
    return bIsOK;
}
