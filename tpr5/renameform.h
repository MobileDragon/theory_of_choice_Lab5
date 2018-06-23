#ifndef RENAMEFORM_H
#define RENAMEFORM_H
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QDebug>
#include <QPushButton>
#include <QEvent>
#include <QCloseEvent>
#include <QLineEdit>


class RenameForm : public QDialog
{
    Q_OBJECT
public:
    explicit RenameForm(int id, int typeElemet, QString name, QWidget *parent = 0);
    ~RenameForm();
signals:
  void renameSignal(int id, int type, QString);
  void closeForm(RenameForm* obj);

private:
    void buildForm();
    int idObject;
    int type;//тип элемента, который следует переименовать
    QString text;

    QPushButton* ok;
    QLineEdit* lineEdit;
    QLabel *label;
    QVBoxLayout* mainVLayout;
private slots:
     void okReleased();
     void nameChanged(QString);
     void closeEvent(QCloseEvent *e);
};

#endif // RENAMEFORM_H
