#include "node.h"

Node::Node(int nodeId)
{
    this->nodeId=nodeId;
    position.setX(10);
    position.setY(10);
    width=60;
    height=35;
    name="узел";
    dClicX=0;
    dClicY=0;
    activate=0;
    pointLigth=0;
    theNode=true;
    updatePoints();
}
//---------------------------------------------------------------------
QPoint Node::getTopLeftPos()
{
    QPoint minPos;
    minPos.setX( position.x()-5 );
    minPos.setY( position.y()-2 );
    return minPos;
}
QPoint Node::getBottomRighPos()
{
    QPoint maxPos;
    maxPos.setX( position.x()+width );
    maxPos.setY( position.y()+height+2 );
    return maxPos;
}
void Node::changePos(int dWidth, int dHeight)
{
    position.setX( position.x() + dWidth );
    position.setY( position.y() + dHeight );
}
//---------------------------------------------------------------------

QPoint Node::getPos()
{
    return(position);
}

QPoint Node::getPosIn()//точка для подключения входной линии(конец линии)
{//верхняя точка элипса
    QPoint posIN;
    posIN.setX( position.x() + width/2 );
    posIN.setY( position.y() - 1 );
    return(posIN);
}
QPoint Node::getPosOut()//точка для подключения выходной линии(начало линии)
{
    if(!theNode)
        return(QPoint(-1,-1));
    QPoint posOUT;
    posOUT.setX( position.x() + width/2 );
    posOUT.setY( position.y() + height + 1 );
    return(posOUT);
}

int Node::getId()
{
    return nodeId;
}

int Node::getWidth()
{
    return(width);
}
int Node::getHeight()
{
    return(height);
}
QString Node::getText()
{
    return(name);
}

bool Node::isActivate()
{
    if(activate!=0)
        return true;
    return false;
}

bool Node::isTextActivate()
{
    if(activate == 2)
        return true;
    return false;
}

void Node::setWidth(int width)
{
    this->width=width;
}
void Node::setHeight(int height)
{
    this->height=height;
}
void Node::setPos(QPoint pos)
{
    position=pos;
}
void Node::setTextLeaf(QString name)
{
    this->name=name;
    theNode=false;
}

void Node::setTextNode(QString name)
{
    this->name=name;
    theNode=true;
}

void Node::setActivate(bool light)
{
    if(light)
        activate=1;
    else
        activate=0;
}
void Node::setMark(bool mark)
{
    if(mark)
    {
        activate=7;
        return;
    }
    if(activate==7)
        activate=0;
}

bool Node::setPointLight(int num)
{
    switch (num) {
    case 0://ничего
    {
        pointLigth = num;
        return true;
    }
    case 1://вход
    {
        pointLigth = num;
        return true;
    }
    case 2://выход
    {
        if(theNode)
        {
            pointLigth = num;
            return true;
        }
        return false;
    }
    default:
    {
        return false;
    }

    }
}

void Node::setTextActivate(bool textSelect)
{
    if(textSelect)
    {
        activate = 2;
        return;
    }
    if(this->isTextActivate() )
    {
        activate = 0;
    }
}

void Node::move(QPoint pos)
{
    switch (activate)
    {
    case 0:
    {
        break;
    }
    case 1:
    {
        position.setX( pos.x()+dClicX );
        position.setY( pos.y()+dClicY );
        break;
    }
    case 2://текст
    {
        break;
    }
    //
    case 3://верхняя левая
    {
        //изменение высоты и ширины
        int w = width + ((position.x()-dClicX)-pos.x());
        int h = height + ((position.y()-dClicY)-pos.y());
        if(w>10)
        {
            width=w;
            position.setX( pos.x()+dClicX );
        }
        if(h>10)
        {
            height=h;
            position.setY( pos.y()+dClicY );
        }
        break;
    }
    case 4://верхняя правая
    {
        //изменение высоты и ширины
        int w = width + (pos.x()-(position.x()-dClicX) );
        int h = height + ((position.y()-dClicY)-pos.y());

        if(w>10)
            width=w;
        if(h>10)
        {
            height=h;
            position.setY( pos.y()+dClicY );
        }
        break;
    }
    case 5://нижняя правая
    {
        //изменение высоты и ширины
        int w = width + (pos.x()-(position.x()-dClicX) );
        int h = height + (pos.y()-(position.y()-dClicY));
        if(w>10)
            width=w;
        if(h>10)
            height=h;
        break;
    }
    case 6://нижняя левая
    {
        //изменение высоты и ширины
        int w = width + ((position.x()-dClicX)-pos.x());
        int h = height + (pos.y()-(position.y()-dClicY));
        if(w>10)
        {
            width=w;
            position.setX( pos.x()+dClicX );
        }
        if(h>10)
            height=h;
        break;
    }
    default:
    {
        break;
    }
    }
    update_dClic(pos);
    updatePoints();
}

void Node::draw(QPainter *p)
{

    int w = QFontMetrics(p->font()).boundingRect(QRect(),//длина всей строки
                                                       Qt::AlignCenter,//не помогает в моем случае
                                                       name ).width();
    p->setBrush(QBrush(Qt::white) );
    switch (activate)
    {
    case 0:
    {
        if(theNode)
            p->setPen(QPen(Qt::black,2,Qt::SolidLine));
        else
            p->setPen(QPen(Qt::darkGreen,2,Qt::SolidLine));
        p->drawEllipse(position.x(),position.y(),width,height);
        p->setPen(QPen(Qt::black,2,Qt::SolidLine));
        drawText(p);
        break;
    }
    case 1:
    {
        drawPointResize(p);
        p->drawEllipse(position.x(),position.y(),width,height);
        drawText(p);
        break;
    }
    case 3:
    {
        drawPointResize(p);
        p->drawEllipse(position.x(),position.y(),width,height);
        drawText(p);
        break;
    }
    case 4:
    {
        drawPointResize(p);
        p->drawEllipse(position.x(),position.y(),width,height);
        drawText(p);
        break;
    }
    case 5:
    {
        drawPointResize(p);
        p->drawEllipse(position.x(),position.y(),width,height);
        drawText(p);
        break;
    }
    case 6:
    {
        drawPointResize(p);
        p->drawEllipse(position.x(),position.y(),width,height);
        drawText(p);
        break;
    }
    case 7:
    {
        p->setPen(QPen(Qt::blue,2,Qt::SolidLine));
        p->drawEllipse(position.x(),position.y(),width,height);
        drawText(p);
        break;
    }
    default:
    {
        break;
    }
    }

    /*switch (pointLigth)
    {
    case 1:
        drawPointIn(p);
        break;
    case 2:
        drawPointOut(p);
        break;
    default:
        break;
    }*/
}
//
bool Node::drawText(QPainter *p)
{
    bool error=true;
    int fullAccesWidth=0;

    int w = QFontMetrics(p->font() ).boundingRect(QRect(),//длина всей строки
                                                   Qt::AlignCenter,//не помогает в моем случае
                                                   name ).width();
    int rezerw = QFontMetrics(p->font()).boundingRect(QRect(),//запас по длине для каждой строки
                                                   Qt::AlignLeft,//не помогает в моем случае
                                                   QString("_") ).width();

    int hChar=QFontMetrics(p->font()).boundingRect(QRect(),//высота строки
                                                Qt::AlignCenter,//не помогает в моем случае
                                                QString("Example") ).height();
    //
    int a=width/2;//полуось
    int b=height/2;
    //позиция центра элипса
    QPoint posCenter;
    posCenter.setX(position.x()+a);
    posCenter.setY(position.y()+b);

    //уравнение элипса
    //float spot = qPow(pos.x()-posCenter.x() ,2)/qPow(a ,2) + qPow(pos.y()-posCenter.y() ,2)/qPow(b ,2);

    //ширина по центру высоты эллипса
    //int curWidth = qPow( (qPow(0-posCenter.y() ,2)/qPow(b ,2)-1)*qPow(a ,2) , 0.5 );
    //int curWidth = qPow( (1 - qPow(b-b ,2)/qPow(b ,2))*qPow(a ,2) , 0.5 );

    int curWidth=10;
    int halfWidth;

    int d=0;//величина смещения по y к краям элипса
    int k=1;//коэфициэнт определения нижней и верхней полуосей
    QVector<ellipseString*> drawStringData;
    while(d+hChar<b)
    {
        halfWidth = qPow( (1 - qPow(d+hChar ,2)/qPow(b ,2))*qPow(a ,2) , 0.5 );//поскольку
            //отрисовка символа идет с нижней части, а к вершине элипс ссужается. Поэтому
            //проверять необходимо на одну высоту строки выше
        fullAccesWidth += halfWidth*2-rezerw;
        ellipseString *eS = new ellipseString(QPoint( posCenter.x()-halfWidth, posCenter.y()-d) );
        eS->maxWidth=halfWidth*2;
        drawStringData.append(eS);
        if(fullAccesWidth >= w)
        {
            error=false;
            break;
        }


        d+=hChar;
        //
        halfWidth = qPow( (1 - qPow(d*-1 ,2)/qPow(b ,2))*qPow(a ,2) , 0.5 );
        fullAccesWidth += halfWidth*2-rezerw;
        eS = new ellipseString(QPoint( posCenter.x()-halfWidth+1, posCenter.y()+d) );
        eS->maxWidth=halfWidth*2-2;
        drawStringData.append(eS);
        if(fullAccesWidth >= w)
        {
            error=false;
            break;
        }
    }
    if(!error)
    {
        QString text=name;
        //for(int i=0; i<drawStringData.length(); i++)
        int iterator;
        int maxKol=drawStringData.length();
        if(maxKol%2 > 0)
            iterator=maxKol-1;
        else
            iterator=maxKol-2;
        int i = iterator;
        while(maxKol>0)
        {
            maxKol--;
            i = qAbs(iterator);
            drawStringData[i]->s = cutTextWidth(text, drawStringData[i]->maxWidth, p);
            p->drawText(drawStringData[i]->pos, drawStringData[i]->s);
            if(iterator != 0)
                iterator-=2;
            else
                iterator--;
        }
        //p->drawText(positionEllipse.x()+10, positionEllipse.y()+height/2, QString("мне кажется это сносно работает, а как ты считаешь? %1").arg(curWidth) );
    }
    return error;
}

void Node::drawPointIn(QPainter *p)
{
    p->setPen(QPen(Qt::red,2,Qt::SolidLine));
    drawPoint(p, getPosIn());
}

void Node::drawPointOut(QPainter *p)
{
    p->setPen(QPen(Qt::red,2,Qt::SolidLine));
    drawPoint(p, getPosOut());
}

void Node::drawPoint(QPainter *p ,QPoint pos)
{
    int size=7;
    QPoint posIn = getPosIn();
    QPoint point1;
    point1.setX( qCos( 315 * M_PI / 180 ) * size + pos.x() );
    point1.setY( qSin( 315 * M_PI / 180 ) * size + pos.y() );
    QPoint point2;
    point2.setX( qCos( 135 * M_PI / 180 ) * size + pos.x() );
    point2.setY( qSin( 135 * M_PI / 180 ) * size + pos.y() );
    p->drawLine(point1, point2);
    //
    QPoint point3;
    point3.setX( qCos( 225 * M_PI / 180 ) * size + pos.x() );
    point3.setY( qSin( 225 * M_PI / 180 ) * size + pos.y() );
    QPoint point4;
    point4.setX( qCos( 45 * M_PI / 180 ) * size + pos.x() );
    point4.setY( qSin( 45 * M_PI / 180 ) * size + pos.y() );
    p->drawLine(point3, point4);
}

QString Node::cutTextWidth(QString &text, int widthText, QPainter *p)
{
    int w;
    w = QFontMetrics(p->font()).boundingRect(QRect(),//длина всей строки
                                               Qt::AlignCenter,//не помогает в моем случае
                                               text ).width();
    QString curText;
    if(w <= widthText)
    {
        curText=text;
        text="";
        return curText;
    }

    int curLength=text.length();
    bool flag=false;
    int k;
    while(w > widthText)//просто убвляй длину посимвольно
    {
        curLength--;
        curText=text.left(curLength);

        w = QFontMetrics(p->font()).boundingRect(QRect(),//длина всей строки
                                                   Qt::AlignCenter,//не помогает в моем случае
                                                   curText ).width();

    }
    text=text.right(text.length() - curLength);
    return curText;

}
//
void Node::drawPointResize(QPainter *p)
{
    updatePoints();
    QBrush brush(Qt::black);
    p->setPen(QPen(Qt::black,1,Qt::SolidLine));
    p->fillRect(resizePoint[0].x(), resizePoint[0].y() ,5 ,5, brush);
    p->fillRect(resizePoint[1].x(), resizePoint[1].y() ,5 ,5, brush);
    p->fillRect(resizePoint[2].x(), resizePoint[2].y() ,5 ,5, brush);
    p->fillRect(resizePoint[3].x(), resizePoint[3].y() ,5 ,5, brush);
    p->setPen(QPen(Qt::red,2,Qt::SolidLine));
}

bool Node::isOnclic(QPoint pos, bool setActiv)
{
    //позиция овала отновительно точки нажатия
    update_dClic(pos);
    //
    int a=width/2;//полуось
    int b=height/2;
    //позиция центра элипса
    QPoint posCenter;
    posCenter.setX(position.x()+a);
    posCenter.setY(position.y()+b);

    //уравнение элипса
    float spot = qPow(pos.x()-posCenter.x() ,2)/qPow(a ,2) + qPow(pos.y()-posCenter.y() ,2)/qPow(b ,2);
    qDebug() <<QString("spot %1").arg(spot);
    qDebug()<<QString("point: %1").arg(activate);
    if(spot<=1)
    {
        if(setActiv)
            activate=1;
        return(true);
    }
    else
        if(activate>0)
        {//поиск нажатия на точки изменения размера
            qDebug()<<"point";
            if(pos.x() >= resizePoint[1].x() && pos.x() <= resizePoint[1].x()+5 )
            {
                if(pos.y() >=  resizePoint[2].y() && pos.y() <=  resizePoint[2].y()+5 )
                {
                    activate=5;
                    return(true);
                }
                if(pos.y() >=  resizePoint[1].y() && pos.y() <=  resizePoint[1].y()+5 )
                {
                    activate=4;
                    return(true);
                }
            }
            if(pos.x() >= resizePoint[0].x() && pos.x() <= resizePoint[0].x()+5 )
            {
                if(pos.y() >=  resizePoint[3].y() && pos.y() <=  resizePoint[3].y()+5 )
                {
                    activate=6;
                    return(true);
                }
                if(pos.y() >=  resizePoint[0].y() && pos.y() <=  resizePoint[0].y()+5 )
                {
                    activate=3;
                    return(true);
                }
            }
        }
    return(false);
}

bool Node::isNode()
{
    return(theNode);
}
bool Node::isLeaf()
{
    return(!theNode);
}

void Node::updatePoints()
{
    resizePoint.clear();
    resizePoint.append(QPoint( position.x()-5, position.y() ));
    resizePoint.append(QPoint( position.x()+width, position.y() ));
    resizePoint.append(QPoint( position.x()+width, position.y()+height-5 ));
    resizePoint.append(QPoint( position.x()-5, position.y()+height-5 ));
}

void Node::update_dClic(QPoint pos)
{
    dClicX = position.x()-pos.x();
    dClicY = position.y()-pos.y();
}
