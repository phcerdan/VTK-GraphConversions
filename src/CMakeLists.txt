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
add_library(vtkPolyDataLinesToGraph vtkPolyDataLinesToGraph.cxx)
target_link_libraries(vtkPolyDataLinesToGraph ${VTK_LIBRARIES})

add_subdirectory(common)
enable_testing()
add_subdirectory(test)
