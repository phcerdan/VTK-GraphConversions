cmake_minimum_required(VERSION 2.8)

PROJECT(GraphConversions)
find_package(VTK COMPONENTS
    vtkCommonCore
    vtkCommonDataModel
    vtkCommonExecutionModel
    vtkFiltersExtraction
    vtkFiltersSources
    vtkIOXML
    vtkInfovisLayout
    vtkInteractionStyle
    vtkRenderingCore
    vtkRenderingOpenGL
    vtkViewsInfovis
    )

INCLUDE(${VTK_USE_FILE})

message("${VTK_INCLUDE_DIRS}")
include_directories(".")
# Modified graphLayoutLibraries
add_library(graphRepre2 vtkRenderedGraphRepresentation2.cxx)
target_link_libraries(graphRepre2 ${VTK_LIBRARIES})
add_library(graphToPoly2 vtkGraphToPolyData2.cxx)
target_link_libraries(graphToPoly2 ${VTK_LIBRARIES})
add_library(graphLayoutView2 vtkGraphLayoutView2.cxx)
target_link_libraries(graphLayoutView2 ${VTK_LIBRARIES})
target_link_libraries(graphLayoutView2 graphRepre2)

add_library(vtkPolyDataLinesToGraph vtkPolyDataLinesToGraph.cxx)
target_link_libraries(vtkPolyDataLinesToGraph ${VTK_LIBRARIES})

add_subdirectory(common)
enable_testing()
add_subdirectory(test)
