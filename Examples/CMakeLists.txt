add_executable(vtkUnstructuredGridToGraphExample vtkUnstructuredGridToGraphExample.cxx)
target_link_libraries(vtkUnstructuredGridToGraphExample UnstructuredGridToGraph)

add_executable(vtkPolyDataToGraphExample vtkPolyDataToGraphExample.cxx)
target_link_libraries(vtkPolyDataToGraphExample PolyDataToGraph )
