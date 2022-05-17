#include "coordinate_system.h"
#include<TopoDS_Vertex.hxx>
#include<BRepBuilderAPI_MakeVertex.hxx>
#include<BRepBuilderAPI_MakeEdge.hxx>
#include<TopoDS_Edge.hxx>
#include<gp_Pnt.hxx>
#include <BRep_Builder.hxx>
#include <TopoDS_Shape.hxx>
coordinate_system::coordinate_system()
{

}
coordinate_system::coordinate_system(gp_Trsf g_gp,double len)
{
    gp =g_gp;
    l=len;
}
void coordinate_system::set(gp_Trsf g_gp, double len)
{
    gp =g_gp;
    l=len;
}
void coordinate_system::draw()
{

     gp_Pnt p0(gp.Value(1,4),gp.Value(2,4),gp.Value(3,4));
     TopoDS_Vertex verti = BRepBuilderAPI_MakeVertex(p0);
     Handle(AIS_Shape) vertishape = new AIS_Shape(verti);
     pnt = vertishape;
     m_context->SetColor(vertishape,Quantity_NOC_BLACK,Standard_False);
     m_context->Display(vertishape,Standard_False);

     gp_Pnt px(l,0,0);
     px = px.Transformed(gp);
     TopoDS_Edge edgex = BRepBuilderAPI_MakeEdge(p0,px);
     Handle(AIS_Shape) a_edgex = new AIS_Shape(edgex);
     ais_edgex = a_edgex;
     m_context->SetColor(ais_edgex,Quantity_NOC_RED,Standard_False);
     m_context->Display(ais_edgex,Standard_False);


     gp_Pnt py(0,l,0);
     py = py.Transformed(gp);
     TopoDS_Edge edgey = BRepBuilderAPI_MakeEdge(p0,py);
     Handle(AIS_Shape) a_edgey = new AIS_Shape(edgey);
     ais_edgey = a_edgey;
     m_context->SetColor(ais_edgey,Quantity_NOC_GREEN,Standard_False);
     m_context->Display(ais_edgey,Standard_False);

     gp_Pnt pz(0,0,l);
     pz = pz.Transformed(gp);
     TopoDS_Edge edgez = BRepBuilderAPI_MakeEdge(p0,pz);
     Handle(AIS_Shape) a_edgez = new AIS_Shape(edgez);
     ais_edgez = a_edgez;
     m_context->SetColor(ais_edgez,Quantity_NOC_BLUE1,Standard_False);
     m_context->Display(ais_edgez,Standard_False);

}
void coordinate_system::hide()
{


    //m_context->Erase(pnt,false);
  //  m_context->Redisplay(pnt,false);
m_context->Remove(pnt,false);

  // m_context->Erase(ais_edgex,false);
  //  m_context->Redisplay(ais_edgex,false);
m_context->Remove(ais_edgex,false);

  //  m_context->Erase(ais_edgey,false);
  //   m_context->Redisplay(ais_edgey,false);
m_context->Remove(ais_edgey,false);

   // m_context->Erase(ais_edgez,false);
  //   m_context->Redisplay(ais_edgez,false);
m_context->Remove(ais_edgez,false);

}
