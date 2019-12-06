#ifndef TreeMenuBean_H
#define TreeMenuBean_H

#include <QString>

class TreeMenuBean
{
public:
    void setId(qlonglong id) { m_nId = id; }
    void setName(QString name) { m_strName = name; }
    void setParentId(qlonglong parentId) { m_nParentId = parentId; }
    void setLevel(int level) { m_nLevel = level; }

    qlonglong getId() const { return m_nId; }
    QString getName() const { return m_strName; }
    qlonglong getParentId() const { return m_nParentId; }
    int getLevel() const { return m_nLevel; }

private:
    qlonglong m_nId;
    QString m_strName;
    qlonglong m_nParentId;
    int m_nLevel;
};

#endif // TreeMenuBean_H
