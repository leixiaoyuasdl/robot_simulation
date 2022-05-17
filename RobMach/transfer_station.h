#ifndef TRANSFER_STATION_H
#define TRANSFER_STATION_H
#include <QWidget>
#include"modelview.h"
#include"modelxml.h"
#include"solverpos.h"
#include"animation.h"
#include<QObject>
#include<QTextEdit>
#include"information.h"
#include"joint_angle.h"
// 按钮现在不直接连接相应的功能函数，而是在这里中转一道(除了 modelview 类)

class Transfer_station:public QWidget
{
    Q_OBJECT
public:
    Transfer_station();

public slots:
    void readxml();
    void readpath();
    void remove();      //停止一切活动  删除当前上下文的所有对象
    void start_animation();  //开始动画
    void forward();       //快进2倍
    void back();         //快退2倍
    void pause();          //暂停动画
    void a_continue();    //恢复动画
    void erase_path();
    void redisplay_path();
    void erase_model();
    void redisplay_model();
    void erase();
    void redisplay();
    void set_joint_angel();
public:
    ModelView *modelview;
    QTextEdit *information_text;
    joint_angle *joint_angle_widget;
private:
    modelxml *ModelXml;
    SolverPos *s_pos;
    animation *anima = new animation();
    QVector<Handle( AIS_InteractiveObject)> model_ais_shape;



};

#endif // TRANSFER_STATION_H
