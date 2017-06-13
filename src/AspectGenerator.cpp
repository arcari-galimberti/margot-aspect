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

AspectGenerator::AspectGenerator(const std::string &xmlPath,
                                 const std::string &outputPath)
    : _xmlPathname(xmlPath), _outputPathname(outputPath), _parser(xmlPath) {}

AspectGenerator::AspectGenerator(const AspectGenerator &oag)
    : _xmlPathname(oag._xmlPathname), _parser(oag._parser),
      _outputPathname(oag._outputPathname), _generatedAspect(),
      _generatedHeaders() {}

void AspectGenerator::outputPathname(const std::string &outputPath) {
  AspectGenerator::_outputPathname = outputPath;
}

const std::string &AspectGenerator::generateAspect() {
  auto monitorGenerators = _parser.parseMonitorGenerators();
  auto selfTuneGens = _parser.parseSelfTuneGenerators();

  auto generatedAspect = std::ostringstream();
  auto mainPointcut =
      std::string("pointcut main_exec() = execution(\"int main(...)\");");
  auto mainAdvice = std::string("advice main_exec() : before() {\n"
                                "    margot::init();\n"
                                "  }");

  // Emit code for header
  generatedAspect << generatedIntro << "\n\n"
                                       "#ifndef GENERATED_MARGOT_ASPECT_AH\n"
                                       "#define GENERATED_MARGOT_ASPECT_AH\n\n"
                                       "#include \"margotAspect.h\"\n"
                                       "#include <margot.hpp>\n";

  // Emit code for pointcuts
  generatedAspect << "\n" << mainPointcut;
  for (auto &ag : monitorGenerators) {
    for (auto &pointcut : ag->generatePointcuts("")) {
      generatedAspect << "\n" << pointcut;
    }
  }

  for (auto &st : selfTuneGens) {
    for (auto &pointcut : st->generatePointcuts("")) {
      generatedAspect << "\n" << pointcut;
    }
  }

  generatedAspect << "\n\naspect MargotAspect {\n";

  // Emit code for advices
  for (auto &ag : monitorGenerators) {
    for (auto &advice : ag->generateAdvices(ind)) {
      generatedAspect << "\n" << advice << '\n';
    }
  }

  for (auto &st : selfTuneGens) {
    for (auto &advice : st->generateAdvices(ind)) {
      generatedAspect << "\n" << advice << '\n';
    }
  }

  generatedAspect << "\n" << ind << mainAdvice << '\n';
  generatedAspect << "\n};\n";

  // Emit trailing code
  generatedAspect << "\n#endif";

  _generatedAspect = generatedAspect.str();
  return _generatedAspect;
}

void AspectGenerator::writeAspect() const {
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

void AspectGenerator::writeHeaders() const {
  using ios = std::ios_base;
  auto hPathname = std::string("margotAspect.h");

  auto of = std::fstream(hPathname, ios::trunc | ios::out);
  if (!of.is_open()) {
    throw std::ios_base::failure("Failed to open " + hPathname);
  }

  of << _generatedHeaders;
  if (!of) {
    throw std::ios_base::failure("Failed to write " + hPathname);
  }
}

const std::string &AspectGenerator::generateHeaders() {
  auto selfTuneGen = _parser.parseSelfTuneGenerators();
  auto gh = std::ostringstream();

  // Emit leading code
  gh << generatedIntro << "\n\n"
                          "#ifndef GENERATED_MARGOT_ASPECT_HEADERS_H\n"
                          "#define GENERATED_MARGOT_ASPECT_HEADERS_H\n\n"
                          "#include <margot.hpp>\n\n";

  // Emit code for goal tuners
  for (auto &st : selfTuneGen) {
    gh << st->generateGoalTuner("");
  }

  // Emit trailing code
  gh << "\n\n#endif";
  _generatedHeaders = gh.str();

  return _generatedHeaders;
}

void generateAspect(const std::string &xmlPathname,
                    const std::string &outputPathname) {
  auto ag = AspectGenerator(xmlPathname, outputPathname);

  auto genAsp = ag.generateAspect();
  if (!genAsp.empty()) {
    ag.writeAspect();
  }

  auto genHead = ag.generateHeaders();
  if (!genHead.empty()) {
    ag.writeHeaders();
  }
}
}
