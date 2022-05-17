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
    //创建OpenGl图形驱动
    if (m_graphic_driver.IsNull())
    {
        m_graphic_driver = new OpenGl_GraphicDriver(m_display_donnection);
    }
    //获取QWidget的窗口系统标识符
    WId window_handle = (WId) winId();
    //创建Windows NT 窗口
    Handle(WNT_Window) wind = new WNT_Window((Aspect_Handle) window_handle);
    //创建3D查看器
    m_viewer = new V3d_Viewer(m_graphic_driver, Standard_ExtString("Visu3D"));
    //创建视图
    m_view = m_viewer->CreateView();
    m_view->SetWindow(wind);
    //激活查看器中的视图
    m_viewer->SetViewOn(m_view);
    //打开窗口
    if (!wind->IsMapped())
    {
        wind->Map();
    }
    m_context = new AIS_InteractiveContext(m_viewer);  //创建交互式上下文
    //配置查看器的光照
    m_viewer->SetDefaultLights();
    m_viewer->SetLightOn();
    //设置视图的背景颜色
    m_view->SetBackgroundColor(Quantity_NOC_GRAY);
    m_view->MustBeResized();
    //显示直角坐标系，可以配置在窗口显示位置、文字颜色、大小、样式
    m_view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);
    //设置显示模式
    m_context->SetDisplayMode(AIS_Shaded, Standard_True);




    // 设置选择模型的风格
    Handle(Prs3d_Drawer) t_select_style = m_context->SelectionStyle();  // 获取选择风格
    t_select_style->SetMethod(Aspect_TOHM_COLOR);  // 颜色显示方式
    t_select_style->SetColor(Quantity_NOC_LIGHTSEAGREEN);   // 设置选择后颜色
    t_select_style->SetDisplayMode(1); // 整体高亮
    t_select_style->SetTransparency(0.4f); // 设置透明度


    // 激活二维网格
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
        // 鼠标左右键齐按：初始化平移
        m_x_max=event->x();
        m_y_max=event->y();
    }
    else if(event->buttons()==Qt::LeftButton)
    {
        // 点击前，将鼠标位置传递到交互环境
        m_context->MoveTo(event->pos().x(),event->pos().y(),m_view,Standard_True);
        // 鼠标左键：选择模型
        AIS_StatusOfPick t_pick_status = AIS_SOP_NothingSelected;
        if(qApp->keyboardModifiers()==Qt::ControlModifier)
        {
            t_pick_status = m_context->ShiftSelect(true);   // 多选
        }
        else
        {
            t_pick_status = m_context->Select(true);        // 单选
        }
    }
    else if(event->buttons()==Qt::MidButton)
    {
        // 鼠标滚轮键：初始化平移
        m_x_max=event->x();
        m_y_max=event->y();
        // 鼠标滚轮键：初始化旋转
        m_view->StartRotation(event->x(),event->y());
    }
}

void ModelView::mouseReleaseEvent(QMouseEvent *event){
    // 将鼠标位置传递到交互环境
    m_context->MoveTo(event->pos().x(),event->pos().y(),m_view,Standard_True);
}

void ModelView::mouseMoveEvent(QMouseEvent *event){
    if((event->buttons()&Qt::LeftButton) && (event->buttons()&Qt::RightButton))
    {
        // 鼠标左右键齐按：执行平移
        m_view->Pan(event->pos().x()-m_x_max,m_y_max-event->pos().y());
        m_x_max=event->x();
        m_y_max=event->y();
    }
    else if(event->buttons()&Qt::MidButton)
    {
        // 鼠标滚轮键
        if(qApp->keyboardModifiers()==Qt::ShiftModifier)    // 且按下Shift键
        {
            // 鼠标滚轮键：执行平移
            m_view->Pan(event->pos().x()-m_x_max,m_y_max-event->pos().y());
            m_x_max=event->x();
            m_y_max=event->y();
        }
        else
        {
            // 鼠标滚轮键：执行旋转
            m_view->Rotation(event->x(),event->y());
        }
    }
    else
    {
        // 将鼠标位置传递到交互环境
        m_context->MoveTo(event->pos().x(),event->pos().y(),m_view,Standard_True);
    }
}

void ModelView::wheelEvent(QWheelEvent *event){
    m_view->StartZoomAtPoint(event->pos().x(),event->pos().y());
    m_view->ZoomAtPoint(0, 0, event->angleDelta().y(), 0); //执行缩放
}
//进行模型选择（边，点，模型）
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
        std::cout << "没有选择文件" <<std::endl;
        return;
    }
    std::string test = fileName.toStdString();  //必须要把toStdString和data函数分开写。
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
