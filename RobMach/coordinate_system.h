#ifndef COORDINATE_SYSTEM_H
#define COORDINATE_SYSTEM_H
#include <gp_Ax2.hxx>
#include<gp_Trsf.hxx>
#include"modelview.h"
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
//坐标系 绘制，设置，删除

class coordinate_system
{
public:
    coordinate_system();
    coordinate_system(gp_Trsf g_gp,double len);   //坐标系初始化设置 姿态g_gp , 坐标轴长度 单位m
    void set(gp_Trsf g_gp,double len);            //坐标系设置 同上
    void draw();                                  //坐标系绘制
    void hide();
public:
    //ModelView modelview;
     Handle(AIS_InteractiveContext) m_context;      //储存要显示的上下文
     Handle(AIS_Shape) ais_edgex;                  //X轴 线对象
     Handle(AIS_Shape) ais_edgey;                   //y轴 线对象
     Handle(AIS_Shape) ais_edgez;                    //z轴 线对象
     Handle(AIS_Shape) pnt;                         //坐标系原点 点对象
private:
    gp_Trsf gp;                                    //储存坐标系姿态
    double l;                                      //储存坐标系长度



};

#endif // COORDINATE_SYSTEM_H
