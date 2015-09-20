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
#include <vtkGraphLayoutView.h>
#include <vtkInteractorStyleTrackballCamera.h>
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
    double p1i[3]    = {0.0, 1.0, 0.0};
    double p1e[3]    = {1.0, 0.0, 1.0};

    vtkSmartPointer<vtkPoints> points =
      vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(p0i);
    points->InsertNextPoint(p0e);
    points->InsertNextPoint(p1i);
    points->InsertNextPoint(p1e);
    //LINES
    vtkSmartPointer<vtkPolyLine> polyLine1 =
      vtkSmartPointer<vtkPolyLine>::New();
    vtkIdList *polyLine1PointsId = polyLine1->GetPointIds();
    polyLine1PointsId->SetNumberOfIds(2);
    polyLine1PointsId->SetId(0,0);
    polyLine1PointsId->SetId(1,1);
    vtkSmartPointer<vtkPolyLine> polyLine2 =
      vtkSmartPointer<vtkPolyLine>::New();
    vtkIdList *polyLine2PointsId = polyLine2->GetPointIds();
    polyLine2PointsId->SetNumberOfIds(2);
    polyLine2PointsId->SetId(0,2);
    polyLine2PointsId->SetId(1,3);
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
    }

  /**********************************************************/
  /*******VISUALIZATION: PolyData and Graph******************/
  /**********************************************************/
  void VisualizePolyData()
    {
    // Setup actor and mapper
    vtkSmartPointer<vtkPolyDataMapper> mapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(polyDataLines_);

    vtkSmartPointer<vtkActor> actor =
      vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

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
    vtkSmartPointer<vtkGraphLayoutView> graphLayoutView =
      vtkSmartPointer<vtkGraphLayoutView>::New();
    graphLayoutView->AddRepresentationFromInput(graph_);
    graphLayoutView->SetLayoutStrategy("Pass Through"); //Move/Add nothing, keep the position of nodes,etc.
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
        vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New(); // like paraview
    graphLayoutView->SetInteractorStyle(style) ;
    graphLayoutView->ResetCamera();
    graphLayoutView->Render();
    graphLayoutView->GetInteractor()->Start();
    }
  vtkSmartPointer<vtkPolyData> polyDataLines_;
  vtkSmartPointer<vtkGraph> graph_;
};

#endif
