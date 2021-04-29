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
target_include_directories(PolyDataToGraph PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>  # <prefix>/include/mylib
)

add_library(UnstructuredGridToGraph vtkUnstructuredGridToGraph.cxx)
target_link_libraries(UnstructuredGridToGraph PUBLIC ${VTK_LIBRARIES})
target_include_directories(UnstructuredGridToGraph PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>  # <prefix>/include/mylib
)

if(VTKGraphConversions_BUILD_TESTING)
  enable_testing()
  add_subdirectory(Testing)
endif()

if(VTKGraphConversions_BUILD_EXAMPLES)
  add_subdirectory(Examples)
endif()



