#include "filexml.h"

FileXML::FileXML()
{

}

FileXML* FileXML::getInstance()
{
    static FileXML instance;
    return &instance;
}
//
bool FileXML::writeFile(QWidget *parent, QVector< QMap<QString, QString> > data, QString filename)
{//data это список карт храняших атрибуты и их значения
    bool result=true;
    if(filename.isNull())
    {
        filename = QFileDialog::getSaveFileName(parent,"Save Xml", ".", "Xml files (*.xml)" );
        if(filename.length() == 0)
            return false;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(&file);//осуществляет запись
    xmlWriter.setAutoFormatting(true);//установка автоформатирования текста
    xmlWriter.writeStartDocument();//запуск записывания в документ
    xmlWriter.writeStartElement("resources");   // Записываем коренной тег
    //
    for(int i=0; i<data.length(); i++)
    {
        QMap<QString, QString> object = data[i];
        xmlWriter.writeStartElement("object");//тег для новой записи
        foreach (QString atribute, object.keys())
        {
             xmlWriter.writeAttribute( atribute, object[atribute] );//запись атрибута и его значения
        }
        xmlWriter.writeCharacters("XM");
        xmlWriter.writeEndElement();//запись закрывающего тега
    }
    xmlWriter.writeEndElement();//Закрываем тег "resources" /коренной тег
    file.close();
    return result;
}
QVector<QMap<QString, QString> > FileXML::readData(QWidget *parent, QString filename)
{
    QVector< QMap<QString, QString> > data;
    if(filename.isNull())
    {
        filename = QFileDialog::getOpenFileName(parent, "Open Xml", ".", "Xml files (*.xml)");
        if(filename.length() == 0)
            return data;
    }

    QFile file(filename);
    if( !file.open( QFile::ReadOnly | QFile::Text ) )
    {
        QMessageBox::warning(parent, "Ошибка файла", "Не удалось открыть файл", QMessageBox::Ok);
        return data;
    }
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);
    xmlReader.readNext();   // Переходит к первому элементу в файле
    while(!xmlReader.atEnd())
    {


        if(xmlReader.isStartElement())//является ли элемент началом тега
        {
            qDebug()<<QString("data123: %1").arg(xmlReader.name().toString());
            if(xmlReader.name().toString() == "object")//проверка имени тега
            {
                QMap<QString, QString> object;
                foreach (const QXmlStreamAttribute &attribute, xmlReader.attributes())
                {
                    object.insert( attribute.name().toString(), attribute.value().toString() );
                }
                data.append(object);
//QMessageBox::warning(parent, "Ошибка файла", "считан тег", QMessageBox::Ok);
            }
            //xmlReader.skipCurrentElement();
        }
        xmlReader.readNext();//переход к первому тегу в файле
    }


    file.close();
    return data;
}
