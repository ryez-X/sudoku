#include "customdelegate.h"

CustomDelegate::CustomDelegate(QTableView* tableView)
{
    // create grid pen
    int gridHint = tableView->style()->styleHint(QStyle::SH_Table_GridLineColor, new QStyleOptionViewItem());
    QColor gridColor = static_cast<QRgb>(gridHint);
    _gridPen = QPen(gridColor, 0, tableView->gridStyle());
    _gridPen.setWidth(10);
    _gridPen.setColor(QColor("#CDCDB4"));
}

void CustomDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem  viewOption(option);
    //高亮显示与普通显示时的前景色一致（即选中行和为选中时候的文字颜色一样）
    viewOption.palette.setColor(QPalette::HighlightedText,index.data(Qt::ForegroundRole).value<QColor>());
    viewOption.palette.setColor(QPalette::Highlight,index.data(Qt::BackgroundRole).value<QColor>());
    QStyledItemDelegate::paint(painter, viewOption, index);

    QPen oldPen = painter->pen();

    QPen newPen;
    newPen.setWidth(2);
    newPen.setColor(QColor("#D2B48C"));

    // paint vertical lines
    if (index.row()==2||index.row()==5)
    {
        painter->setPen(_gridPen);
        painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
    }
    else if(index.row()!=8)
    {
        painter->setPen(newPen);
        painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
    }
    // paint horizontal lines
    if(index.column()==2||index.column()==5)
    {
        painter->setPen(_gridPen);
        painter->drawLine(option.rect.topRight(), option.rect.bottomRight());
    }
    else if(index.column()!=8)
    {
        painter->setPen(newPen);
        painter->drawLine(option.rect.topRight(), option.rect.bottomRight());
    }

    painter->setPen(oldPen);
}
