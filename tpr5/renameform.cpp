#include "renameform.h"

RenameForm::RenameForm(int id, int typeElemet, QString name, QWidget *parent):
    QDialog(parent)
{
    idObject=id;
    type=typeElemet;
    text=name;
    buildForm();
}

RenameForm::~RenameForm()
{
//сделать!
    delete(ok);
    delete(lineEdit);
    delete(label);
    delete(mainVLayout);
}

void RenameForm::buildForm()
{
    ok = new QPushButton("ok");
    connect(ok, SIGNAL(released()), this, SLOT(okReleased()) );
    lineEdit = new QLineEdit(text);
    connect(lineEdit, SIGNAL(textChanged(QString)), this,SLOT(nameChanged(QString) ));
    switch (type) {
    case 1://линия
        this->setWindowTitle("Переименование линии");
        label = new QLabel("Укажите вариант ответа");
        break;
    case 2://узел
        this->setWindowTitle("Переименование узла");
        label = new QLabel("Задайте условие перехода");
        break;
    case 3://лист
        this->setWindowTitle("Переименование листа");
        label = new QLabel("Задайте конечный результат");
        break;
    default:
        label = new QLabel("укажите новое имя");
        break;
    }
    mainVLayout = new QVBoxLayout;
    mainVLayout->addWidget(label);
    mainVLayout->addWidget(lineEdit);
    mainVLayout->addWidget(ok);
    mainVLayout->setAlignment(ok, Qt::AlignCenter);


    this->setLayout(mainVLayout);
}

void RenameForm::closeEvent(QCloseEvent *e)
{
    e->accept();
    this->finished(0);//рассказать про успех завершения работы
    emit closeForm(this);
    this->deleteLater();
}

void RenameForm::nameChanged(QString s)
{
    emit renameSignal(idObject, type, s);
}

void RenameForm::okReleased()
{
    this->close();
}
