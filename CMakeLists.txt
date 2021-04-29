cmake_minimum_required(VERSION 2.6)

project(VTKGraphConversions)

# Check first for VTK VERSION
find_package(VTK REQUIRED)
if(VTK_VERSION VERSION_GREATER_EQUAL 8.90.0)
    set(_modern_VTK_available TRUE)
    set(_vtk_prefix "")
else()
    set(_modern_VTK_available FALSE)
    set(_vtk_prefix "vtk")
endif()

option( VTKGraphConversions_BUILD_TESTING ON)
option( VTKGraphConversions_BUILD_EXAMPLES ON)

set(_vtk_required_modules
  CommonCore
  CommonDataModel
  CommonExecutionModel
  FiltersExtraction
  FiltersSources
  )

if(VTKGraphConversions_BUILD_TESTING OR VTKGraphConversions_BUILD_EXAMPLES)
    list(APPEND _vtk_required_modules IOXML)
endif()

list(TRANSFORM _vtk_required_modules PREPEND "${_vtk_prefix}")

find_package(VTK REQUIRED COMPONENTS
    ${_vtk_required_modules}
)
if(NOT _modern_VTK_available)
    include(${VTK_USE_FILE})
endif()

# Core
add_library(PolyDataToGraph vtkPolyDataToGraph.cxx)
target_link_libraries(PolyDataToGraph PUBLIC ${VTK_LIBRARIES})

add_library(UnstructuredGridToGraph vtkUnstructuredGridToGraph.cxx)
target_link_libraries(UnstructuredGridToGraph PUBLIC ${VTK_LIBRARIES})

if(VTKGraphConversions_BUILD_TESTING)
    enable_testing()
    add_executable(NewGraphConversions NewGraphConversions.cxx)
    target_link_libraries(NewGraphConversions PRIVATE ${VTK_LIBRARIES})
    add_test(NewGraphConversions NewGraphConversions)

    add_executable(vtkUnstructuredGridToGraphTest vtkUnstructuredGridToGraphTest.cxx)
    target_link_libraries(vtkUnstructuredGridToGraphTest UnstructuredGridToGraph)
    add_test(vtkUnstructuredGridToGraphTest vtkUnstructuredGridToGraphTest)

    add_executable(vtkPolyDataToGraphTest vtkPolyDataToGraphTest.cxx)
    target_link_libraries(vtkPolyDataToGraphTest PolyDataToGraph)
    add_test(vtkPolyDataToGraphTest vtkPolyDataToGraphTest)
endif()

if(VTKGraphConversions_BUILD_EXAMPLES)
    add_executable(vtkUnstructuredGridToGraphExample vtkUnstructuredGridToGraphExample.cxx)
    target_link_libraries(vtkUnstructuredGridToGraphExample UnstructuredGridToGraph)

    add_executable(vtkPolyDataToGraphExample vtkPolyDataToGraphExample.cxx)
    target_link_libraries(vtkPolyDataToGraphExample PolyDataToGraph )
endif()



