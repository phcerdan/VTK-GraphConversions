
#include "vtkPolyDataToGraph.h"
#include "vtkUnstructuredGridToGraph.h"
// #include <vtkDataReader.h> // Legacy...
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLUnstructuredGridReader.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkNew.h>
#include <vtkPolyData.h>
#include <vtkUnstructuredGrid.h>
#include <vtkGraphToPolyData.h>

#include "cxxopts.hpp"
#include <filesystem>
#include <string>

namespace fs = std::filesystem;


int main(int argc, char * argv[]) {
  cxxopts::Options options("runGraphConversions",
      "Convert PolyData or Unstructured Grid to a vtkGraph");
  options.add_options()
    ("v,verbose", "Verbose output to cout")
    ("p,polydata", "Assume input file is a PolyData file, regardless of extension.")
    ("u,ugrid", "Assume input file is a UnstructuredGrid file, regardless of extension.")
    ("i,input", "File name, either a vtkPolyData file (.vtp) or vtkUnstructuredGrid (.vtu)", cxxopts::value<std::string>())
    ("o,output", "File name for the vtkGraph output (.vtp)", cxxopts::value<std::string>())
    ("h,help", "Print usage")
    ;

  auto opts = options.parse(argc, argv);
  if(opts.count("help")) {
    std::cout << options.help() << std::endl;
    return EXIT_SUCCESS;
  }

  if(!opts.count("input") || !opts.count("output")) {
    std::cout << "input and output options are required." << std::endl;
    std::cout << options.help() << std::endl;
    return EXIT_FAILURE;
  }

  auto input = opts["input"].as<std::string>();
  auto output = opts["output"].as<std::string>();

  const bool verbose = opts["verbose"].as<bool>();
  const bool assumePolyData = opts["polydata"].as<bool>();
  const bool assumeUGrid = opts["ugrid"].as<bool>();


  if(verbose) {
    std::cout << "input: " << input << std::endl;
    std::cout << "output: " << output << std::endl;
  }

  auto input_path = fs::path(input);
  auto output_path = fs::path(output);

  if(output_path.extension() != ".vtp") {
    std::cerr << "Output filename must be .vtp (PolyData)" << std::endl;
    return EXIT_FAILURE;
  }

  if(assumePolyData || input_path.extension() == ".vtp" ) {
    vtkNew<vtkXMLPolyDataReader> reader;
    reader->SetFileName(input_path.string().c_str());
    reader->Update();

    vtkNew<vtkPolyDataToGraph> pdToGraphFilter;
    pdToGraphFilter->SetInputConnection(reader->GetOutputPort());
    pdToGraphFilter->Update();

    vtkNew<vtkGraphToPolyData> graphToPdFilter;
    graphToPdFilter->SetInputConnection(pdToGraphFilter->GetOutputPort());
    graphToPdFilter->Update();

    vtkNew<vtkXMLPolyDataWriter> writer;
    writer->SetFileName(output_path.string().c_str());
    writer->SetInputConnection(graphToPdFilter->GetOutputPort());
    writer->Write();

    return EXIT_SUCCESS;
  }

  if(assumeUGrid || input_path.extension() == ".vtu" ) {
    vtkNew<vtkXMLUnstructuredGridReader> reader;

    reader->SetFileName(input_path.string().c_str());
    reader->Update();

    vtkNew<vtkUnstructuredGridToGraph> ugridToGraphFilter;
    ugridToGraphFilter->SetInputConnection(reader->GetOutputPort());
    ugridToGraphFilter->Update();

    vtkNew<vtkGraphToPolyData> graphToPdFilter;
    graphToPdFilter->SetInputConnection(ugridToGraphFilter->GetOutputPort());
    graphToPdFilter->Update();

    vtkNew<vtkXMLPolyDataWriter> writer;
    writer->SetFileName(output_path.string().c_str());
    writer->SetInputConnection(graphToPdFilter->GetOutputPort());
    writer->Write();

    return EXIT_SUCCESS;
  }

}
