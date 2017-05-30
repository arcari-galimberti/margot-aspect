//
// Created by Andrea Galimberti on 30/05/2017.
//

#ifndef ASPECT_PROJECT_MONITORGENERATOR_H
#define ASPECT_PROJECT_MONITORGENERATOR_H

#include "AdviceGenerator.h"

namespace ag {

    class MonitorGenerator : public AdviceGenerator {
    public:
        MonitorGenerator(const std::string &functionName, const std::string &returnType,
                         const std::vector<Argument> &arguments);

        std::string generateCode() override;
    };

}

#endif //ASPECT_PROJECT_MONITORGENERATOR_H
