#include "DBUtil.h"
#include "ConnectionPool.h"
#include "util/Config.h"

int DBUtil::insert(const QString &sql, const QVariantMap &params) {
    int id = -1;

    executeSql(sql, params, [&id](QSqlQuery *query) {
        id = query->lastInsertId().toInt(); // 插入行的主键
    });

    return id;
}

bool DBUtil::update(const QString &sql, const QVariantMap &params) {
    bool result;

    executeSql(sql, params, [&result](QSqlQuery *query) {
        result = query->lastError().type() == QSqlError::NoError;
    });

    return result;
}

int DBUtil::selectInt(const QString &sql, const QVariantMap &params) {
    return selectVariant(sql, params).toInt();
}

qint64 DBUtil::selectInt64(const QString &sql, const QVariantMap &params) {
    return selectVariant(sql, params).toLongLong();
}

QString DBUtil::selectString(const QString &sql, const QVariantMap &params) {
    return selectVariant(sql, params).toString();
}

QDate DBUtil::selectDate(const QString &sql, const QVariantMap &params) {
    return selectVariant(sql, params).toDate();
}

QDateTime DBUtil::selectDateTime(const QString &sql, const QVariantMap &params) {
    return selectVariant(sql, params).toDateTime();
}

QVariant DBUtil::selectVariant(const QString &sql, const QVariantMap &params) {
    QVariant result;

    executeSql(sql, params, [&result](QSqlQuery *query) {
        if (query->next()) {
            result = query->value(0);
        }
    });

    return result;
}

QStringList DBUtil::selectStrings(const QString &sql, const QVariantMap &params) {
    QStringList strings;

    executeSql(sql, params, [&strings](QSqlQuery *query) {
        while (query->next()) {
            strings.append(query->value(0).toString());
        }
    });

    return strings;
}

QVariantMap DBUtil::selectMap(const QString &sql, const QVariantMap &params) {
    return selectMaps(sql, params).value(0);
}

QList<QVariantMap> DBUtil::selectMaps(const QString &sql, const QVariantMap &params) {
    QList<QVariantMap> maps;

    executeSql(sql, params, [&maps](QSqlQuery *query) {
        maps = queryToMaps(query);
    });

    return maps;
}

void DBUtil::bindValues(QSqlQuery *query, const QVariantMap &params) {
    for (QVariantMap::const_iterator i=params.constBegin(); i!=params.constEnd(); ++i) {
        query->bindValue(":" + i.key(), i.value());
    }
}

QStringList DBUtil::getFieldNames(const QSqlQuery &query) {
    QSqlRecord record = query.record();
    QStringList names;
    int count = record.count();

    for (int i = 0; i < count; ++i) {
        names << record.fieldName(i);
    }

    return names;
}

QList<QVariantMap > DBUtil::queryToMaps(QSqlQuery *query) {
    QList<QVariantMap > rowMaps;
    QStringList fieldNames = getFieldNames(*query);

    while (query->next()) {
        QVariantMap rowMap;

        foreach (const QString &fieldName, fieldNames) {
            rowMap.insert(fieldName, query->value(fieldName));
        }

        rowMaps.append(rowMap);
    }

    return rowMaps;
}

void DBUtil::debug(const QSqlQuery &query, const QVariantMap &params) {
    if (Singleton<Config>::getInstance().isDatabaseDebug()) {
        if (query.lastError().type() != QSqlError::NoError) {
            qDebug() << "    => SQL Error: " << query.lastError().text().trimmed();
        }

        qDebug() << "    => SQL Query:" << query.lastQuery();

        if (params.size() > 0) {
            qDebug() << "    => SQL Params: " << params;
        }
    }
}

void DBUtil::executeSql(const QString &sql,
                        const QVariantMap &params,
                        std::function<void (QSqlQuery *query)> handleResult) {
    QSqlDatabase db = Singleton<ConnectionPool>::getInstance().openConnection();
    QSqlQuery query(db);
    query.prepare(sql);
    bindValues(&query, params);

    if (query.exec()) {
        handleResult(&query);
    }

    debug(query, params);
    Singleton<ConnectionPool>::getInstance().closeConnection(db);
}
