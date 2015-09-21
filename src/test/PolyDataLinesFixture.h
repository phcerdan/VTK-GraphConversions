#ifndef POLYDATALINESFIXTURE_H_
#define POLYDATALINESFIXTURE_H_
#include "gtest/gtest.h"
//Visualize:
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkGraphLayoutStrategy.h>
// #include <vtkGraphLayoutView.h>
#include <vtkGraphLayoutView2.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkProperty.h>
class PolyDataLinesFixture : public ::testing::Test {
protected:
  /**********************************************************/
  /********************* SETUP DATA *************************/
  /**********************************************************/

  /***********************   X   ****************************/
  /// Setup 2 PolyLines in 3D: X-shape.
  void SetUpPolyDataX()
    {
    //POINTS
    // double origin[3] = {0.0, 0.0, 0.0};
    double p0i[3]    = {0.0, 0.0, 0.0};
    double p0e[3]    = {1.0, 1.0 ,1.0};
    double p0mid[3]  = {0.8, 0.8, 0.7};
    double p1i[3]    = {0.0, 1.0, 0.0};
    double p1e[3]    = {1.0, 0.0, 1.0};

    vtkSmartPointer<vtkPoints> points =
      vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(p0i);
    points->InsertNextPoint(p0mid);
    points->InsertNextPoint(p0e);
    points->InsertNextPoint(p1i);
    points->InsertNextPoint(p1e);
    //LINES
    vtkSmartPointer<vtkPolyLine> polyLine1 =
      vtkSmartPointer<vtkPolyLine>::New();
    vtkIdList *polyLine1PointsId = polyLine1->GetPointIds();
    polyLine1PointsId->SetNumberOfIds(3);
    polyLine1PointsId->SetId(0,0);
    polyLine1PointsId->SetId(1,1);
    polyLine1PointsId->SetId(2,2);
    vtkSmartPointer<vtkPolyLine> polyLine2 =
      vtkSmartPointer<vtkPolyLine>::New();
    vtkIdList *polyLine2PointsId = polyLine2->GetPointIds();
    polyLine2PointsId->SetNumberOfIds(2);
    polyLine2PointsId->SetId(0,3);
    polyLine2PointsId->SetId(1,4);
    //CELLS
    vtkSmartPointer<vtkCellArray> cells =
      vtkSmartPointer<vtkCellArray>::New();
    cells->InsertNextCell(polyLine1);
    cells->InsertNextCell(polyLine2);
    //PolyData
    polyDataLines_=
      vtkSmartPointer<vtkPolyData>::New();
    polyDataLines_->SetPoints(points);
    polyDataLines_->SetLines(cells);
    // polyDataLines_->SetPolys(cells);
    }

  /**Add vertices (points) information to polyData.
   * Requires previous initialization of polyDataLines_ */
  void SetUpVertices()
  {
    vtkSmartPointer<vtkCellArray> pointsArray =
      vtkSmartPointer<vtkCellArray>::New();
    auto ncells = polyDataLines_->GetNumberOfCells();
    // std::cout << "Ncells: " << ncells << std::endl;
    for (vtkIdType j = 0; j < ncells; j++)
      {
      vtkSmartPointer<vtkIdList> pointIdList =
        vtkSmartPointer<vtkIdList>::New();
      polyDataLines_->GetCellPoints(j, pointIdList);
      auto npoints = pointIdList->GetNumberOfIds();
      // std::cout << "cell:"<< j << "; Npoints: " << npoints << std::endl;
      for (vtkIdType i = 0; i < npoints ; i++ )
        {
        pointsArray->InsertNextCell(1);
        pointsArray->InsertCellPoint(pointIdList->GetId(i));
        }
      }
    polyDataLines_->SetVerts(pointsArray);
  }
  /**********************************************************/
  /*******VISUALIZATION: PolyData and Graph******************/
  /**********************************************************/
  void VisualizePolyData(int p_vertexSize = 10)
    {
    // Setup actor and mapper
    vtkSmartPointer<vtkPolyDataMapper> mapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(polyDataLines_);

    vtkSmartPointer<vtkActor> actor =
      vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetPointSize(p_vertexSize);

    // Setup render window, renderer, and interactor
    vtkSmartPointer<vtkRenderer> renderer =
      vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow =
      vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);
    renderer->AddActor(actor);

    renderWindow->Render();
    renderWindowInteractor->Start();
    }


  void VisualizeGraph()
    {
    vtkSmartPointer<vtkGraphLayoutView2> graphLayoutView2 =
      vtkSmartPointer<vtkGraphLayoutView2>::New();
    graphLayoutView2->AddRepresentationFromInput(graph_);
    graphLayoutView2->SetLayoutStrategy("Pass Through"); //Move/Add nothing, keep the position of nodes,etc.
    // graphLayoutView2->EdgeVisibilityOff();
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
        vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New(); // like paraview
    graphLayoutView2->SetInteractorStyle(style) ;
    graphLayoutView2->ResetCamera();
    graphLayoutView2->Render();
    graphLayoutView2->GetInteractor()->Start();
    }
  vtkSmartPointer<vtkPolyData> polyDataLines_;
  vtkSmartPointer<vtkGraph> graph_;
};

#endif
