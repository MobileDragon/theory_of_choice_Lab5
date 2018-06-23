#ifndef DIALOGQVEST_H
#define DIALOGQVEST_H

#include <QWidget>
#include <QList>
#include <QMap>
#include <QVBoxLayout>
#include "textforquest.h"
#include "buttonforquest.h"
#include <QPushButton>
#include <QScrollArea>

class DialogQvest : public QWidget//имеет на себе VerticalLayout который содержит массив кнопок хранимый здесь
{
    Q_OBJECT
public:
    explicit DialogQvest(QWidget *parent = nullptr);
    ~DialogQvest();
    void setData(const QString &question, const QMap<int, QString> &variants);//уствновка отображаемых вариантов действий
    //int getSelectedVariant();//возвращает активированный вариант

private:
    void build();
    QVBoxLayout *vBox;
    QString question;
    QMap<int, QString> variants;
    TextForQuest *questionArea;
    QList<ButtonForQuest*> buttons;

signals:
    void variantReleased(int);//посылает сигнал с номером выбранного варианта

public slots:
    void variantSelection(int id);
};

#endif // DIALOGQVEST_H
