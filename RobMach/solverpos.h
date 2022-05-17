#ifndef SOLVERPOS_H
#define SOLVERPOS_H
#include"modelxml.h"
#include"coordinate_system.h"
#include <QWidget>
#include <iostream>
#include <Eigen/Dense>
#include <frames.hpp>
#include<joint.hpp>
#include <chain.hpp>
#include <jntarray.hpp>
using namespace Eigen;
using namespace KDL;
#include<QVector>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include<QTextEdit>
//功能： 读取路径点保存文件 （TXT） 然后逆解出关节角 并 实现动画

class SolverPos:public QWidget
{
     Q_OBJECT
private:
    QVector<JntArray> jnt_array;               //储存每一路径点 逆解出来的关节角
    coordinate_system draw;

public:
    QVector<coordinate_system> path_coordinate_system;
    modelxml *axml;       //储存modelxml 类
    QVector<QVector<gp_Trsf>>  gptr_array;    //储存每一路径点 每一关节的 姿态 从0关节开始 ，末端没有记录
    QTextEdit *information_text;
public:
    SolverPos();
    void translation(QVector<JntArray> array);   //将关节角 转换为 每一关节的姿态 姿态 从0关节开始 ，末端没有记录
    JntArray solver(JntArray q_init,Frame pos_goal);  //求逆解  给定初始关节角 q_init 末端姿态 pos_goal 返回 逆解出的关节角
public slots:
    bool readpath();   //读取路径点 保存在TXT文件

};

#endif // SOLVERPOS_H
