#include "modelxml.h"
#include <QtWidgets>
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

#include<QDialog>

#include <QStringList>
#include <Poly_Triangulation.hxx>
#include <XSDRAWSTLVRML_DataSource.hxx>
#include <MeshVS_Mesh.hxx>
#include <MeshVS_MeshPrsBuilder.hxx>
#include <MeshVS_Drawer.hxx>
#include<MeshVS_DrawerAttribute.hxx>
#include <RWStl.hxx>
#include<gp_EulerSequence.hxx>
#include <gp_Quaternion.hxx>
#include<QDebug>

#include <Eigen/Dense>
#include <frames.hpp>
#include<joint.hpp>
#include <chain.hpp>
#include <jntarray.hpp>
using namespace Eigen;
using namespace KDL;


#include <BRep_Builder.hxx>
#include <Draw_ProgressIndicator.hxx>
#include <Message_ProgressIndicator.hxx>
#include <DBRep.hxx>
#include <TopoDS_Face.hxx>

#include <BRepBuilderAPI_MakeVertex.hxx>

#include <gp_Dir.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include<gp_Pnt.hxx>
#include<Geom_TrimmedCurve.hxx>
#include <GC_MakeSegment.hxx>
#include"coordinate_system.h"
#include<QLineEdit>
#include<QVector>
#include<QPushButton>
#include<QDoubleValidator>
#include<transfer_station.h>
modelxml::modelxml()
{

}



Handle(AIS_Shape)  modelxml::Readstl(Standard_CString filename)
{
    TopoDS_Shape aShape;
    Handle( Message_ProgressIndicator) aProgress ;
    Handle(Poly_Triangulation) aTriangulation = RWStl::ReadFile (filename, aProgress);

    TopoDS_Face aFace;
    BRep_Builder aB;
    aB.MakeFace (aFace);
    aB.UpdateFace (aFace, aTriangulation);
    aShape = aFace;

    Handle(AIS_Shape) shape = new AIS_Shape(aFace);

<<<<<<< HEAD
=======

>>>>>>> c4ff94bbf0db489e74a4af36be3499e72d03de24
    modelview->m_context->SetColor(shape,Quantity_NOC_YELLOW,Standard_False);
    modelview->m_context->SetDisplayMode(shape,1,false);
     modelview->m_context->Display(shape, Standard_False);
 //    modelview->m_context->AddSelect(shape);


     return shape;



    /* Handle( Message_ProgressIndicator) aProgress ;
    Handle(Poly_Triangulation) aSTLMesh = RWStl::ReadFile (filename, aProgress);
    Handle( XSDRAWSTLVRML_DataSource ) aDS = new XSDRAWSTLVRML_DataSource( aSTLMesh );

    // create mesh.
    Handle( MeshVS_Mesh ) aMesh = new MeshVS_Mesh();
    aMesh->SetDataSource( aDS );

    // use default presentation builder.
  //  Handle_MeshVS_MeshPrsBuilder aBuilder = new MeshVS_MeshPrsBuilder(aMesh,MeshVS_DMF_User   );
  //  aMesh->AddBuilder(aBuilder, Standard_True);

    modelview->m_context->SetColor(aMesh,Quantity_NOC_YELLOW,Standard_False);
    modelview->m_context->SetDisplayMode(aMesh,1,false);
     modelview->m_context->Display(aMesh, Standard_False);
     modelview->m_context->AddSelect(aMesh);

    return aMesh;*/
}

bool modelxml::Readxml()
{
    information_text->append(" opening xml file...");
    qDebug()<<"xml start";
    QString fileName = QFileDialog::getOpenFileName(nullptr,
                                                          QObject::tr("read a xml file"),
                                                          "F:",
                                                          QObject::tr("STEP Files(*.xml)"));

    QFileInfo fileinfo;
  fileinfo = QFileInfo(fileName);
  xmlfilename=fileinfo.absolutePath();

      if(fileName.isNull())
      {
          std::cout << "没有选择文件" <<std::endl;
          information_text->append(" no select xml file ");
          return false;
      }
      std::string test = fileName.toStdString();  //必须要把toStdString和data函数分开写。
      Standard_CString aFileName = test.data();




      QFile file(aFileName);
      if (!file.open(QFile::ReadOnly | QFile::Text))
      {

         // QMessageBox::critical(this, tr("Error"),
        //   tr("Cannot read file %1").arg(fileName));
          return false;
      }
      reader.setDevice(&file);
      while (!reader.atEnd())
      {
          if (reader.isStartElement())
          {
              if (reader.name() == "WorkCell")
              {
                  readWorkCellElement();//递归下降算法，层层读取
              }
              else
              {reader.raiseError(tr("Not a valid book file")); }
          }
          else
          {
              reader.readNext(); //循坏调用首次移动3次，后面移动一次
          }
      }
      file.close();

      if (reader.hasError())
            {
             //   QMessageBox::critical(this, tr("Error"),  tr("Failed to parse file %1").arg(fileName));
                return false;
            }    else
                if (file.error() != QFile::NoError)
                {
                 //   QMessageBox::critical(this, tr("Error"),tr("Cannot read file %1").arg(fileName));
                    return false;
                }

   /*   gp_Trsf gp ;
      gp_Quaternion aQ;
      aQ.SetEulerAngles(gp_YawPitchRoll,0,0,0);
      gp_Vec gvec(0,0,0);
      gp.SetTransformation(aQ,gvec);
   modelview->m_view->Camera()->Transform(gp);*/

  // gp_Dir dir(0,1,0);
   //modelview->m_view->Camera()->SetDirection(dir);

      //modelview->m_context->FitSelected(modelview->m_view);
    //  modelview->m_context->ClearSelected(false);
      modelview->m_view->FitAll();
      addchain();
      addlimit();
      settool();
      qDebug()<<"xml文件读取完成";
      information_text->append(" xml文件读取完成");
      return true;

}

void modelxml::readWorkCellElement()
{
    int i=1;
    reader.readNext(); // 读取下一个记号，它返回记号的类型
    while (!reader.atEnd())
    {
        if (reader.isEndElement())
        {
            reader.readNext();
            break;
        }
        if (reader.isStartElement())
        {
            if (reader.name() == "SerialDevice")
            {
                readSerialDeviceElement(i);
                i++;
            }
            else     {skipUnknownElement();}
        }
        else {reader.readNext();}
    }




}

void modelxml::readSerialDeviceElement(int a)
{
    reader.readNext();
    int i=0,j=0,k=0;
    while (!reader.atEnd())
    {
        if (reader.isEndElement())
        { reader.readNext(); break;}
        if (reader.isStartElement())
        {
            if (reader.name() == "Frame")
            {readFrameElement(a,1,i);sum[a][1]++;i++;}
            else
                if (reader.name() == "Joint")
                {readJointElement(a,1,i);sum[a][1]++;i++; }
                 else
                    if (reader.name() == "Drawable")
                    {readDrawableElement(a,2,j);j++; }
                    else
                        if (reader.name() == "PosLimit")
                        {readPosLimitElement(a,3,k);k++; }
                            else  {skipUnknownElement();}
        }
        else{reader.readNext();}
    }


}

void modelxml::readPosLimitElement(int a,int b,int c)
{
        o_object[a][b][c].name = reader.attributes().value("refjoint").toString();
        o_object[a][b][c].min = reader.attributes().value("min").toDouble();
        o_object[a][b][c].max = reader.attributes().value("max").toDouble();
        reader.readNext();
        reader.readNext();
}
void  modelxml::readFrameElement(int a,int b,int c)
{
    o_object[a][b][c].name = reader.attributes().value("name").toString();

    reader.readNext();
    while (!reader.atEnd())
    {
        if (reader.isEndElement())
        { reader.readNext(); break;}
        if (reader.isStartElement())
        {
            if (reader.name() == "RPY")
            {readRPYElement(a,b,c);}
            else
                if (reader.name() == "Pos")
                {readPosElement(a,b,c);}
                else {skipUnknownElement();}
        }
        else{reader.readNext();}
    }

}

void modelxml::readJointElement(int a,int b,int c)
{
    QDoubleValidator* lineDouble = new QDoubleValidator(0, 360, 3, joint_angle_widget);

    label[c] = new QLabel;
   label[c]->setText("joint"+QString::number(c,10));
   line[c] = new QLineEdit("0");
   line[c]->setValidator(lineDouble);
   line_sum++;



    o_object[a][b][c].name = reader.attributes().value("name").toString();
    reader.readNext();
    while (!reader.atEnd())
    {
        if (reader.isEndElement())
        { reader.readNext(); break;}
        if (reader.isStartElement())
        {
            if (reader.name() == "RPY")
            {readRPYElement(a,b,c);}
            else
                if (reader.name() == "Pos")
                {readPosElement(a,b,c);}
                else {skipUnknownElement();}
        }
        else{reader.readNext();}
    }
}

void modelxml::readDrawableElement(int a,int b,int c)
{
    o_object[a][b][c].name = reader.attributes().value("name").toString();
    o_object[a][b][c].refframe = reader.attributes().value("refframe").toString();
    reader.readNext();
    while (!reader.atEnd())
    {
        if (reader.isEndElement())
        { reader.readNext(); break;}
        if (reader.isStartElement())
        {
            if (reader.name() == "RPY")
            {readRPYElement(a,b,c);}
            else
                if (reader.name() == "Pos")
                {readPosElement(a,b,c);}
                   else
                    if (reader.name() == "Polytope")
                    {readPolytopeElement(a,b,c);}
                    else {skipUnknownElement();}
        }
        else{reader.readNext();}
    }
}


void modelxml::skipUnknownElement()
{
    reader.readNext();
    while (!reader.atEnd())
    {
        if (reader.isEndElement())
        {reader.readNext();break;}
        if (reader.isStartElement())
        {skipUnknownElement();}
        else
        {reader.readNext();}
    }
}
void modelxml::readRPYElement(int a,int b,int c)
{

    double x,y,z;
    QString str = reader.readElementText();
    str=str.trimmed();
    if (reader.isEndElement())
    {
        reader.readNext();
    }
    QStringList strList = str.split(" ");
    x=strList[0].toDouble();
    y=strList[1].toDouble();
    z=strList[2].toDouble();


    x = x/180*M_PI;
    y = y/180*M_PI;
    z = z/180*M_PI;

    o_object[a][b][c].rpy_x=x;
    o_object[a][b][c].rpy_y=y;
    o_object[a][b][c].rpy_z=z;
      //设置欧拉角
      gp_Quaternion aQ;
      aQ.SetEulerAngles(gp_YawPitchRoll ,x,y,z);
      o_object[a][b][c].m_trsf_Rotation= aQ;
}
void modelxml::readPosElement(int a,int b,int c)
{
    double x,y,z;
    QString str = reader.readElementText();
    str=str.trimmed();
    if (reader.isEndElement())
    {
        reader.readNext();
    }
    QStringList strList = str.split(" ");
    x=strList[0].toDouble();
    y=strList[1].toDouble();
    z=strList[2].toDouble();
    o_object[a][b][c].pos_x=x;
    o_object[a][b][c].pos_y=y;
    o_object[a][b][c].pos_z=z;
        //设置平移向量
    gp_Trsf aTrsf_Translation;
    gp_Vec theVectorOfTrans(x,y,z);
o_object[a][b][c].m_trsf_Translation = theVectorOfTrans;

o_object[a][b][c].o_gp.SetTransformation(o_object[a][b][c].m_trsf_Rotation,theVectorOfTrans);


}
void modelxml::readPolytopeElement(int a,int b,int c)
{
    QString stlfilename;
    stlfilename = xmlfilename + "/" + reader.attributes().value("file").toString()+".stl";
    reader.readNext();
    reader.readNext();
    int sum;
    for (int i=0;;i++)
    {
      if(o_object[a][b][c].refframe == o_object[a][b-1][i].name)
      {
          sum = i;
          break;
      }

    }
    std::string test = stlfilename.toStdString();  //必须要把toStdString和data函数分开写。
    Standard_CString astlfilename = test.data();
    Handle(AIS_Shape) anmesh = Readstl(astlfilename);
    meshes[a][c] = anmesh;

    model_ais_shape.append(anmesh);



    gp_Trsf  gp ,base;
    base = o_object[1][1][0].o_gp;
    if(a==2) gp =base * o_object[2][1][0].o_gp  ;
    else gp =base;
    for(int i=1;i<=sum;i++)
        gp=gp*o_object[a][b-1][i].o_gp;
    gp=gp*o_object[a][b][c].o_gp;
    modelview->m_context->SetLocation(anmesh,gp);
    modelview->m_context->Update(anmesh,Standard_True);

}

void modelxml::addchain()
{
    int n=sum[1][1];
    for(int i=1;i<n;i++)
     {

<<<<<<< HEAD
       KDL::Vector ve(o_object[1][1][i].pos_x,o_object[1][1][i].pos_y,o_object[1][1][i].pos_z);
=======
       Vector ve(o_object[1][1][i].pos_x,o_object[1][1][i].pos_y,o_object[1][1][i].pos_z);
>>>>>>> c4ff94bbf0db489e74a4af36be3499e72d03de24
       Rotation ro = Rotation::RPY(o_object[1][1][i].rpy_z,o_object[1][1][i].rpy_y,o_object[1][1][i].rpy_x);
       if(i==1||i==n-1)
        {
            pum.addSegment(Segment(Joint(Joint::None),
<<<<<<< HEAD
                                               KDL::Frame(ro,ve),
=======
                                               Frame(ro,ve),
>>>>>>> c4ff94bbf0db489e74a4af36be3499e72d03de24
                                               RigidBodyInertia::Zero()));

        }
        else
        {
            pum.addSegment(Segment(Joint(Joint::RotZ),
<<<<<<< HEAD
                                               KDL::Frame(ro,ve),
=======
                                               Frame(ro,ve),
>>>>>>> c4ff94bbf0db489e74a4af36be3499e72d03de24
                                               RigidBodyInertia::Zero()));
        }


     }


}

void modelxml::addlimit()
{
       int n=pum.getNrOfJoints();
       j_max.resize(n);
       j_min.resize(n);
       for (int i=0;i<n;i++)
       {
           j_max.operator()(i,0) = o_object[1][3][i].max;
           j_min.operator()(i,0) = o_object[1][3][i].min;
       }
}
void modelxml::settool()
{
    qDebug()<<"asdasdasdasd";
    QHBoxLayout *hlayout[10];
    for(int i=1;i<=line_sum;i++)
    {
        hlayout[i] = new QHBoxLayout;
        hlayout[i]->addWidget(label[i]);
        hlayout[i]->addWidget(line[i]);
    }

    qDebug()<<"asdasdasdasd";
    button_set_angel = new QPushButton(tr("设置"));


    QHBoxLayout *hhlayout = new QHBoxLayout;
    hhlayout->addWidget(button_set_angel);

    QVBoxLayout *pLayout = new QVBoxLayout();   // 使用垂直布局作为主布局
    for(int i=1;i<=line_sum;i++)
    {
     pLayout->addLayout(hlayout[i]);
    }
    pLayout->addStretch();                      // 增加弹簧

    pLayout->addLayout(hhlayout);

    QWidget *pWidget = new QWidget(joint_angle_widget);  // 这是QDockWidget布局的重点！！！
    pWidget->setLayout(pLayout);
    joint_angle_widget->setWidget(pWidget);
    joint_angle_widget->show();


}
