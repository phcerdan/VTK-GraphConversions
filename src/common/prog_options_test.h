#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <fstream>
namespace po = boost::program_options;

po::variables_map program_options(const int &argc, char** const & argv){

  po::options_description generic("Generic options");
  generic.add_options()
      ("visualize,v", po::bool_switch()->default_value(false), "Visualize output")
      ;

  po::options_description cmdline_options;
  cmdline_options.add(generic);

  po::options_description visible("Allowed options");
  visible.add(generic);

  po::variables_map vm;
  store(po::command_line_parser(argc, argv).
          options(cmdline_options).run(), vm);
  std::cout << "To visualize test output add --visualize or -v"<<std::endl;
  notify(vm);
  return vm;
}
