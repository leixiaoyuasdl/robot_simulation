#ifndef MODETREE_H
#define MODETREE_H

#include <QDockWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>

namespace Ui {
class ModelTree;
}

class ModelTree : public QDockWidget
{
    Q_OBJECT

public:
    explicit ModelTree(QWidget *parent = 0);
    ~ModelTree();

private:
    Ui::ModelTree *ui;

    QTreeWidget *modelList;
    QTreeWidgetItem *Item1;
    QTreeWidgetItem *Item2;

public slots:
    void itemClick(QTreeWidgetItem *item, int column);
    void itemClickDouble(QTreeWidgetItem *item, int column);
};

#endif // MODETREE_H
