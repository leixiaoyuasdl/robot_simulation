#include "modelview.h"
#include <iostream>
#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>
#include <AIS_InteractiveContext.hxx>
#include <Graphic3d_GraphicDriver.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <WNT_Window.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <TopoDS_Shape.hxx>
#include <AIS_Shape.hxx>
#include <TopoDS.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <STEPControl_Reader.hxx>
#include <IFSelect_ReturnStatus.hxx>
#include <qfiledialog.h>
#include <QtWidgets>
#include"coordinate_system.h"
ModelView::ModelView(QWidget* parent): QWidget(parent)
{
    // No Background
    setBackgroundRole( QPalette::NoRole );

    // set focus policy to threat QContextMenuEvent from keyboard
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);

    // Enable the mouse tracking, by default the mouse tracking is disabled.
    setMouseTracking( true );

    init();
}

void ModelView::init(){
    Handle(Aspect_DisplayConnection) m_display_donnection = new Aspect_DisplayConnection();
    //����OpenGlͼ������
    if (m_graphic_driver.IsNull())
    {
        m_graphic_driver = new OpenGl_GraphicDriver(m_display_donnection);
    }
    //��ȡQWidget�Ĵ���ϵͳ��ʶ��
    WId window_handle = (WId) winId();
    //����Windows NT ����
    Handle(WNT_Window) wind = new WNT_Window((Aspect_Handle) window_handle);
    //����3D�鿴��
    m_viewer = new V3d_Viewer(m_graphic_driver, Standard_ExtString("Visu3D"));
    //������ͼ
    m_view = m_viewer->CreateView();
    m_view->SetWindow(wind);
    //����鿴���е���ͼ
    m_viewer->SetViewOn(m_view);
    //�򿪴���
    if (!wind->IsMapped())
    {
        wind->Map();
    }
    m_context = new AIS_InteractiveContext(m_viewer);  //��������ʽ������
    //���ò鿴���Ĺ���
    m_viewer->SetDefaultLights();
    m_viewer->SetLightOn();
    //������ͼ�ı�����ɫ
    m_view->SetBackgroundColor(Quantity_NOC_GRAY);
    m_view->MustBeResized();
    //��ʾֱ������ϵ�����������ڴ�����ʾλ�á�������ɫ����С����ʽ
    m_view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);
    //������ʾģʽ
    m_context->SetDisplayMode(AIS_Shaded, Standard_True);




    // ����ѡ��ģ�͵ķ��
    Handle(Prs3d_Drawer) t_select_style = m_context->SelectionStyle();  // ��ȡѡ����
    t_select_style->SetMethod(Aspect_TOHM_COLOR);  // ��ɫ��ʾ��ʽ
    t_select_style->SetColor(Quantity_NOC_LIGHTSEAGREEN);   // ����ѡ�����ɫ
    t_select_style->SetDisplayMode(1); // �������
    t_select_style->SetTransparency(0.4f); // ����͸����


    // �����ά����
    m_viewer->SetRectangularGridValues(0,0,1,1,0);
    m_viewer->SetRectangularGridGraphicValues(2.01,2.01,0);
    m_viewer->ActivateGrid(Aspect_GT_Rectangular,Aspect_GDM_Lines);

}

QPaintEngine* ModelView::paintEngine() const
{
    return 0;
}

void ModelView::paintEvent(QPaintEvent *){
    m_view->Redraw();
}

void ModelView::resizeEvent(QResizeEvent *){
    if( !m_view.IsNull() )
    {
        m_view->MustBeResized();
    }
}

void ModelView::mousePressEvent(QMouseEvent *event){
    if((event->buttons()==Qt::LeftButton) && (event->buttons()==Qt::RightButton))
    {
        // ������Ҽ��밴����ʼ��ƽ��
        m_x_max=event->x();
        m_y_max=event->y();
    }
    else if(event->buttons()==Qt::LeftButton)
    {
        // ���ǰ�������λ�ô��ݵ���������
        m_context->MoveTo(event->pos().x(),event->pos().y(),m_view,Standard_True);
        // ��������ѡ��ģ��
        AIS_StatusOfPick t_pick_status = AIS_SOP_NothingSelected;
        if(qApp->keyboardModifiers()==Qt::ControlModifier)
        {
            t_pick_status = m_context->ShiftSelect(true);   // ��ѡ
        }
        else
        {
            t_pick_status = m_context->Select(true);        // ��ѡ
        }
    }
    else if(event->buttons()==Qt::MidButton)
    {
        // �����ּ�����ʼ��ƽ��
        m_x_max=event->x();
        m_y_max=event->y();
        // �����ּ�����ʼ����ת
        m_view->StartRotation(event->x(),event->y());
    }
}

void ModelView::mouseReleaseEvent(QMouseEvent *event){
    // �����λ�ô��ݵ���������
    m_context->MoveTo(event->pos().x(),event->pos().y(),m_view,Standard_True);
}

void ModelView::mouseMoveEvent(QMouseEvent *event){
    if((event->buttons()&Qt::LeftButton) && (event->buttons()&Qt::RightButton))
    {
        // ������Ҽ��밴��ִ��ƽ��
        m_view->Pan(event->pos().x()-m_x_max,m_y_max-event->pos().y());
        m_x_max=event->x();
        m_y_max=event->y();
    }
    else if(event->buttons()&Qt::MidButton)
    {
        // �����ּ�
        if(qApp->keyboardModifiers()==Qt::ShiftModifier)    // �Ұ���Shift��
        {
            // �����ּ���ִ��ƽ��
            m_view->Pan(event->pos().x()-m_x_max,m_y_max-event->pos().y());
            m_x_max=event->x();
            m_y_max=event->y();
        }
        else
        {
            // �����ּ���ִ����ת
            m_view->Rotation(event->x(),event->y());
        }
    }
    else
    {
        // �����λ�ô��ݵ���������
        m_context->MoveTo(event->pos().x(),event->pos().y(),m_view,Standard_True);
    }
}

void ModelView::wheelEvent(QWheelEvent *event){
    m_view->StartZoomAtPoint(event->pos().x(),event->pos().y());
    m_view->ZoomAtPoint(0, 0, event->angleDelta().y(), 0); //ִ������
}
//����ģ��ѡ�񣨱ߣ��㣬ģ�ͣ�
void ModelView::edge(){
    m_context->Deactivate();
    m_context->Activate(2,false);
    m_edge =  TopoDS::Edge(m_context->SelectedShape());
}

void ModelView::Vertex(){
    m_context->Deactivate();
    m_context->Activate(1,false);
    work_vertex = TopoDS::Vertex(m_context->SelectedShape());
}

void ModelView::model(){
    m_context->Deactivate();
    m_context->Activate(0,false);
    m_model = m_context->SelectedShape();
    m_object = m_context->SelectedInteractive();
}

void ModelView::ReadStep(){
    Handle(TopTools_HSequenceOfShape) aSequence= new TopTools_HSequenceOfShape();
    QString fileName = QFileDialog::getOpenFileName(nullptr,
                                                        QObject::tr("read a stp file"),
                                                        "F:",
                                                        QObject::tr("STEP Files(*.stp *.step)"));
    if(fileName.isNull())
    {
        std::cout << "û��ѡ���ļ�" <<std::endl;
        return;
    }
    std::string test = fileName.toStdString();  //����Ҫ��toStdString��data�����ֿ�д��
    Standard_CString aFileName = test.data();
    aSequence->Clear();
    // create additional log file
    STEPControl_Reader aReader;
    IFSelect_ReturnStatus status = aReader.ReadFile(aFileName);
    if(status !=IFSelect_RetDone)
    {
        switch (status)
        {
        case IFSelect_RetError:
            std::cout << "Not a valid Step file" << std::endl;
            break;
        case IFSelect_RetFail :
            std::cout << "Reading has failed" << std::endl;
            break;
        case IFSelect_RetVoid :
            std::cout << "Nothing to transfer" << std::endl;
            break;
        default:
            break;
        }
        return;
    }
    // Root transfers
    aReader.TransferRoots();
    // Collecting resulting entities
    Standard_Integer nbs = aReader.NbShapes();
    if (nbs == 0) return;
    TopoDS_Shape tShape = aReader.OneShape();
    aSequence->Append(tShape);
//    for (Standard_Integer i=1; i<=nbs; i++)
//        aSequence->Append(aReader.Shape(i));

     if (!aSequence.IsNull())
     {
         for(int i=1;i<= aSequence->Length();i++)
         {
             aShape = new AIS_Shape(aSequence->Value(i));

             m_context->SetColor(aShape,Quantity_NOC_YELLOW,Standard_False);
             m_context->SetMaterial(aShape,Graphic3d_NOM_PLASTIC,Standard_False);
             m_context->SetDisplayMode(aShape,1,Standard_False);
             //anInteractiveContext->SetTransparency(aShape,0.2,Standard_False);
             m_context->Display(aShape, Standard_False);
             m_view->FitAll();
         }

     }

}
Handle(AIS_InteractiveContext) ModelView::get_context()
{

    return m_context;
}
Handle(V3d_View) ModelView::get_view()
{

    return m_view;
}

void ModelView::camera_right()
{
    gp_Dir diry(0,-1,0);
    m_view->Camera()->SetDirection(diry);
    gp_Dir dirz(0,0,1);
    m_view->Camera()->SetUp(dirz);
    m_view->FitAll();
}
void ModelView::camera_left()
{
    gp_Dir diry(0,1,0);
    m_view->Camera()->SetDirection(diry);
    gp_Dir dirz(0,0,1);
    m_view->Camera()->SetUp(dirz);
    m_view->FitAll();
}
void ModelView::camera_up()
{
    gp_Dir diry(0,0,-1);
    m_view->Camera()->SetDirection(diry);
    gp_Dir dirz(-1,0,0);
    m_view->Camera()->SetUp(dirz);
    m_view->FitAll();
}
void ModelView::camera_down()
{
    gp_Dir diry(0,0,1);
    m_view->Camera()->SetDirection(diry);
    gp_Dir dirz(1,0,0);
    m_view->Camera()->SetUp(dirz);
    m_view->FitAll();
}
void ModelView::camera_front()
{
    gp_Dir diry(-1,0,0);
    m_view->Camera()->SetDirection(diry);
    gp_Dir dirz(0,0,1);
    m_view->Camera()->SetUp(dirz);
    m_view->FitAll();
}
void ModelView::camera_back()
{
    gp_Dir diry(1,0,0);
    m_view->Camera()->SetDirection(diry);
    gp_Dir dirz(0,0,1);
    m_view->Camera()->SetUp(dirz);
    m_view->FitAll();
}
