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
  graph_ = polyToGraph->GetOutput();
  int ne = graph_->GetNumberOfEdges();
  int nv = graph_->GetNumberOfVertices();
  EXPECT_EQ(ne,2);
  EXPECT_EQ(nv,4);

  if(VFLAG) VisualizeGraph();
}
