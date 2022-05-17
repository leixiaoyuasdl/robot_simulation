/*
*展示三维界面
*/

#ifndef MODELVIEW_H
#define MODELVIEW_H

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



class ModelView: public QWidget
{
    Q_OBJECT

public:
    ModelView(QWidget* parent = 0);

protected:
    //!三维场景转换模式
    enum CurrentAction3d
    {
        CurAction3d_Nothing,
        CurAction3d_DynamicPanning, //平移
        CurAction3d_DynamicZooming, //缩放
        CurAction3d_DynamicRotation //旋转
    };
public:
    Handle(AIS_InteractiveContext) get_context();
    Handle(V3d_View) get_view();
public:
    //交互式上下文能够管理一个或多个查看器(viewer)中的图形行为和交互式对象的选择
    Handle(AIS_InteractiveContext) m_context;
    //定义查看器(viewer)类型对象上的服务
    Handle(V3d_Viewer) m_viewer;
    //创建一个视图
    Handle(V3d_View) m_view;
private:

    //创建3d接口定义图形驱动程序
    Handle(Graphic3d_GraphicDriver) m_graphic_driver;
    Handle(AIS_Shape) aShape;
    Standard_Integer m_x_max;    //记录鼠标平移坐标X
    Standard_Integer m_y_max;    //记录鼠标平移坐标Y
    CurrentAction3d m_current_mode; //三维场景转换模式
    TopoDS_Vertex work_vertex;
    TopoDS_Shape m_model;
    TopoDS_Edge m_edge;
    Handle(AIS_InteractiveObject) m_object;
protected:
    void init();
    virtual QPaintEngine* paintEngine() const;
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);


public:

public slots:
    void ReadStep();
    void edge();
    void Vertex();
    void model();
    void camera_front();     //调整视角 front 正面 下面类似
    void camera_back();
    void camera_up();
    void camera_down();
    void camera_right();
    void camera_left();

};

#endif // MODELVIEW_H
