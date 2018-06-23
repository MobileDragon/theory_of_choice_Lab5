#include "dialogqvest.h"

DialogQvest::DialogQvest(QWidget *parent) : QWidget(parent)
{
    questionArea=0;
    vBox=0;
}

DialogQvest::~DialogQvest()
{
    variants.clear();
    foreach (ButtonForQuest* button, buttons)
    {
        button->destroyed();
    }
    buttons.clear();
    //удаление группировшика
    delete(vBox);
    delete(questionArea);
}

void DialogQvest::build()//-----------------------
{
    delete(vBox);
    vBox = new QVBoxLayout();
    this->setLayout(vBox);

    foreach (ButtonForQuest* button, buttons)
    {
        delete(button);
    }
    buttons.clear();

    delete(questionArea);
    //vBox = layout;//будет содержать список вариантов
    //удаление старых вариантов



    //создание новых вариантов
    vBox->setAlignment(Qt::AlignTop);

    questionArea = new TextForQuest(question);
    questionArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    vBox->addWidget(questionArea);

    foreach (int key, variants.keys() )
    {
        QString curName = variants[key];
        ButtonForQuest *button = new ButtonForQuest(key, curName);
        connect(button, SIGNAL(buttonReleased(int)) , this, SLOT(variantSelection(int)) );
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        //button->setMinimumSize(100,20);
        buttons.append(button);
        vBox->addWidget(button);
        //vBox->setAlignment(button, Qt::AlignBottom);
    }
    this->update();
}

void DialogQvest::setData(const QString &question, const QMap<int, QString> &variants)//QList<QString>
{
    this->question = question;

    this->variants.clear();

    foreach (int id, variants.keys())
    {
        this->variants.insert(id,variants[id] );
    }

    if(this->variants.isEmpty())
    {
        return;
    }
    build();
}

void DialogQvest::variantSelection(int id)
{
    emit variantReleased(id);
}
