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

        friend class AdviceGenerator;

    private:
        std::string type;
        std::string name;
    };

    class AdviceGenerator {
    public:
        AdviceGenerator(const std::string &functionName, const std::string &returnType,
                        const std::vector<Argument> &arguments);
        virtual std::string generateCode()=0;

    protected:
        std::string functionName;
        std::string returnType;
        std::vector<Argument> arguments;

    };


}

#endif //ASPECT_PROJECT_ADVICEGENERATOR_H
