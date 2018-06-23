#ifndef LINE_H
#define LINE_H
#include <QPoint>
#include <QPainter>
#include <node.h>
#include <QtMath>
#include <QDebug>

struct IdData
{
    //IdData();
    //IdData(int start, int end, int line): nodeStart(start), nodeEnd(end), line(line) {}
    int nodeStart;
    int nodeEnd;
};

class Line
{
public:
    Line(int lineId);
    /**
     * при перетаскивании конца линии по вджету, виджет смотрит, нет ли там
     * узла, если так то узел активируется, на месте стыка линия рисует точку зеленого цвета(вместо красной)
     * линия по умолчанию красная, активированая она зеленая
     *
     */
    QPoint getTopLeftPos();//размеры рисунка для изменения размера окна
    QPoint getBottomRighPos();//размеры рисунка для изменения размера окна
    void changePos(int dWidth, int dHeight);//изменяется позиция устанавливается только для незафиксированых сторон линий


    void setPos(QPoint pos);
    void setStartNode(Node* node);
    void setEndNode(Node* node);
    //------------------------
    QPoint getPosStart();//-
    QPoint getPosEnd();//-
    QPoint getPosText();//-
    void setStartPos(QPoint pos);//отменять привязку к узлам
    void setEndPos(QPoint pos);//отменять привязку к узлам
    void setTextPos(QPoint pos);
    //------------------------
    int getId();

    bool nodeConnect();//определяет подключать необходимо начало/конец линии по значению int activate
    void nodeDisconnect();//полное отключение всех узлов
    void update();//обновляет точки привязанные по узлам, если таковые имеются
    QString getText();//надпись возле линии
    void setText(QString name);
    void setActivate(bool light);//включает/выключает активицию всей линии(2)//для дезактивации части тоже
    void setMark(bool mark);
    void setTextActivate(bool textSelect);//вызывается при переименовании
    bool isAcivate();//возвращает true, если линия частично, или полностью активирована
    bool isTextActivate();//
    IdData move(QPoint pos, Node *underPos = nullptr);//перемещает активированую часть
    //возвращает структуру хранящую id узла к которому подключео начало , id узла к которому подключен конец, свой id
    IdData getIdData();
    void draw(QPainter *p);
    bool isOnclic(QPoint pos, bool setActiv = true);//производит активацию определенной части(2,1,3,0)//и только здесь


protected:
    QPoint start;//расположения изображения на плоскости формируется по началу линии
    QPoint end;
    int lineId;
    QPoint posText;//позиция отрисовки текста
    int dClicX;//смещение позиции нажетия на линию относительно ее начала по X//когда выделена вся линия
    int dClicY;
    //
    int dTextLineX;//смещение текста относительно начала линии
    int dTextLineY;

    Node *startN;//указатели для установки позиций в случае перемешения узлов
    Node *endN;//

    int width;
    int height;
    int rPoint;//радиус точек начала и конца линиии для активации
    int arrowSize;
    int arrowAngle;
    QPoint arrowLeft;
    QPoint arrowRight;
    int activate;//отмечает активирована вся линия(2) ее начало(1) конец(3) не активирована(0)
                    //(4) текст линии //для отображения
    void drawText(QPainter *p, bool select = false);
    void updatePosText();
    void drawArrow(QPainter *p, bool select = false);
    void updateArrow();
    void prepareNode(Node *node);
    Node* nodeHover;//узел к которому может подключиться линия в случае отпуска миши
    int widthTextBlock;
    int heightTextBlock;
    QString name;
};

#endif // LINE_H
