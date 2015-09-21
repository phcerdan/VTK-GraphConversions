/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkGraphToPolyData2.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/*-------------------------------------------------------------------------
  Copyright 2008 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
  the U.S. Government retains certain rights in this software.
-------------------------------------------------------------------------*/
// .NAME vtkGraphToPolyData2 - convert a vtkGraph to vtkPolyData
//
// .SECTION Description
// Converts a vtkGraph to a vtkPolyData.  This assumes that the points
// of the graph have already been filled (perhaps by vtkGraphLayout),
// and coverts all the edge of the graph into lines in the polydata.
// The vertex data is passed along to the point data, and the edge data
// is passed along to the cell data.
//
// Only the owned graph edges (i.e. edges with ghost level 0) are copied
// into the vtkPolyData.

#ifndef vtkGraphToPolyData2_h
#define vtkGraphToPolyData2_h

#include "vtkFiltersSourcesModule.h" // For export macro
#include "vtkPolyDataAlgorithm.h"

class VTKFILTERSSOURCES_EXPORT vtkGraphToPolyData2 : public vtkPolyDataAlgorithm
{
public:
  static vtkGraphToPolyData2 *New();
  vtkTypeMacro(vtkGraphToPolyData2,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Create a second output containing points and orientation vectors
  // for drawing arrows or other glyphs on edges.  This output should be
  // set as the first input to vtkGlyph3D to place glyphs on the edges.
  // vtkGlyphSource2D's VTK_EDGEARROW_GLYPH provides a good glyph for
  // drawing arrows.
  // Default value is off.
  vtkSetMacro(EdgeGlyphOutput, bool);
  vtkGetMacro(EdgeGlyphOutput, bool);
  vtkBooleanMacro(EdgeGlyphOutput, bool);

  // Description:
  // The position of the glyph point along the edge.
  // 0 puts a glyph point at the source of each edge.
  // 1 puts a glyph point at the target of each edge.
  // An intermediate value will place the glyph point between the source and target.
  // The default value is 1.
  vtkSetMacro(EdgeGlyphPosition, double);
  vtkGetMacro(EdgeGlyphPosition, double);

protected:
  vtkGraphToPolyData2();
  ~vtkGraphToPolyData2() {}

  bool EdgeGlyphOutput;
  double EdgeGlyphPosition;
  bool ArcEdges;
  vtkIdType NumberOfArcSubdivisions;

  // Description:
  // Convert the vtkGraph into vtkPolyData.
  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

  // Description:
  // Set the input type of the algorithm to vtkGraph.
  int FillInputPortInformation(int port, vtkInformation* info);

private:
  vtkGraphToPolyData2(const vtkGraphToPolyData2&);  // Not implemented.
  void operator=(const vtkGraphToPolyData2&);  // Not implemented.
};

#endif
