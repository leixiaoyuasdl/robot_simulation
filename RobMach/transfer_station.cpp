#include"transfer_station.h"
#include<QDebug>
#include<QLineEdit>

#include <chainfksolverpos_recursive.hpp>
Transfer_station::Transfer_station()
{

}

void Transfer_station::readxml()
{
    modelxml *a =new modelxml();
    a->modelview = modelview;
    a->joint_angle_widget = joint_angle_widget;
    a->information_text  = information_text;
    if(a->Readxml()==true)
    {

        ModelXml = a;
        connect(ModelXml->button_set_angel,SIGNAL(clicked()),this,SLOT(set_joint_angel()));
    }
    else delete  a;
}
void Transfer_station::readpath()
{
    SolverPos *b = new SolverPos();
    b->axml = ModelXml;
    b->information_text = information_text;
    if(b->readpath()==true)
        s_pos = b;
    else delete b;
}
void Transfer_station::remove()
{
    anima->sign_delete=true;
    delete s_pos;
    delete ModelXml;
    modelview->m_context->RemoveAll(false);
}
void Transfer_station::start_animation()
{
    anima->sign_delete=false;
    anima->pos = s_pos;
    anima->information_text = information_text;
    anima->start();
}
void Transfer_station::forward()
{

    anima->speed = anima->speed*2;
    qDebug()<<"当前动画倍数  "<<anima->speed ;
    information_text->append(" 当前动画倍数 "+QString::number(anima->speed,'.', 10));

}
void Transfer_station::back()
{
    anima->speed = anima->speed/2;
    qDebug()<<"当前动画倍数  "<<anima->speed ;
    information_text->append(" 当前动画倍数 "+QString::number(anima->speed,'.', 10));
}
void Transfer_station::pause()
{
    anima->sign_pause = true ;
    qDebug()<<"动画已经暂停 " ;
    information_text->append(" 动画已经暂停 ");
}
void Transfer_station::a_continue()
{
    anima->sign_pause = false ;
     qDebug()<<"动画继续 " ;
     information_text->append(" 继续动画 ");
}
void Transfer_station::erase_path()
{
  for(int i=0;i<s_pos->path_coordinate_system.size();i++)
  {
      modelview->m_context->Erase(s_pos->path_coordinate_system[i].pnt,false);
      modelview->m_context->Erase(s_pos->path_coordinate_system[i].ais_edgex,false);
      modelview->m_context->Erase(s_pos->path_coordinate_system[i].ais_edgey,false);
      modelview->m_context->Erase(s_pos->path_coordinate_system[i].ais_edgez,false);
  }
  modelview->m_view->Redraw();
}
void Transfer_station::redisplay_path()
{
    for(int i=0;i<s_pos->path_coordinate_system.size();i++)
    {
        modelview->m_context->Display(s_pos->path_coordinate_system[i].pnt,false);
        modelview->m_context->Display(s_pos->path_coordinate_system[i].ais_edgex,false);
        modelview->m_context->Display(s_pos->path_coordinate_system[i].ais_edgey,false);
        modelview->m_context->Display(s_pos->path_coordinate_system[i].ais_edgez,false);
    }
   modelview->m_view->Redraw();
}
void Transfer_station::erase_model()
{
  for(int i=0;i<ModelXml->model_ais_shape.size();i++)
  {
      modelview->m_context->Erase(ModelXml->model_ais_shape[i],false);
  }
  modelview->m_view->Redraw();
}
void Transfer_station::redisplay_model()
{
    for(int i=0;i<ModelXml->model_ais_shape.size();i++)
    {
        modelview->m_context->Display(ModelXml->model_ais_shape[i],false);
    }
    modelview->m_view->Redraw();
}
void Transfer_station::erase()
{
    modelview->m_context->InitSelected();
    model_ais_shape.append(modelview->m_context->SelectedInteractive());
    modelview->m_context->Erase(modelview->m_context->SelectedInteractive(),false);
    modelview->m_view->Redraw();
}
void Transfer_station::redisplay()
{
    for(int i=0;i<model_ais_shape.size();i++)
    {
        modelview->m_context->Display(model_ais_shape[i],false);
    }
    modelview->m_view->Redraw();
}
void Transfer_station::set_joint_angel()
{
     JntArray jnt;
     int n = ModelXml->line_sum;
     jnt.resize(n);
    for(int i=1;i<=n;i++)
    {
        QString text = ModelXml->line[i]->text();
        jnt.operator()(i-1,0) = text.toDouble()/180*M_PI;
    }
    ChainFkSolverPos_recursive fwdkin(ModelXml->pum);


    int sum = ModelXml->pum.getNrOfSegments();
            for(int i=1;i<=sum;i++)
            {
                Frame pos_goal;
                fwdkin.JntToCart(jnt, pos_goal,i);
                gp_Trsf gp;
                gp.SetValues(pos_goal.operator()(0,0),pos_goal.operator()(0,1),pos_goal.operator()(0,2),pos_goal.operator()(0,3),
                             pos_goal.operator()(1,0),pos_goal.operator()(1,1),pos_goal.operator()(1,2),pos_goal.operator()(1,3),
                             pos_goal.operator()(2,0),pos_goal.operator()(2,1),pos_goal.operator()(2,2),pos_goal.operator()(2,3)
                             );
                gp = ModelXml->o_object[1][1][0].o_gp*gp*ModelXml->o_object[1][1][i].o_gp.Inverted();
                gp = gp*ModelXml->o_object[1][2][i-1].o_gp;
                modelview->m_context->SetLocation(ModelXml->meshes[1][i-1],gp);
                modelview->m_view->Redraw();
            }


}
