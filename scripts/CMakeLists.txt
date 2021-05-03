add_executable(runGraphConversion runGraphConversion.cxx)
target_link_libraries(runGraphConversion PRIVATE
  PolyDataToGraph
  UnstructuredGridToGraph
)
target_include_directories(runGraphConversion PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  )
