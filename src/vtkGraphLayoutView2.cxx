/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkGraphLayoutView2.cxx

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

#include "vtkGraphLayoutView2.h"

#include "vtkAlgorithmOutput.h"
#include "vtkCamera.h"
#include "vtkCommand.h"
#include "vtkDirectedGraph.h"
#include "vtkFast2DLayoutStrategy.h"
#include "vtkInteractorStyle.h"
#include "vtkObjectFactory.h"
#include "vtkRenderedGraphRepresentation2.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSelection.h"
#include "vtkSimple2DLayoutStrategy.h"
#include "vtkTextProperty.h"

vtkStandardNewMacro(vtkGraphLayoutView2);
//----------------------------------------------------------------------------
vtkGraphLayoutView2::vtkGraphLayoutView2()
{
  this->SetInteractionModeTo2D();
  this->SetSelectionModeToFrustum();
  this->ReuseSingleRepresentationOn();
  this->VertexLabelsRequested = false;
  this->EdgeLabelsRequested = false;
  this->Interacting = false;
}

//----------------------------------------------------------------------------
vtkGraphLayoutView2::~vtkGraphLayoutView2()
{
}

//----------------------------------------------------------------------------
vtkRenderedGraphRepresentation2* vtkGraphLayoutView2::GetGraphRepresentation()
{
  vtkRenderedGraphRepresentation2* graphRep = 0;
  for (int i = 0; i < this->GetNumberOfRepresentations(); ++i)
    {
    vtkDataRepresentation* rep = this->GetRepresentation(i);
    graphRep = vtkRenderedGraphRepresentation2::SafeDownCast(rep);
    if (graphRep)
      {
      break;
      }
    }
  if (!graphRep)
    {
    vtkSmartPointer<vtkDirectedGraph> g = vtkSmartPointer<vtkDirectedGraph>::New();
    graphRep = vtkRenderedGraphRepresentation2::SafeDownCast(
      this->AddRepresentationFromInput(g));
    }
  return graphRep;
}

//----------------------------------------------------------------------------
vtkDataRepresentation* vtkGraphLayoutView2::CreateDefaultRepresentation(
  vtkAlgorithmOutput* port)
{
  vtkRenderedGraphRepresentation2* rep = vtkRenderedGraphRepresentation2::New();
  rep->SetInputConnection(port);
  return rep;
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::ProcessEvents(
  vtkObject* caller, unsigned long eventId, void* callData)
{
    if (eventId == vtkCommand::StartInteractionEvent)
    {
        if( GetHideVertexLabelsOnInteraction() && this->VertexLabelsRequested )
        {
            this->Interacting = true;
            this->GetGraphRepresentation()->SetVertexLabelVisibility(false);
        }
        if( GetHideEdgeLabelsOnInteraction() && this->EdgeLabelsRequested )
        {
            this->Interacting = true;
            this->GetGraphRepresentation()->SetEdgeLabelVisibility(false);
        }
    }
    if (eventId == vtkCommand::EndInteractionEvent)
    {
        bool forceRender = false ;
        if( GetHideVertexLabelsOnInteraction() && this->VertexLabelsRequested )
        {
            this->Interacting = false;
            this->GetGraphRepresentation()->SetVertexLabelVisibility(true);
            // Force the labels to reappear
            forceRender = true;
        }
        if( GetHideEdgeLabelsOnInteraction() && this->EdgeLabelsRequested )
        {
            this->Interacting = false;
            this->GetGraphRepresentation()->SetEdgeLabelVisibility(true);
            // Force the labels to reappear
            forceRender = true;
        }
        if(forceRender)
            // Force the labels to reappear
            this->Render();
    }
    if (eventId != vtkCommand::ComputeVisiblePropBoundsEvent)
    {
        this->Superclass::ProcessEvents(caller, eventId, callData);
    }
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetVertexLabelArrayName(const char* name)
{
  this->GetGraphRepresentation()->SetVertexLabelArrayName(name);
}

//----------------------------------------------------------------------------
const char* vtkGraphLayoutView2::GetVertexLabelArrayName()
{
  return this->GetGraphRepresentation()->GetVertexLabelArrayName();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetEdgeLabelArrayName(const char* name)
{
  this->GetGraphRepresentation()->SetEdgeLabelArrayName(name);
}

//----------------------------------------------------------------------------
const char* vtkGraphLayoutView2::GetEdgeLabelArrayName()
{
  return this->GetGraphRepresentation()->GetEdgeLabelArrayName();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetVertexLabelVisibility(bool vis)
{
    this->VertexLabelsRequested = vis ;
    // Don't update the visibility of the vertex label actor while an interaction
    // is in progress
    if(!this->Interacting)
        this->GetGraphRepresentation()->SetVertexLabelVisibility(vis);
}

//----------------------------------------------------------------------------
bool vtkGraphLayoutView2::GetVertexLabelVisibility()
{
  return this->GetGraphRepresentation()->GetVertexLabelVisibility();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetHideVertexLabelsOnInteraction(bool vis)
{
    this->GetGraphRepresentation()->SetHideVertexLabelsOnInteraction(vis);
}

//----------------------------------------------------------------------------
bool vtkGraphLayoutView2::GetHideVertexLabelsOnInteraction()
{
    return this->GetGraphRepresentation()->GetHideVertexLabelsOnInteraction();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetEdgeVisibility(bool vis)
{
  this->GetGraphRepresentation()->SetEdgeVisibility(vis);
}

//----------------------------------------------------------------------------
bool vtkGraphLayoutView2::GetEdgeVisibility()
{
  return this->GetGraphRepresentation()->GetEdgeVisibility();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetEdgeLabelVisibility(bool vis)
{
    this->EdgeLabelsRequested = vis ;
    // Don't update the visibility of the edge label actor while an interaction
    // is in progress
    if(!this->Interacting)
        this->GetGraphRepresentation()->SetEdgeLabelVisibility(vis);
}

//----------------------------------------------------------------------------
bool vtkGraphLayoutView2::GetEdgeLabelVisibility()
{
  return this->GetGraphRepresentation()->GetEdgeLabelVisibility();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetHideEdgeLabelsOnInteraction(bool vis)
{
    this->GetGraphRepresentation()->SetHideEdgeLabelsOnInteraction(vis);
}

//----------------------------------------------------------------------------
bool vtkGraphLayoutView2::GetHideEdgeLabelsOnInteraction()
{
    return this->GetGraphRepresentation()->GetHideEdgeLabelsOnInteraction();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetVertexColorArrayName(const char* name)
{
  this->GetGraphRepresentation()->SetVertexColorArrayName(name);
}

//----------------------------------------------------------------------------
const char* vtkGraphLayoutView2::GetVertexColorArrayName()
{
  return this->GetGraphRepresentation()->GetVertexColorArrayName();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetColorVertices(bool vis)
{
  this->GetGraphRepresentation()->SetColorVerticesByArray(vis);
}

//----------------------------------------------------------------------------
bool vtkGraphLayoutView2::GetColorVertices()
{
  return this->GetGraphRepresentation()->GetColorVerticesByArray();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetVertexScalarBarVisibility(bool vis)
{
  this->GetGraphRepresentation()->SetVertexScalarBarVisibility(vis);
}

//----------------------------------------------------------------------------
bool vtkGraphLayoutView2::GetVertexScalarBarVisibility()
{
  return this->GetGraphRepresentation()->GetVertexScalarBarVisibility();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetEdgeColorArrayName(const char* name)
{
  this->GetGraphRepresentation()->SetEdgeColorArrayName(name);
}

//----------------------------------------------------------------------------
const char* vtkGraphLayoutView2::GetEdgeColorArrayName()
{
  return this->GetGraphRepresentation()->GetEdgeColorArrayName();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetColorEdges(bool vis)
{
  this->GetGraphRepresentation()->SetColorEdgesByArray(vis);
}

//----------------------------------------------------------------------------
bool vtkGraphLayoutView2::GetColorEdges()
{
  return this->GetGraphRepresentation()->GetColorEdgesByArray();
}
//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetEdgeSelection(bool vis)
{
  this->GetGraphRepresentation()->SetEdgeSelection(vis);
}

//----------------------------------------------------------------------------
bool vtkGraphLayoutView2::GetEdgeSelection()
{
  return this->GetGraphRepresentation()->GetEdgeSelection();
}
//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetEdgeScalarBarVisibility(bool vis)
{
  this->GetGraphRepresentation()->SetEdgeScalarBarVisibility(vis);
}

//----------------------------------------------------------------------------
bool vtkGraphLayoutView2::GetEdgeScalarBarVisibility()
{
  return this->GetGraphRepresentation()->GetEdgeScalarBarVisibility();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetEnabledEdgesArrayName(const char* name)
{
  this->GetGraphRepresentation()->SetEnabledEdgesArrayName(name);
}

//----------------------------------------------------------------------------
const char* vtkGraphLayoutView2::GetEnabledEdgesArrayName()
{
  return this->GetGraphRepresentation()->GetEnabledEdgesArrayName();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetEnableEdgesByArray(bool vis)
{
  this->GetGraphRepresentation()->SetEnableEdgesByArray(vis);
}

//----------------------------------------------------------------------------
int vtkGraphLayoutView2::GetEnableEdgesByArray()
{
  return this->GetGraphRepresentation()->GetEnableEdgesByArray();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetEnabledVerticesArrayName(const char* name)
{
  this->GetGraphRepresentation()->SetEnabledVerticesArrayName(name);
}

//----------------------------------------------------------------------------
const char* vtkGraphLayoutView2::GetEnabledVerticesArrayName()
{
  return this->GetGraphRepresentation()->GetEnabledVerticesArrayName();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetEnableVerticesByArray(bool vis)
{
  this->GetGraphRepresentation()->SetEnableVerticesByArray(vis);
}

//----------------------------------------------------------------------------
int vtkGraphLayoutView2::GetEnableVerticesByArray()
{
  return this->GetGraphRepresentation()->GetEnableVerticesByArray();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetGlyphType(int type)
{
  this->GetGraphRepresentation()->SetGlyphType(type);
}

//----------------------------------------------------------------------------
int vtkGraphLayoutView2::GetGlyphType()
{
  return this->GetGraphRepresentation()->GetGlyphType();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetScaledGlyphs(bool arg)
{
  this->GetGraphRepresentation()->SetScaling(arg);
}

//----------------------------------------------------------------------------
bool vtkGraphLayoutView2::GetScaledGlyphs()
{
  return this->GetGraphRepresentation()->GetScaling();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetScalingArrayName(const char* name)
{
  this->GetGraphRepresentation()->SetScalingArrayName(name);
}

//----------------------------------------------------------------------------
const char* vtkGraphLayoutView2::GetScalingArrayName()
{
  return this->GetGraphRepresentation()->GetScalingArrayName();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetIconArrayName(const char* name)
{
  this->GetGraphRepresentation()->SetVertexIconArrayName(name);
}

//----------------------------------------------------------------------------
const char* vtkGraphLayoutView2::GetIconArrayName()
{
  return this->GetGraphRepresentation()->GetVertexIconArrayName();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::AddIconType(char *type, int index)
{
  this->GetGraphRepresentation()->AddVertexIconType(type, index);
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::ClearIconTypes()
{
  this->GetGraphRepresentation()->ClearVertexIconTypes();
}

//----------------------------------------------------------------------------
int vtkGraphLayoutView2::IsLayoutComplete()
{
  return this->GetGraphRepresentation()->IsLayoutComplete();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::UpdateLayout()
{
  this->GetGraphRepresentation()->UpdateLayout();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetLayoutStrategy(vtkGraphLayoutStrategy* s)
{
  this->GetGraphRepresentation()->SetLayoutStrategy(s);
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetLayoutStrategy(const char* name)
{
  this->GetGraphRepresentation()->SetLayoutStrategy(name);
}

//----------------------------------------------------------------------------
vtkGraphLayoutStrategy* vtkGraphLayoutView2::GetLayoutStrategy()
{
  return this->GetGraphRepresentation()->GetLayoutStrategy();
}

//----------------------------------------------------------------------------
const char* vtkGraphLayoutView2::GetLayoutStrategyName()
{
  return this->GetGraphRepresentation()->GetLayoutStrategyName();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetEdgeLayoutStrategy(vtkEdgeLayoutStrategy *s)
{
  this->GetGraphRepresentation()->SetEdgeLayoutStrategy(s);
}

//----------------------------------------------------------------------------
vtkEdgeLayoutStrategy* vtkGraphLayoutView2::GetEdgeLayoutStrategy()
{
  return this->GetGraphRepresentation()->GetEdgeLayoutStrategy();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetEdgeLayoutStrategy(const char* name)
{
  this->GetGraphRepresentation()->SetEdgeLayoutStrategy(name);
}

//----------------------------------------------------------------------------
const char* vtkGraphLayoutView2::GetEdgeLayoutStrategyName()
{
  return this->GetGraphRepresentation()->GetEdgeLayoutStrategyName();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetIconAlignment(int alignment)
{
  this->GetGraphRepresentation()->SetVertexIconAlignment(alignment);
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetIconVisibility(bool b)
{
  this->GetGraphRepresentation()->SetVertexIconVisibility(b);
}

//----------------------------------------------------------------------------
bool vtkGraphLayoutView2::GetIconVisibility()
{
  return this->GetGraphRepresentation()->GetVertexIconVisibility();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetVertexLabelFontSize(const int size)
{
  this->GetGraphRepresentation()->GetVertexLabelTextProperty()->SetFontSize(size);
}

//----------------------------------------------------------------------------
int vtkGraphLayoutView2::GetVertexLabelFontSize()
{
  return this->GetGraphRepresentation()->GetVertexLabelTextProperty()->GetFontSize();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::SetEdgeLabelFontSize(const int size)
{
  this->GetGraphRepresentation()->GetEdgeLabelTextProperty()->SetFontSize(size);
}

//----------------------------------------------------------------------------
int vtkGraphLayoutView2::GetEdgeLabelFontSize()
{
  return this->GetGraphRepresentation()->GetEdgeLabelTextProperty()->GetFontSize();
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::ZoomToSelection()
{
  double bounds[6];
  this->GetGraphRepresentation()->ComputeSelectedGraphBounds(bounds);
  this->Renderer->ResetCamera(bounds);
}

//----------------------------------------------------------------------------
void vtkGraphLayoutView2::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
