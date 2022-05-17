#include "solverpos.h"
#include"modelxml.h"
#include <chainiksolver.hpp>
#include <jntarray.hpp>
#include <chainfksolverpos_recursive.hpp>
#include <chainiksolverpos_lma.hpp>
#include<QDebug>
#include <chainiksolver.hpp>
#include<QFile>
#include<QFileDialog>
#include<QString>
#include<QObject>
#include<QTextStream>
#include<QStringList>
#include <QIODevice>
#include <AIS_AnimationObject.hxx>
#include <AIS_Animation.hxx>
#include <AIS_AnimationCamera.hxx>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <Image_VideoRecorder.hxx>
#include <Draw_Interpretor.hxx>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <QApplication>

#include <chainiksolvervel_pinv.hpp>
#include <chainiksolverpos_nr_jl.hpp>
#include <chainiksolverpos_nr.hpp>

#include<QVector>

#include"coordinate_system.h"
using namespace KDL;
SolverPos::SolverPos()
{

}


JntArray SolverPos::solver(JntArray q_init,Frame pos_goal)
 {
      int n = axml->pum.getNrOfJoints();
      JntArray q_sol(n);
      ChainFkSolverPos_recursive fwdkin(axml->pum);
      ChainIkSolverVel_pinv iksolver_pinv(axml->pum);
      JntArray min(axml->j_min);
      JntArray max(axml->j_max);
      ChainIkSolverPos_NR_JL solverpos_nr_jl(
                  axml->pum,
                  min,
                  max,
                  fwdkin,
                  iksolver_pinv
                  );
      solverpos_nr_jl.CartToJnt(q_init, pos_goal, q_sol);
      jnt_array.append(q_sol);
      return q_sol;
 }

 bool SolverPos::readpath()
 {
     draw.m_context=axml->modelview->m_context;
    information_text->append(" opening text file ");
    QString filename = QFileDialog::getOpenFileName(nullptr,
                                                          QObject::tr("read a txt file"),
                                                          "F:",
                                                          QObject::tr("STEP Files(*.txt)"));
    QFile file(filename);
      if(! file.open(QIODevice::ReadOnly|QIODevice::Text))
      {qDebug()<<"没有打开txt文件";information_text->append(" no select text file ");;return false;}
      else
           {qDebug()<<"txt 打开成功";information_text->append(" txt 打开成功 ");}
      file.seek(0);

      QTextStream shuru(&file);

       double xyz[10];

       int n = axml->pum.getNrOfJoints();
       JntArray q_init(n);
      q_init.data(0);

      gp_Trsf ggp ;
      gp_Quaternion aQ;
      aQ.SetEulerAngles(gp_YawPitchRoll,0,-0.5*M_PI,-M_PI);
      gp_Vec ggvec(1,0,0.71);
      //gp_Vec ggvec(0.95,-0.2,0.15);
      ggp.SetTransformation(aQ,ggvec);
      ggp= axml->o_object[1][1][0].o_gp*ggp;
      KDL::Vector ve1(ggp.Value(1,4),ggp.Value(2,4),ggp.Value(3,4));
       Rotation ro1(ggp.Value(1,1),ggp.Value(1,2),ggp.Value(1,3),
                    ggp.Value(2,1),ggp.Value(2,2),ggp.Value(2,3),
                    ggp.Value(3,1),ggp.Value(3,2),ggp.Value(3,3)
                   );
     KDL::Frame s_frame1(ro1,ve1) ;

     draw.set(ggp,0.05);
     draw.draw();
     path_coordinate_system.append(draw);


      while(! shuru.atEnd())
      {
        QString line=shuru.readLine();
        QStringList strlist=line.split(",");
        for(int i=0;i<strlist.size();i++)
        {
            xyz[i]=strlist[i].toDouble();
        }

       KDL::Vector ve(xyz[0],xyz[1],xyz[2]);
        Rotation ro=Rotation::EulerZYX(xyz[3],xyz[4],xyz[5]);

        KDL::Frame s_frame(ro,ve);
       s_frame = s_frame1*s_frame;
       q_init = solver(q_init,s_frame);

       gp_Trsf gp ;
       gp_Quaternion aQ;
       aQ.SetEulerAngles(gp_YawPitchRoll,xyz[3],xyz[4],xyz[5]);
       gp_Vec gvec(xyz[0],xyz[1],xyz[2]);
       gp.SetTransformation(aQ,gvec);
       gp= ggp*gp;

       draw.set(gp,0.01);
       draw.draw();
       path_coordinate_system.append(draw);


      }
      translation(jnt_array);
      file.close();
      qDebug()<<"path 读取完成";
      information_text->append(" path 读取完成 ");
      return true;
}

 void SolverPos::translation(QVector<JntArray> array)
 {
     ChainFkSolverPos_recursive fwdkin(axml->pum);
     for (int j = 0; j < array.size(); j++)
     {
             QVector<gp_Trsf> gp_tr;
             int n = axml->pum.getNrOfSegments();
             for(int i=1;i<=n;i++)
             {
                 Frame pos_goal;
                 fwdkin.JntToCart(array[j], pos_goal,i);
                 gp_Trsf gp;
                 gp.SetValues(pos_goal.operator()(0,0),pos_goal.operator()(0,1),pos_goal.operator()(0,2),pos_goal.operator()(0,3),
                              pos_goal.operator()(1,0),pos_goal.operator()(1,1),pos_goal.operator()(1,2),pos_goal.operator()(1,3),
                              pos_goal.operator()(2,0),pos_goal.operator()(2,1),pos_goal.operator()(2,2),pos_goal.operator()(2,3)
                              );
                 gp = axml->o_object[1][1][0].o_gp*gp*axml->o_object[1][1][i].o_gp.Inverted();
                 gp_tr.append(gp);
             }
             gptr_array.append(gp_tr);
      }


 }
