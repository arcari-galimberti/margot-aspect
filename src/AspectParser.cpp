//
// Created by Andrea Galimberti on 30/05/2017.
//

#include "../include/AspectParser.h"
#include <iostream>
#include <stack>

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

std::map<std::string, std::vector<AspectParser::MonGenPtr>>
AspectParser::parseMonitor() const {
  auto generatorsMap =
      std::map<std::string, std::vector<AspectParser::MonGenPtr>>();
  for (auto aspectNode : _aspect.children("aspect")) {
    auto generators = std::vector<AspectParser::MonGenPtr>();
    auto blockNameValue = aspectNode.attribute("block_name").value();
    auto blockName = std::string(blockNameValue);
    for (auto monitor : aspectNode.children("monitor")) {
      auto functionName = monitor.child("function-name").text();
      auto returnType = monitor.child("return-type").text();
      auto arguments = std::vector<Argument>();
      for (auto argument : monitor.children("argument")) {
        auto argType = argument.child("type").text();
        auto name = argument.child("name").text();
        auto knobValue = argument.attribute("sw-knob").value();
        auto knobString = std::string(knobValue);
        bool knob;
        if (knobString == "yes") {
          knob = true;
        } else if (knobString == "no") {
          knob = false;
        }
        arguments.push_back(
            Argument(argType.as_string(), name.as_string(), knob));
      }
      auto configureCall =
          std::string(monitor.child("configure-call").text().as_string());
      if (configureCall.empty()) {
        generators.push_back(std::make_unique<MonitorGenerator>(
            functionName.as_string(), returnType.as_string(),
            std::move(arguments), blockName));
      } else {
        generators.push_back(std::make_unique<MonitorGenerator>(
            functionName.as_string(), returnType.as_string(),
            std::move(arguments), configureCall, blockName));
      }
    }
    generatorsMap[blockName] = std::move(generators);
  }
  return generatorsMap;
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

static auto parseSimplePredicate(const pugi::xml_node &predicate) {
  auto predOperand = predicate.text();
  auto predTypeValue = predicate.attribute("type").value();
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

  return std::make_unique<SimplePredicate>(predOperand.as_string(), predType);
}

static auto parseRule(const pugi::xml_node &rule) {
  using Context = std::tuple<std::string, std::unique_ptr<Predicate>,
                             std::unique_ptr<Predicate>>;
  auto cxtStack = std::stack<Context>();
  auto retStack = std::stack<std::unique_ptr<Predicate>>();

  if (rule.child("predicate")) {
    auto predNode = rule.child("predicate");
    auto pred = parseSimplePredicate(predNode);
    auto goalValue = rule.child("value").text();
    return Rule(goalValue.as_string(), std::move(pred));
  } else {
    auto parentNode = pugi::xml_node();
    auto predNode =
        ((rule.child("and")) ? rule.child("and") : rule.child("or"));
    auto topLevelContext = std::make_tuple(std::string(predNode.name()),
                                           std::unique_ptr<Predicate>(nullptr),
                                           std::unique_ptr<Predicate>(nullptr));
    cxtStack.push(std::move(topLevelContext));

    while (!cxtStack.empty()) {
      auto cxt = std::move(cxtStack.top());
      cxtStack.pop();
      auto ret = std::unique_ptr<Predicate>();
      auto nodeAssigned = false;

      if (!retStack.empty()) {
        ret = std::move(retStack.top());
        retStack.pop();
      }

      auto lhcType = std::string(predNode.first_child().name());
      auto rhcType = std::string(predNode.last_child().name());

      // Check if SimplePredicate
      if (!std::get<1>(cxt) && lhcType == "predicate") {
        std::get<1>(cxt) = parseSimplePredicate(predNode.first_child());
      }
      if (!std::get<2>(cxt) && rhcType == "predicate") {
        std::get<2>(cxt) = parseSimplePredicate(predNode.last_child());
      }

      // Handle left hand tree node
      if (!std::get<1>(cxt) && !ret) {
        parentNode = predNode;
        predNode = predNode.first_child();
        cxtStack.push(std::move(cxt));
        cxtStack.push({lhcType, nullptr, nullptr});
        continue;
      }
      if (!std::get<1>(cxt) && ret) {
        std::get<1>(cxt) = std::move(ret);
        nodeAssigned = true;
      }

      // Handle right hand tree node
      // Here we force the resolution of left hand side first, asking that
      // left hand side has the Predicate object fully built before diving
      // the right hand tree side
      if (!std::get<2>(cxt) && std::get<1>(cxt) && nodeAssigned) {
        parentNode = predNode;
        predNode = predNode.last_child();
        cxtStack.push(std::move(cxt));
        cxtStack.push({rhcType, nullptr, nullptr});
        continue;
      }
      if (!std::get<2>(cxt) && ret && std::get<1>(cxt) && !nodeAssigned) {
        std::get<2>(cxt) = std::move(ret);
      }

      // Predicate fully constructed. No more recusion from here.
      if (std::get<1>(cxt) && std::get<2>(cxt)) {
        if (std::get<0>(cxt) == "and") {
          predNode = parentNode;
          parentNode = parentNode.parent();
          retStack.push(std::make_unique<AndPredicate>(
              std::move(std::get<1>(cxt)), std::move(std::get<2>(cxt))));
        } else if (std::get<0>(cxt) == "or") {
          predNode = parentNode;
          parentNode = parentNode.parent();
          retStack.push(std::make_unique<OrPredicate>(
              std::move(std::get<1>(cxt)), std::move(std::get<2>(cxt))));
        } else {
          throw std::invalid_argument{
              "Invalid Predicate type. Expected [and|or]. Found " +
              std::get<0>(cxt)};
        }
      }
    }

    // Pop return value from last iteration. This is our fully built
    // Predicate object.
    auto pred = std::move(retStack.top());
    auto goalValue = rule.child("value").text();
    return Rule(goalValue.as_string(), std::move(pred));
  }
}

std::map<std::string, std::vector<AspectParser::GTPtr>>
AspectParser::parseGoalTuner() const {
  auto generatorsMap =
      std::map<std::string, std::vector<AspectParser::GTPtr>>();
  for (auto aspectNode : _aspect.children("aspect")) {
    auto generators = std::vector<AspectParser::GTPtr>();
    auto blockNameValue = aspectNode.attribute("block_name").value();
    auto blockName = std::string(blockNameValue);
    for (auto goalTuner : aspectNode.children("goal-tuner")) {
      auto cvNode = goalTuner.child("control-var");
      auto cvType = cvNode.child("type").text();
      auto cvName = cvNode.child("name").text();
      auto controlVar = ControlVar(cvType.as_string(), cvName.as_string());
      auto goalName = goalTuner.child("goal-name").text();
      auto rules = std::vector<Rule>();
      for (auto rule : goalTuner.children("rule")) {
        rules.push_back(parseRule(rule));
      }
      generators.push_back(std::make_unique<GoalTuner>(
          controlVar, goalName.as_string(), std::move(rules), blockName));
    }
    generatorsMap[blockName] = std::move(generators);
  }
  return generatorsMap;
}

std::map<std::string, std::vector<AspectParser::STPtr>>
AspectParser::parseStateTuner() const {
  auto generatorsMap =
      std::map<std::string, std::vector<AspectParser::STPtr>>();
  for (auto aspectNode : _aspect.children("aspect")) {
    auto generators = std::vector<AspectParser::STPtr>();
    auto blockNameValue = aspectNode.attribute("block_name").value();
    auto blockName = std::string(blockNameValue);
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
        auto pred = std::make_unique<SimplePredicate>(predOperand.as_string(),
                                                      predType);
        auto goalValue = rule.child("value").text();
        rules.push_back(Rule(goalValue.as_string(), std::move(pred)));
      }
      generators.push_back(std::make_unique<StateTuner>(
          controlVar, std::move(rules), blockName));
    }
    generatorsMap[blockName] = std::move(generators);
  }
  return generatorsMap;
}
} // namespace ag
