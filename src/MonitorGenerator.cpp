//
// Created by Andrea Galimberti on 30/05/2017.
//

#include "../include/MonitorGenerator.h"

ag::MonitorGenerator::MonitorGenerator(const std::string &functionName, const std::string &returnType,
                                       const std::vector<ag::Argument> &arguments)
        : AdviceGenerator(functionName, returnType, arguments) {}

std::string ag::MonitorGenerator::generateCode() {
    return nullptr;
}
