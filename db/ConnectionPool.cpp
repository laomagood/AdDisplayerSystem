#include "ConnectionPool.h"
#include "util/Config.h"
#include <QDebug>
#include <QtSql>
#include <QStack>
#include <QString>
#include <QMutex>
#include <QSemaphore>

/*-----------------------------------------------------------------------------|
 |                          ConnectionPoolPrivate 的定义                        |
 |----------------------------------------------------------------------------*/
class ConnectionPoolPrivate {
public:
    ConnectionPoolPrivate();
    ~ConnectionPoolPrivate();

    QStack<QString> usedConnectionNames;   // 已使用的数据库连接名
    QStack<QString> unusedConnectionNames; // 未使用的数据库连接名

    // 数据库信息
    QString hostName;
    QString databaseName;
    QString username;
    QString password;
    QString databaseType;
    int     port;

    bool    testOnBorrow;    // 取得连接的时候验证连接有效
    QString testOnBorrowSql; // 测试访问数据库的 SQL
    int maxWaitTime;         // 获取连接最大等待时间
    int maxConnectionCount;  // 最大连接数

    QSemaphore *semaphore;

    static QMutex mutex;
    static int lastKey; // 用来创建连接的名字，保证连接名字不会重复
};

QMutex ConnectionPoolPrivate::mutex;
int ConnectionPoolPrivate::lastKey = 0;

ConnectionPoolPrivate::ConnectionPoolPrivate() {
    Config &config = Singleton<Config>::getInstance();

    // 从配置文件里读取
    hostName           = config.getDatabaseHost();
    databaseName       = config.getDatabaseName();
    username           = config.getDatabaseUsername();
    password           = config.getDatabasePassword();
    databaseType       = config.getDatabaseType();
    port               = config.getDatabasePort();
    testOnBorrow       = config.getDatabaseTestOnBorrow();
    testOnBorrowSql    = config.getDatabaseTestOnBorrowSql();
    maxWaitTime        = config.getDatabaseMaxWaitTime();
    maxConnectionCount = config.getDatabaseMaxConnectionCount();

    semaphore = new QSemaphore(maxConnectionCount);
}

ConnectionPoolPrivate::~ConnectionPoolPrivate() {
    // 销毁连接池的时候删除所有的连接
    foreach(QString connectionName, usedConnectionNames) {
        QSqlDatabase::removeDatabase(connectionName);
    }

    foreach(QString connectionName, unusedConnectionNames) {
        QSqlDatabase::removeDatabase(connectionName);
    }

    delete semaphore;
}

/*-----------------------------------------------------------------------------|
 |                             ConnectionPool 的定义                            |
 |----------------------------------------------------------------------------*/
ConnectionPool::ConnectionPool() : d(new ConnectionPoolPrivate) {

}

ConnectionPool::~ConnectionPool() {
    delete d;
}

QSqlDatabase ConnectionPool::openConnection() {
    Q_ASSERT(NULL != d);

    if (d->semaphore->tryAcquire(1, d->maxWaitTime)) {
        // 有已经回收的连接，复用它们
        // 没有已经回收的连接，则创建新的连接
        ConnectionPoolPrivate::mutex.lock();
        QString connectionName = d->unusedConnectionNames.size() > 0 ?
                    d->unusedConnectionNames.pop() :
                    QString("C%1").arg(++ConnectionPoolPrivate::lastKey);
        d->usedConnectionNames.push(connectionName);
        ConnectionPoolPrivate::mutex.unlock();

        // 创建连接，因为创建连接很耗时，所以不放在 lock 的范围内，提高并发效率
        QSqlDatabase db = createConnection(connectionName);

        if (!db.isOpen()) {
            ConnectionPoolPrivate::mutex.lock();
            d->usedConnectionNames.removeOne(connectionName); // 无效连接删除
            ConnectionPoolPrivate::mutex.unlock();

            d->semaphore->release(); // 没有消耗连接
        }

        return db;
    } else {
        // 创建连接超时，返回一个无效连接
        qDebug() << "Time out to create connection.";
        return QSqlDatabase();
    }
}

void ConnectionPool::closeConnection(const QSqlDatabase &connection) {
    Q_ASSERT(NULL != d);
    QString connectionName = connection.connectionName();

    // 如果是我们创建的连接，并且已经被使用，则从 used 里删除，放入 unused 里
    if (d->usedConnectionNames.contains(connectionName)) {
        QMutexLocker locker(&ConnectionPoolPrivate::mutex);
        d->usedConnectionNames.removeOne(connectionName);
        d->unusedConnectionNames.push(connectionName);
        d->semaphore->release();
    }
}

QSqlDatabase ConnectionPool::createConnection(const QString &connectionName) {
    Q_ASSERT(NULL != d);

    // 连接已经创建过了，复用它，而不是重新创建
    if (QSqlDatabase::contains(connectionName)) {
        QSqlDatabase existingDb = QSqlDatabase::database(connectionName);

        if (d->testOnBorrow) {
            // 返回连接前访问数据库，如果连接断开，重新建立连接
            qDebug() << QString("Test connection on borrow, execute: %1, for connection %2")
                        .arg(d->testOnBorrowSql).arg(connectionName);
            QSqlQuery query(d->testOnBorrowSql, existingDb);

            if (query.lastError().type() != QSqlError::NoError && !existingDb.open()) {
                qDebug() << "Open datatabase error:" << existingDb.lastError().text();
                return QSqlDatabase();
            }
        }

        return existingDb;
    }

    // 创建一个新的连接
    QSqlDatabase db = QSqlDatabase::addDatabase(d->databaseType, connectionName);
    db.setHostName(d->hostName);
    db.setDatabaseName(d->databaseName);
    db.setUserName(d->username);
    db.setPassword(d->password);

    if (d->port != 0) {
        db.setPort(d->port);
    }

    if (!db.open()) {
        qDebug() << "Open datatabase error:" << db.lastError().text();
        return QSqlDatabase();
    }

    return db;
}

void ConnectionPool::destroy() {
    if (NULL != d) {
        ConnectionPoolPrivate::mutex.lock();
        delete d;
        d = NULL;
        ConnectionPoolPrivate::mutex.unlock();

        qDebug() << "Destroy connection pool";
    }
}
