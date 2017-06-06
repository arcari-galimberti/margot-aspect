//
// Created by leonardoarcari on 06/06/17.
//

#include "../include/SelfTuneGenerator.h"

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

SelfTuneGenerator::SelfTuneGenerator(const std::string &controlVar,
                                     const std::string &goalName,
                                     std::vector<Rule> &&rules)
    : _controlVar(controlVar), _goalName(goalName),
      _rules(std::forward(rules)) {}

SelfTuneGenerator::SelfTuneGenerator(SelfTuneGenerator &&other)
    : _controlVar(std::move(other._controlVar)),
      _goalName(std::move(other._goalName)), _rules(std::move(other._rules)) {}

std::vector<std::string>
SelfTuneGenerator::generateAdvices(std::string indent) {
  // TODO: Write implementation
  return std::vector<std::string>();
}

std::vector<std::string>
SelfTuneGenerator::generatePointcuts(std::string indent) {
  // TODO: Write implementation
  return std::vector<std::string>();
}
}