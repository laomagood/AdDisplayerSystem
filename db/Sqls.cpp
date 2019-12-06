#include "Sqls.h"
#include "util/Config.h"

#include <QFile>
#include <QDebug>
#include <QXmlInputSource>
#include <QXmlAttributes>
#include <QXmlParseException>
#include <QXmlDefaultHandler>

// static 全局变量作用域为当前文件
static const char * const SQL_ID                 = "id";
static const char * const SQL_INCLUDED_DEFINE_ID = "defineId";
static const char * const SQL_TAGNAME_SQL        = "sql";
static const char * const SQL_TAGNAME_SQLS       = "sqls";
static const char * const SQL_TAGNAME_DEFINE     = "define";
static const char * const SQL_TAGNAME_INCLUDE    = "include";
static const char * const SQL_NAMESPACE          = "namespace";

/*-----------------------------------------------------------------------------|
 |                         SqlsPrivate implementation                          |
 |----------------------------------------------------------------------------*/
class SqlsPrivate : public QXmlDefaultHandler {
public:
    SqlsPrivate(Sqls *context);
    static QString buildKey(const QString &sqlNamespace, const QString &id);

protected:
    bool startElement(const QString &namespaceURI,
                      const QString &localName,
                      const QString &qName,
                      const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
    bool characters(const QString &str);
    bool fatalError(const QXmlParseException &exception);

private:
    QHash<QString, QString> defines;
    QString sqlNamespace;
    QString currentText;
    QString currentSqlId;
    QString currentDefineId;
    QString currentIncludedDefineId;

    Sqls *context;
};

SqlsPrivate::SqlsPrivate(Sqls *context) : context(context) {
    QStringList sqlFiles = Singleton<Config>::getInstance().getDatabaseSqlFiles();

    foreach (QString fileName, sqlFiles) {
        qDebug() << QString("Loading SQL file: %1").arg(fileName);

        QFile file(fileName);
        QXmlInputSource inputSource(&file);
        QXmlSimpleReader reader;
        reader.setContentHandler(this);
        reader.setErrorHandler(this);
        reader.parse(inputSource);

        defines.clear();
    }
}

QString SqlsPrivate::buildKey(const QString &sqlNamespace, const QString &id) {
    return sqlNamespace + "::" + id;
}

bool SqlsPrivate::startElement(const QString &namespaceURI,
                               const QString &localName,
                               const QString &qName,
                               const QXmlAttributes &attributes) {
    Q_UNUSED(namespaceURI)
    Q_UNUSED(localName)

    // 1. 取得 SQL 得 xml 文档中得 namespace, sql id, include 的 defineId, include 的 id
    // 2. 如果是 <sql> 标签，清空 currentText
    // 3. 如果是 <define> 标签，清空 currentText
    if (SQL_TAGNAME_SQL == qName) {
        currentSqlId = attributes.value(SQL_ID);
        currentText = "";
    } else if (SQL_TAGNAME_INCLUDE == qName) {
        currentIncludedDefineId = attributes.value(SQL_INCLUDED_DEFINE_ID);
    } else if (SQL_TAGNAME_DEFINE == qName) {
        currentDefineId = attributes.value(SQL_ID);
        currentText = "";
    } else if (SQL_TAGNAME_SQLS == qName) {
        sqlNamespace = attributes.value(SQL_NAMESPACE);
    }

    return true;
}

bool SqlsPrivate::endElement(const QString &namespaceURI, const QString &localName, const QString &qName) {
    Q_UNUSED(namespaceURI)
    Q_UNUSED(localName)

    // 1. 如果是 <sql> 标签，则插入 sqls
    // 2. 如果是 <include> 标签，则从 defines 里取其内容加入 sql
    // 3. 如果是 <define> 标签，则存入 defines
    if (SQL_TAGNAME_SQL == qName) {
        // 取到一个完整的 SQL 语句
        context->sqls.insert(buildKey(sqlNamespace, currentSqlId), currentText.simplified());
        currentText = "";
    } else if (SQL_TAGNAME_INCLUDE == qName) {
        QString defKey = buildKey(sqlNamespace, currentIncludedDefineId);
        QString def    = defines.value(defKey);

        if (!def.isEmpty()) {
            currentText += def;
        } else {
            qDebug() << "Cannot find define: " << defKey;
        }
    } else if (SQL_TAGNAME_DEFINE == qName) {
        defines.insert(buildKey(sqlNamespace, currentDefineId), currentText.simplified());
    }

    return true;
}

bool SqlsPrivate::characters(const QString &str) {
    currentText += str;
    return true;
}

bool SqlsPrivate::fatalError(const QXmlParseException &exception) {
    qDebug() << QString("Parse error at line %1, column %2, message: %3")
                .arg(exception.lineNumber())
                .arg(exception.columnNumber())
                .arg(exception.message());
    return false;
}

/*-----------------------------------------------------------------------------|
 |                             Sqls implementation                             |
 |----------------------------------------------------------------------------*/
Sqls::Sqls() {
    SqlsPrivate(this); // 读取 SQL 文件，内容放到 QHash sqls 里
}

Sqls::~Sqls() {
}

QString Sqls::getSql(const QString &sqlNamespace, const QString &sqlId) {
    QString sql = sqls.value(SqlsPrivate::buildKey(sqlNamespace, sqlId));

    if (sql.isEmpty()) {
        qDebug() << QString("Cannot find SQL for %1::%2").arg(sqlNamespace).arg(sqlId);
    }

    return sql;
}
