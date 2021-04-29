cmake_minimum_required(VERSION 3.12)

project(VTKGraphConversions
  VERSION 0.1
  LANGUAGES CXX C)

include(GNUInstallDirs) # Define CMAKE_INSTALL_xxx: LIBDIR, INCLUDEDIR

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

# Components required for the core libraries. Used in Config.cmake.in.
set(vtk_required_components
  CommonCore
  CommonDataModel
  CommonExecutionModel
  FiltersExtraction
  FiltersSources
  )

# Final components including tests and examples
set(vtk_components ${vtk_required_components})

if(VTKGraphConversions_BUILD_TESTING OR VTKGraphConversions_BUILD_EXAMPLES)
    list(APPEND vtk_components IOXML)
endif()

list(TRANSFORM vtk_components PREPEND "${_vtk_prefix}")

find_package(VTK REQUIRED COMPONENTS
    ${vtk_components}
)
if(NOT _modern_VTK_available)
    include(${VTK_USE_FILE})
endif()


set(_headers
  vtkPolyDataToGraph.h
  vtkUnstructuredGridToGraph.h
  )
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

set(_targets PolyDataToGraph UnstructuredGridToGraph)

# export to the build tree.
set(VTKGraphConversions_export_file "${PROJECT_BINARY_DIR}/VTKGraphConversionsTargets.cmake")
# TODO: is namespace VTK invasive?
export( TARGETS ${_targets}
  NAMESPACE VTK::
  APPEND FILE ${VTKGraphConversions_export_file})

install(TARGETS ${_targets}
        EXPORT VTKGraphConversionsTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtime
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT runtime
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT development
        )

install(FILES ${_headers}
  DESTINATION include
  COMPONENT development
  )


# INSTALL
set(install_cmake_dir "${CMAKE_INSTALL_LIBDIR}/cmake")

install (EXPORT VTKGraphConversionsTargets
  NAMESPACE VTK::
  DESTINATION ${install_cmake_dir} )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cmake/VTKGraphConversionsConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/VTKGraphConversionsConfigVersion.cmake
              DESTINATION ${install_cmake_dir} )

include(CMakePackageConfigHelpers)
write_basic_package_version_file(VTKGraphConversionsConfigVersion.cmake
  VERSION ${VTKGraphConversions_VERSION}
  COMPATIBILITY SameMajorVersion)

# Build tree
set(VTKGraphConversions_TARGETS_FILE ${VTKGraphConversions_export_file})
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/VTKGraphConversionsConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/VTKGraphConversionsConfig.cmake
  INSTALL_DESTINATION ${install_cmake_dir}
  PATH_VARS VTKGraphConversions_TARGETS_FILE
  NO_CHECK_REQUIRED_COMPONENTS_MACRO # VTKGraphConversions does not provide components
  INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}
  )

# Install tree
set(VTKGraphConversions_TARGETS_FILE ${CMAKE_INSTALL_PREFIX}/${install_cmake_dir}/VTKGraphConversionsTargets.cmake)
message(WARNING "VTKGraphConversions_TARGETS_FILE: ${VTKGraphConversions_TARGETS_FILE}")
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/VTKGraphConversionsConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/VTKGraphConversionsConfig.cmake
  INSTALL_DESTINATION ${install_cmake_dir}
  PATH_VARS VTKGraphConversions_TARGETS_FILE
  NO_CHECK_REQUIRED_COMPONENTS_MACRO # VTKGraphConversions does not provide components
  )

# Testing and examples (not packaged or installed)
if(VTKGraphConversions_BUILD_TESTING)
  enable_testing()
  add_subdirectory(Testing)
endif()

if(VTKGraphConversions_BUILD_EXAMPLES)
  add_subdirectory(Examples)
endif()


