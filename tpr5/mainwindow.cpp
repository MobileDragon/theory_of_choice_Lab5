#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tempFile = QDir::currentPath()+QString("/.tmp.XML");
    build();

}

MainWindow::~MainWindow()
{
    delete graphWidget;
    delete area;

    delete addNode;
    delete addLine;
    delete cutExtraSpace;
    delete saveAs;
    delete openFile;
    delete runGraph;
    delete newGraph;
    delete ui;
}

void MainWindow::build()
{
    graphWidget = new GraphTree();
    //QWidget *w=new QWidget;
    graphWidget->setMinimumHeight(100);
    graphWidget->setMinimumWidth(100);

    ui->verticalLayout->setMargin(5);

    area=new QScrollArea;
    area->setWidget(graphWidget);
    ui->verticalLayout->addWidget(area);
    //
    addNode = new QPushButton("Добавить узел");
    connect(addNode, SIGNAL(released()), this, SLOT(addNodeReleased()) );
    addLine = new QPushButton("Добавить линию");
    connect(addLine, SIGNAL(released()), this, SLOT(addLineReleased()) );
    cutExtraSpace = new QPushButton("Подогнать размер");
    connect(cutExtraSpace, SIGNAL(released()), this, SLOT(cutExtraSpaceReleased()) );
    saveAs = new QPushButton("Сохранить граф");
    connect(saveAs, SIGNAL(released()), this, SLOT(saveAsReleased()) );
    openFile = new QPushButton("Открыть файл");
    connect(openFile, SIGNAL(released()), this, SLOT(openFileReleased()) );
    runGraph = new QPushButton("Выполнить граф");
    connect(runGraph, SIGNAL(released()), this, SLOT(runGraphReleased()) );
    newGraph = new QPushButton("Очистить граф");
    connect(newGraph, SIGNAL(released()), this, SLOT(newGraphReleased()) );
    ui->verticalLayout_2->setAlignment(Qt::AlignTop);
    ui->verticalLayout_2->addWidget(addNode);
    ui->verticalLayout_2->addWidget(addLine);
    ui->verticalLayout_2->addWidget(cutExtraSpace);
    ui->verticalLayout_2->addWidget(saveAs);
    ui->verticalLayout_2->addWidget(openFile);
    ui->verticalLayout_2->addWidget(runGraph);
    ui->verticalLayout_2->addWidget(newGraph);
}
//
void MainWindow::addNodeReleased()
{
    graphWidget->addNode();
}
void MainWindow::addLineReleased()
{
    graphWidget->addLine();
}
void MainWindow::cutExtraSpaceReleased()
{
    graphWidget->cutExtraSpace();
}
void MainWindow::saveAsReleased()
{
    graphWidget->saveGraph();
}
void MainWindow::openFileReleased()
{
    graphWidget->loadGraph();
    graphWidget->cutExtraSpace();
}
void MainWindow::runGraphReleased()
{   //сохранение текущей диаграммы
    graphWidget->saveGraph(tempFile);
    //новое окно
    RunGraphForm *runForm = new RunGraphForm();//сама удалится по завершению работы
    if(runForm->setGraph(tempFile))
        runForm->show();
    /*
    wArgument = new CriterionForm(kriterion, "Установите приоритет критериев");
    connect(wArgument, SIGNAL(finished(int)) ,this,SLOT(argumentDestroy(int) ));
    wArgument->show();
*/
    //this->hide();
}
void MainWindow::newGraphReleased()
{
    graphWidget->cleanBuffers();
}
