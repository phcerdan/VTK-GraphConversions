#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkImageData.h>
#include <vtkSphereSource.h>
#include <vtkGraphToPolyData.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkExtractEdges.h>
#include <vtkPoints.h>
#include <vtkPolyLine.h>
#include <vtkCellArray.h>
#include "gtest/gtest.h"
#include "vtkPolyDataLinesToGraph.h"
#include "PolyDataLinesFixture.h"
#include "common/prog_options_test.h"
#include <vtkIntersectionPolyDataFilter.h>
/** Flag to visualize results.
 *  Set at test run-time from command line -v or --visualize. */
bool VFLAG;
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    auto option_map = program_options(argc, argv);
    VFLAG = option_map["visualize"].as<bool>();
    return RUN_ALL_TESTS();
}
TEST_F(PolyDataLinesFixture, DataX)
{
  SetUpPolyDataX();
  auto nlines = polyDataLines_->GetNumberOfLines();
  auto npoints = polyDataLines_->GetNumberOfPoints();
  EXPECT_EQ(5, npoints);
  // 2 PolyLines
  EXPECT_EQ(2, nlines);
  if(VFLAG) VisualizePolyData();
}
TEST_F(PolyDataLinesFixture, DataXWithVertices)
{
  SetUpPolyDataX();
  SetUpVertices();
  auto nvertices = polyDataLines_->GetNumberOfVerts();
  EXPECT_EQ(5, nvertices);
  auto npoints = polyDataLines_->GetNumberOfPoints();
  EXPECT_EQ(npoints, nvertices);
  if(VFLAG) VisualizePolyData();
}
TEST_F(PolyDataLinesFixture, DataXToGraph)
{
  SetUpPolyDataX();
  // Transform PolyData to Graph
  vtkSmartPointer<vtkPolyDataLinesToGraph> polyToGraph =
    vtkSmartPointer<vtkPolyDataLinesToGraph>::New();
  polyToGraph->SetInputData(polyDataLines_);
  polyToGraph->Update();
  graph_  = polyToGraph->GetOutput();
  int ne  = graph_->GetNumberOfEdges();
  int nv  = graph_->GetNumberOfVertices();
  int nep0 = graph_->GetNumberOfEdgePoints(1);
  int nep1 = graph_->GetNumberOfEdgePoints(0);
  int nepTotal = nep0 + nep1;
  EXPECT_EQ(2, ne);
  EXPECT_EQ(4, nv);
  EXPECT_EQ(1, nepTotal);

  if(VFLAG) VisualizeGraph();
}
