//
// Created by Andrea Galimberti on 30/05/2017.
//

#include "../include/MonitorGenerator.h"
#include <iostream>
#include <sstream>

namespace ag {

Argument::Argument(const std::string &type, const std::string &name)
    : _type(type), _name(name) {}

ag::MonitorGenerator::MonitorGenerator(const std::string &functionName,
                                       const std::string &returnType,
                                       const std::vector<Argument> &arguments,
                                       const std::string &blockName)
    : _functionName(functionName), _returnType(returnType),
      _arguments(arguments), _blockName(blockName) {}

std::vector<std::string>
ag::MonitorGenerator::generateAdvices(std::string indent) {
  auto argZeroName = _arguments.front().name();
  auto argZeroType = _arguments.front().type();
  std::vector<std::string> advice;
  auto dind = indent + indent;
  auto trind = dind + indent;

  // before advice
  auto ss = std::stringstream();
  ss << indent << "advice " << _functionName << "_exec(" << argZeroName
     << ") : before(" << argZeroType + " " << argZeroName << ") {\n";

  ss << dind << "if (margot::foo::update(" << argZeroName << ")) {\n";
  ss << trind << "margot::foo::manager.configuration_applied();\n";
  ss << dind << "}\n";
  ss << dind << "margot::foo::start_monitor();\n";
  ss << indent << "}";

  advice.push_back(ss.str());
  ss.str("");

  // after advice
  ss << indent << "advice " << _functionName << "_exec(" << argZeroName
     << ") : after(" << argZeroType << " " << argZeroName << ") {\n";
  ss << dind << "margot::foo::stop_monitor();\n";
  ss << dind << "margot::foo::log();\n";
  ss << indent << "}";

  advice.push_back(ss.str());
  return advice;
}

std::vector<std::string>
MonitorGenerator::generatePointcuts(std::string indent) {
  auto argZeroName = _arguments.front().name();
  auto argZeroType = _arguments.front().type();
  auto ArgZeroReturnType = _arguments.front().type();
  std::vector<std::string> pointcut;

  pointcut.push_back(indent + "pointcut " + _functionName + "_exec(" +
                     argZeroType + " " + argZeroName + ") = execution(\"" +
                     _returnType + " " + _functionName + "(...)\") && args(" +
                     argZeroName + ");");

  return pointcut;
}
} // namespace ag