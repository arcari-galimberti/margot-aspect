//
// Created by leonardoarcari on 22/06/17.
//

#include "../include/RegionMonitor.h"

namespace ag {

ag::RegionMonitor::RegionMonitor(std::vector<std::string> knobNames,
                                 const std::string &configureCall,
                                 const std::string &blockName)
    : _knobNames(knobNames), _configureCall(configureCall),
      _blockName(blockName) {}

ag::RegionMonitor::RegionMonitor(std::vector<std::string> knobNames,
                                 const std::string &blockName)
    : _knobNames(knobNames), _configureCall(""), _blockName(blockName) {}

std::vector<std::string>
ag::RegionMonitor::generateAdvices(std::string indent) {
  return std::vector<std::string>();
}

std::vector<std::string>
ag::RegionMonitor::generatePointcuts(std::string indent) {
  return std::vector<std::string>();
  //
}

std::vector<std::string>
ag::RegionMonitor::generateHeaders(std::string indent) {
  return std::vector<std::string>();
}

} // namespace ag
