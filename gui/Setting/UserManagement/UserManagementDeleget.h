#ifndef USERMANAGEMENTDELEGET_H
#define USERMANAGEMENTDELEGET_H

#include <QStyledItemDelegate>
#include <QPushButton>

class UserManagementDeleget : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit UserManagementDeleget(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index);

signals:
    void edit(const QModelIndex &index);
    void deleteData(const QModelIndex &index);

private:
    QPoint m_mousePoint;                            // 鼠标位置
    QScopedPointer<QPushButton> m_pEditButton;      // 编辑按钮
    QScopedPointer<QPushButton> m_pDeleteButton;    // 删除按钮
    QStringList m_list;                             // 列表
    int m_nSpacing;                                 // 按钮之间的间距
    int m_nWidth;                                   // 按钮宽度
    int m_nHeight;                                  // 按钮高度
    int m_nType;                                    // 按钮状态 1：划过 2：按下
};

#endif // USERMANAGEMENTDELEGET_H
