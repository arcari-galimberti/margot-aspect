//
// Created by Andrea Galimberti on 30/05/2017.
//

#ifndef ASPECT_PROJECT_ASPECTPARSER_H
#define ASPECT_PROJECT_ASPECTPARSER_H

#include "GoalTuner.h"
#include "MonitorGenerator.h"
#include "StateTuner.h"

#include <memory>
#include <pugixml.hpp>
#include <string>
#include <map>

namespace ag {

class AspectParser {
public:
  using MonGenPtr = std::unique_ptr<MonitorGenerator>;
  using GTPtr = std::unique_ptr<GoalTuner>;
  using STPtr = std::unique_ptr<StateTuner>;

  explicit AspectParser(const std::string &pathname);
  AspectParser(const AspectParser &oap);
  std::map<std::string, std::vector<MonGenPtr>> parseMonitor() const;
  std::map<std::string, std::vector<GTPtr>> parseGoalTuner() const;
  std::map<std::string, std::vector<STPtr>> parseStateTuner() const;

private:
  std::string _pathname;
  pugi::xml_document _aspect;
};
} // namespace ag

#endif // ASPECT_PROJECT_ASPECTPARSER_H
