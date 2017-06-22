//
// Created by leonardoarcari on 22/06/17.
//

#include "../include/RegionMonitor.h"

namespace ag {
std::vector<std::string> RegionMonitor::generateAdvices(std::string indent) {
  return std::vector<std::string>();
}
std::vector<std::string> RegionMonitor::generatePointcuts(std::string indent) {
  return std::vector<std::string>();
}
std::vector<std::string> RegionMonitor::generateHeaders(std::string indent) {
  return std::vector<std::string>();
}
RegionMonitor::RegionMonitor(std::vector<std::string> knobNames,
                             const std::string &configureCall,
                             const std::string &blockName)
    : _knobNames(knobNames), _configureCall(configureCall),
      _blockName(blockName) {}
}