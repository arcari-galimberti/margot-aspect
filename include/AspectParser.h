//
// Created by Andrea Galimberti on 30/05/2017.
//

#ifndef ASPECT_PROJECT_ASPECTPARSER_H
#define ASPECT_PROJECT_ASPECTPARSER_H

#include "FunctionMonitor.h"
#include "GoalTuner.h"
#include "RegionMonitor.h"
#include "StateTuner.h"

#include <map>
#include <memory>
#include <pugixml.hpp>
#include <string>

namespace ag {

/**
 * AspectParser is the top-level class to parse a MarGotAspect XML file
 * describing code desired to be generated. For the XML document parsing purpose
 * it leverages on <i>pugi::xml library</i>.
 */
class AspectParser {
public:
  using FMGPtr = std::unique_ptr<FunctionMonitor>;
  using RMPtr = std::unique_ptr<RegionMonitor>;
  using GTPtr = std::unique_ptr<GoalTuner>;
  using STPtr = std::unique_ptr<StateTuner>;

  /**
   * AspectParser constructor. On construction it tries to load the XML file
   * from \p pathname. In case of failure throws an exception.
   * @param [in] pathname the XML file to parse.
   */
  explicit AspectParser(const std::string &pathname);

  /**
   * Copy constructor. It loads an XML file from the same pathname \p oap loaded
   * its own from.
   * @param [in] oap other AspectParser to copy into this.
   */
  AspectParser(const AspectParser &oap);

  /**
   * Parses the XML document for <function-monitor> blocks, building
   * FunctionMonitor objects representing them.
   * @return a std::map whose entries are (blockName, FunctionMonitor)
   */
  std::map<std::string, std::vector<FMGPtr>> parseFunctionMonitor() const;

  /**
   * Parses the XML document for <region-monitor> blocks, building
   * RegionMonitor objects representing them.
   * @return a std::map whose entries are (blockName, RegionMonitor)
   */
  std::map<std::string, std::vector<RMPtr>> parseRegionMonitor() const;

  /**
   * Parses the XML document for <goal-tuner> blocks, building GoalTuner
   * objects representing them.
   * @return a std::map whose entries are (blockName, GoalTuner)
   */
  std::map<std::string, std::vector<GTPtr>> parseGoalTuner() const;

  /**
   * Parses the XML document for <state-tuner> blocks, building StateTuner
   * objects representing them.
   * @return a std::map whose entries are (blockName, StateTuner)
   */
  std::map<std::string, std::vector<STPtr>> parseStateTuner() const;

private:
  std::string _pathname;
  pugi::xml_document _aspect;
};
} // namespace ag

#endif // ASPECT_PROJECT_ASPECTPARSER_H
