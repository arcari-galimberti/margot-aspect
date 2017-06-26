//
// Created by leonardoarcari on 15/06/17.
//

#ifndef ASPECT_PROJECT_TUNERELEMENTS_H
#define ASPECT_PROJECT_TUNERELEMENTS_H

#include <memory>
#include <string>

namespace ag {

enum class PredicateType { EQ, NEQ, GT, LT, GTE, LTE };

struct Predicate {
public:
  virtual std::string
  generateCondition(const std::string &controlVar) const = 0;
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

struct AndPredicate : public Predicate {
  AndPredicate(std::unique_ptr<Predicate> lhp, std::unique_ptr<Predicate> rhp);
  AndPredicate(AndPredicate &&other);

  std::string generateCondition(const std::string &controlVar) const override;
  std::unique_ptr<Predicate> clone() const override;

private:
  std::unique_ptr<Predicate> _lhp;
  std::unique_ptr<Predicate> _rhp;
};

struct OrPredicate : public Predicate {
  OrPredicate(std::unique_ptr<Predicate> lhp, std::unique_ptr<Predicate> rhp);
  OrPredicate(OrPredicate &&other);

  std::string generateCondition(const std::string &controlVar) const override;
  std::unique_ptr<Predicate> clone() const override;

private:
  std::unique_ptr<Predicate> _lhp;
  std::unique_ptr<Predicate> _rhp;
};

struct NotPredicate : public Predicate {
public:
  NotPredicate(std::unique_ptr<Predicate> pred);
  NotPredicate(NotPredicate &&other);

  std::string generateCondition(const std::string &controlVar) const override;
  std::unique_ptr<Predicate> clone() const override;

private:
  std::unique_ptr<Predicate> _pred;
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

class Argument {
public:
  Argument(const std::string &type, const std::string &name, const bool &swKnob)
      : _type(type), _name(name), _swKnob(swKnob){};

  const inline std::string &type() const { return _type; }
  const inline std::string &name() const { return _name; }
  inline bool swKnob() const { return _swKnob; }

private:
  std::string _type;
  std::string _name;
  bool _swKnob;
};
}

#endif // ASPECT_PROJECT_TUNERELEMENTS_H
