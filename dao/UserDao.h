#ifndef UserDao_H
#define UserDao_H

#include <QString>
class UserBean;

class UserDao
{
public:
    UserBean findById(qlonglong id);
    QList<UserBean> findAll();
    QList<UserBean> findBy(QString loginName);          // 模糊查询
    bool isExistLoginName(QString loginName);           // 查询是否存在

    bool insert(const UserBean &userBean);
    bool updateBy(const UserBean &userBean);
    bool updateBy(const int &id, const QString &password);
    bool updateBy(const int &id, const int &role);
    bool deleteById(qlonglong id);
};

#endif // UserDao_H
