#ifndef MYSQUER_H
#define MYSQUER_H
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QString>
using namespace std;
struct block {
    //default name = "Hole";
    string name;
    int size;
    int start;
    int end;
};
class MySquer :public QGraphicsItem
{

public:

    QString name_squ;
    int start , size;
    MySquer();
    MySquer(block ne,bool hole);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    bool Pressed;
};

#endif // MYSQUER_H
