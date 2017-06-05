#include <boost/program_options.hpp>
#include <iostream>

#include "../include/AspectGenerator.h"

namespace po = boost::program_options;

std::tuple<po::variables_map, po::options_description>
buildParser(int argc, char **argv) {
  auto run_desc = "Generate AspectC++ aspects for Margot Framework";
  auto run = po::options_description(run_desc);

  auto generateDesc = "Aspect description XML file";
  auto outputDesc = "Output (.ah) file";
  auto helpDesc = "Print this help message";

  run.add_options()
      ("generate,g", po::value<std::string>(), generateDesc)
      ("output,o", po::value<std::string>(), outputDesc)
      ("help,h", helpDesc)
      ;

  auto vm = po::variables_map();
  po::store(po::command_line_parser(argc, argv)
                .options(run)
                .run(),
            vm);
  po::notify(vm);
  return std::make_tuple(vm, run);
};

int main(int argc, char *argv[]) {
  auto parserPack = buildParser(argc, argv);
  auto vm = std::get<0>(parserPack);
  auto opt = std::get<1>(parserPack);

  if (vm.count("help")) {
    std::cout << opt << "\n";
    return 0;
  }

  if (vm.count("generate")) {
    if (vm.count("output")) {
      auto xmlPath = vm["generate"].as<std::string>();
      auto outputPath = vm["output"].as<std::string>();

      ag::generateAspect(xmlPath, outputPath);
      return 0;
    }
  }

  std::cout << "Usage: " << argv[0] << " [options]\n";
  std::cout << opt << "\n";
  return 0;
}