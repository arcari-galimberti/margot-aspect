//
// Created by leonardoarcari on 22/06/17.
//

#include "../include/RegionMonitor.h"
#include <sstream>

namespace ag {

ag::RegionMonitor::RegionMonitor(std::vector<Argument> knobs,
                                 const std::string &configureCall,
                                 const std::string &blockName)
    : _knobs(knobs), _configureCall(configureCall), _blockName(blockName) {}

ag::RegionMonitor::RegionMonitor(std::vector<Argument> knobs,
                                 const std::string &blockName)
    : _knobs(knobs), _configureCall(""), _blockName(blockName) {}

std::vector<std::string>
ag::RegionMonitor::generateAdvices(std::string indent) {
  auto advices = std::vector<std::string>();
  auto ss = std::stringstream();
  auto ktSs = std::stringstream();
  auto dind = indent + "  ";
  auto trind = dind + "  ";

  for (auto i = 0; i < _knobs.size(); ++i) {
    ss << _knobs[i].name() << ((i != _knobs.size() - 1) ? ", " : "");
    ktSs << _knobs[i].type() << " " << _knobs[i].name()
         << ((i != _knobs.size() - 1) ? ", " : "");
  }
  auto typeKnobList = ktSs.str();
  auto knobList = ss.str();
  ss.str("");

  // Before advice
  ss << indent << "advice " << _blockName << "_start_roi(" << knobList
     << ") : before(" << typeKnobList << ") {\n"
     << dind << "if (margot::" << _blockName << "::update(" << knobList
     << ")) {\n";
  if (!_configureCall.empty()) {
    ss << trind << _configureCall << ";\n";
  }
  ss << trind << "margot::" << _blockName
     << "::manager.configuration_applied();\n";
  ss << dind << "}\n";
  ss << dind << "margot::" << _blockName << "::start_monitor();\n"
     << indent << "}";
  advices.push_back(ss.str());
  ss.str("");

  // After advice
  ss << indent << "advice " << _blockName << "_end_roi() : before() {\n";
  ss << dind << "margot::" << _blockName << "::stop_monitor();\n";
  ss << dind << "margot::" << _blockName << "::log();\n";
  ss << indent << "}";

  advices.push_back(ss.str());
  return advices;
}

std::vector<std::string>
ag::RegionMonitor::generatePointcuts(std::string indent) {
  auto ss = std::stringstream();
  auto ktSs = std::stringstream();

  for (auto i = 0; i < _knobs.size(); ++i) {
    ss << _knobs[i].name() << ((i != _knobs.size() - 1) ? ", " : "");
    ktSs << _knobs[i].type() << " " << _knobs[i].name()
         << ((i != _knobs.size() - 1) ? ", " : "");
  }
  auto typeKnobList = ktSs.str();
  auto knobList = ss.str();

  return {indent + "pointcut " + _blockName + "_start_roi(" + typeKnobList +
              ") = execution(\"void margot_" + _blockName +
              "_start_roi()\") && args(" + knobList + ");",
          indent + "pointcut " + _blockName +
              "_end_roi() = execution(\"void margot_" + _blockName +
              "_end_roi()\");"};
}

std::string ag::RegionMonitor::generateHeaders(std::string indent) {
  auto ktSs = std::stringstream();
  for (auto i = 0; i < _knobs.size(); ++i) {
    ktSs << _knobs[i].type() << " " << _knobs[i].name()
         << ((i != _knobs.size() - 1) ? ", " : "");
  }
  auto typeKnobList = ktSs.str();

  return indent + "void margot_" + _blockName + "_start_roi(" + typeKnobList +
         ") { }\n" + indent + "void margot_" + _blockName + "_end_roi() { }";
}

} // namespace ag
