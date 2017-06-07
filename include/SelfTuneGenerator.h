//
// Created by leonardoarcari on 06/06/17.
//

#ifndef ASPECT_PROJECT_SELFTUNEGENERATOR_H
#define ASPECT_PROJECT_SELFTUNEGENERATOR_H

#include <memory>
#include <string>
#include <vector>

namespace ag {

enum class PredicateType { EQ, GT, LT, GTE, LTE };

struct Predicate {
public:
  virtual std::string generateCondition(const std::string &controlVar) = 0;
  virtual std::unique_ptr<Predicate> clone() const = 0;
};

struct SimplePredicate : public Predicate {
public:
  SimplePredicate(const std::string &operand, PredicateType type);
  SimplePredicate(SimplePredicate &&other);

  std::string generateCondition(const std::string &controlVar) override;
  std::unique_ptr<Predicate> clone() const override;

private:
  PredicateType _type;
  std::string _operand;
};

struct Rule {
public:
  Rule(const std::string &goalValue, std::unique_ptr<Predicate> predicate);
  Rule(const Rule& other);
  Rule(Rule &&rule);

  const std::string &goalValue() const;
  const std::unique_ptr<Predicate> &predicate() const;

private:
  std::string _goalValue;
  std::unique_ptr<Predicate> _predicate;
};

struct ControlVar {
public:
  ControlVar(const std::string &_type, const std::string &_name);
  ControlVar(const ControlVar& other);
  ControlVar(ControlVar&& other);

  const std::string &type() const;
  const std::string &name() const;

private:
  std::string _type;
  std::string _name;
};

class SelfTuneGenerator {
public:
  SelfTuneGenerator(const ControlVar &controlVar, const std::string &goalName,
                    std::vector<Rule> &&rules);
  SelfTuneGenerator(SelfTuneGenerator &&other);

  std::vector<std::string> generateAdvices(std::string indent);
  std::vector<std::string> generatePointcuts(std::string indent);
  std::string generateGoalTuner(std::string indent);

private:
  ControlVar _controlVar;
  std::string _goalName;
  std::vector<Rule> _rules;
};
}

#endif // ASPECT_PROJECT_SELFTUNEGENERATOR_H
