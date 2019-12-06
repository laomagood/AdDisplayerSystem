#include "Json.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QRegularExpression>
#include <QJsonParseError>

/*-----------------------------------------------------------------------------|
 |                         JsonPrivate implementation                          |
 |----------------------------------------------------------------------------*/
struct JsonPrivate {
    JsonPrivate(const QString &jsonOrJsonFilePath, bool fromFile);

    void setValue(QJsonObject &parent, const QString &path, const QJsonValue &newValue);
    QJsonValue getValue(const QString &path, const QJsonObject &fromNode) const;

    QJsonObject root; // Json 的根节点
};

JsonPrivate::JsonPrivate(const QString &jsonOrJsonFilePath, bool fromFile) {
    QByteArray json("{}"); // json 的内容

    // 如果传人的是 Json 文件的路径，则读取内容
    if (fromFile) {
        QFile file(jsonOrJsonFilePath);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            json = file.readAll();
        } else {
            qDebug() << QString("Cannot open the file: %1").arg(jsonOrJsonFilePath);
        }
    } else {
        json = jsonOrJsonFilePath.toUtf8();
    }

    // 解析 Json
    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json, &error);
    root = jsonDocument.object();

    if (QJsonParseError::NoError != error.error) {
        qDebug() << error.errorString() << ", Offset: " << error.offset;
    }
}

// 使用递归+引用设置 Json 的值，因为 toObject() 等返回的是对象的副本，对其修改不会改变原来的对象，所以需要用引用来实现
void JsonPrivate::setValue(QJsonObject &parent, const QString &path, const QJsonValue &newValue) {
    const int indexOfDot = path.indexOf('.');
    const QString property = path.left(indexOfDot); // 第一个 . 之前的内容，如果 indexOfDot 是 -1 则返回整个字符串
    const QString subPath = (indexOfDot>0) ? path.mid(indexOfDot+1) : QString(); // 第一个 . 后面的内容

    QJsonValue subValue = parent[property];

    if(subPath.isEmpty()) {
        subValue = newValue;
    } else {
        QJsonObject obj = subValue.toObject();
        setValue(obj, subPath, newValue);
        subValue = obj;
    }

    parent[property] = subValue;
}

// 读取属性的值，如果 fromNode 为空，则从跟节点开始访问
QJsonValue JsonPrivate::getValue(const QString &path, const QJsonObject &fromNode) const {
    QJsonObject parent(fromNode.isEmpty() ? root : fromNode);

    QStringList tokens = path.split(QRegularExpression("\\."));
    int size = tokens.size();

    // 定位到要访问的属性的 parent，
    // 如 "user.address.street"，要访问的属性 "street" 的 parent 是 "address"
    for (int i = 0; i < size - 1; ++i) {
        if (parent.isEmpty()) {
            return QJsonValue();
        }

        parent = parent.value(tokens.at(i)).toObject();
    }

    return parent.value(tokens.last());
}

/*-----------------------------------------------------------------------------|
 |                             Json implementation                             |
 |----------------------------------------------------------------------------*/
Json::Json(const QString &jsonOrJsonFilePath, bool fromFile) : d(new JsonPrivate(jsonOrJsonFilePath, fromFile)) {
}

Json::~Json() {
    delete d;
}

int Json::getInt(const QString &path, int def, const QJsonObject &fromNode) const {
    return getJsonValue(path, fromNode).toInt(def);
}

bool Json::getBool(const QString &path, bool def, const QJsonObject &fromNode) const {
    return getJsonValue(path, fromNode).toBool(def);
}

double Json::getDouble(const QString &path, double def, const QJsonObject &fromNode) const {
    return getJsonValue(path, fromNode).toDouble(def);
}

QString Json::getString(const QString &path, const QString &def, const QJsonObject &fromNode) const {
    return getJsonValue(path, fromNode).toString(def);
}

QStringList Json::getStringList(const QString &path, const QJsonObject &fromNode) const {
    QStringList result;
    QJsonArray array = getJsonValue(path, fromNode).toArray();

    for (QJsonArray::const_iterator iter = array.begin(); iter != array.end(); ++iter) {
        QJsonValue value = *iter;
        result << value.toString();
    }

    return result;
}

QJsonArray Json::getJsonArray(const QString &path, const QJsonObject &fromNode) const {
    return getJsonValue(path, fromNode).toArray();
}

QJsonObject Json::getJsonObject(const QString &path, const QJsonObject &fromNode) const {
    return getJsonValue(path, fromNode).toObject();
}

QJsonValue Json::getJsonValue(const QString &path, const QJsonObject &fromNode) const {
    return d->getValue(path, fromNode);
}


void Json::set(const QString &path, const QJsonValue &value) {
    d->setValue(d->root, path, value);
}

void Json::set(const QString &path, const QStringList &strings) {
    QJsonArray array;

    foreach (const QString &str, strings) {
        array.append(str);
    }

    d->setValue(d->root, path, array);
}

void Json::save(const QString &path, QJsonDocument::JsonFormat format) {
    QFile file(path);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        return;
    }

    QTextStream out(&file);
    out << QJsonDocument(d->root).toJson(format);
}

QString Json::toString(QJsonDocument::JsonFormat format) const {
    return QJsonDocument(d->root).toJson(format);
}
