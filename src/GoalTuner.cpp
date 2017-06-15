//
// Created by leonardoarcari on 06/06/17.
//

#include "../include/GoalTuner.h"
#include <sstream>

namespace ag {

GoalTuner::GoalTuner(const ControlVar &controlVar, const std::string &goalName,
                     std::vector<Rule> &&rules, const std::string &blockName)
    : _controlVar(controlVar), _goalName(goalName), _rules(rules),
      _blockName(blockName) {}

GoalTuner::GoalTuner(GoalTuner &&other)
    : _controlVar(std::move(other._controlVar)),
      _goalName(std::move(other._goalName)), _rules(std::move(other._rules)),
      _blockName(std::move(other._blockName)) {}

std::vector<std::string> GoalTuner::generateAdvices(std::string indent) {
  auto advices = std::vector<std::string>();
  auto ss = std::stringstream();

  auto dind = indent + indent;

  ss << indent << "advice " << _controlVar.name() << "_set() : after () {\n"
     << dind << "tune_" << _goalName << "(*tjp->entity());\n"
     << indent << "}";

  advices.push_back(ss.str());
  return advices;
}

std::vector<std::string> GoalTuner::generatePointcuts(std::string indent) {
  auto pointcuts = std::vector<std::string>();
  auto ss = std::stringstream();

  ss << indent << "pointcut " << _controlVar.name() << "_set() = set(\""
     << _controlVar.type() << " ...::" << _controlVar.name() << "\");";

  pointcuts.push_back(ss.str());
  return pointcuts;
}

std::string GoalTuner::generateGoalTuner(std::string indent) {
  auto ss = std::stringstream();
  auto dind = indent + "  ";
  auto trind = dind + "  ";

  auto goalSetter =
      std::string("margot::") + _blockName + "::goal::" + _goalName + ".set";

  ss << indent << "void tune_" << _goalName << "(" << _controlVar.type() << " "
     << _controlVar.name() << ") {\n";

  for (auto i = 0; i < _rules.size(); ++i) {
    ss << ((i == 0) ? (dind + "if ") : (dind + "} else if ")) << "("
       << _rules[i].predicate().generateCondition(_controlVar.name()) << ") {\n"
       << trind << goalSetter << "(" << _rules[i].value() << ");\n";
  }

  ss << dind << "}\n" << indent << "}";
  return ss.str();
}
const std::string &GoalTuner::blockName() const { return _blockName; }
}