//
// Created by leonardoarcari on 15/06/17.
//

#include "../include/TunerElements.h"

namespace ag {
std::string
SimplePredicate::generateCondition(const std::string &controlVar) const {
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

Rule::Rule(const std::string &value, std::unique_ptr<Predicate> predicate)
    : _value(value), _predicate(std::move(predicate)) {}

Rule::Rule(const Rule &other)
    : _value(other._value), _predicate(other.predicate().clone()) {}

Rule::Rule(Rule &&rule)
    : _value(std::move(rule._value)), _predicate(std::move(rule._predicate)) {}

const std::string &Rule::value() const { return _value; }

const Predicate &Rule::predicate() const { return *_predicate; }

ControlVar::ControlVar(const std::string &_type, const std::string &_name)
    : _type(_type), _name(_name) {}

const std::string &ControlVar::type() const { return _type; }

const std::string &ControlVar::name() const { return _name; }

ControlVar::ControlVar(ControlVar &&other)
    : _type(std::move(other._type)), _name(std::move(other._name)) {}

ControlVar::ControlVar(const ControlVar &other)
    : _type(other._type), _name(other._name) {}

std::string
AndPredicate::generateCondition(const std::string &controlVar) const {
  return std::string("(") + _lhp->generateCondition(controlVar) + " && " +
         _rhp->generateCondition(controlVar) + ")";
}

std::unique_ptr<Predicate> AndPredicate::clone() const {
  return std::make_unique<AndPredicate>(_lhp->clone(), _rhp->clone());
}

AndPredicate::AndPredicate(std::unique_ptr<Predicate> lhp,
                           std::unique_ptr<Predicate> rhp)
    : _lhp(std::move(lhp)), _rhp(std::move(rhp)) {}

AndPredicate::AndPredicate(AndPredicate &&other)
    : _lhp(std::move(other._lhp)), _rhp(std::move(other._rhp)) {}

std::string
OrPredicate::generateCondition(const std::string &controlVar) const {
  return std::string("(") + _lhp->generateCondition(controlVar) + " || " +
         _rhp->generateCondition(controlVar) + ")";
}

std::unique_ptr<Predicate> OrPredicate::clone() const {
  return std::make_unique<OrPredicate>(_lhp->clone(), _rhp->clone());
}

OrPredicate::OrPredicate(std::unique_ptr<Predicate> lhp,
                         std::unique_ptr<Predicate> rhp)
    : _lhp(std::move(lhp)), _rhp(std::move(rhp)) {}

OrPredicate::OrPredicate(OrPredicate &&other)
    : _lhp(std::move(other._lhp)), _rhp(std::move(other._rhp)) {}
}