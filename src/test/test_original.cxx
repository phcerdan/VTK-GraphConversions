#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkImageData.h>
#include <vtkSphereSource.h>
#include <vtkGraphToPolyData.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkExtractEdges.h>

#include "vtkPolyDataLinesToGraph.h"

int main (int argc, char *argv[])
{
  vtkSmartPointer<vtkSphereSource> sphereSource =
      vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->Update();

  vtkSmartPointer<vtkExtractEdges> extractEdges =
      vtkSmartPointer<vtkExtractEdges>::New();
  extractEdges->SetInputConnection(sphereSource->GetOutputPort());
  extractEdges->Update();

  vtkSmartPointer<vtkPolyDataLinesToGraph> polyDataToGraphFilter =
      vtkSmartPointer<vtkPolyDataLinesToGraph>::New();
  polyDataToGraphFilter->SetInputConnection(extractEdges->GetOutputPort());
  polyDataToGraphFilter->Update();

  if(extractEdges->GetOutput()->GetNumberOfPoints() != polyDataToGraphFilter->GetOutput()->GetNumberOfVertices())
    {
    std::cerr << "Number of points " << extractEdges->GetOutput()->GetNumberOfPoints()
      << " does not match number of vertices " << polyDataToGraphFilter->GetOutput()->GetNumberOfVertices() << " !";
    return EXIT_FAILURE;
    }

  if(extractEdges->GetOutput()->GetNumberOfCells() != polyDataToGraphFilter->GetOutput()->GetNumberOfEdges())
    {
    std::cerr << "Number of cells(lines) " << extractEdges->GetOutput()->GetNumberOfCells()
      << " does not match number of edges " << polyDataToGraphFilter->GetOutput()->GetNumberOfEdges() << " !";
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
