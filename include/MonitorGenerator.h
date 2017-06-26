//
// Created by leonardoarcari on 22/06/17.
//

#ifndef ASPECT_PROJECT_MONITORGENERATOR_H
#define ASPECT_PROJECT_MONITORGENERATOR_H

#include <string>
#include <vector>

namespace ag {
/**
 * MonitorGenerator defines the interface of a Monitor class.
 * A MonitorGenerator must expose two methods:
 *  - MonitorGenerator::generateAdvices to return a vector of strings each of
 *    which containing the AspectC++ code of an _advice_
 *  - MonitorGenerator::generatePointcuts to return a vector of strings each of
 *    which containing the AspectC++ code to define a _pointcut_.
 */
class MonitorGenerator {
public:
  /**
   * Generates AspectC++ code for advices
   * @param [in] indent a std::string for the indent factor to prepend to each
   *             generated code line.
   * @return a vector of strings each of which containing the AspectC++ code
   *         for an _advice_
   */
  virtual std::vector<std::string> generateAdvices(std::string indent) = 0;

  /**
   * Generates AspectC++ code to define pointcuts
   * @param [in] indent a std::string for the indent factor to prepend to each
   *             generated code line.
   * @return a vector of strings each of which containing the AspectC++ code
   *         to define a _pointcut_.
   */
  virtual std::vector<std::string> generatePointcuts(std::string indent) = 0;
};
}

#endif // ASPECT_PROJECT_MONITORGENERATOR_H