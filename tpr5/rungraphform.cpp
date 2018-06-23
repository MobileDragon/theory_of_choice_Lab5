#include "rungraphform.h"

RunGraphForm::RunGraphForm(QWidget *parent):
    QDialog(parent)
{
    area=0;
    graphWidget=0;
    quest=0;
    build();
}
void RunGraphForm::build()
{
    delete(area);
    delete(graphWidget);
    delete(quest);

    graphWidget = new GraphTree();
    graphWidget->setMinimumHeight(100);
    graphWidget->setMinimumWidth(100);

    QHBoxLayout *hLayout = new QHBoxLayout;
    this->setLayout(hLayout);
    hLayout->setMargin(5);

    area=new QScrollArea;
    area->setWidget(graphWidget);


    hLayout->addWidget(area);
    /*
    QPushButton *d = new QPushButton("кнопка");
    d->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    hLayout->addWidget(d);
    hLayout->setAlignment(d,Qt::AlignTop);//
*/
    quest = new DialogQvest();
    //quest->setData("ты кто?", QList<QString>()<<"привет, ты отрисовыв1ает текст в зависимости от ширины виджета отрисовывает текст в зависимости от ширины виджета отрисовывает текст в зависимости от ширины виджета кто?!"<<"ПРИВЕТ");
    //quest->setFixedSize(100,170);

    quest->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);//расширение на всю допустимую область
    quest->setMaximumWidth(400);
    hLayout->addWidget(quest);

    connect(quest, SIGNAL(variantReleased(int)) , this, SLOT(branchSelection(int)) );

}

RunGraphForm::~RunGraphForm()
{
    delete(area);
    delete(graphWidget);
    delete(quest);
}

bool RunGraphForm::setGraph(QString name)
{
    graphWidget->loadGraph(name);
    graphWidget->setLockingControl(true);
    graphWidget->cutExtraSpace();


    int mainIdNode = graphWidget->getMainNode();
    QString nameFromNode = graphWidget->getNameFromNode(mainIdNode, true);
    QMap<int, QString> data = graphWidget->getNodeVariants(mainIdNode);
    if(data.isEmpty() )
    {
        QMessageBox::warning(this, "Ошибка графа", "Даный граф нельзя протестировать", QMessageBox::Ok);
        this->close();
        return false;
    }

    quest->setData(nameFromNode, data);
    return true;
}


void RunGraphForm::closeEvent(QCloseEvent *e)
{
    e->accept();
}

void RunGraphForm::branchSelection(int id)
{
    QString s = QString("%1").arg(id);

    //полученый id линии отправить на GraphTree и получить
    //1)название узла
    //2)QMap<int, QString> связь id-название линий
    //если 2) пункт ничего не вернул, то вывести в MassageBox 1) пункт
    //все!
    //void RunGraphForm::build() записівает первый узел с вариантами

    int IdNode = graphWidget->getNextNode(id, true);
    QString nameFromNode = graphWidget->getNameFromNode(IdNode, true);
    QMap<int, QString> data = graphWidget->getNodeVariants(IdNode);
    if(data.isEmpty() )
    {
        QMessageBox::information(this, "Результат прохождения графа", nameFromNode, QMessageBox::Ok);
        this->close();
    }

    quest->setData(nameFromNode, data);
}
