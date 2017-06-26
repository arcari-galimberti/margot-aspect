//
// Created by Andrea Galimberti on 30/05/2017.
//

#ifndef ASPECT_PROJECT_FUNCTIONMONITOR_H
#define ASPECT_PROJECT_FUNCTIONMONITOR_H

#include "MonitorGenerator.h"
#include "TunerElements.h"
#include <string>
#include <vector>

namespace ag {

/**
 * A FunctionMonitor is a MonitorGenerator to monitor the execution of a
 * function. FunctionMonitor is the class representing the XML element
 * <function-monitor>. Being a generator it exposes methods to:
 *  - generate code for AspectC++ pointcuts.
 *  - generate code for AspectC++ advices.
 * FunctionMonitor, in order to monitor some function, generates AspectC++ code
 * to perform the following before function call:
 *  - Check if software knobs have to be updated.
 *  - Run an optional environment setup function to react to software knob
 *    changes.
 *  - Start the monitor.
 * and to perform the following after function returns:
 *  - Stop the monitor.
 *  - Log the results.
 */
class FunctionMonitor : public MonitorGenerator {
public:
  /**
   * FunctionMonitor constructor
   * @param [in] functionName the name of the function to monitor.
   * @param [in] returnType the return type of the function to monitor.
   * @param [in] arguments a std::vector of Arguments the function takes.
   * @param [in] configureCall a C++ line of code to call to react to software
   *             knob changes.
   * @param [in] blockName the name of the mARGOt block.
   */
  FunctionMonitor(const std::string &functionName,
                  const std::string &returnType,
                  const std::vector<Argument> &arguments,
                  const std::string &configureCall,
                  const std::string &blockName);

  /**
   * FunctionMonitor constructor. Here no configure-call is defined so no
   * function will be called to react to software knob changes.
   * @param [in] functionName the name of the function to monitor.
   * @param [in] returnType the return type of the function to monitor.
   * @param [in] arguments a std::vector of Arguments the function takes.
   * @param [in] blockName the name of the mARGOt block
   */
  FunctionMonitor(const std::string &functionName,
                  const std::string &returnType,
                  const std::vector<Argument> &arguments,
                  const std::string &blockName);

  /**
   * @see MonitorGenerator::generateAdvices
   * Generates AspectC++ code for advices according to what is stated above
   * in class documentation.
   * @param [in] indent a std::string for the indent factor to prepend to each
   *             generated code line.
   * @return a vector of strings each of which containing the AspectC++ code
   *         for an _advice_
   */
  std::vector<std::string> generateAdvices(std::string indent) override;

  /**
   * @see MonitorGenerator::generatePointcuts
   * Generates AspectC++ code for pointcuts to match a call to the function
   * to monitor with a list of Argument as parameters
   * @param [in] indent a std::string for the indent factor to prepend to each
   *             generated code line.
   * @return a vector of strings each of which containing the AspectC++ code
   *         to define a _pointcut_.
   */
  std::vector<std::string> generatePointcuts(std::string indent) override;

private:
  std::string _functionName;
  std::string _returnType;
  std::vector<Argument> _arguments;
  std::string _configureCall;
  std::string _blockName;
};
} // namespace ag

#endif // ASPECT_PROJECT_FUNCTIONMONITOR_H
