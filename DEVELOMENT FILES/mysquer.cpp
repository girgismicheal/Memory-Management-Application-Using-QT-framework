#include "mysquer.h"
MySquer::MySquer(){}
MySquer::MySquer(block ne,bool hole)
{
    name_squ= QString::fromStdString(ne.name);
    size=ne.size;
    start=ne.start;
    Pressed=hole;

    //setFlag(ItemIsMovable);
}

QRectF MySquer::boundingRect() const
{
    return QRectF(0,start,100,size);
}

void MySquer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec =boundingRect();
    QBrush brush (Qt::blue);
    if (!Pressed){
        brush.setColor(Qt::red);
    }
    else{
        brush.setColor(Qt::green);
    }
    painter->fillRect(rec,brush);
    painter->drawRect(rec);
}

//void MySquer::mousePressEvent(QGraphicsSceneMouseEvent *event)
//{
//    Pressed =true ;
//    update();
//    QGraphicsItem::mousePressEvent(event);
//}
//void MySquer::paint_line(int x){
//    x/size;
//    QPainter p;
//    p.drawLine(QLine(10,10,10,120));
//}
//void MySquer::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
//{
//    Pressed =false ;
//    update();
//    QGraphicsItem::mouseReleaseEvent(event);

//}
