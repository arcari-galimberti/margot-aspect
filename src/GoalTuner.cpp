//
// Created by leonardoarcari on 06/06/17.
//

#include "../include/GoalTuner.h"
#include <sstream>

namespace ag {

std::string SimplePredicate::generateCondition(const std::string &controlVar) const {
  auto condition = std::string(controlVar);

  if (_type == PredicateType::EQ)
    condition += " == ";
  else if (_type == PredicateType::GT)
    condition += " > ";
  else if (_type == PredicateType::LT)
    condition += " < ";
  else if (_type == PredicateType::GTE)
    condition += " >= ";
  else if (_type == PredicateType::LTE)
    condition += " <= ";

  return condition += _operand;
}

SimplePredicate::SimplePredicate(const std::string &operand, PredicateType type)
    : _operand(operand), _type(type) {}

SimplePredicate::SimplePredicate(SimplePredicate &&other)
    : _operand(other._operand), _type(other._type) {}

std::unique_ptr<Predicate> SimplePredicate::clone() const {
  return std::make_unique<SimplePredicate>(_operand, _type);
}

Rule::Rule(const std::string &goalValue, std::unique_ptr<Predicate> predicate)
    : _goalValue(goalValue), _predicate(std::move(predicate)) {}

Rule::Rule(const Rule &other)
    : _goalValue(other._goalValue), _predicate(other.predicate().clone()) {}

Rule::Rule(Rule &&rule)
    : _goalValue(std::move(rule._goalValue)),
      _predicate(std::move(rule._predicate)) {}

const std::string &Rule::goalValue() const { return _goalValue; }

const Predicate & Rule::predicate() const { return *_predicate; }

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

  auto goalSetter = std::string("margot::foo::goal::") + _goalName + ".set";

  ss << indent << "void tune_" << _goalName << "(" << _controlVar.type() << " "
     << _controlVar.name() << ") {\n";

  for (auto i = 0; i < _rules.size(); ++i) {
    ss << ((i == 0) ? (dind + "if ") : (dind + "} else if ")) << "("
       << _rules[i].predicate().generateCondition(_controlVar.name())
       << ") {\n"
       << trind << goalSetter << "(" << _rules[i].goalValue() << ");\n";
  }

  ss << dind << "}\n" << indent << "}";
  return ss.str();
}

ControlVar::ControlVar(const std::string &_type, const std::string &_name)
    : _type(_type), _name(_name) {}

const std::string &ControlVar::type() const { return _type; }

const std::string &ControlVar::name() const { return _name; }

ControlVar::ControlVar(ControlVar &&other)
    : _type(std::move(other._type)), _name(std::move(other._name)) {}

ControlVar::ControlVar(const ControlVar &other)
    : _type(other._type), _name(other._name) {}
}