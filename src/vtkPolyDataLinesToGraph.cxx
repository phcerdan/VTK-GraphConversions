#include "vtkPolyDataLinesToGraph.h"

#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkExtractEdges.h"
#include "vtkLine.h"
#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkMutableUndirectedGraph.h"
#include "vtkAdjacentVertexIterator.h"
#include "vtkPolyData.h"
#include "vtkImageData.h"
#include "vtkSmartPointer.h"
#include <vtkCellType.h>
#include <vtkPolyLine.h>
#include <vtkCellArray.h>
#include <vtkCellIterator.h>
#include <vtkGraphEdge.h>

vtkStandardNewMacro(vtkPolyDataLinesToGraph);

int vtkPolyDataLinesToGraph::RequestDataObject(vtkInformation *vtkNotUsed(request),
                                         vtkInformationVector **inputVector,
                                         vtkInformationVector *outputVector)
{
  vtkMutableUndirectedGraph* output = 0;
  output = vtkMutableUndirectedGraph::New();

  this->GetExecutive()->SetOutputData(0, output);
  output->Delete();

  return 1;
}
/*
//it works with or without this re-implementation
int vtkMeshToGraph::FillOutputPortInformation(
  int vtkNotUsed(port), vtkInformation* info)
{
  // now add our info
  info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkMutableUndirectedGraph");
  return 1;
}
*/

//----------------------------------------------------------------------------
int vtkPolyDataLinesToGraph::FillInputPortInformation(
                                            int vtkNotUsed(port), vtkInformation* info)
{
  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPolyData");
  return 1;
}

int vtkPolyDataLinesToGraph::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  // get the input and ouptut
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkPolyData *input = vtkPolyData::SafeDownCast(
      inInfo->Get(vtkDataObject::DATA_OBJECT()));

  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkMutableUndirectedGraph *output = vtkMutableUndirectedGraph::SafeDownCast(
		  outInfo->Get(vtkDataObject::DATA_OBJECT()));

  vtkSmartPointer<vtkCellArray> cellLines =
    vtkSmartPointer<vtkCellArray>::New();
  cellLines  = input->GetLines();
  vtkSmartPointer<vtkIdList> pointsCellLines =
    vtkSmartPointer<vtkIdList>::New();
  cellLines->InitTraversal();
  vtkSmartPointer<vtkPoints> vertexPoints =
    vtkSmartPointer<vtkPoints>::New();

  while (cellLines->GetNextCell(pointsCellLines))
    {
    // If >= 2 ids is a well defined line, neccesary?.
    vtkIdType ini = 0, end = 0;
    if(pointsCellLines->GetNumberOfIds() >= 2)
      {
      vtkSmartPointer<vtkPoints> edgePoints =
        vtkSmartPointer<vtkPoints>::New();
      for (int n = 0; n < pointsCellLines->GetNumberOfIds(); ++n)
        {
        if(n == 0)
          {
          ini = output->AddVertex();
          vertexPoints->InsertNextPoint(
            input->GetPoint(pointsCellLines->GetId(n)));
          }
        else if(n == pointsCellLines->GetNumberOfIds() - 1)
          {
          end = output->AddVertex();
          vertexPoints->InsertNextPoint(
            input->GetPoint(pointsCellLines->GetId(n)));
          }
        else
          {
          edgePoints->InsertNextPoint(
            input->GetPoint(pointsCellLines->GetId(n)));
          }
        }

      vtkEdgeType edg  = output->AddEdge(ini,end);
      for (int npe = 0; npe < edgePoints->GetNumberOfPoints(); ++npe)
        {
        output->AddEdgePoint(edg.Id,edgePoints->GetPoint(npe));
        }

      // output->SetEdgePoints(edg.Id,
      //   edgePoints->GetNumberOfPoints(),
      //   static_cast<double*>(edgePoints->GetData()->GetVoidPointer(0)));
      }
    }

  //add a vertex for every point
  // for(vtkIdType i = 0; i < input->GetNumberOfPoints(); i++)
  //   {
  //   output->AddVertex();
  //   }

  //output->GetVertexData()->ShallowCopy(input->GetPointData());
  // output->GetVertexData()->PassData(input->GetPointData());

  //add the edge between every point and every point connected to it (do not allow duplicates)
  // for(vtkIdType i = 0; i < input->GetNumberOfPoints(); i++)
  //   {
  //   vtkSmartPointer<vtkIdList> idList =
  //       vtkSmartPointer<vtkIdList>::New();
  //   this->GetConnectedVertices(input, i, idList);
  //
  //   for(vtkIdType id = 0; id < idList->GetNumberOfIds(); id++)
  //     {
  //     if(!EdgeExists(output, i, idList->GetId(id)))
  //       {
  //       output->AddEdge(i, idList->GetId(id));
  //       }
  //     }
  //   }

  output->SetPoints(vertexPoints);
  // output->SetPoints(input->GetPoints());

  {
  //ShallowCopy to check errors / change type.
  vtkSmartPointer<vtkMutableUndirectedGraph> outputGraph =
      vtkSmartPointer<vtkMutableUndirectedGraph>::New();
  outputGraph->ShallowCopy(output);
  outputGraph->DeepCopyEdgePoints(output);
  }

  return 1;
}

bool vtkPolyDataLinesToGraph::EdgeExists(vtkSmartPointer<vtkGraph> g, int pointID, int neighborID)
{
  //check if the edge already exists
  vtkSmartPointer<vtkAdjacentVertexIterator> iterator =
      vtkSmartPointer<vtkAdjacentVertexIterator>::New();
  g->GetAdjacentVertices(pointID, iterator);

  bool edgeExists = false;
  while(iterator->HasNext())
    {
      if(iterator->Next() == neighborID)
        {
        edgeExists = true;
        break;
        }
    }
  return edgeExists;
}


void vtkPolyDataLinesToGraph::GetConnectedVertices(vtkSmartPointer<vtkPolyData> mesh, int seed, vtkSmartPointer<vtkIdList> connectedVertices)
{

  //get all cells that vertex 'seed' is a part of
  vtkSmartPointer<vtkIdList> cellIdList =
      vtkSmartPointer<vtkIdList>::New();
  mesh->GetPointCells(seed, cellIdList);

  //cout << "There are " << cellIdList->GetNumberOfIds() << " cells that use point " << seed << endl;

  //loop through all the cells that use the seed point
  for(vtkIdType i = 0; i < cellIdList->GetNumberOfIds(); i++)
    {

    vtkCell* cell = mesh->GetCell(cellIdList->GetId(i));
    //cout << "The cell has " << cell->GetNumberOfEdges() << " edges." << endl;

    //if the cell doesn't have any edges, it is a line
    if(cell->GetNumberOfEdges() <= 0)
      {
      //vtkLine* line = vtkLine::SafeDownCast(input->GetCell(i));
    vtkLine* line = vtkLine::SafeDownCast(mesh->GetCell(cellIdList->GetId(i)));

      //if the cell didn't have any edges, and it is not a line, it must be a vertex, so skip it
      if(!line)
        {
        continue;
        }
      else
        {
        int p0 = line->GetPointId(0);
        int p1 = line->GetPointId(1);
        if(p0 == seed)
          {
          connectedVertices->InsertNextId(p1);
          }
        else
          {
          connectedVertices->InsertNextId(p0);
          }

        }
        continue;
      }

    //if we get to here, the cell is a polygon, so extract its border edges
    for(vtkIdType e = 0; e < cell->GetNumberOfEdges(); e++)
      {
      vtkCell* edge = cell->GetEdge(e);

      vtkIdList* pointIdList = edge->GetPointIds();
      //cout << "This cell uses " << pointIdList->GetNumberOfIds() << " points" << endl;
      /*
      for(vtkIdType p = 0; p < pointIdList->GetNumberOfIds(); p++)
        {
        cout << "Edge " << i << " uses point " << pointIdList->GetId(p) << endl;
        }
        */
      if(pointIdList->GetId(0) == seed || pointIdList->GetId(1) == seed)
        {
        if(pointIdList->GetId(0) == seed)
          {
          connectedVertices->InsertNextId(pointIdList->GetId(1));
          }
        else
          {
          connectedVertices->InsertNextId(pointIdList->GetId(0));
          }
        }
      }


    }

    //cout << "There are " << connectedVertices->GetNumberOfIds() << " points connected to point " << seed << endl;
}
