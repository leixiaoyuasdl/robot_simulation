/*
*用来显示软件总体界面
*
*/

#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include "modelview.h"
#include "modeltree.h"
#include "modelxml.h"
#include "solverpos.h"
#include"transfer_station.h"
#include"information.h"
#include"joint_angle.h"
namespace Ui {
class View;
}

class View : public QMainWindow
{
    Q_OBJECT

public:
    explicit View(QWidget *parent = 0);
    ~View();
private:
    Ui::View  *ui;
    ModelView *modelView;
    ModelTree *modelTree;
    information *information_widget;
    joint_angle *joint_angle_widget;
    Transfer_station *station;

protected:
    void createToolBars();
    void createActions();
};

#endif // VIEW_H
