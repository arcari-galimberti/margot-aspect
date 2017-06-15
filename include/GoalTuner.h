//
// Created by leonardoarcari on 06/06/17.
//

#ifndef ASPECT_PROJECT_SELFTUNEGENERATOR_H
#define ASPECT_PROJECT_SELFTUNEGENERATOR_H

#include <memory>
#include <string>
#include <vector>
#include "TunerElements.h"

namespace ag {

class GoalTuner {
public:
  GoalTuner(const ControlVar &controlVar, const std::string &goalName,
            std::vector<Rule> &&rules, const std::string &blockName);
  GoalTuner(GoalTuner &&other);

  std::vector<std::string> generateAdvices(std::string indent);
  std::vector<std::string> generatePointcuts(std::string indent);
  std::string generateGoalTuner(std::string indent);

  const std::string &blockName() const;

private:
  ControlVar _controlVar;
  std::string _goalName;
  std::vector<Rule> _rules;
  std::string _blockName;
};
}

#endif // ASPECT_PROJECT_SELFTUNEGENERATOR_H
