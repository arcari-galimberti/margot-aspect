//
// Created by Andrea Galimberti on 30/05/2017.
//

#include "../include/MonitorGenerator.h"
#include <iostream>

namespace ag {

ag::MonitorGenerator::MonitorGenerator(
    const std::string &functionName, const std::string &returnType,
    const std::vector<ag::Argument> &arguments)
    : AdviceGenerator(functionName, returnType, arguments) {}

std::string ag::MonitorGenerator::generateCode() {
  auto argZeroName = _arguments.front().name();
  auto argZeroType = _arguments.front().type();

  // before advice
  std::cout << "advice " << _functionName << "_exec(" << argZeroName
            << ") : before(" << argZeroType << " " << argZeroName << ") {\n";
  std::cout << "\tif (margot::foo::update(" << argZeroName << ")) {\n";
  std::cout << "\t\tmargot::foo::manager.configuration_applied();\n";
  std::cout << "\t}\n";
  std::cout << "\tmargot::foo::start_monitor();";
  std::cout << "}\n\n";
  // after advice
  std::cout << "advice " << _functionName << "_exec(" << argZeroName
            << ") : after(" << argZeroType << " " << argZeroName << ") {\n";
  std::cout << "\tmargot::foo::stop_monitor();\n";
  std::cout << "\tmargot::foo::log();\n";
  std::cout << "}\n\n";
}
}
