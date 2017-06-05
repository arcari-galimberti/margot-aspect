//
// Created by Andrea Galimberti on 30/05/2017.
//

#include "../include/MonitorGenerator.h"
#include <iostream>
#include <sstream>

namespace ag {

ag::MonitorGenerator::MonitorGenerator(
    const std::string &functionName, const std::string &returnType,
    const std::vector<ag::Argument> &arguments)
    : AdviceGenerator(functionName, returnType, arguments) {}

std::vector<std::string> ag::MonitorGenerator::generateAdvices() {
  auto argZeroName = _arguments.front().name();
  auto argZeroType = _arguments.front().type();
  std::vector<std::string> advice;

  // before advice
  auto ss = std::stringstream();
  ss << "advice " << _functionName << "_exec(" << argZeroName
                  << ") : before(" << argZeroType + " " << argZeroName
                  << ") {\n";
  ss << "\tif (margot::foo::update(" << argZeroName << ")) {\n";
  ss << "\t\tmargot::foo::manager.configuration_applied();\n";
  ss << "\t}\n";
  ss << "\tmargot::foo::start_monitor();";
  ss << "}\n\n";

  advice.push_back(ss.str());
  ss.str("");

  // after advice
  ss << "advice " << _functionName << "_exec(" << argZeroName
            << ") : after(" << argZeroType << " " << argZeroName << ") {\n";
  ss << "\tmargot::foo::stop_monitor();\n";
  ss << "\tmargot::foo::log();\n";
  ss << "}\n\n";

  advice.push_back(ss.str());
  return advice;
}

std::vector<std::string> MonitorGenerator::generatePointcuts() {
  auto argZeroName = _arguments.front().name();
  auto argZeroType = _arguments.front().type();
  auto ArgZeroReturnType = _arguments.front().type();
  std::vector<std::string> pointcut;

  pointcut.push_back("pointcut " + _functionName +"_exec(" + argZeroType + " "
            + argZeroName + ") = execution(\"" + _returnType + " "
            + _functionName + "(...)\") && args(" + argZeroName + ");");

  return pointcut;
}
}