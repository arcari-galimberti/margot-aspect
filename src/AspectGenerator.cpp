//
// Created by leonardoarcari on 01/06/17.
//

#include "../include/AspectGenerator.h"
#include <algorithm>
#include <fstream>
#include <set>
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
  auto monMap = _parser.parseMonitor();
  auto gtMap = _parser.parseGoalTuner();
  auto stMap = _parser.parseStateTuner();

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

  // Emit code for mandatory aspect
  generatedAspect << "\naspect GeneralAspect {\n"
                  << ind << mainPointcut << '\n'
                  << ind << mainAdvice << "\n};\n";

  auto blockNames = std::vector<std::string>();

  for (auto &kv : monMap) {
    if (std::find(blockNames.begin(), blockNames.end(), kv.first) !=
        blockNames.end()) {
      blockNames.push_back(kv.first);
    }
  }

  for (auto &kv : gtMap) {
    if (std::find(blockNames.begin(), blockNames.end(), kv.first) !=
        blockNames.end()) {
      blockNames.push_back(kv.first);
    }
  }

  for (auto &kv : stMap) {
    if (std::find(blockNames.begin(), blockNames.end(), kv.first) !=
        blockNames.end()) {
      blockNames.push_back(kv.first);
    }
  }

  std::sort(blockNames.begin(), blockNames.end());

  // Emit aspect code for each block name
  for (auto &bn : blockNames) {
    generatedAspect << "\n\naspect " << bn << "Aspect {\n";

    // Emit code for MonitorGenerators
    if (monMap.count(bn)) {
      for (auto &mg : monMap[bn]) {
        for (auto &pointcut : mg->generatePointcuts(ind)) {
          generatedAspect << "\n" << pointcut;
        }
        for (auto &advice : mg->generateAdvices(ind)) {
          generatedAspect << "\n" << advice << '\n';
        }
      }
    }

    // Emit code for GoalTuners
    if (gtMap.count(bn)) {
      for (auto &gt : gtMap[bn]) {
        for (auto &pointcut : gt->generatePointcuts(ind)) {
          generatedAspect << "\n" << pointcut;
        }
        for (auto &advice : gt->generateAdvices(ind)) {
          generatedAspect << "\n" << advice << '\n';
        }
      }
    }

    // Emit code for StateTuners
    if (stMap.count(bn)) {
      for (auto &st : stMap[bn]) {
        for (auto &pointcut : st->generatePointcuts(ind)) {
          generatedAspect << "\n" << pointcut;
        }
        for (auto &advice : st->generateAdvices(ind)) {
          generatedAspect << "\n" << advice << '\n';
        }
      }
    }
    generatedAspect << "\n};\n";
  }

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
  auto goalTuners = _parser.parseGoalTuner();
  auto stateTuners = _parser.parseStateTuner();

  auto gh = std::ostringstream();

  // Emit leading code
  gh << generatedIntro << "\n\n"
                          "#ifndef GENERATED_MARGOT_ASPECT_HEADERS_H\n"
                          "#define GENERATED_MARGOT_ASPECT_HEADERS_H\n\n"
                          "#include <margot.hpp>\n\n";

  // Emit code for goal tuners
  for (auto &kv : goalTuners) {
    for (auto &gt : kv.second) {
      gh << gt->generateGoalTuner("");
    }
  }


  // Emit code for state tuners
  for (auto &kv : stateTuners) {
    for (auto &st : kv.second) {
      gh << st->generateStateTuner("");
    }
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
