//
// Created by Andrea Galimberti on 30/05/2017.
//

#ifndef ASPECT_PROJECT_FUNCTIONMONITOR_H
#define ASPECT_PROJECT_FUNCTIONMONITOR_H

#include <string>
#include <vector>
#include "MonitorGenerator.h"

namespace ag {

class Argument {
public:
  Argument(const std::string &type, const std::string &name,
           const bool &swKnob);

  const inline std::string &type() const { return _type; }
  const inline std::string &name() const { return _name; }
  inline bool swKnob() const { return _swKnob; }

private:
  std::string _type;
  std::string _name;
  bool _swKnob;
};

class FunctionMonitor : public MonitorGenerator {
public:
  FunctionMonitor(const std::string &functionName,
                   const std::string &returnType,
                   const std::vector<Argument> &arguments,
                   const std::string &configureCall,
                   const std::string &blockName);

  FunctionMonitor(const std::string &functionName,
                   const std::string &returnType,
                   const std::vector<Argument> &arguments,
                   const std::string &blockName);

  std::vector<std::string> generateAdvices(std::string indent) override;
  std::vector<std::string> generatePointcuts(std::string indent) override;

private:
  std::string _functionName;
  std::string _returnType;
  std::vector<Argument> _arguments;
  std::string _configureCall;
  std::string _blockName;
};
} // namespace ag

#endif // ASPECT_PROJECT_MONITORGENERATOR_H
