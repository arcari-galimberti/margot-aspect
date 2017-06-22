//
// Created by Andrea Galimberti on 30/05/2017.
//

#ifndef ASPECT_PROJECT_ASPECTPARSER_H
#define ASPECT_PROJECT_ASPECTPARSER_H

#include "GoalTuner.h"
#include "FunctionMonitorGenerator.h"
#include "StateTuner.h"

#include <map>
#include <memory>
#include <pugixml.hpp>
#include <string>

namespace ag {

class AspectParser {
public:
  using FMGPtr = std::unique_ptr<FunctionMonitorGenerator>;
  using GTPtr = std::unique_ptr<GoalTuner>;
  using STPtr = std::unique_ptr<StateTuner>;

  explicit AspectParser(const std::string &pathname);
  AspectParser(const AspectParser &oap);
  std::map<std::string, std::vector<FMGPtr>> parseFunctionMonitor() const;
  std::map<std::string, std::vector<GTPtr>> parseGoalTuner() const;
  std::map<std::string, std::vector<STPtr>> parseStateTuner() const;

private:
  std::string _pathname;
  pugi::xml_document _aspect;
};
} // namespace ag

#endif // ASPECT_PROJECT_ASPECTPARSER_H
