//
// Created by leonardoarcari on 13/06/17.
//

#include "../include/StateTuner.h"
#include <sstream>

namespace ag {

StateTuner::StateTuner(const ControlVar &controlVar,
                       const std::vector<Rule> &&rules,
                       const std::string &blockName)
    : _controlVar(controlVar), _rules(rules), _blockName(blockName) {}

StateTuner::StateTuner(StateTuner &&other)
    : _controlVar(std::move(other._controlVar)),
      _rules(std::move(other._rules)), _blockName(std::move(other._blockName)) {

}

std::vector<std::string> StateTuner::generateAdvices(std::string indent) {
  auto advices = std::vector<std::string>();
  auto ss = std::stringstream();

  auto dind = indent + indent;

  ss << indent << "advice " << _controlVar.name() << "_set() : after () {\n"
     << dind << "tune_" << _blockName << "_state"
     << "(*tjp->entity());\n"
     << indent << "}";

  advices.push_back(ss.str());
  return advices;
}

std::vector<std::string> StateTuner::generatePointcuts(std::string indent) {
  auto pointcuts = std::vector<std::string>();
  auto ss = std::stringstream();

  ss << indent << "pointcut " << _controlVar.name() << "_set() = set(\""
     << _controlVar.type() << " ...::" << _controlVar.name() << "\");";

  pointcuts.push_back(ss.str());
  return pointcuts;
}

std::string StateTuner::generateStateTuner(std::string indent) {
  auto ss = std::stringstream();
  auto dind = indent + indent;
  auto trind = dind + indent;

  auto stateSetter =
      std::string("margot::") + _blockName + "::manager.change_active_state";

  ss << indent << "void tune_" << _blockName << "_state(" << _controlVar.type()
     << " " << _controlVar.name() << ") {\n";

  for (auto i = 0; i < _rules.size(); ++i) {
    ss << ((i == 0) ? (dind + "if ") : (dind + "} else if ")) << "("
       << _rules[i].predicate().generateCondition(_controlVar.name()) << ") {\n"
       << trind << stateSetter << "(" << _rules[i].value() << ");\n";
  }

  ss << dind << "}\n" << indent << "}";
  return ss.str();
}

const std::string &StateTuner::blockName() const { return _blockName; }
} // namespace ag