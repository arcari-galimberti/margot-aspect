//
// Created by leonardoarcari on 13/06/17.
//

#ifndef ASPECT_PROJECT_STATETUNER_H
#define ASPECT_PROJECT_STATETUNER_H

#include "TunerElements.h"
#include <vector>

namespace ag {

class StateTuner {
public:
  StateTuner(const ControlVar &controlVar, const std::vector<Rule> &&rules,
             const std::string &blockName);
  StateTuner(StateTuner &&other);

  std::vector<std::string> generateAdvices(std::string indent);
  std::vector<std::string> generatePointcuts(std::string indent);
  std::string generateHeaders(std::string indent);

  const std::string &blockName() const;

private:
  ControlVar _controlVar;
  std::vector<Rule> _rules;
  std::string _blockName;
};
}

#endif // ASPECT_PROJECT_STATETUNER_H
