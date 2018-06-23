#include "buttonforquest.h"

ButtonForQuest::ButtonForQuest(int num, QString name, QWidget *parent) : QWidget(parent)
{
    setValues(num, name);
    border=3;
    this->setMinimumWidth(210);
    //this->setFixedSize(210,60);
    //this->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    this->update();
}

ButtonForQuest::~ButtonForQuest()
{
    foreach (DrawingString *drawString, drawStringData)
    {
        delete(drawString);//очистка предыдущих данных
    }
    drawStringData.clear();
}

void ButtonForQuest::setValues(int num, QString name)
{
    this->num = num;
    this->name = name;
    //this->width();//считывается и устанавливаются переносы
    //this->height();ширина устанавливается фиксированой в зависимости от обьема name
}

//
void ButtonForQuest::build(QPainter *p)
{

    textWidth = this->width() - border*2;
    textHeight = updateRows(p);
    this->setFixedHeight(textHeight + border*2);
}

int ButtonForQuest::updateRows(QPainter *p)
{//возвращает высоту необходимую для вывода всего текста
    //если слово длинее ширины виджета, оно выводится все в одну строку (сколько влезит)
    foreach (DrawingString *drawString, drawStringData)
    {
        delete(drawString);//очистка предыдущих данных
    }
    drawStringData.clear();

    int hChar=QFontMetrics(p->font()).boundingRect(QRect(),//высота строки
                                                Qt::AlignLeft,//не помогает в моем случае
                                                QString("Example") ).height();

    /*
    if(w > textWidth)
    {
        DrawingString *tempDS = new DrawingString(QPoint( border, border + hChar),name );
        drawStringData.append(tempDS);
        return(hChar);
    }*/

    QStringList words = name.split(QRegExp("\\s+"));//РаЗБИТИЕ СТРОКИ НА СЛОВА

    QString tSring = "";

    QStringList::Iterator t = words.begin();
    tSring.append(*t);
    ++t;
    int numString = 1;//номер строки добавленной в drawStringData
    int width;
    while(t != words.end())
    {
        QString temp = tSring+" "+*t;
        width = QFontMetrics(p->font() ).boundingRect(QRect(),//длина всей строки
                                                       Qt::AlignLeft,//не помогает в моем случае
                                                       temp ).width();
        if(width < textWidth)
        {
            tSring = temp;
        }
        else
        {
            width = QFontMetrics(p->font() ).boundingRect(QRect(),//длина всей строки
                                                           Qt::AlignLeft,//не помогает в моем случае
                                                           tSring ).width();
            DrawingString *temp2 = new DrawingString(QPoint( border, border + numString*hChar),tSring, width);
            drawStringData.append(temp2);
            tSring = *t;
            ++numString;
        }

        ++t;
    }
    width = QFontMetrics(p->font() ).boundingRect(QRect(),//длина всей строки
                                                   Qt::AlignLeft,//не помогает в моем случае
                                                   tSring ).width();
    DrawingString *temp3 = new DrawingString(QPoint( border, border + numString*hChar),tSring, width);
    drawStringData.append(temp3);
    //++numString;

    return(numString * hChar);
}

void ButtonForQuest::drawTitle(QPainter *p)
{
    if(!hover)
        p->setPen(QPen(Qt::black, 1,Qt::SolidLine));
    else
        p->setPen(QPen(Qt::darkGreen, 1,Qt::SolidLine));

    foreach (DrawingString *var, drawStringData)
    {
        p->drawText(var->pos.x(), var->pos.y(), var->s);
        if(hover)
            p->drawLine(var->pos.x(), var->pos.y()+1, var->pos.x()+var->maxWidth, var->pos.y()+1);
    }
    //p->drawText(drawStringData. ->pos, drawStringData[i]->s);
}


void ButtonForQuest::mousePressEvent(QMouseEvent *event)
{//посылает свой идентификационный номер при нажатии
    if (event->button() != Qt::LeftButton &&
            event->button() != Qt::RightButton)
        return;
    emit buttonReleased(num);
}
void ButtonForQuest::enterEvent(QEvent *e)
{//включается подсветка возможно подчеркивание..
    hover = true;
    this->update();
}
void ButtonForQuest::leaveEvent(QEvent *e)
{//выключается подсветка
    hover = false;
    this->update();
}


void ButtonForQuest::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    setFont(QFont("Arial", 10, 50, 0));

    build(&painter);
    drawTitle(&painter);
}
