//
// Created by leonardoarcari on 22/06/17.
//

#ifndef ASPECT_PROJECT_REGIONMONITOR_H
#define ASPECT_PROJECT_REGIONMONITOR_H

#include "MonitorGenerator.h"
#include "TunerElements.h"

namespace ag {
class RegionMonitor : public MonitorGenerator {
public:
  RegionMonitor(std::vector<Argument> knobs,
                const std::string &configureCall, const std::string &blockName);
  RegionMonitor(std::vector<Argument> knobs,
                const std::string &blockName);

  std::vector<std::string> generateAdvices(std::string indent) override;
  std::vector<std::string> generatePointcuts(std::string indent) override;
  std::string generateHeaders(std::string indent);

private:
  std::vector<Argument> _knobs;
  std::string _configureCall;
  std::string _blockName;
};
} // namespace ag

#endif // ASPECT_PROJECT_REGIONMONITOR_H
