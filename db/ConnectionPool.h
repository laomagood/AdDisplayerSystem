#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H
#include "util/Singleton.h"

/**
 * 实现了一个简易的数据库连接池，简化了数据库连接的获取。通过配置最大的连接数可创建多个连接支持多线程访问数据库，
 * Qt 里同一个数据库连接不能被多个线程共享。连接使用完后释放回连接池而不是直接关闭，再次使用的时候不必重新建立连接，
 * 建立连接是很耗时的操作，底层是 Socket 连接。
 *
 * 如果 testOnBorrow 为 true，则连接断开后会自动重新连接（例如数据库程序崩溃了，网络的原因导致连接断了等），
 * 但是每次获取连接的时候都会先访问一下数据库测试连接是否有效，如果无效则重新建立连接。testOnBorrow 为 true 时，
 * 需要提供一条 SQL 语句用于测试查询，例如 MySQL 下可以用 SELECT 1。
 *
 * 如果 testOnBorrow 为 false，则连接断开后不会自动重新连接，这时获取到的连接调用 QSqlDatabase::isOpen() 返回的值
 * 仍然是 true（因为先前的时候已经建立好了连接，Qt 里没有提供判断底层连接断开的方法或者信号）。
 *
 * 当程序结束后，需要调用 Singleton<ConnectionPool>::getInstance().destroy() 关闭所有数据库的连接（一般在 main() 函数返回前调用）。
 *
 * 使用方法：
 * 1. 从数据库连接池里取得连接
 *    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().openConnection();
 *
 * 2. 使用 db 访问数据库，如
 *    QSqlQuery query(db);
 *
 * 3. 数据库连接使用完后需要释放回数据库连接池
 *    Singleton<ConnectionPool>::getInstance().closeConnection(db);
 *
 * 4. 程序结束的时候真正的关闭所有数据库连接
 *    Singleton<ConnectionPool>::getInstance().destroy();
 */

class QSqlDatabase;
class ConnectionPoolPrivate;

class ConnectionPool {
    SINGLETON(ConnectionPool)

public:
    void destroy(); // 销毁连接池，关闭所有的数据库连接
    QSqlDatabase openConnection();                        // 获取数据库连接
    void closeConnection(const QSqlDatabase &connection); // 释放数据库连接回连接池

private:
    QSqlDatabase createConnection(const QString &connectionName); // 创建数据库连接
    ConnectionPoolPrivate *d;
};

#endif // CONNECTIONPOOL_H
