//This class converts all of the points in a vtkPolyData into vertices in a vtkGraph, and all
// "edges" in a vtkPolyData (borders of polygons and vtkLines) into edges in a vtkGraph

#ifndef __vtkPolyDataLinesToGraph_h
#define __vtkPolyDataLinesToGraph_h

#include "vtkGraphAlgorithm.h"
#include "vtkSmartPointer.h"

class vtkPolyData;
class vtkIdList;

class vtkPolyDataLinesToGraph : public vtkGraphAlgorithm
{
public:

  static vtkPolyDataLinesToGraph *New();

protected:
  vtkPolyDataLinesToGraph(){}
  ~vtkPolyDataLinesToGraph(){}

  virtual int FillInputPortInformation( int port, vtkInformation* info );

  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

  int RequestDataObject(vtkInformation *vtkNotUsed(request),
                                         vtkInformationVector **inputVector,
                                         vtkInformationVector *outputVector);


private:
  vtkPolyDataLinesToGraph(const vtkPolyDataLinesToGraph&); // delete
  void operator=(const vtkPolyDataLinesToGraph&);          // delete

  //should not be part of this class
  bool EdgeExists(vtkSmartPointer<vtkGraph> g, int pointID, int neighborID);
  void GetConnectedVertices(vtkSmartPointer<vtkPolyData> mesh, int seed, vtkSmartPointer<vtkIdList> connectedVertices);
};


#endif
