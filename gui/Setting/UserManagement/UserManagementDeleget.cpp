#include "UserManagementDeleget.h"
#include "UserManagementModel.h"
#include <QDebug>
#include <QCheckBox>
#include <QApplication>
#include <QComboBox>
#include <QPainter>
#include <QToolTip>
#include <QMouseEvent>

UserManagementDeleget::UserManagementDeleget(QObject *parent)
    : QStyledItemDelegate(parent),
      m_pEditButton(new QPushButton()),
      m_pDeleteButton(new QPushButton()),
      m_nSpacing(5),
      m_nWidth(25),
      m_nHeight(20)

{
    m_pEditButton->setStyleSheet("QPushButton { border: none; background-color: transparent; image:url(./image/Setting/edit.png);}"
                                 "QPushButton:hover {image:url(./image/Setting/editHover.png);}"
                                 "QPushButton:pressed {image:url(./image/Setting/editPressed.png);}"
                                 );

    m_pDeleteButton->setStyleSheet("QPushButton {border: none; background-color: transparent; image:url(./image/Setting/delete.png);}"
                                   "QPushButton:hover {image:url(./image/Setting/deleteHover.png);} "
                                   "QPushButton:pressed {image:url(./image/Setting/deletePressed.png);}");
    m_list << QStringLiteral("查看编辑") << QStringLiteral("删除");
}

void UserManagementDeleget::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;
    QStyledItemDelegate::paint(painter, viewOption, index);

    // 选择栏
    if (index.column() == (int)UserManagementModel::Column::CheckBox) {
        bool data = index.model()->data(index, Qt::UserRole).toBool();

        QStyleOptionButton checkBoxStyle;
        checkBoxStyle.state = data ? QStyle::State_On : QStyle::State_Off;
        checkBoxStyle.state |= QStyle::State_Enabled;
        checkBoxStyle.iconSize = QSize(20, 20);
        checkBoxStyle.rect = option.rect;

        QCheckBox checkBox;
        QApplication::style()->drawPrimitive(QStyle::PE_IndicatorCheckBox, &checkBoxStyle, painter, &checkBox);
    }
    // 编辑栏
    else if(index.column() == (int)UserManagementModel::Column::Edit) {
        // 计算按钮显示区域
        int nCount = m_list.count();
        int nHalf = (option.rect.width() - m_nWidth * nCount - m_nSpacing * (nCount - 1)) / 2;
        int nTop = (option.rect.height() - m_nHeight) / 2;

        for (int i = 0; i < nCount; ++i)
        {
            // 绘制按钮
            QStyleOptionButton button;
            button.rect = QRect(option.rect.left() + nHalf + m_nWidth * i + m_nSpacing * i,
                                option.rect.top() + nTop,  m_nWidth, m_nHeight);
            button.state |= QStyle::State_Enabled;

            if (button.rect.contains(m_mousePoint)) {
                if (m_nType == 0) {
                    button.state |= QStyle::State_MouseOver;
                } else if (m_nType == 1) {
                    button.state |= QStyle::State_Sunken;
                }
            }

            QWidget *pWidget = (i == 0) ? m_pEditButton.data() : m_pDeleteButton.data();
            pWidget->style()->drawControl(QStyle::CE_PushButton, &button, painter, pWidget);
        }
    }
}

bool UserManagementDeleget::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    QRect decorationRect = option.rect;

    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if (event->type() == QEvent::MouseButtonPress && decorationRect.contains(mouseEvent->pos())) {
        if (index.column() == (int)UserManagementModel::Column::CheckBox) {
            bool data = model->data(index, Qt::UserRole).toBool();
            model->setData(index, !data, Qt::UserRole);
            qDebug() <<" model->setData(index, !data, Qt::UserRole);";
        }
    }

    if (index.column() == (int)UserManagementModel::Column::Edit) {
        m_nType = -1;
        bool bRepaint = false;
        QMouseEvent *pEvent = static_cast<QMouseEvent *> (event);
        m_mousePoint = pEvent->pos();

        int nCount = m_list.count();
        int nHalf = (option.rect.width() - m_nWidth * nCount - m_nSpacing * (nCount - 1)) / 2;
        int nTop = (option.rect.height() - m_nHeight) / 2;

        // 还原鼠标样式
        QApplication::restoreOverrideCursor();

        for (int i = 0; i < nCount; ++i)
        {
            QStyleOptionButton button;
            button.rect = QRect(option.rect.left() + nHalf + m_nWidth * i + m_nSpacing * i,
                                option.rect.top() + nTop,  m_nWidth, m_nHeight);
            // 鼠标位于按钮之上
            if (!button.rect.contains(m_mousePoint))
                continue;

            bRepaint = true;
            switch (event->type())
            {
            // 鼠标滑过
            case QEvent::MouseMove:
            {
                // 设置鼠标样式为手型
                QApplication::setOverrideCursor(Qt::PointingHandCursor);

                m_nType = 0;
                QToolTip::showText(pEvent->globalPos(), m_list.at(i));
                break;
            }
                // 鼠标按下
            case QEvent::MouseButtonPress:
            {
                m_nType = 1;
                break;
            }
                // 鼠标释放
            case QEvent::MouseButtonRelease:
            {
                if (i == 0) {
                    emit edit(index);
                } else {
                    emit deleteData(index);
                }
                break;
            }
            default:
                break;
            }
        }

        return bRepaint;
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QWidget *UserManagementDeleget::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(parent);
    Q_UNUSED(option);
    Q_UNUSED(index);
    return 0;
}

void UserManagementDeleget::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    Q_UNUSED(editor);
    Q_UNUSED(index);
}

void UserManagementDeleget::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    Q_UNUSED(editor);
    Q_UNUSED(model);
    Q_UNUSED(index);
}
