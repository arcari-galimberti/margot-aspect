//
// Created by leonardoarcari on 15/06/17.
//

#ifndef ASPECT_PROJECT_TUNERELEMENTS_H
#define ASPECT_PROJECT_TUNERELEMENTS_H

#include <memory>
namespace ag {

enum class PredicateType { EQ, GT, LT, GTE, LTE };

struct Predicate {
public:
  virtual std::string generateCondition(const std::string &controlVar) const = 0;
  virtual std::unique_ptr<Predicate> clone() const = 0;
};

struct SimplePredicate : public Predicate {
public:
  SimplePredicate(const std::string &operand, PredicateType type);
  SimplePredicate(SimplePredicate &&other);

  std::string generateCondition(const std::string &controlVar) const override;
  std::unique_ptr<Predicate> clone() const override;

private:
  PredicateType _type;
  std::string _operand;
};

struct Rule {
public:
  Rule(const std::string &value, std::unique_ptr<Predicate> predicate);
  Rule(const Rule &other);
  Rule(Rule &&rule);

  const std::string &value() const;
  const Predicate &predicate() const;

private:
  std::string _value;
  std::unique_ptr<Predicate> _predicate;
};

struct ControlVar {
public:
  ControlVar(const std::string &_type, const std::string &_name);
  ControlVar(const ControlVar &other);
  ControlVar(ControlVar &&other);

  const std::string &type() const;
  const std::string &name() const;

private:
  std::string _type;
  std::string _name;
};
}


#endif //ASPECT_PROJECT_TUNERELEMENTS_H
