/*
*չʾ��ά����
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
    //!��ά����ת��ģʽ
    enum CurrentAction3d
    {
        CurAction3d_Nothing,
        CurAction3d_DynamicPanning, //ƽ��
        CurAction3d_DynamicZooming, //����
        CurAction3d_DynamicRotation //��ת
    };
public:
    Handle(AIS_InteractiveContext) get_context();
    Handle(V3d_View) get_view();
public:
    //����ʽ�������ܹ�����һ�������鿴��(viewer)�е�ͼ����Ϊ�ͽ���ʽ�����ѡ��
    Handle(AIS_InteractiveContext) m_context;
    //����鿴��(viewer)���Ͷ����ϵķ���
    Handle(V3d_Viewer) m_viewer;
    //����һ����ͼ
    Handle(V3d_View) m_view;
private:

    //����3d�ӿڶ���ͼ����������
    Handle(Graphic3d_GraphicDriver) m_graphic_driver;
    Handle(AIS_Shape) aShape;
    Standard_Integer m_x_max;    //��¼���ƽ������X
    Standard_Integer m_y_max;    //��¼���ƽ������Y
    CurrentAction3d m_current_mode; //��ά����ת��ģʽ
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
    void camera_front();     //�����ӽ� front ���� ��������
    void camera_back();
    void camera_up();
    void camera_down();
    void camera_right();
    void camera_left();

};

#endif // MODELVIEW_H
