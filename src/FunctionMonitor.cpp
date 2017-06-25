//
// Created by Andrea Galimberti on 30/05/2017.
//

#include "../include/FunctionMonitor.h"
#include <iostream>
#include <sstream>

namespace ag {
ag::FunctionMonitor::FunctionMonitor(const std::string &functionName,
                                     const std::string &returnType,
                                     const std::vector<Argument> &arguments,
                                     const std::string &configureCall,
                                     const std::string &blockName)
    : _functionName(functionName), _returnType(returnType),
      _arguments(arguments), _configureCall(configureCall),
      _blockName(blockName) {}

ag::FunctionMonitor::FunctionMonitor(const std::string &functionName,
                                     const std::string &returnType,
                                     const std::vector<Argument> &arguments,
                                     const std::string &blockName)
    : _functionName(functionName), _returnType(returnType),
      _arguments(arguments), _configureCall(""), _blockName(blockName) {}

std::vector<std::string>
ag::FunctionMonitor::generateAdvices(std::string indent) {
  std::vector<std::string> advice;
  auto dind = indent + indent;
  auto trind = dind + indent;

  auto argSSType = std::stringstream();
  auto argSSNoType = std::stringstream();
  auto knobsSS = std::stringstream();
  auto firstArg = true;
  auto firstKnob = true;
  for (auto &arg : _arguments) {
    if (!firstArg) {
      argSSType << ", ";
      argSSNoType << ", ";
    } else {
      firstArg = false;
    }
    argSSType << arg.type() << " " << arg.name();
    argSSNoType << arg.name();
    if (arg.swKnob()) {
      if (!firstKnob) {
        knobsSS << ", ";
      } else {
        firstKnob = false;
      }
      knobsSS << arg.name();
    }
  }

  auto argStringType = argSSType.str();
  auto argStringNoType = argSSNoType.str();
  auto knobString = knobsSS.str();

  // before advice
  auto ss = std::stringstream();
  ss << indent << "advice " << _functionName << "_exec(" << argStringNoType
     << ") : before(" << argStringType << ") {\n";

  ss << dind << "if (margot::" << _blockName << "::update(" << knobString
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
FunctionMonitor::generatePointcuts(std::string indent) {
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