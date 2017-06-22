//
// Created by leonardoarcari on 22/06/17.
//

#ifndef ASPECT_PROJECT_MONITORGENERATOR_H
#define ASPECT_PROJECT_MONITORGENERATOR_H

#include <string>
#include <vector>

namespace ag {
class MonitorGenerator {
public:
  virtual std::vector<std::string> generateAdvices(std::string indent) = 0;
  virtual std::vector<std::string> generatePointcuts(std::string indent) = 0;
};
}

#endif //ASPECT_PROJECT_MONITORGENERATOR_H
