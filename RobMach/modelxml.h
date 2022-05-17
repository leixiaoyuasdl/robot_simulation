#ifndef MODELXML_H
#define MODELXML_H
#include <QWidget>
#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>
#include <AIS_InteractiveContext.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <AIS_Shape.hxx>
#include <QMouseEvent>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>

#include <MeshVS_Mesh.hxx>
#include<QTreeWidgetItem>
#include<QXmlStreamReader>
#include <gp_Quaternion.hxx>
#include <Eigen/Dense>
#include <frames.hpp>
#include<joint.hpp>
#include <chain.hpp>
#include <jntarray.hpp>
#include<gp_Pnt.hxx>
#include"modelview.h"
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include"information.h"
#include<QTextEdit>
#include"joint_angle.h"
#include<QLabel>
#include<QPushButton>
using namespace Eigen;
using namespace KDL;

//功能： 读取xml 文件，建立模型
//      建立运动链 并加入关节角限制

struct anobject
{
    double pos_x,pos_y,pos_z;          //储存标签的pos  x,y,z
    double rpy_x,rpy_y,rpy_z;          //储存标签的rpy  x,y,z
    QString name,refframe;             //储存标签的name,refframe 属性
    gp_Quaternion m_trsf_Rotation;     //将RPY 转化为 gp_Quaternion
    gp_Vec m_trsf_Translation;         //将POS 转化为 gp_Vec
    gp_Trsf o_gp;                      //将gp_Quaternion和gp_Vec  转化为 gp_Trsf
    double max,min;                    //储存 poslimit 标签的 最大关节角和最小关节角
};

class modelxml:public QWidget
{
    Q_OBJECT
private:
    QString xmlfilename;
    QXmlStreamReader reader;

public:
    ModelView *modelview;                 //继承modeview 类
    JntArray j_max,j_min;                 //最大关节角，最小关节角
    anobject o_object[3][4][10];          //描述 第a个serial标签下  b类标签的  第c个标签
    int sum[3][3]={0};                    //记录 每个serial标签下有多少个 种类为1 的标签
    Handle(AIS_Shape) meshes[3][10];      //储存 每个serial标签下读到的stl模型
    Chain pum;                            //运动链
    QVector<Handle(AIS_Shape)> model_ais_shape;
    QTextEdit *information_text;
    joint_angle *joint_angle_widget;
     //QVector<QLineEdit> joint_angle_text;
    QLineEdit *line[10];
    QLabel *label[10] ;
    int line_sum = 0;
    QPushButton *button_set_angel;

public:
    modelxml();
    void readWorkCellElement();
    void readSerialDeviceElement(int a);
    void readFrameElement(int a,int b,int c);    //a,b,c   第a个serial标签  （frame 和 joint看作同一种类标签（b=1）  drawable看作另一类标签(b=2) poslimit(b=3)） c是某类标签下的第几个标签
    void readJointElement(int a,int b,int c);
    void readDrawableElement(int a,int b,int c);
    void readPolytopeElement(int a,int b,int c);
    void readRPYElement(int a,int b,int c);
    void readPosElement(int a,int b,int c);
    void skipUnknownElement();
    void readPosLimitElement(int a,int b,int c);
    void addchain();            //建立运动链
    void addlimit();            //加入关节角限制
    Handle(AIS_Shape) Readstl(Standard_CString filename);
    void settool();
public slots:
    bool Readxml();

};

#endif // MODELXML_H
