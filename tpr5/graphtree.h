#ifndef GRAPHTREE_H
#define GRAPHTREE_H

#include <QWidget>
#include <QMap>
#include <QVector>
#include <QSize>
#include <QRect>
#include <QtCore/QList>
#include <QtCore/QPoint>

#include <QtCore/QLine>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

#include <line.h>
#include <node.h>
#include <QPushButton>
#include <QLayout>
#include <QMenu>
#include <QDebug>
#include "renameform.h"
#include "filexml.h"



struct StructN
{
    StructN(Node *node)
    {
        this->node=node;
    }
    ~StructN()
    {
        delete(node);
    }
    Node *node;
    QVector<int> input;//id линий для карты map(id, StructL*)
    QVector<int> out;
};
struct StructL
{
    StructL(Line *line)
    {
        this->line=line;
        out=-1;//линия не на кого не указывает
        in=-1;
    }
    ~StructL()
    {
        delete(line);
    }
    Line* line;
    int out;//id узла на который указывает линия для карты map(id, StructN*)
    int in;
};

class GraphTree : public QWidget
{
    Q_OBJECT
public:
    explicit GraphTree(QWidget *parent = nullptr);
    ~GraphTree();
    //
    void setLockingControl(bool lock);//установка блокировки мыши

    int getMainNode();//возвращает id главного узела дерева, если он единственный
    int getNextNode(int idLine, bool mark = false);//возвращает id узла, на который указывает линия
    QString getNameFromNode(int idNode, bool mark = false);//возвращает имя узла по его id
    QMap<int, QString> getNodeVariants(int idNode);//по id узла возвращает список действительных переходов узла
    //возвращает связь id-название

    void cutExtraSpace();//обрезает лишнее не заполненое пространство по краям
    void addSpaceForActivObj();//увеличивать пространство для передвигаемого объекта


    void buildMenu();
    void addNode();//вызывается после того как пользователь выбрав узел кликнул на этом виджете
    void removeNode(int id);//удаляет узел содержащий id
    void cleanBuffers();//очистка всего графа
    void addLine();
    void removeLine(int id);//удаляет линию содержащую id
    void disactivate();//отменяет отметку выбранного элемента(элемент один)
    //
    bool saveGraph(QString name=0);
    bool loadGraph(QString name=0);

protected:
    Node* searchFirstNode(QPoint pos);//нахождение первого попавшегося узла пол курсором

    //метод определяющий пересекается ли переданая линия с любой другой или элементом
    virtual void mousePressEvent(QMouseEvent *event);
    //virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    //virtual bool eventFilter( QObject *obj, QEvent *evt );

    virtual void paintEvent(QPaintEvent *event);
    //
    //virtual void contextMenuEvent(QContextMenuEvent* event);
private:
    QMap<int, StructL*> lBufer;//id линии и структура с указателем на саму линию
    QMap<int, StructN*> nBufer;//id узла и структура с указателем на сам узел
    //

    QMenu *menuForN;//контекстное меню дл узла/литста
    QMenu *menuForL;

    IdData activLineData;//id узлов начала и конца для активированой линии
    bool dragMode;//был ли активирован элемент, для его передвижения
    bool controlLock;//блокировка перефирии пользователя
    int countID;//идентификатор последнего добавленного элемента
    int activNodeID;//id активированого узла(например мышкой)
    int activLineID;//id активированой линии

    QVector< QString >fieldsForMenuN;
    QVector< QString >fieldsForMenuL;

    //RenameForm *renameForm;
    QVector<RenameForm* > garbageWindow;//список действующих диалоговых окон

signals:

public slots:
    void menuNActivated(QAction* action);
    void menuLActivated(QAction* action);
    void renameActivObj(int id, int type, QString newName);
    void closeDialogWindow(RenameForm* form);
};

#endif // GRAPHTREE_H
