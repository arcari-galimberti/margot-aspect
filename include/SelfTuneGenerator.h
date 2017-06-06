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
};

struct SimplePredicate : public Predicate {
public:
  SimplePredicate(const std::string &operand, PredicateType type);
  SimplePredicate(SimplePredicate &&other);

  std::string generateCondition(const std::string &controlVar) override;

private:
  PredicateType _type;
  std::string _operand;
};

struct Rule {
public:
  Rule(const std::string &goalValue, std::unique_ptr<Predicate> predicate);
  Rule(Rule &&rule);

private:
  std::string _goalValue;
  std::unique_ptr<Predicate> _predicate;
};

class SelfTuneGenerator {
public:
  SelfTuneGenerator(const std::string &controlVar, const std::string &goalName,
                    std::vector<Rule> &&rules);
  SelfTuneGenerator(SelfTuneGenerator &&other);

  std::vector<std::string> generateAdvices(std::string indent);
  std::vector<std::string> generatePointcuts(std::string indent);

private:
  std::string _controlVar;
  std::string _goalName;
  std::vector<Rule> _rules;
};
}

#endif // ASPECT_PROJECT_SELFTUNEGENERATOR_H
