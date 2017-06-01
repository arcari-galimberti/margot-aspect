//
// Created by Andrea Galimberti on 30/05/2017.
//

#include "../include/AdviceGenerator.h"

namespace ag {
Argument::Argument(const std::string &type, const std::string &name)
    : _type(type), _name(name) {}

AdviceGenerator::AdviceGenerator(const std::string &functionName,
                                 const std::string &returnType,
                                 const std::vector<Argument> &arguments)
    : _functionName(functionName), _returnType(returnType), _arguments(arguments) {
}
}
