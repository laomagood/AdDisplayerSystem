#ifndef CanvasLayout_H
#define CanvasLayout_H

#include <QLayout>

class CanvasLayout : public QLayout
{
public:
    explicit CanvasLayout(QWidget *parent = 0);
    ~CanvasLayout();

    void addItem(QLayoutItem *item);
    int count() const;
    QLayoutItem *itemAt(int index) const;
    QLayoutItem *takeAt(int index);
    QSize sizeHint() const;

private:
    QList<QLayoutItem*> list;
};

#endif // CanvasLayout_H
