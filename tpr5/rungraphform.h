#ifndef RUNGRAPHFORM_H
#define RUNGRAPHFORM_H
#include <QDialog>
#include <QEvent>
#include <QList>
#include <QMap>
#include <QCloseEvent>
#include "graphtree.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include "dialogqvest.h"
#include <QMessageBox>

class RunGraphForm : public QDialog//прохождение графа
{
    Q_OBJECT
public:
    explicit RunGraphForm(QWidget *parent = 0);
    ~RunGraphForm();
    bool setGraph(QString name);
private:
    GraphTree *graphWidget;
    DialogQvest* quest;
    void build();
    QScrollArea *area;

private slots:
     void closeEvent(QCloseEvent *e);
     void branchSelection(int id);
};

#endif // RUNGRAPHFORM_H
