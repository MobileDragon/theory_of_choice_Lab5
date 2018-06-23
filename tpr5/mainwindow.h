#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graphtree.h"
#include "rungraphform.h"
#include <QScrollArea>
#include <QPushButton>
#include <QDir>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    GraphTree *graphWidget;
    QScrollArea *area;
    //
    QPushButton *addNode;
    QPushButton *addLine;
    QPushButton *cutExtraSpace;
    QPushButton *saveAs;
    QPushButton *openFile;
    QPushButton *runGraph;
    QPushButton *newGraph;
    void build();

private:
    Ui::MainWindow *ui;
    QString tempFile;

public slots:
    void addNodeReleased();
    void addLineReleased();
    void cutExtraSpaceReleased();
    void saveAsReleased();
    void openFileReleased();
    void runGraphReleased();
    void newGraphReleased();
};

#endif // MAINWINDOW_H
