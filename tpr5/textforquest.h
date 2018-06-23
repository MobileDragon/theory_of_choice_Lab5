#ifndef TEXTFORQUEST_H
#define TEXTFORQUEST_H

#include <QWidget>
#include <QtCore/QLine>
#include <QtGui/QPainter>
#include <QtCore/QPoint>
#include <QRegExp>
#include <QStringList>
#include <QList>
#include "drawstring.h"

class TextForQuest : public QWidget
{
    Q_OBJECT
public:
    explicit TextForQuest(QString text = "", QWidget *parent = nullptr);
    ~TextForQuest();
    void setText(QString text);
private:
    void build(QPainter *p);
    int updateRows(QPainter *p);
    void drawText(QPainter *p);

    QString text;//текст
    QList <DrawingString*> drawStringData;//список строк подготовленных для отображения
    int textWidth;//ширина занимаемая текстом
    int textHeight;//высота занимаемая текстом
    int border;//расстояние от граници виджета до текста

protected:
    virtual void paintEvent(QPaintEvent *event);

signals:

public slots:
};

#endif // TEXTFORQUEST_H
