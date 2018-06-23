#ifndef FILEXML_H
#define FILEXML_H
#include <QDebug>
#include <QFileDialog>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QXmlStreamAttribute>
#include <QMessageBox>
#include <QFile>
#include <QVector>
#include <QMap>


class FileXML
{
public:
    static FileXML* getInstance();
    bool writeFile(QWidget *parent , QVector< QMap<QString, QString> > data, QString filename=0);//возвращает true, если удалось записать данные
    //data это список карт храняших атрибуты и их значения
    QVector< QMap<QString, QString> > readData(QWidget *parent, QString filename=0);//возвращает список списков данных для объектов

private:
    FileXML();
    //QFile *file;
};

#endif // FILEXML_H
