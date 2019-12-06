#include "MaterialDao.h"
#include "bean/MaterialBean.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

MaterialBean MaterialDao::findById(qlonglong id)
{
    MaterialBean materialBean;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
            return materialBean;
        }

        QSqlQuery query(db);
        query.prepare("SELECT * FROM TMaterial WHERE material_id = :id");
        query.bindValue(":id", id);
        query.exec();

        while(query.next())
        {
            materialBean.setId(query.value("material_id").toLongLong());
            materialBean.setName(query.value("material_name").toString());
            materialBean.setGenerateTime(query.value("gen_time").toLongLong());
            materialBean.setFileType(query.value("file_type").toLongLong());
            materialBean.setFrequency(query.value("frequency").toLongLong());
            materialBean.setSize(query.value("material_size").toLongLong());
        }
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    return materialBean;
}

QList<MaterialBean> MaterialDao::findAll()
{
    QList<MaterialBean> materialBeanList;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
        }

        QSqlQuery query(db);
        query.prepare("SELECT * FROM TMaterial");
        query.exec();

        while(query.next())
        {
            MaterialBean materialBean;
            materialBean.setId(query.value("material_id").toLongLong());
            materialBean.setName(query.value("material_name").toString());
            materialBean.setGenerateTime(query.value("gen_time").toLongLong());
            materialBean.setFileType(query.value("file_type").toLongLong());
            materialBean.setFrequency(query.value("frequency").toLongLong());
            materialBean.setSize(query.value("material_size").toLongLong());
            materialBeanList.append(materialBean);
        }
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    return materialBeanList;
}

QList<MaterialBean> MaterialDao::findBy(QString name, MaterialActionBarWidget::ArrangeType arrangeType)
{
    // 排序方式
    QString strArrange = " ORDER BY ";
    switch (arrangeType) {
    case MaterialActionBarWidget::ArrangeTime:
        strArrange += "gen_time";
        break;
    case MaterialActionBarWidget::ArrangeFrequency:
        strArrange += "frequency";
        break;
    case MaterialActionBarWidget::ArrangeName:
        strArrange += "material_name";
        break;
    case MaterialActionBarWidget::ArrangeSize:
        strArrange += "material_size";
        break;
    default:
        break;
    }
    // ASC:升序 DESC:降序
    strArrange += " DESC";

    QString strQuery = "SELECT * FROM TMaterial WHERE material_name like :name";
    strQuery += strArrange;
    QList<MaterialBean> materialBeanList;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
        }
        name = "%" + name + "%";
        QSqlQuery query(db);
        query.prepare(strQuery);
        query.bindValue(":name",  name);
        query.exec();

        while(query.next())
        {
            MaterialBean materialBean;
            materialBean.setId(query.value("material_id").toLongLong());
            materialBean.setName(query.value("material_name").toString());
            materialBean.setGenerateTime(query.value("gen_time").toLongLong());
            materialBean.setFileType(query.value("file_type").toLongLong());
            materialBean.setFrequency(query.value("frequency").toLongLong());
            materialBean.setSize(query.value("material_size").toLongLong());
            materialBeanList.append(materialBean);
        }
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    return materialBeanList;
}

QList<MaterialBean> MaterialDao::findBy(FileType fileType, MaterialActionBarWidget::ArrangeType arrangeType)
{

    // 排序方式
    QString strArrange = " ORDER BY ";
    switch (arrangeType) {
    case MaterialActionBarWidget::ArrangeTime:
        strArrange += "gen_time";
        break;
    case MaterialActionBarWidget::ArrangeFrequency:
        strArrange += "frequency";
        break;
    case MaterialActionBarWidget::ArrangeName:
        strArrange += "material_name";
        break;
    case MaterialActionBarWidget::ArrangeSize:
        strArrange += "material_size";
        break;
    default:
        break;
    }
    // ASC:升序 DESC:降序
    strArrange += " DESC";

    QString strQuery = "SELECT * FROM TMaterial";
    QList<MaterialBean> materialBeanList;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
        }

        QSqlQuery query(db);
        if (fileType == FileType::AllType) {
            strQuery += strArrange;
            query.prepare(strQuery);
        } else {
            strQuery += " WHERE file_type = :file_type";
            strQuery += strArrange;
            query.prepare(strQuery);
            query.bindValue(":file_type",  (int)fileType);
        }
        query.exec();

        while(query.next())
        {
            MaterialBean materialBean;
            materialBean.setId(query.value("material_id").toLongLong());
            materialBean.setName(query.value("material_name").toString());
            materialBean.setGenerateTime(query.value("gen_time").toLongLong());
            materialBean.setFileType(query.value("file_type").toLongLong());
            materialBean.setFrequency(query.value("frequency").toLongLong());
            materialBean.setSize(query.value("material_size").toLongLong());
            materialBeanList.append(materialBean);
        }
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    return materialBeanList;
}

bool MaterialDao::isExistName(QString name)
{
    MaterialBean materialBean;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
        db.setDatabaseName("./data/AdDisplayerSystem_Server.db");

        if (!db.open()) {
            qDebug() << "connect to database error:" << db.lastError().text();
        }

        QSqlQuery query(db);
        query.prepare("SELECT * FROM TMaterial WHERE material_name = :name");
        query.bindValue(":name", name );
        query.exec();

        while(query.next())
        {
            qDebug() << "id:";
            materialBean.setId(query.value("material_id").toLongLong());
        }
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    qDebug() << "MaterialBean.getID:" << materialBean.getID();
    return materialBean.getID() == -1 ? false : true;
}

bool MaterialDao::insert(const MaterialBean &materialBean)
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
        query.prepare("INSERT INTO TMaterial(material_name, gen_time, file_type, material_size) "
                      "VALUES(:material_name, :gen_time, :file_type, :material_size)");
        query.bindValue(":material_name", materialBean.getName());
        query.bindValue(":gen_time", materialBean.getGenerateTime());
        query.bindValue(":file_type", (int)materialBean.getFileType());
        query.bindValue(":material_size", materialBean.getSize());
        bIsOK = query.exec();
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    qDebug() << "bIsOK:" << bIsOK;
    return bIsOK;
}

bool MaterialDao::updateBy(const MaterialBean &materialBean)
{

}

bool MaterialDao::updateBy(const int &id, const QString &name)
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
        query.prepare("UPDATE TMaterial SET material_name = :name WHERE material_id = :id");
        query.bindValue(":name", name);
        query.bindValue(":id", id);
        bIsOK = query.exec();
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    qDebug() << "bIsOK:" << bIsOK;
    return bIsOK;
}

bool MaterialDao::updateBy(const int &id, const int &frequency)
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
        query.prepare("UPDATE TMaterial SET frequency = :frequency WHERE material_id = :id");
        query.bindValue(":frequency", frequency);
        query.bindValue(":id", id);
        bIsOK = query.exec();
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    qDebug() << "bIsOK:" << bIsOK;
    return bIsOK;
}

bool MaterialDao::deleteById(qlonglong id)
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
        query.prepare("DELETE FROM TMaterial WHERE material_id = :id");
        query.bindValue(":id", id);
        bIsOK = query.exec();
    }
    QSqlDatabase::removeDatabase("Connection_Name");
    qDebug() << "bIsOK:" << bIsOK;
    return bIsOK;
}
