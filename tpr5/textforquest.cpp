#include "textforquest.h"

TextForQuest::TextForQuest(QString text, QWidget *parent) : QWidget(parent)
{
    setText(text);
    border=3;
    this->setMinimumWidth(210);
    this->update();
}

TextForQuest::~TextForQuest()
{
    foreach (DrawingString *drawString, drawStringData)
    {
        delete(drawString);//очистка предыдущих данных
    }
    drawStringData.clear();
}

void TextForQuest::setText(QString text)
{
     this->text = text;
}

void TextForQuest::build(QPainter *p)
{

    textWidth = this->width() - border*2;
    textHeight = updateRows(p);
    this->setFixedHeight(textHeight + border*2);
}

int TextForQuest::updateRows(QPainter *p)
{
    //возвращает высоту необходимую для вывода всего текста
    //если слово длинее ширины виджета, оно выводится все в одну строку (сколько влезит)
    foreach (DrawingString *drawString, drawStringData)
    {
        delete(drawString);//очистка предыдущих данных
    }
    drawStringData.clear();
    int hChar=QFontMetrics(p->font()).boundingRect(QRect(),//высота строки
                                                Qt::AlignLeft,//не помогает в моем случае
                                                QString("Example") ).height();

    QStringList words = text.split(QRegExp("\\s+"));//РаЗБИТИЕ СТРОКИ НА СЛОВА

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

    return(numString * hChar);
}

void TextForQuest::drawText(QPainter *p)
{
    p->setPen(QPen(Qt::black));
    setFont(QFont("Arial", 10, 50, 0));

    foreach (DrawingString *var, drawStringData)
    {
        p->drawText(var->pos.x(), var->pos.y(), var->s);
    }
}

void TextForQuest::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    build(&painter);
    drawText(&painter);
}
