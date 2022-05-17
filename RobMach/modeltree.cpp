#include "modeltree.h"
#include "ui_modeltree.h"
#include <QDebug>

ModelTree::ModelTree(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ModelTree)
{
    ui->setupUi(this);

    modelList =new QTreeWidget(this);
    modelList->setColumnCount(1);
    modelList->setHeaderLabel(tr("Robot"));//set title
    modelList->setGeometry(0,20,this->width(),this->height());//set size
    //parent node
    Item1 = new QTreeWidgetItem(modelList,QStringList(QString("model")));
    Item2 = new QTreeWidgetItem(modelList,QStringList(QString("frame")));
    modelList->insertTopLevelItem(0, Item1);
    modelList->insertTopLevelItem(1, Item2);

    QTreeWidgetItem *fItem1a = new QTreeWidgetItem(Item1,QStringList(QString("f1a")));

    connect(modelList,&QTreeWidget::itemClicked,this,&ModelTree::itemClick);//click once
    connect(modelList,&QTreeWidget::itemDoubleClicked,this,&ModelTree::itemClickDouble);//click double
}

ModelTree::~ModelTree()
{
    delete ui;
}

void ModelTree::itemClick(QTreeWidgetItem *item, int column){
    qDebug()<<"click once";
}

void ModelTree::itemClickDouble(QTreeWidgetItem *item, int column){
    qDebug()<<"double";
}
