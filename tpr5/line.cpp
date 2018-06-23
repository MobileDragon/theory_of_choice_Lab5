#include "line.h"

Line::Line(int lineId)
{
    this->lineId=lineId;
    start.setX(10);
    start.setY(10);
    end.setX(60);
    end.setY(60);
    dTextLineX=0;
    dTextLineY=0;
    startN=0;
    endN=0;
    rPoint=6;
    arrowSize=12;
    arrowAngle=20;//радус отклонения от прямой
    activate=0;
    dClicX=0;
    dClicY=0;
    name="линия";
    heightTextBlock=0;
    widthTextBlock=0;
    nodeHover=nullptr;
    updatePosText();
    updateArrow();
}
//---------------------------------------------------------------------
QPoint Line::getTopLeftPos()
{
    QPoint minPos;
    minPos.setX(posText.x());
    minPos.setY(posText.y()-heightTextBlock);

    if(start.x()-rPoint < minPos.x() )
        minPos.setX( start.x()-rPoint );
    if(start.y()-rPoint < minPos.y() )
            minPos.setY( start.y()-rPoint );
    //
    if(end.x()-rPoint < minPos.x() )
        minPos.setX( end.x()-rPoint );
    if(end.y()-rPoint < minPos.y() )
            minPos.setY( end.y()-rPoint );
    return minPos;
}
QPoint Line::getBottomRighPos()
{
    QPoint maxPos;
    maxPos.setX(posText.x() + widthTextBlock);
    maxPos.setY(posText.y());

    if(start.x()-rPoint > maxPos.x() )
        maxPos.setX( start.x()-rPoint );
    if(start.y()-rPoint > maxPos.y() )
            maxPos.setY( start.y()-rPoint );
    //
    if(end.x()-rPoint > maxPos.x() )
        maxPos.setX( end.x()-rPoint );
    if(end.y()-rPoint > maxPos.y() )
            maxPos.setY( end.y()-rPoint );
    return maxPos;
}
void Line::changePos(int dWidth, int dHeight)
{
    if(startN == nullptr )
    {
        start.setX( start.x() + dWidth );
        start.setY( start.y() + dHeight );
    }
    if(endN == nullptr )
    {
        end.setX( end.x() + dWidth );
        end.setY( end.y() + dHeight );
    }
}
//---------------------------------------------------------------------

void Line::setPos(QPoint pos)
{
    activate=2;
    dClicX=0;
    dClicY=0;
    move(pos, nullptr);
    activate=0;
}

void Line::setStartNode(Node* node)
{
    startN = node;
    update();
}

void Line::setEndNode(Node* node)
{
    endN = node;
    update();
}

//------------------------
QPoint Line::getPosStart()
{
    return(start);
}
QPoint Line::getPosEnd()
{
    return(end);
}
QPoint Line::getPosText()
{
    return posText;
}
void Line::setStartPos(QPoint pos)
{
    start = pos;
}
void Line::setEndPos(QPoint pos)
{
    end = pos;
}
void Line::setTextPos(QPoint pos)
{
    posText = pos;
    dTextLineX = pos.x() - ( (start.x()+end.x()) / 2 );
    dTextLineY = pos.y() - ( (start.y()+end.y()) / 2 );
}
//------------------------

int Line::getId()
{
    return lineId;
}

bool Line::nodeConnect()//определяет подключать необходимо начало/конец линии по значению int activate
{
    if(activate == 1)//выбрано начало
    {
        if(nodeHover == nullptr || endN != nodeHover)
            startN = nodeHover;
        return(true);
    }
    if(activate == 3)//выбран конец
    {
        if(nodeHover == nullptr || startN != nodeHover)
            endN = nodeHover;
        return(true);
    }
    return(false);
}
void Line::nodeDisconnect()
{
    nodeHover=nullptr;
    startN=nullptr;
    endN=nullptr;
}

void Line::update()//обновляет точки привязанные по узлам, если таковые имеются
{
    if(startN!=0)
    {
        if(startN->getPosOut().x() > 0 )
            start=startN->getPosOut();
        else
            startN=nullptr;
    }
    if(endN!=0)
    {
        if(endN->getPosIn().x() > 0 )
            end=endN->getPosIn();
        else
            endN=nullptr;
    }
    updateArrow();
}

QString Line::getText()//надпись возле линии
{
    return(name);
}

void Line::setText(QString name)
{
    this->name=name;
}
void Line::setActivate(bool light)//включает/выключает активицию всей линии(2)//для дезактивации части тоже
{
    if(light)
        activate = 2;
    else
        activate = 0;
}

void Line::setMark(bool mark)
{
    if(mark)
    {
        activate=7;
        return;
    }
    if(activate==7)
        activate=0;
}

void Line::setTextActivate(bool textSelect)
{
    if(textSelect)
    {
        activate = 4;
        return;
    }
    if(this->isTextActivate() )
    {
        activate = 0;
    }
}

bool Line::isAcivate()
{
    if(activate!=0)
        return true;
    return false;
}

bool Line::isTextActivate()
{
    if(activate == 4)
        return true;
    return false;
}

IdData Line::move(QPoint pos, Node* underPos)//перемещает активированую часть
{
    switch (activate)
    {
    case 1://начало
    {
        start=pos;
        prepareNode(underPos);
        nodeConnect();
        break;
    }
    case 2://вся линия
    {
        nodeDisconnect();
        //eeeeeeeeeeeeeeeeee
        //qDebug() <<QString("dClicX %1").arg(dClicX);
        //qDebug() <<QString("dClicY %1").arg(dClicY);
        int dx = end.x()-start.x();//разница между началом и концом линии
        int dy = end.y()-start.y();

        start.setX( pos.x()+dClicX );
        start.setY( pos.y()+dClicY );
        end.setX( start.x()+dx );
        end.setY( start.y()+dy );


        break;
    }
    case 3://конец
    {
        end=pos;
        prepareNode(underPos);
        nodeConnect();
        break;
    }
    case 4://текст
    {
        dTextLineX = pos.x() + dClicX - ( (start.x()+end.x()) / 2 );
        dTextLineY = pos.y() + dClicY - ( (start.y()+end.y()) / 2 );
        break;
    }
    default:
    {
    }
    }
    updatePosText();
    return getIdData();

}

IdData Line::getIdData()
{
    IdData result;
    if(startN != nullptr)
        result.nodeStart = startN->getId();
    else
        result.nodeStart = -1;
    if(endN != nullptr)
        result.nodeEnd = endN->getId();
    else
        result.nodeEnd = -1;

    return result;
}

void Line::draw(QPainter *p)
{
    update();//
    switch (activate)
    {
    case 0:
    {
        p->setPen(QPen(Qt::black,2,Qt::SolidLine));
        p->drawLine(start, end);
        drawArrow(p);
        drawText(p);
        return;
    }
    case 1:
    {
        p->setPen(QPen(Qt::black,2,Qt::SolidLine));
        p->drawLine(start, end);
        drawText(p);
        p->setPen(QPen(Qt::red,1,Qt::SolidLine));
        p->setBrush(QBrush(Qt::red) );
        p->drawEllipse(start,rPoint,rPoint);
        drawArrow(p);
        return;
    }
    case 2:
    {
        p->setPen(QPen(Qt::red,2,Qt::SolidLine));
        p->drawLine(start, end);
        drawArrow(p, true);
        drawText(p, true);
        return;
    }
    case 3:
    {
        p->setPen(QPen(Qt::red,1,Qt::SolidLine));
        p->setBrush(QBrush(Qt::red) );
        p->drawEllipse(end,rPoint,rPoint);
        p->setPen(QPen(Qt::black,2,Qt::SolidLine));
        p->drawLine(start, end);
        drawText(p);
        drawArrow(p);
        return;
    }
    case 4:
    {
        p->setPen(QPen(Qt::black,2,Qt::SolidLine));
        p->drawLine(start, end);
        drawArrow(p);
        drawText(p, true);
        return;
    }
    case 7:
    {
        p->setPen(QPen(Qt::blue,2,Qt::SolidLine));
        p->drawLine(start, end);
        drawArrow(p, true);
        drawText(p, true);
        return;
    }
    default:
    {
    }

    }

/*
    p->drawLine(start, end);


    qDebug() <<QString("fX %1").arg(start.x());
    qDebug() <<QString("fY %1").arg(start.y());

    QPoint center;
    center.setX( (start.x()+end.x()) / 2 );
    center.setY( (start.y()+end.y()) / 2 );
    //p->drawText(center, name);
    updatePosText();
    drawText(p);*/
}
bool Line::isOnclic(QPoint pos, bool setActiv)//производит активацию определенной части(2,1,3,0)//и только здесь
{//определяется расстояние от точки нажатия до прямой, если попало, то устанавливается смешение относительно старта

    //проверка на активацию текста
    if(pos.x() >= posText.x() && pos.x() <= widthTextBlock+posText.x() &&
            pos.y() <= posText.y() && pos.y() >= posText.y() - heightTextBlock )
    {
        qDebug()<<"текст активирован!";
        if(setActiv)
            activate=4;//текст
        dClicX=posText.x()-pos.x();
        dClicY=posText.y()-pos.y();

        return(true);
    }

    //qDebug() <<QString("d444444444ClicX");
    //проверка расстояния от центра начала до точки
    int dS = qSqrt( qPow( pos.x() - start.x(), 2 ) + qPow( pos.y() - start.y(), 2 ) );
    if( dS<=rPoint )
    {
        if(setActiv)
            activate=1;//начало
        return(true);
    }
    //проверка расстояния от центра конца линии до точки нажатия мыши
    int dF = qSqrt( qPow( pos.x() - end.x(), 2 ) + qPow( pos.y() - end.y(), 2 ) );
    if( dF<=rPoint )
    {
        if(setActiv)
            activate=3;//конец
        return(true);
    }
    //проверка на активацию всей линии
    int a = start.y() - end.y();
    int b = end.x() - start.x();
    int c = start.x() * end.y() - end.x() * start.y();
    //ближайшая точка на прямой имеет следующие координаты
    int onlineX = (b * ( b * pos.x() - a * pos.y() ) - a * c) / (qPow(a, 2) + qPow(b, 2));
    int onlineY = (a * ( -1 * b * pos.x() + a * pos.y() ) - b * c) / (qPow(a, 2) + qPow(b, 2));
    //проверка на расстояние от точки нажатия до ближайщей точки на линии
    int dL = qSqrt( qPow( pos.x() - onlineX, 2 ) + qPow( pos.y() - onlineY, 2 ) );//выход за границы int
    if( dL>rPoint )
        return false;
    if( ( start.x()-onlineX >= 0 && end.x()-onlineX <=0 ) || ( start.x()-onlineX <= 0 && end.x()-onlineX >=0 ) )
    {//
        if( ( start.y()-onlineY >= 0 && end.y()-onlineY <=0 ) || ( start.y()-onlineY <= 0 && end.y()-onlineY >=0 ) )
        {
            if(setActiv)
                activate=2;//вся
            //подсчет смещения нажатия относительно начала линии
            dClicX=start.x()-pos.x();
            dClicY=start.y()-pos.y();

    return true;
        }
    }
    return(false);
}

void Line::drawText(QPainter *p, bool select)
{
    updatePosText();
    int hChar=QFontMetrics(p->font()).boundingRect(QRect(),//высота строки
                                                Qt::AlignLeft,//не помогает в моем случае
                                                QString("Example") ).height();
    widthTextBlock = 2 + QFontMetrics(p->font()).boundingRect(QRect(),//высота строки
                                                   Qt::AlignLeft,//не помогает в моем случае
                                                   name ).width();
    heightTextBlock = hChar+2;

    QBrush brush(Qt::white);
    p->setPen(QPen(Qt::black,1,Qt::SolidLine));
    p->fillRect(posText.x()-1, posText.y()-hChar-1 ,widthTextBlock, hChar+2, brush);
    if(activate == 7)
        p->setPen(QPen(Qt::blue,2,Qt::SolidLine));
    else
        if(select)
            p->setPen(QPen(Qt::red,1,Qt::SolidLine));

    p->drawText(posText, name);

}

void Line::updatePosText()
{//отталкивается от смещеня текста относительно центра линии
    posText.setX( (start.x()+end.x()) / 2 + dTextLineX);
    posText.setY( (start.y()+end.y()) / 2 + dTextLineY);
}

void Line::updateArrow()
{//в компьютерной граффике ргадусы разбиваются по часовой стрелке
    qreal angl=0;
    if( end.x() > start.x() )
    {
        angl = 180/M_PI * qAtan( (qreal)( start.y()-end.y() ) / ( start.x()-end.x() )  );
        if( end.y() > start.y() )
            angl = angl;
        else
            angl = angl+360;
    }
    else
        if( end.x() < start.x() )
        {
            angl = 180/M_PI * qAtan( (qreal)( start.y()-end.y() ) / ( start.x()-end.x() )  );
            angl = angl+180;
        }
        else
        {
            if(start.y() > end.y())
                angl=270;
            else
                angl=90;
        }
    qDebug()<<angl;
    arrowLeft.setX( qCos( (angl + 180 - arrowAngle) * M_PI / 180 ) * arrowSize + end.x() );
    arrowLeft.setY( qSin( (angl + 180 - arrowAngle) * M_PI / 180 ) * arrowSize + end.y() );
    arrowRight.setX( qCos( (angl + 180 + arrowAngle) * M_PI / 180 ) * arrowSize + end.x() );
    arrowRight.setY( qSin( (angl + 180 + arrowAngle) * M_PI / 180 ) * arrowSize + end.y() );

}

void Line::drawArrow(QPainter *p, bool select)
{
    if(!select)
        p->setPen(QPen(Qt::black,2,Qt::SolidLine));
    else
        if(activate == 7)
            p->setPen(QPen(Qt::blue,2,Qt::SolidLine));
        else
            p->setPen(QPen(Qt::red,2,Qt::SolidLine));
    p->drawLine(end, arrowLeft);
    p->drawLine(end, arrowRight);
}
//

void Line::prepareNode(Node *node)
{//подготавливает все на случай отпуска мыши// и ее наведении/отведения над узлом
    if(nodeHover != node)//если наведенный узел раньше, и сейчас не совпадают
    {
        if(nodeHover != nullptr)
            nodeHover->setPointLight(0);
        if(node != nullptr)
        {
            if(activate == 3)//конец линии(стрелка)
            {
                node->setPointLight(1);
                nodeHover = node;
                return;
            }
            if(activate == 1)
            {
                if(node->setPointLight(2))//начало линии
                    nodeHover = node;
                else
                    nodeHover = nullptr;
                return;
            }
        }
        else
            nodeHover = nullptr;

    }
}

