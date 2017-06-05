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
constexpr char AspectGenerator::mainPointcut[];
constexpr char AspectGenerator::mainAdvice[];

AspectGenerator::AspectGenerator(const std::string &xmlPath,
                                 const std::string &outputPath)
    : _xmlPathname(xmlPath), _outputPathname(outputPath), _parser(xmlPath) {}

void AspectGenerator::outputPathname(const std::string &outputPath) {
  AspectGenerator::_outputPathname = outputPath;
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
  generatedAspect << "\n" << mainPointcut;
  for (auto &ag : adviceGenerators) {
    for (auto &pointcut : ag->generatePointcuts()) {
      generatedAspect << "\n" << pointcut;
    }
    generatedAspect << "\n\n"
                       "aspect MargotAspect {\n";
    generatedAspect << "\n\n" << ind << mainAdvice;
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
AspectGenerator::AspectGenerator(const AspectGenerator &oag)
    : _xmlPathname(oag._xmlPathname), _parser(oag._parser),
      _outputPathname(oag._outputPathname), _generatedAspect() {}

void generateAspect(const std::string &xmlPathname,
                    const std::string &outputPathname) {
  auto ag = AspectGenerator(xmlPathname, outputPathname);

  auto gend = ag.generateAspect();
  if (!gend.empty()) {
    ag.writeOnOutput();
  }
}
}
