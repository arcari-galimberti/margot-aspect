//
// Created by Andrea Galimberti on 30/05/2017.
//

#ifndef ASPECT_PROJECT_ASPECTPARSER_H
#define ASPECT_PROJECT_ASPECTPARSER_H

#include "AdviceGenerator.h"
#include "SelfTuneGenerator.h"

#include <memory>
#include <pugixml.hpp>
#include <string>

namespace ag {

class AspectParser {
public:
  using AdvGenPtr = std::unique_ptr<AdviceGenerator>;
  using STGenPtr = std::unique_ptr<SelfTuneGenerator>;

  explicit AspectParser(const std::string &pathname);
  AspectParser(const AspectParser &oap);
  std::vector<AdvGenPtr> parseAdviceGenerators() const;
  std::vector<STGenPtr> parseSelfTuneGenerators() const;

private:
  std::string _pathname;
  pugi::xml_document _aspect;
};
}

#endif // ASPECT_PROJECT_ASPECTPARSER_H
