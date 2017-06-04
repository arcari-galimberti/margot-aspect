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

std::vector<std::string> ag::MonitorGenerator::generateAdvices() {
  auto argZeroName = _arguments.front().name();
  auto argZeroType = _arguments.front().type();
  std::vector<std::string> advice;
  // before advice
  advice.push_back("advice " + _functionName + "_exec(" + argZeroName
                  + ") : before(" + argZeroType + " " + argZeroName
                  + ") {\n");
  advice.push_back("\tif (margot::foo::update(" + argZeroName + ")) {\n");
  advice.push_back("\t\tmargot::foo::manager.configuration_applied();\n");
  advice.push_back("\t}\n");
  advice.push_back("\tmargot::foo::start_monitor();");
  advice.push_back("}\n\n");
  // after advice
  advice.push_back("advice " + _functionName + "_exec(" + argZeroName
            + ") : after(" + argZeroType + " " + argZeroName + ") {\n");
  advice.push_back("\tmargot::foo::stop_monitor();\n");
  advice.push_back("\tmargot::foo::log();\n");
  advice.push_back("}\n\n");
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