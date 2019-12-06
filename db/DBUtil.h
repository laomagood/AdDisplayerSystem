#ifndef DBUTIL_H
#define DBUTIL_H

#include <QMap>
#include <QList>
#include <QtSql>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QVariantMap>
#include <functional>

/**
 * 本类封装了一些操作数据库的通用方法，例如插入、更新操作、查询结果返回整数，时间类型，
 * 还可以把查询结果映射成 map，甚至通过传入的映射函数把 map 映射成对象等，也就是 Bean，
 * 极大的简化了操作数据库的类的实现，参考 UserDao.cpp.
 * 所谓的 bean，就是一个简单的对象，只有属性，getter 和 setter 方法，主要目的是用来传输数据，
 *
 * sql 可以是一个简单的 SQL，如 SELECT id, username, password FROM user，
 * 也可以是一个需要绑定参数的 SQL，如 UPDATE user SET username=:username, password=:password WHERE id=:id，
 * 这时需要把要绑定的参数放在 map 里然后与 sql 一起作为参数传入，如
 *      QVariantMap params;
 *      params["id"]       = user.id;
 *      params["username"] = user.username;
 *      params["password"] = user.password;
 *
 *      DaoTemplate::update(sql, params);
 *
 * 比较常用的方法有:
 *     insert
 *     update: 包括更新和删除
 *
 *     selectMap
 *     selectMaps
 *     selectBean
 *     selectBeans
 *     selectStrings
 */
class DBUtil {
public:
    /**
     * 执行插入语句，并返回插入行的 id.
     * @param sql
     * @param params
     * @return 如果执行成功返插入的记录的 id，否则返回 -1.
     */
    static int insert(const QString &sql, const QVariantMap &params = QVariantMap());

    /**
     * 执行更新语句 (update 和 delete 语句都是更新语句).
     * @param sql
     * @param params
     * @return 如没有错误返回 true， 有错误返回 false.
     */
    static bool update(const QString &sql, const QVariantMap &params = QVariantMap());

    /**
     * 执行查询语句，查询到一条记录，并把其映射成 map: key 是列名，value 是列值.
     * @param sql
     * @param params
     * @return 返回记录映射的 map.
     */
    static QVariantMap selectMap(const QString &sql, const QVariantMap &params = QVariantMap());

    /**
     * 执行查询语句，查询到多条记录，并把每一条记录其映射成一个 map，Key 是列名，Value 是列值.
     * 所有的 map 都放在 list 里。
     * @param sql
     * @param params
     * @return 返回记录映射的 map 的 list.
     */
    static QList<QVariantMap> selectMaps(const QString &sql, const QVariantMap &params = QVariantMap());

    /**
     * 查询结果是一个整数值，如查询记录的个数，和等.
     * @param result
     * @param sql
     * @param params
     * @return 返回 int
     */
    static int selectInt(const QString &sql, const QVariantMap &params = QVariantMap());

    /**
     * 查询结果是一个长整数值, 如果返回的是时间戳时很方便.
     * @param result
     * @param sql
     * @param params
     * @return 返回长整数
     */
    static qint64 selectInt64(const QString &sql, const QVariantMap &params = QVariantMap());

    /**
     * 查询结果是一个字符串.
     * @param str
     * @param sql
     * @param params
     * @return 返回字符串
     */
    static QString selectString(const QString &sql, const QVariantMap &params = QVariantMap());

    /**
     * 查询结果是多个字符串.
     * @param strs
     * @param sql
     * @param params
     * @return 返回 string list.
     */
    static QStringList selectStrings(const QString &sql, const QVariantMap &params = QVariantMap());

    /**
     * 查询结果是一个日期类型.
     * @param date
     * @param sql
     * @param params
     * @return 返回 date
     */
    static QDate selectDate(const QString &sql, const QVariantMap &params = QVariantMap());

    /**
     * 查询结果是一个日期时间类型.
     * @param date
     * @param sql
     * @param params
     * @return 返回 date time
     */
    static QDateTime selectDateTime(const QString &sql, const QVariantMap &params = QVariantMap());

    /**
     * 查询结果是一个 QVariant.
     * @param str
     * @param sql
     * @param params
     * @return 返回 variant
     */
    static QVariant selectVariant(const QString &sql, const QVariantMap &params = QVariantMap());

    /**
     * 查询结果封装成一个对象 bean.
     * @param sql
     * @param mapToBean - 把 map 映射成对象的函数.
     * @return 返回查找到的 bean, 如果没有查找到，返回 T 的默认对象，其 id 最好是 -1，这样便于有效的对象区别。
     */
    template <typename T>
    static T selectBean(T mapToBean(const QVariantMap &rowMap), const QString &sql, const QVariantMap &params = QVariantMap()) {
        // 把 map 都映射成一个 bean 对象
        return mapToBean(selectMap(sql, params));
    }

    /**
     * 执行查询语句，查询到多个结果并封装成 bean 的 list.
     * @param sql
     * @param params
     * @param mapToBean - 把 map 映射成 bean 对象函数.
     * @return 返回 bean 的 list，如果没有查找到，返回空的 list.
     */
    template<typename T>
    static QList<T> selectBeans(T mapToBean(const QVariantMap &rowMap), const QString &sql, const QVariantMap &params = QVariantMap()) {
        QList<T> beans;

        // 每一个 map 都映射成一个 bean 对象
        foreach (const QVariantMap row, selectMaps(sql, params)) {
            beans.append(mapToBean(row));
        }

        return beans;
    }

private:
    /**
     * 定义了访问数据库算法的骨架，SQL 语句执行的结果使用传进来的 Lambda 表达式处理
     * @param sql
     * @param params
     * @param fn - 处理 SQL 语句执行的结果的 Lambda 表达式
     */
    static void executeSql(const QString &sql, const QVariantMap &params, std::function<void(QSqlQuery *query)> fn);

    /**
     * 取得 query 的 labels(没用别名就是数据库里的列名).
     * @param query
     * @return string list.
     */
    static QStringList getFieldNames(const QSqlQuery &query);

    /**
     * 把 map 中的 key 和 value 绑定到 query 里.
     * @param query
     * @param params
     */
    static void bindValues(QSqlQuery *query, const QVariantMap &params);

    /**
     * 把 query 中的查询得到的所有行映射为 map 的 list.
     * @param query
     * @return 返回 key 为列名，值为列的值的 map 的 list.
     */
    static QList<QVariantMap> queryToMaps(QSqlQuery *query);

    /**
     * 如果 app.ini 里 output_sql 为 true，则输出执行的 SQL，如果为 false，则不输出.
     * @param query
     */
    static void debug(const QSqlQuery &query, const QVariantMap &params);
};

#endif // DBUTIL_H
