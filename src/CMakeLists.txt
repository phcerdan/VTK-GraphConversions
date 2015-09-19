cmake_minimum_required(VERSION 2.8)

PROJECT(GraphConversions)

find_package(VTK COMPONENTS
    vtkCommonCore
    vtkCommonDataModel
    vtkCommonExecutionModel
    vtkCommonMath
    vtkCommonMisc
    vtkCommonSystem
    vtkCommonTransforms
    vtkIOCore
    vtkIOGeometry
    vtkIOXML
    vtkIOXMLParser
    vtkRenderingOpenGL
    )
INCLUDE(${VTK_USE_FILE})

message("${VTK_INCLUDE_DIRS}")

include_directories(".")
add_library(vtkPolyDataLinesToGraph vtkPolyDataLinesToGraph.cxx)
target_link_libraries(vtkPolyDataLinesToGraph ${VTK_LIBRARIES})
ENABLE_TESTING()
add_subdirectory(test)
