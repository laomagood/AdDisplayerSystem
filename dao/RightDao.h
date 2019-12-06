#ifndef RightDao_H
#define RightDao_H

#include <QString>
class RightBean;

class RightDao
{
public:
    RightBean findById(qlonglong id);
    int insert(const RightBean &rightBean);
    bool updateBy(const RightBean &rightBean);
    bool deleteById(qlonglong id);
};

#endif // RightDao_H
