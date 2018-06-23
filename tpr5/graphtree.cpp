#include "graphtree.h"

GraphTree::GraphTree(QWidget *parent) : QWidget(parent)
{
    countID=0;//нулевоо элемента нет в ссвязи оссобнностей добавления
    activNodeID=-1;
    activLineID=-1;

    buildMenu();
    dragMode=false;
    controlLock=false;
    this->setFixedSize(500,500);
}

GraphTree::~GraphTree()
{
    cleanBuffers();
    delete(menuForN);
    delete(menuForL);
}

void GraphTree::setLockingControl(bool lock)
{
    controlLock=lock;
}

//---------------------------------------------------------------------
void GraphTree::cutExtraSpace()//устанавливает размеры виджета подогнанными под содержимое
{
    QPoint minPos;
    QPoint maxPos;
    int minimallSpace = 10;//минимальное допкстимое расстояние до грниц виджета
    if(nBufer.size() > 0)
    {
        minPos = nBufer.first()->node->getTopLeftPos();
        maxPos = nBufer.first()->node->getBottomRighPos();
    }
    else
        if(lBufer.size() > 0)
        {
            minPos = lBufer.first()->line->getTopLeftPos();
            maxPos = lBufer.first()->line->getBottomRighPos();
        }
    foreach (int id, nBufer.keys())//узлы
    {
        QPoint topLeftPos = nBufer[id]->node->getTopLeftPos();
        QPoint bottomRighPos = nBufer[id]->node->getBottomRighPos();
        if( topLeftPos.x() < minPos.x() )
        {
            minPos.setX( topLeftPos.x() );
        }
        if( topLeftPos.y() < minPos.y() )
        {
            minPos.setY( topLeftPos.y() );
        }//
        if( bottomRighPos.x() > maxPos.x() )
        {
            maxPos.setX( bottomRighPos.x() );
        }
        if( bottomRighPos.y() > maxPos.y() )
        {
            maxPos.setY( bottomRighPos.y() );
        }
    }
    foreach (int id, lBufer.keys())//линии
    {
        QPoint topLeftPos = lBufer[id]->line->getTopLeftPos();
        QPoint bottomRighPos = lBufer[id]->line->getBottomRighPos();
        if( topLeftPos.x() < minPos.x() )
        {
            minPos.setX( topLeftPos.x() );
        }
        if( topLeftPos.y() < minPos.y() )
        {
            minPos.setY( topLeftPos.y() );
        }//
        if( bottomRighPos.x() > maxPos.x() )
        {
            maxPos.setX( bottomRighPos.x() );
        }
        if( bottomRighPos.y() > maxPos.y() )
        {
            maxPos.setY( bottomRighPos.y() );
        }
    }
    QSize size;
    size.setWidth(maxPos.x() - minPos.x() + minimallSpace*3);
    size.setHeight(maxPos.y() - minPos.y() + minimallSpace*3);
    //size.setWidth(100);
    //size.setHeight(200);
    //установка новых размеров виджета
    this->setFixedSize(size);
    //
    qDebug()<<"minPosX"<<minPos.x();
    qDebug()<<"minPosY"<<minPos.y();
    qDebug()<<"maxPosX"<<maxPos.x();
    qDebug()<<"maxPosY"<<maxPos.y();
    int dWidth = -minPos.x()+ minimallSpace;//расстояние на которое нужно передвинуть все обьекты на виджете
    int dHeight = -minPos.y()+ minimallSpace;//расстояние на которое нужно передвинуть все обьекты на виджете
    //переместить элементы на необходимое смещение
    if( dWidth != 0 || dHeight != 0 )
    {
        foreach (int id, lBufer.keys() )
        {
            lBufer[id]->line->changePos(dWidth, dHeight);
        }
        foreach (int id, nBufer.keys() )
        {
            nBufer[id]->node->changePos(dWidth, dHeight);
        }
    }
    this->update();
}
void GraphTree::addSpaceForActivObj()
{//при приближении границы на расстояние меньше 5 - отодвинуть ее до 10
    int installSpace = 12;//расстояние которое устанавливается до грниц виджта
    int minimallSpace = 10;//минимальное допкстимое расстояние до грниц виджета
    QSize size = this->size();
    int dWidth=0;//расстояние на которое нужно передвинуть все обьекты на виджете
    int dHeight=0;//расстояние на которое нужно передвинуть все обьекты на виджете
    //
    int leftSpace=0;
    int rightSpace=0;
    int topSpace=0;
    int bottomSpace=0;
    bool flag = false;
    if( nBufer.find(activNodeID) != nBufer.end() )
    {
        leftSpace = nBufer[activNodeID]->node->getTopLeftPos().x();//текущее расстояние до левого поля
        rightSpace = nBufer[activNodeID]->node->getBottomRighPos().x();//крайняя правая позиция
        topSpace = nBufer[activNodeID]->node->getTopLeftPos().y();//расстояние до верхнего поля
        bottomSpace = nBufer[activNodeID]->node->getBottomRighPos().y();//крайняя нижняя позиция
        flag=true;
    }
    else
        if( lBufer.find(activLineID) != lBufer.end() )
        {
            leftSpace = lBufer[activLineID]->line->getTopLeftPos().x();//текущее расстояние до левого поля
            rightSpace = lBufer[activLineID]->line->getBottomRighPos().x();//крайняя правая позиция
            topSpace = lBufer[activLineID]->line->getTopLeftPos().y();//расстояние до верхнего поля
            bottomSpace = lBufer[activLineID]->line->getBottomRighPos().y();//крайняя нижняя позиция
            flag=true;
        }
    if(!flag)
        return;
    //===
    if(leftSpace < minimallSpace)
    {
        dWidth = installSpace - leftSpace;
        size.setWidth( size.width() + dWidth );
    }
    if( rightSpace + minimallSpace > size.width() )
    {
        size.setWidth( rightSpace + installSpace );
    }
    //
    if(topSpace < minimallSpace)
    {
        dHeight = installSpace - topSpace;
        size.setHeight( size.height() + dHeight );
    }
    if( bottomSpace + minimallSpace > size.height() )
    {
        size.setHeight( bottomSpace + installSpace );
    }
    //установка новых размеров виджета
    this->setFixedSize(size);
    //переместить элементы на необходимое смещение
    if( dWidth != 0 || dHeight != 0 )
    {
        foreach (int id, lBufer.keys() )
        {
            lBufer[id]->line->changePos(dWidth, dHeight);
        }
        foreach (int id, nBufer.keys() )
        {
            nBufer[id]->node->changePos(dWidth, dHeight);
        }
    }

}
//---------------------------------------------------------------------
void GraphTree::buildMenu()
{
    fieldsForMenuN<<"Вопрос для узла"<<"Текст для листа"<<"Удалить";
    fieldsForMenuL<<"Условие перехода"<<"Удалить";
    //
    menuForN = new QMenu(this);
    menuForN->addAction(fieldsForMenuN[0]);
    menuForN->addAction(fieldsForMenuN[1]);
    menuForN->addSeparator();
    menuForN->addAction(fieldsForMenuN[2]);
    connect(menuForN, SIGNAL(triggered(QAction*)), SLOT(menuNActivated(QAction*)) );
    menuForL = new QMenu(this);
    menuForL->addAction(fieldsForMenuL[0]);
    menuForL->addSeparator();
    menuForL->addAction(fieldsForMenuL[1]);
    connect(menuForL, SIGNAL(triggered(QAction*)), SLOT(menuLActivated(QAction*)) );
}

void GraphTree::addNode()//вызывается после того как пользователь выбрав узел кликнул на этом виджете
{
    countID++;
    Node *node=new Node(countID);
    QRect rect = this->visibleRegion().boundingRect();
    node->setPos(QPoint( rect.x() + rect.width() / 2, rect.y() + rect.height() / 2 ) );
    StructN *chein=new StructN(node);
    nBufer.insert(countID, chein);
    this->update();
}

void GraphTree::removeNode(int id)//--
{
    if(nBufer.find(id) == nBufer.end())
        return;
    foreach (int lineId, nBufer[id]->input)
    {//вход в узел это выход линии
        lBufer[lineId]->line->setEndNode(nullptr);
        lBufer[lineId]->out = -1;
    }
    foreach (int lineId, nBufer[id]->out)
    {//выход из узла это вход линии
        lBufer[lineId]->line->setStartNode(nullptr);
        lBufer[lineId]->in = -1;
    }

    delete(nBufer[id]);
    nBufer.remove(id);
    this->update();
}

void GraphTree::cleanBuffers()
{
    activLineID=-1;
    activNodeID=-1;
    foreach (RenameForm *f, garbageWindow)
    {
        f->close();
    }
    garbageWindow.clear();
    countID=0;
    foreach (int id, lBufer.keys())
    {
        delete(lBufer[id]);
    }
    lBufer.clear();
    foreach (int id, nBufer.keys())
    {
        delete(nBufer[id]);
    }
    nBufer.clear();
    this->update();
}
void GraphTree::addLine()
{
    countID++;
    Line *line=new Line(countID);
    QRect rect = this->visibleRegion().boundingRect();
    line->setPos(QPoint( rect.x() + rect.width() / 2, rect.y() + rect.height() / 2 ));
    StructL *connector=new StructL(line);
    lBufer.insert(countID, connector);
    this->update();
}

void GraphTree::removeLine(int id)//--
{
    if(lBufer.find(id) == lBufer.end())
        return;
    if(lBufer[id]->in > 0)
        nBufer[lBufer[id]->in]->out.removeOne(id);
    if(lBufer[id]->out > 0)
        nBufer[lBufer[id]->out]->input.removeOne(id);

    delete(lBufer[id]);
    lBufer.remove(id);
    this->update();
}

void GraphTree::disactivate()
{
    qDebug()<<"kzkzkz";
    if(activNodeID > 0)
    {
        nBufer[activNodeID]->node->setActivate(false);
        activNodeID = -1;
    }
    else
        if(activLineID > 0)
        {
            lBufer[activLineID]->line->setActivate(false);
            activLineID = -1;
        }
}
//----------------------------------
bool GraphTree::saveGraph(QString name)
{
    QVector< QMap<QString, QString> > data;

    foreach (int id, lBufer.keys())
    {
        QMap<QString, QString> obj;
        obj.insert("type", "1");//это линия
        obj.insert("id", QString("%1").arg(id) );//id линии
        if(lBufer[id]->in > 0)
            obj.insert("start_id", QString("%1").arg(lBufer[id]->in ) );//id узла с которого начинается линия
        else
        {
            obj.insert("start_x", QString("%1").arg(lBufer[id]->line->getPosStart().x() ) );
            obj.insert("start_y", QString("%1").arg(lBufer[id]->line->getPosStart().y() ) );
        }
        if(lBufer[id]->out > 0)
            obj.insert("end_id", QString("%1").arg(lBufer[id]->out ) );//id узла на который указывает линия
        else
        {
            obj.insert("end_x", QString("%1").arg(lBufer[id]->line->getPosEnd().x() ) );
            obj.insert("end_y", QString("%1").arg(lBufer[id]->line->getPosEnd().y() ) );
        }
        obj.insert("text", lBufer[id]->line->getText() );
        obj.insert("text_x", QString("%1").arg(lBufer[id]->line->getPosText().x() ) );
        obj.insert("text_y", QString("%1").arg(lBufer[id]->line->getPosText().y() ) );
        data.append(obj);
    }
    //
    foreach (int id, nBufer.keys())
    {
        QMap<QString, QString> obj;
        if( nBufer[id]->node->isNode() )
            obj.insert("type", "2");//это узел
        else
            obj.insert("type", "3");//это лист
        obj.insert("id", QString("%1").arg(id) );//id узла
        obj.insert("pos_x", QString("%1").arg(nBufer[id]->node->getPos().x() ) );
        obj.insert("pos_y", QString("%1").arg(nBufer[id]->node->getPos().y() ) );
        obj.insert("width", QString("%1").arg(nBufer[id]->node->getWidth() ) );
        obj.insert("height", QString("%1").arg(nBufer[id]->node->getHeight() ) );
        obj.insert("text", nBufer[id]->node->getText() );
        data.append(obj);
    }
    bool result = FileXML::getInstance()->writeFile(this ,data, name);
    return result;
}
bool GraphTree::loadGraph(QString name)
{
    countID=0;
    //
    cleanBuffers();
    QVector< QMap<QString, QString> > data = FileXML::getInstance()->readData(this, name);
    if(data.length() == 0)
        return false;
    qDebug()<<QString("data НЕ ПУСТАЯ!: %1").arg(data.length());
    int id=0;
    for(int i=0; i<data.length(); i++)//запись только узлов
    {
        QMap<QString, QString> object = data[i];
        if(object["type"].toInt() == 2 || object["type"].toInt() == 3 )//узел, либо лист
        {
            id = object["id"].toInt();
            if(id > countID)
                countID = id;

            Node *node = new Node(id);
            if(object["type"].toInt() == 2)
                node->setTextNode(object["text"]);
            else
                node->setTextLeaf(object["text"]);
            node->setPos( QPoint( object["pos_x"].toInt(), object["pos_y"].toInt() ) );
            node->setHeight(object["height"].toInt());
            node->setWidth(object["width"].toInt());
            nBufer.insert(id, new StructN(node));
        }
    }
    for(int i=0; i<data.length(); i++)//запись только линий
    {
        QMap<QString, QString> object = data[i];
        if(object["type"].toInt() == 1 )//если это линия
        {
            id = object["id"].toInt();
            if(id > countID)
                countID = id;

            Line *line = new Line(id);
            line->setText(object["text"]);
            StructL *connector = new StructL(line);
            if(object.find( "start_id" ) != object.end())
            {
                qDebug()<<QString("START LINE");
                connector->in = object["start_id"].toInt();
                line->setStartNode( nBufer[ connector->in ]->node );
            }
            else
                line->setStartPos( QPoint( object["start_x"].toInt(), object["start_y"].toInt() ) );
            //
            if(object.find( "end_id" ) != object.end())
            {
                connector->out = object["end_id"].toInt();
                line->setEndNode( nBufer[ connector->out ]->node );
            }
            else
                line->setEndPos( QPoint( object["end_x"].toInt(), object["end_y"].toInt() ) );
            line->setTextPos(QPoint( object["text_x"].toInt(), object["text_y"].toInt() ));

            lBufer.insert(id, connector);

        }
    }
    //дозапись в nBufer id входных и выходных линий
    foreach (int id, lBufer.keys())
    {
        qDebug()<<"data НЕ ПУСТАЯ!";
        if( lBufer[id]->in > 0 )
        {
            nBufer[ lBufer[id]->in ]->out.append(id);
        }
        if( lBufer[id]->out > 0 )
        {
            nBufer[ lBufer[id]->out ]->input.append(id);
        }

    }
    this->update();
    return true;
}
//----------------------------------

Node* GraphTree::searchFirstNode(QPoint pos)
{
    foreach (int id, nBufer.keys() )
    {
        if(nBufer[id]->node->isOnclic( pos, false ) )
        {
            return (nBufer[id]->node);
        }
    }
    return nullptr;
}

int GraphTree::getMainNode()//возвращает главный узел дерева, если он единственный
{
    int kount =0;
    int mainNode=0;
    foreach (int id, nBufer.keys() )
    {
        if(nBufer[id]->input.isEmpty() )//делай-------------
        {
            mainNode = id;
            kount++;
        }
    }
    if(kount == 1)
        return mainNode;
    return -1;
}

int GraphTree::getNextNode(int idLine, bool mark)//возвращает id узла, на который указывает линия
{//отмечает линию
    if(lBufer.contains(idLine) )
    {
        if(mark)
        {
            lBufer[idLine]->line->setMark(true);
            this->update();
        }
        return lBufer[idLine]->out;
    }
    return -1;
}

QString GraphTree::getNameFromNode(int idNode, bool mark)//возвращает имя узла по его id
{//отмечает узел
    if(nBufer.contains(idNode) )
    {
        if(mark)
        {
            nBufer[idNode]->node->setMark(true);
            this->update();
        }
        return nBufer[idNode]->node->getText();
    }
    return "-1";
}

QMap<int, QString> GraphTree::getNodeVariants(int idNode)
{
    QMap<int, QString> data;
    if(!nBufer.contains(idNode) )
        return data;
    foreach (int out, nBufer[idNode]->out)
    {
        if(lBufer[out]->out > 0)
            data.insert(out, lBufer[out]->line->getText() );
    }
    return data;
}

/*
void GraphTree::contextMenuEvent(QContextMenuEvent* event)
{

}*/

void GraphTree::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::black,2,Qt::SolidLine));
    setFont(QFont("Arial", 8, 50, 0));
    foreach (int id, nBufer.keys())
    {
        nBufer[id]->node->draw(&painter);
    }
    foreach (int id, lBufer.keys())
    {
        lBufer[id]->line->draw(&painter);
    }
    if(activNodeID > 0)
        nBufer[activNodeID]->node->draw(&painter);
    else
        if(activLineID > 0)
            lBufer[activLineID]->line->draw(&painter);
}

void GraphTree::mousePressEvent(QMouseEvent *event)
{
    if(controlLock)
        return;
    if (event->button() != Qt::LeftButton &&
            event->button() != Qt::RightButton)
        return;
    //------------------выполнено ли нажатие по активированому элементу?
    if(activLineID > 0)
    {
        if( lBufer[activLineID]->line->isOnclic( event->pos(), true ) )
        {
            event->accept();
            this->update();
            if(event->button() == Qt::RightButton)
                menuForL->exec(event->globalPos());
            else
            {
                activLineData = lBufer[activLineID]->line->getIdData();
            }
            return;//не выполнять повторной активации
        }
    }
    if(activNodeID > 0)
    {
        if( nBufer[activNodeID]->node->isOnclic( event->pos(), true ) )
        {
            event->accept();
            this->update();
            if(event->button() == Qt::RightButton)
                menuForN->exec(event->globalPos());
            return;//не выполнять повторной активации
        }
    }

    //------------------активация не активированого элемента?
    //дезактивация предыдущего элемента
    disactivate();

    dragMode=false;
    foreach (int id, lBufer.keys() )
    {
        if(lBufer[id]->line->isOnclic( event->pos(), true ) )//уже активировано
        {
            activLineID=id;
            event->accept();
            this->update();
            dragMode=true;
            if (event->button() == Qt::RightButton)
            {
                menuForL->exec(event->globalPos());
            }
            else
            {
                activLineData = lBufer[activLineID]->line->getIdData();
            }
            return;
        }
    }

    foreach (int id, nBufer.keys() )
    {
        if(nBufer[id]->node->isOnclic( event->pos(), true ) )//уже активировано
        {
            activNodeID=id;
            event->accept();
            this->update();
            dragMode=true;
            if (event->button() == Qt::RightButton)
            {
                menuForN->exec(event->globalPos());
            }
            return;
        }
    }
    this->update();
}
/*
void GraphTree::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(activLineID > 0)
        {
            lBufer[activLineID]->line->nodeConnect();
            this->update();
        }
    }
}*/

void GraphTree::mouseMoveEvent(QMouseEvent *event)
{//отмечает узел, на который был наведен конец линии
 //линия получает первый найденный узел под розицией мыши, и делает то, что ей нужно

    if(!dragMode)
        return;
    if(event->pos().x() < 0 || event->pos().y() < 0)
        return;

    if(activNodeID > 0)
    {
        nBufer[activNodeID]->node->move(event->pos());
    }
    else
        if(activLineID > 0)
        {
            //проверить есть ли узел в данной позиции
            IdData data = lBufer[activLineID]->line->move(event->pos(), searchFirstNode(event->pos()));
            //data это узлы к которым привязана линия
            if(activLineData.nodeStart != data.nodeStart )//проверка на изменения указателя начала линии
            {
                if(activLineData.nodeStart > 0)
                    nBufer[activLineData.nodeStart]->out.removeOne(activLineID);
                if(data.nodeStart > 0)
                    nBufer[data.nodeStart]->out.append(activLineID);//записывается начало линии
                lBufer[activLineID]->in = data.nodeStart;//вход в линию это ее начало
            }
            if(activLineData.nodeEnd != data.nodeEnd )//проверка на изменения указателя конца линии
            {
                if(activLineData.nodeEnd > 0)
                    nBufer[activLineData.nodeEnd]->input.removeOne(activLineID);
                if(data.nodeEnd > 0)
                    nBufer[data.nodeEnd]->input.append(activLineID);//записывается конец линии
                lBufer[activLineID]->out = data.nodeEnd;
            }
            activLineData=data;
        }
    addSpaceForActivObj();

    this->update();
    //event->accept();
}



void GraphTree::menuNActivated(QAction* action)
{
    if( nBufer.find(activNodeID) == nBufer.end() )
        return;
    int n = fieldsForMenuN.indexOf( action->text() );
    switch (n)
    {
    case 0://имя для узла
    {
        RenameForm *renameForm = new RenameForm(activNodeID, 2,nBufer[activNodeID]->node->getText());
        garbageWindow.append(renameForm);
        connect(renameForm, SIGNAL(renameSignal(int,int,QString)), this, SLOT(renameActivObj(int,int,QString)) );
        connect(renameForm, SIGNAL(closeForm(RenameForm*)) , this, SLOT(closeDialogWindow(RenameForm*)) );
        renameForm->show();
        break;
    }
    case 1://имя для листа
    {
        RenameForm *renameForm = new RenameForm(activNodeID, 3,nBufer[activNodeID]->node->getText());
        garbageWindow.append(renameForm);
        connect(renameForm, SIGNAL(renameSignal(int,int,QString)), this, SLOT(renameActivObj(int,int,QString)) );
        connect(renameForm, SIGNAL(closeForm(RenameForm*)) , this, SLOT(closeDialogWindow(RenameForm*)) );
        renameForm->show();
        break;
    }
    case 2:
    {
        removeNode(activNodeID);

        activNodeID=-1;
        this->update();
        break;
    }
    default:
    {
        break;
    }
    }
}

void GraphTree::menuLActivated(QAction* action)
{
    if( lBufer.find(activLineID) == lBufer.end() )
        return;
    int n = fieldsForMenuL.indexOf( action->text() );
    switch (n)
    {
    case 0:
    {
        RenameForm *renameForm = new RenameForm(activLineID, 1,lBufer[activLineID]->line->getText());
        garbageWindow.append(renameForm);
        connect(renameForm, SIGNAL(renameSignal(int,int,QString)), this, SLOT(renameActivObj(int,int,QString)) );
        connect(renameForm, SIGNAL(closeForm(RenameForm*)) , this, SLOT(closeDialogWindow(RenameForm*)) );
        renameForm->show();
        break;
    }
    case 1:
    {
        removeLine(activLineID);

        activLineID=-1;
        this->update();
        break;
    }
    default:
    {
        break;
    }
    }
}

void GraphTree::renameActivObj(int id, int type, QString newName)
{
    if( nBufer.find(id) != nBufer.end() )
    {
        if(type==2)
            nBufer[id]->node->setTextNode(newName);
        else
            nBufer[id]->node->setTextLeaf(newName);
        this->update();
    }
    else
        if( lBufer.find(id) != lBufer.end() )
        {
            lBufer[id]->line->setText(newName);
            this->update();
        }

}

void GraphTree::closeDialogWindow(RenameForm* form)
{
    garbageWindow.removeOne(form);
}
