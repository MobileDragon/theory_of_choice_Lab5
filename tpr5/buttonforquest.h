#ifndef BUTTONFORQUEST_H
#define BUTTONFORQUEST_H

#include <QWidget>
#include <QtCore/QList>
#include <QtCore/QPoint>
#include <QRegExp>
#include <QStringList>
#include <QList>
#include "drawstring.h"

#include <QtCore/QLine>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

class ButtonForQuest : public QWidget
{
    Q_OBJECT
public:
    explicit ButtonForQuest(int num = -1, QString name ="", QWidget *parent = nullptr);
    ~ButtonForQuest();
    void setValues(int num, QString name ="");

private:
    QString name;//исходная стока названия кнопки
    QList <DrawingString*> drawStringData;//список строк подготовленных для отображения
    int textWidth;//ширина занимаемая текстом
    int textHeight;//высота занимаемая текстом
    int border;//расстояние от граници виджета до текста
    int num;//идентификационный номер
    bool hover;//наведение курсора мыши
    void build(QPainter *p);//отрисовывает текст в зависимости от ширины виджета
    int updateRows(QPainter *p);//отображает текст и возвращает высоту выведеного текста
    void drawTitle(QPainter *p);//отображение текста в зависимости от установленных свойств

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void enterEvent(QEvent *e);
    virtual void leaveEvent(QEvent *e);

signals:
    void buttonReleased(int);//посылает сигнал с номером выбранного сигнала

public slots:
};

#endif // BUTTONFORQUEST_H
