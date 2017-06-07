//
// Created by Andrea Galimberti on 30/05/2017.
//

#include "../include/AspectParser.h"
#include "../include/MonitorGenerator.h"
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

std::vector<AspectParser::AdvGenPtr>
AspectParser::parseAdviceGenerators() const {
  auto generators = std::vector<AspectParser::AdvGenPtr>();

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

    if (std::string(type) == "monitor") {
      generators.push_back(std::make_unique<MonitorGenerator>(
          functionName.as_string(), returnType.as_string(),
          std::move(arguments)));
    }
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

std::vector<AspectParser::STGenPtr>
AspectParser::parseSelfTuneGenerators() const {
  auto generators = std::vector<AspectParser::STGenPtr>();
  auto aspectNode = _aspect.child("aspect");
  for (auto selfTune : aspectNode.children("self-tune")) {
    auto cvNode = selfTune.child("control-var");
    auto cvType = cvNode.child("type").text();
    auto cvName = cvNode.child("name").text();
    auto controlVar = ControlVar(cvType.as_string(), cvName.as_string());
    auto goalName = selfTune.child("goal-name").text();
    auto rules = std::vector<Rule>();

    for (auto rule : selfTune.children("rule")) {
      auto predNode = rule.child("predicate");
      auto predOperand = predNode.text();
      auto predTypeValue = predNode.attribute("type").value();
      auto predTypeString = std::string(predTypeValue);

      PredicateType predType;
      if(predTypeString == "eq") {
        predType = PredicateType::EQ;
      }
      else if(predTypeString == "gt") {
        predType = PredicateType::GT;
      }
      else if(predTypeString == "lt") {
        predType = PredicateType::LT;
      }
      else if(predTypeString == "gte") {
        predType = PredicateType::GTE;
      }
      else if(predTypeString == "lte") {
        predType = PredicateType::LTE;
      }

      auto pred = std::make_unique<SimplePredicate>(predOperand.as_string(), predType);
      auto goalValue = rule.child("goal-value").text();
      rules.push_back(Rule(goalValue.as_string(), std::move(pred)));
    }

    generators.push_back(std::make_unique<SelfTuneGenerator>(
        controlVar, goalName.as_string(),
        std::move(rules)));
  }
  return generators;
}
}
