#ifndef NODE_H
#define NODE_H
#include <QWidget>
#include <QPoint>
#include <QPainter>
#include <QtMath>
#include <QDebug>
#include <QFont>
#include <QVector>

struct ellipseString
{
    ellipseString(QPoint pos)
    {
        this->pos.setX(pos.x());
        this->pos.setY(pos.y());
        maxWidth=0;
        s="";
    }
    QPoint pos;
    int maxWidth;
    QString s;


};

class Node
{
public:
    Node(int nodeId);
    QPoint getTopLeftPos();//размеры рисунка для изменения размера окна
    QPoint getBottomRighPos();//размеры рисунка для изменения размера окна
    //, а еще для легкого определения - полностью ли попадает рисунок в выделенную область
    void changePos(int dWidth, int dHeight);


    QPoint getPos();
    QPoint getPosIn();//точка для подключения входной линии(конец линии)
    QPoint getPosOut();//точка для подключения выходной линии(начало линии)
    int getId();
    int getWidth();
    int getHeight();
    QString getText();
    bool isActivate();
    bool isTextActivate();//
    void setWidth(int width);
    void setHeight(int height);
    void setPos(QPoint pos);
    void setTextLeaf(QString name);
    void setTextNode(QString name);
    void setActivate(bool light);//полная активация/дезактивация
    void setMark(bool mark);
    bool setPointLight(int num);
    void setTextActivate(bool textSelect);//вызывается при переименовании
    void move(QPoint pos);
    void draw(QPainter *p);
    bool isOnclic(QPoint pos, bool setActiv = true);
    bool isNode();
    bool isLeaf();
protected:
    QPoint position;//оасположения изображения на плоскости
    QVector< QPoint > resizePoint;
    int nodeId;
    int width;
    int height;
    int activate;//0) не активировано 1) активировн весь объект 2)активирован текст (3,4,5,6)point resize
    int pointLigth;//0) не подсвечивать точки 1)точка входа светится 2)точка выхода светится
    int dClicX;//смещение нажатия на узел относительно его позиции
    int dClicY;
    QString name;
    bool theNode;//определяет узел это или лист

private:
    void updatePoints();
    void update_dClic(QPoint pos);//просчет разницы отрисовки относительно координат
    void drawPointResize(QPainter *p);
    bool drawText(QPainter *p);//отображает текст точно внутри овала, либо увеличивает его меньшую сторону
                               //возвращает true, если пришлось изменить размер
    void drawPointIn(QPainter *p);
    void drawPointOut(QPainter *p);
    void drawPoint(QPainter *p ,QPoint pos);
    QString cutTextWidth(QString &text, int widthText, QPainter *p);//откусывает от text строку определенной длины
                                                           //в пикселях и возвращает ее
};

#endif // NODE_H
