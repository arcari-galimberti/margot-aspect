//
// Created by Andrea Galimberti on 30/05/2017.
//

#include "../include/AspectParser.h"
#include <iostream>

namespace ag {

AspectParser::AspectParser(const std::string &pathname)
    : _pathname(pathname), _aspect() {
  auto result = _aspect.load_file(_pathname.c_str());
  if (!result) {
    std::cout << "XML " << _pathname
              << " parsing error\n  Description: " << result.description()
              << '\n';
  }
}

std::vector<AspectParser::MonGenPtr> AspectParser::parseMonitor() const {
  auto generators = std::vector<AspectParser::MonGenPtr>();
  auto blockNameValue = aspectNode.attribute("block_name").value();
  auto blockNameString = std::string(blockNameValue);
  auto aspectNode = _aspect.child("aspect");
  for (auto advice : aspectNode.children("advice")) {
    auto type = advice.attribute("type").value();
    auto functionName = advice.child("function-name").text();
    auto returnType = advice.child("return-type").text();
    auto arguments = std::vector<Argument>();
    for (auto argument : advice.children("argument")) {
      auto argType = argument.child("type").text();
      auto name = argument.child("name").text();
      arguments.push_back(Argument(argType.as_string(), name.as_string()));
    }
    generators.push_back(std::make_unique<MonitorGenerator>(
        functionName.as_string(), returnType.as_string(), std::move(arguments),
        blockNameString));
  }
  return generators;
}

AspectParser::AspectParser(const AspectParser &oap)
    : _pathname(oap._pathname), _aspect() {
  auto result = _aspect.load_file(_pathname.c_str());
  if (!result) {
    std::cout << "XML " << _pathname
              << " parsing error\n  Description: " << result.description()
              << '\n';
  }
}

std::vector<AspectParser::GTPtr> AspectParser::parseGoalTuner() const {
  auto generators = std::vector<AspectParser::GTPtr>();
  auto aspectNode = _aspect.child("aspect");
  auto blockNameValue = aspectNode.attribute("block_name").value();
  auto blockNameString = std::string(blockNameValue);
  for (auto goalTuner : aspectNode.children("goal-tuner")) {
    auto cvNode = goalTuner.child("control-var");
    auto cvType = cvNode.child("type").text();
    auto cvName = cvNode.child("name").text();
    auto controlVar = ControlVar(cvType.as_string(), cvName.as_string());
    auto goalName = goalTuner.child("goal-name").text();
    auto rules = std::vector<Rule>();

    for (auto rule : goalTuner.children("rule")) {
      auto predNode = rule.child("predicate");
      auto predOperand = predNode.text();
      auto predTypeValue = predNode.attribute("type").value();
      auto predTypeString = std::string(predTypeValue);

      PredicateType predType;
      if (predTypeString == "eq") {
        predType = PredicateType::EQ;
      } else if (predTypeString == "gt") {
        predType = PredicateType::GT;
      } else if (predTypeString == "lt") {
        predType = PredicateType::LT;
      } else if (predTypeString == "gte") {
        predType = PredicateType::GTE;
      } else if (predTypeString == "lte") {
        predType = PredicateType::LTE;
      }

      auto pred =
          std::make_unique<SimplePredicate>(predOperand.as_string(), predType);
      auto goalValue = rule.child("value").text();
      rules.push_back(Rule(goalValue.as_string(), std::move(pred)));
    }

    generators.push_back(std::make_unique<GoalTuner>(
        controlVar, goalName.as_string(), std::move(rules), blockNameString));
  }
  return generators;
}

std::vector<AspectParser::STPtr> AspectParser::parseStateTuner() const {
  auto generators = std::vector<AspectParser::STPtr>();
  auto blockNameValue = aspectNode.attribute("block_name").value();
  auto blockNameString = std::string(blockNameValue);
  for (auto stateTuner : aspectNode.children("state-tuner")) {
    auto cvNode = stateTuner.child("control-var");
    auto cvType = cvNode.child("type").text();
    auto cvName = cvNode.child("name").text();
    auto controlVar = ControlVar(cvType.as_string(), cvName.as_string());
    auto rules = std::vector<Rule>();

    for (auto rule : stateTuner.children("rule")) {
      auto predNode = rule.child("predicate");
      auto predOperand = predNode.text();
      auto predTypeValue = predNode.attribute("type").value();
      auto predTypeString = std::string(predTypeValue);

      PredicateType predType;
      if (predTypeString == "eq") {
        predType = PredicateType::EQ;
      } else if (predTypeString == "gt") {
        predType = PredicateType::GT;
      } else if (predTypeString == "lt") {
        predType = PredicateType::LT;
      } else if (predTypeString == "gte") {
        predType = PredicateType::GTE;
      } else if (predTypeString == "lte") {
        predType = PredicateType::LTE;
      }

      auto pred =
          std::make_unique<SimplePredicate>(predOperand.as_string(), predType);
      auto goalValue = rule.child("value").text();
      rules.push_back(Rule(goalValue.as_string(), std::move(pred)));
    }

    generators.push_back(std::make_unique<StateTuner>(
        controlVar, std::move(rules), blockNameString));
  }
  return generators;
}
} // namespace ag
