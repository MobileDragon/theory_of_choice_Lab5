#ifndef DRAWSTRING_H
#define DRAWSTRING_H

struct DrawingString
{
    DrawingString(QPoint pos, QString s = "", int width = 0)
    {
        this->pos.setX(pos.x());
        this->pos.setY(pos.y());
        maxWidth=width;
        this->s = s;
    }
    QPoint pos;
    int maxWidth;
    QString s;

};

#endif // DRAWSTRING_H
