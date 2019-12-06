#include "CanvasLayout.h"

CanvasLayout::CanvasLayout(QWidget *parent) : QLayout(parent)
{

}

CanvasLayout::~CanvasLayout()
{
    QLayoutItem *item;
    while ((item = takeAt(0)))
        delete item;
}

void CanvasLayout::addItem(QLayoutItem *item)
{
    list.append(item);
}

int CanvasLayout::count() const
{
    return list.size();
}

QLayoutItem *CanvasLayout::itemAt(int index) const
{
    return list.value(index);
}

QLayoutItem *CanvasLayout::takeAt(int index)
{
    return index >= 0 && index < list.size() ? list.takeAt(index) : 0;
}

QSize CanvasLayout::sizeHint() const
{

}
