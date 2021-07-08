#ifndef CUSTOMDELEGATE_H
#define CUSTOMDELEGATE_H

#include<QStyledItemDelegate>
#include<QTableView>
#include<QtWidgets>

class CustomDelegate : public QStyledItemDelegate
{
public:
    CustomDelegate(QTableView* tableView);
protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
private:
    QPen _gridPen;
};

#endif // CUSTOMDELEGATE_H
