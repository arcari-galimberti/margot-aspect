//
// Created by Andrea Galimberti on 30/05/2017.
//

#ifndef ASPECT_PROJECT_ASPECTPARSER_H
#define ASPECT_PROJECT_ASPECTPARSER_H

#include <string>
#include <pugixml.hpp>
#include "AdviceGenerator.h"

namespace ag {

    class AspectParser {
    public:
        using AdvGenPtr = std::unique_ptr<AdviceGenerator>;

        explicit AspectParser(const std::string &pathname);
        std::vector<AdvGenPtr> parseAdviceGenerators() const;
    private:
        std::string pathname;
        pugi::xml_document aspect;
    };

}


#endif //ASPECT_PROJECT_ASPECTPARSER_H
