#include "view.h"
#include "ui_view.h"
#include "modelview.h"
#include "modeltree.h"
#include "modelxml.h"
#include "solverpos.h"
#include"transfer_station.h"
#include"information.h"
#include<QTextEdit>
#include"joint_angle.h"
View::View(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::View)
{


    ui->setupUi(this);
    modelView = new ModelView(this);
    setCentralWidget(modelView);

    //create modeltree
    modelTree = new ModelTree(this);
    modelTree->setAllowedAreas(Qt::AllDockWidgetAreas);
    this->addDockWidget(Qt::LeftDockWidgetArea, modelTree);
    //show or hide modeltree
    ui->menutool->addAction(modelTree->toggleViewAction());

    //create information_widget
    information_widget = new information(this);
    information_widget->setAllowedAreas(Qt::BottomDockWidgetArea);
    information_widget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    QTextEdit *information_text = new QTextEdit();
    information_text->setReadOnly(true);
    information_widget->setWidget(information_text);
    this->addDockWidget(Qt::BottomDockWidgetArea, information_widget);

    //create joint_angle
    joint_angle_widget = new joint_angle(this);
    joint_angle_widget->setAllowedAreas(Qt::AllDockWidgetAreas);
    this->addDockWidget(Qt::RightDockWidgetArea, joint_angle_widget);
    //show or hide modeltree
    ui->menutool->addAction(joint_angle_widget->toggleViewAction());
    joint_angle_widget->hide();

    station = new Transfer_station;
    station->modelview =  modelView;
    station->information_text = information_text;
    station->joint_angle_widget = joint_angle_widget;
    createToolBars();
    createActions();
}

View::~View()
{
    delete ui;
}

void View::createToolBars(){
//    QToolBar* aToolBar = addToolBar(tr("&File"));
//    aToolBar->addAction(ui->actionOpenStep);
}

void View::createActions(){
    connect(ui->actionOpenStep, SIGNAL(triggered()), modelView, SLOT(ReadStep()));
    connect(ui->actionvertex, SIGNAL(triggered()), modelView, SLOT(Vertex()));
    connect(ui->actionedge, SIGNAL(triggered()), modelView, SLOT(edge()));
    connect(ui->actionmodel, SIGNAL(triggered()), modelView, SLOT(model()));


    connect(ui->actionfront, SIGNAL(triggered()), modelView, SLOT(camera_front()));
    connect(ui->actionback_2, SIGNAL(triggered()), modelView, SLOT(camera_back()));
    connect(ui->actionup, SIGNAL(triggered()), modelView, SLOT(camera_up()));
    connect(ui->actiondown, SIGNAL(triggered()), modelView, SLOT(camera_down()));
    connect(ui->actionright, SIGNAL(triggered()), modelView, SLOT(camera_right()));
    connect(ui->actionleft, SIGNAL(triggered()), modelView, SLOT(camera_left()));

    connect(ui->actionremove, SIGNAL(triggered()), station, SLOT(remove()));
    connect(ui->actionOpenXml, SIGNAL(triggered()), station, SLOT(readxml()));
    connect(ui->actionReadPath, SIGNAL(triggered()), station, SLOT(readpath()));
    connect(ui->actionstart, SIGNAL(triggered()), station, SLOT(start_animation()));
    connect(ui->actionforward, SIGNAL(triggered()), station, SLOT(forward()));
    connect(ui->actionback, SIGNAL(triggered()), station, SLOT(back()));
    connect(ui->actionpause, SIGNAL(triggered()), station, SLOT(pause()));
    connect(ui->actioncontinue, SIGNAL(triggered()), station, SLOT(a_continue()));

     connect(ui->actionerase_path, SIGNAL(triggered()), station, SLOT(erase_path()));
     connect(ui->actionredisplay_path, SIGNAL(triggered()), station, SLOT(redisplay_path()));

     connect(ui->actionerase_model, SIGNAL(triggered()), station, SLOT(erase_model()));
     connect(ui->actionredisplay_model, SIGNAL(triggered()), station, SLOT(redisplay_model()));

     connect(ui->actionerase, SIGNAL(triggered()), station, SLOT(erase()));
     connect(ui->actionredisplay, SIGNAL(triggered()), station, SLOT(redisplay()));
}

