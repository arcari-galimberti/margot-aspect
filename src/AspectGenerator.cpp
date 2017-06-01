//
// Created by leonardoarcari on 01/06/17.
//

#include "../include/AspectGenerator.h"
#include <algorithm>
#include <fstream>
#include <sstream>

namespace ag {

// For linkage purpose
constexpr char AspectGenerator::ind[];
constexpr char AspectGenerator::generatedIntro[];

AspectGenerator::AspectGenerator(const std::string &_xmlPathname,
                                 const std::string &_outputPathname)
    : _xmlPathname(_xmlPathname), _outputPathname(_outputPathname),
      _parser(_xmlPathname) {}

void AspectGenerator::outputPathname(const std::string &_outputPathname) {
  AspectGenerator::_outputPathname = _outputPathname;
}

const std::string &AspectGenerator::generateAspect() {
  auto adviceGenerators = _parser.parseAdviceGenerators();
  auto generatedAspect = std::ostringstream();

  // Emit code for header
  generatedAspect << generatedIntro << "\n\n"
                                       "#ifndef GENERATED_MARGOT_ASPECT_AH\n"
                                       "#define GENERATED_MARGOT_ASPECT_AH\n\n"
                                       "#include <margot.hpp>\n";

  // Emit code for pointcuts
  for (auto &ag : adviceGenerators) {
    for (auto &pointcut : ag->generatePointcuts()) {
      generatedAspect << "\n" << pointcut;
    }
    generatedAspect << "\n\n"
                       "aspect MargotAspect {\n";
    for (auto &advice : ag->generateAdvices()) {
      generatedAspect << "\n\n" << ind << advice;
    }
    generatedAspect << "\n};\n";
  }

  // Emit trailing code
  generatedAspect << "\n#endif";

  _generatedAspect = generatedAspect.str();
  return _generatedAspect;
}

void AspectGenerator::writeOnOutput() {
  using ios = std::ios_base;

  auto of = std::fstream(_outputPathname, ios::trunc | ios::out);
  if (!of.is_open()) {
    throw std::ios_base::failure("Failed to open " + _outputPathname);
  }

  of << _generatedAspect;
  if (!of) {
    throw std::ios_base::failure("Failed to write " + _outputPathname);
  }
}
}
