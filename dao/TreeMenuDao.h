#ifndef TreeMenuDao_H
#define TreeMenuDao_H

#include <QString>
class TreeMenuBean;

class TreeMenuDao
{
public:
    TreeMenuBean findById(qlonglong id);
    QList<TreeMenuBean> findtAll();

    qlonglong insert(const TreeMenuBean &TreeMenuBean);
    bool update(const TreeMenuBean &TreeMenuBean);
    bool deleteById(qlonglong id);
};

#endif // TreeMenuDao_H
