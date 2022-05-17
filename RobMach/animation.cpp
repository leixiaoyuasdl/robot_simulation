#include "animation.h"
#include"coordinate_system.h"
#include<QDebug>
#include <AIS_AnimationObject.hxx>
#include <AIS_Animation.hxx>
#include <AIS_AnimationCamera.hxx>
#include"modelview.h"
#include <QApplication>
#include<BRepAlgoAPI_Common.hxx>
#include<TopTools_ListOfShape.hxx>
#include <TopoDS_Shape.hxx>
#include<Bnd_Box.hxx>
#include<BRepBndLib.hxx>
#include<QString>
animation::animation()
{
sign_delete = false;
sign_pause = false;
speed = 1;
}
void animation::start()
{
    qDebug()<<"动画start";
   information_text->append(" 动画start ");
   coordinate_system aaa(pos->axml->meshes[1][7]->LocalTransformation()*pos->axml->o_object[1][2][7].o_gp.Inverted()*pos->axml->o_object[1][1][8].o_gp,0.04);
   aaa.m_context = pos->axml->modelview->m_context;
   double time=0.2;
    speed=1;
    sign_pause =false;
    qDebug()<<"当前动画倍数  "<<speed ;
    information_text->append(" 当前动画倍数 "+QString::number(speed,'.', 10));
    for(int i=0;i<pos->gptr_array.size();i++)
    {
        if(i==0) time=1;
        else time=0.2;
        Handle(AIS_Animation) ais_animation = new AIS_Animation("obj1");
        Handle(AIS_AnimationObject) ais_ao;
        int n=pos->gptr_array[i].size();
        for(int j=0;j<n;j++)
        {
            ais_ao = new AIS_AnimationObject("obj1",
                                             pos->axml->modelview->m_context,
                                             pos->axml->meshes[1][j],
                                             pos->axml->meshes[1][j]->LocalTransformation(),
                                             pos->gptr_array[i][j]*pos->axml->o_object[1][2][j].o_gp);
            ais_ao->SetOwnDuration(time);
            ais_ao->SetStartPts(0);
            ais_animation->Add(ais_ao);
        }

        ais_animation->StartTimer(0, speed, true);

        int sign =0;

         while (!ais_animation->IsStopped())
        {
             if(sign_delete == true)
             {
                 aaa.hide();
                 return;

             }

             if(sign%pos->gptr_array[i].size() == 0)
             {
                 aaa.hide();
                 aaa.set(pos->axml->meshes[1][7]->LocalTransformation()*
                         pos->axml->o_object[1][2][7].o_gp.Inverted()*
                         pos->axml->o_object[1][1][8].o_gp,
                         0.04);
                 aaa.draw();
                 if(sign_pause == true)
                 {
                     ais_animation->Pause();
                     while(sign_pause==true)
                        qApp->processEvents();
                 }
             }

            ais_animation->UpdateTimer();
            pos->axml->modelview->m_view->Redraw();
            sign++;

            qApp->processEvents();
        }

    }

    qDebug()<<"动画结束";

    information_text->append(" 动画结束 ");

}

bool animation::iscommon()
{
    //从AIS类型获得TOPO类型


      //  m_TppoTargetBody = m_AISTargetBody->Shape();
       // m_TopoMoveBody = m_AISMoveBody->Shape();

        //没有实时性要求，采用bool求交的方法
    /*    Standard_Boolean bRunParallel;
        Standard_Real aFuzzyValue;
        BRepAlgoAPI_Common aBuilder;

        // perpare the arguments
        TopTools_ListOfShape aLS;
        TopTools_ListOfShape aLT;

            aLS.Append(pos->axml->meshes[1][7]->Shape());


         //xx   m_TppoTargetBody
        aLT.Append(pos->axml->meshes[2][0]->Shape());  //yy   m_TopoMoveBody

        bRunParallel=Standard_True;
        aFuzzyValue=2.1e-5;

        // set the arguments
        aBuilder.SetArguments(aLS);
        aBuilder.SetTools(aLT);

        aBuilder.SetRunParallel(bRunParallel);
        aBuilder.SetFuzzyValue(aFuzzyValue);

        // run the algorithm
       // aBuilder.Build();

        // result of the operation aR
        const TopoDS_Shape& aR=aBuilder.Shape();

        Bnd_Box B;
        BRepBndLib::Add(aR, B);

      //  double Xmin, Ymin, Zmin, Xmax, Ymax, Zmax;

       // int nb = aR.NbChildren();

        if(!B.IsVoid())   //交集部分的最小包围box存在，则说明碰撞
        {
          //  std::cout << "common" << std::endl;
            return true;
        }
        else
        {
          //  std::cout << "no common" << std::endl;
            return false;
        }
*/
    return false;

}
