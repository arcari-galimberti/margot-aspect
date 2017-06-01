//
// Created by Andrea Galimberti on 30/05/2017.
//

#ifndef ASPECT_PROJECT_ADVICEGENERATOR_H
#define ASPECT_PROJECT_ADVICEGENERATOR_H

#include <string>
#include <vector>

namespace ag {

class Argument {
public:
  Argument(const std::string &type, const std::string &name);

  const inline std::string &type() const { return _type; }
  const inline std::string &name() const { return _name; }

private:
  std::string _type;
  std::string _name;
};

class AdviceGenerator {
public:
  AdviceGenerator(const std::string &functionName,
                  const std::string &returnType,
                  const std::vector<Argument> &arguments);
  virtual std::string generateCode() = 0;

protected:
  std::string _functionName;
  std::string _returnType;
  std::vector<Argument> _arguments;
};
}

#endif // ASPECT_PROJECT_ADVICEGENERATOR_H
