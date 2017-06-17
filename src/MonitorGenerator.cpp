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
                                       const std::string &configureCall,
                                       const std::string &blockName)
    : _functionName(functionName), _returnType(returnType),
      _arguments(arguments), _configureCall(configureCall),
      _blockName(blockName) {}

ag::MonitorGenerator::MonitorGenerator(const std::string &functionName,
                                       const std::string &returnType,
                                       const std::vector<Argument> &arguments,
                                       const std::string &blockName)
    : _functionName(functionName), _returnType(returnType),
      _arguments(arguments), _configureCall(""), _blockName(blockName) {}

std::vector<std::string>
ag::MonitorGenerator::generateAdvices(std::string indent) {
  std::vector<std::string> advice;
  auto dind = indent + indent;
  auto trind = dind + indent;

  auto argSSType = std::stringstream();
  auto argSSNoType = std::stringstream();
  auto firstArg = true;
  for (auto &arg : _arguments) {
    if (!firstArg) {
      argSSType << ", ";
      argSSNoType << ", ";
    } else {
      firstArg = false;
    }
    argSSType << arg.type() << " " << arg.name();
    argSSNoType << arg.name();
  }
  auto argStringType = argSSType.str();
  auto argStringNoType = argSSNoType.str();

  // before advice
  auto ss = std::stringstream();
  ss << indent << "advice " << _functionName << "_exec(" << argStringNoType
     << ") : before(" << argStringType << ") {\n";

  ss << dind << "if (margot::" << _blockName << "::update(" << argStringNoType
     << ")) {\n";
  if (!_configureCall.empty()) {
    ss << trind << _configureCall << ";\n";
  }
  ss << trind << "margot::" << _blockName
     << "::manager.configuration_applied();\n";
  ss << dind << "}\n";
  ss << dind << "margot::" << _blockName << "::start_monitor();\n";
  ss << indent << "}";

  advice.push_back(ss.str());
  ss.str("");

  // after advice
  ss << indent << "advice " << _functionName << "_exec(" << argStringNoType
     << ") : after(" << argStringType << ") {\n";
  ss << dind << "margot::" << _blockName << "::stop_monitor();\n";
  ss << dind << "margot::" << _blockName << "::log();\n";
  ss << indent << "}";

  advice.push_back(ss.str());
  return advice;
}

std::vector<std::string>
MonitorGenerator::generatePointcuts(std::string indent) {
  std::vector<std::string> pointcut;

  auto argSSType = std::stringstream();
  auto argSSNoType = std::stringstream();
  auto firstArg = true;
  for (auto &arg : _arguments) {
    if (!firstArg) {
      argSSType << ", ";
      argSSNoType << ", ";
    } else {
      firstArg = false;
    }
    argSSType << arg.type() << " " << arg.name();
    argSSNoType << arg.name();
  }
  auto argStringType = argSSType.str();
  auto argStringNoType = argSSNoType.str();

  pointcut.push_back(indent + "pointcut " + _functionName + "_exec(" +
                     argStringType + ") = execution(\"" + _returnType + " " +
                     _functionName + "(...)\") && args(" + argStringNoType +
                     ");");

  return pointcut;
}
} // namespace ag