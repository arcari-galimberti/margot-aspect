//
// Created by leonardoarcari on 13/06/17.
//

#include "../include/StateTuner.h"

namespace ag {

StateTuner::StateTuner(const ControlVar &controlVar,
                       const std::vector<Rule> &&rules,
                       const std::string &blockName)
    : _controlVar(controlVar), _rules(rules), _blockName(blockName) {}

StateTuner::StateTuner(StateTuner &&other)
    : _controlVar(std::move(other._controlVar)),
      _rules(std::move(other._rules)), _blockName(std::move(other._blockName)) {

}
std::vector<std::string> StateTuner::generateAdvices(std::string indent) {
  return std::vector<std::string>();
}
std::vector<std::string> StateTuner::generatePointcuts(std::string indent) {
  return std::vector<std::string>();
}
std::string StateTuner::generateStateTuner(std::string indent) {
  return std::__cxx11::string();
}

}