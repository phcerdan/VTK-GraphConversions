include_directories(".")
ADD_EXECUTABLE(vtkPolyDataLinesToGraphTestOriginal test_original.cxx)
TARGET_LINK_LIBRARIES(vtkPolyDataLinesToGraphTestOriginal vtkPolyDataLinesToGraph )
ADD_TEST(vtkPolyDataLinesToGraphOriginal vtkPolyDataLinesToGraphTestOriginal )

SET(GTEST_INCLUDE_DIR ${gtest_SOURCE_DIR}/include)
SET(GTEST_LIBRARY ${gtest})
INCLUDE_DIRECTORIES(SYSTEM
    ${GTEST_INCLUDE_DIR}
)
find_package(Boost COMPONENTS
        filesystem	system
        program_options
        REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

ADD_EXECUTABLE(vtkPolyDataLinesToGraphTestDriven test_vtkPolyDataLinesToGraph.cxx)
TARGET_LINK_LIBRARIES(vtkPolyDataLinesToGraphTestDriven vtkPolyDataLinesToGraph )
target_link_libraries(vtkPolyDataLinesToGraphTestDriven
    gtest_main
    ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(vtkPolyDataLinesToGraphTestDriven
    ${Boost_LIBRARIES})
# TestLibrary
target_link_libraries(vtkPolyDataLinesToGraphTestDriven
    graphLayoutView2 graphRepre2 graphToPoly2)
ADD_TEST(testDriven vtkPolyDataLinesToGraphTestDriven )
