//
// Created by leonardoarcari on 06/06/17.
//

#include "../include/SelfTuneGenerator.h"
#include <sstream>

namespace ag {

std::string SimplePredicate::generateCondition(const std::string &controlVar) {
  // TODO: Write implementation
  return nullptr;
}

SimplePredicate::SimplePredicate(const std::string &operand, PredicateType type)
    : _operand(operand), _type(type) {}

SimplePredicate::SimplePredicate(SimplePredicate &&other)
    : _operand(other._operand), _type(other._type) {}

Rule::Rule(const std::string &goalValue, std::unique_ptr<Predicate> predicate)
    : _goalValue(goalValue), _predicate(std::move(predicate)) {}

Rule::Rule(Rule &&rule)
    : _goalValue(rule._goalValue), _predicate(std::move(rule._predicate)) {}

const std::string &Rule::goalValue() const { return _goalValue; }

const std::unique_ptr<Predicate> &Rule::predicate() const { return _predicate; }

SelfTuneGenerator::SelfTuneGenerator(const ControlVar &controlVar,
                                     const std::string &goalName,
                                     std::vector<Rule> &&rules)
    : _controlVar(controlVar), _goalName(goalName),
      _rules(std::forward(rules)) {}

SelfTuneGenerator::SelfTuneGenerator(SelfTuneGenerator &&other)
    : _controlVar(std::move(other._controlVar)),
      _goalName(std::move(other._goalName)), _rules(std::move(other._rules)) {}

std::vector<std::string>
SelfTuneGenerator::generateAdvices(std::string indent) {
  auto advices = std::vector<std::string>();
  auto ss = std::stringstream();

  auto dind = indent + indent;

  ss << indent << "advice " << _controlVar.name() << "_set() : after () {\n"
     << dind << "tune_" << _goalName << "(*tjp->entity());\n"
     << indent << "}";

  advices.push_back(ss.str());
  return advices;
}

std::vector<std::string>
SelfTuneGenerator::generatePointcuts(std::string indent) {
  auto pointcuts = std::vector<std::string>();
  auto ss = std::stringstream();

  ss << indent << "pointcut " << _controlVar.name() << "_set() = set(\""
     << _controlVar.type() << " ...::" << _controlVar.name() << "\");";

  pointcuts.push_back(ss.str());
  return pointcuts;
}

std::vector<std::string>
SelfTuneGenerator::generateGoalTuner(std::string indent) {
  // TODO: Write implementation
  return std::vector<std::string>();
}

ControlVar::ControlVar(const std::string &_type, const std::string &_name)
    : _type(_type), _name(_name) {}

const std::string &ControlVar::type() const { return _type; }

const std::string &ControlVar::name() const { return _name; }

ControlVar::ControlVar(ControlVar &&other)
    : _type(other._type), _name(other._name) {}
}