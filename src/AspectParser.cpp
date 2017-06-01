//
// Created by Andrea Galimberti on 30/05/2017.
//

#include "../include/AspectParser.h"
#include "../include/MonitorGenerator.h"
#include <iostream>

namespace ag {

AspectParser::AspectParser(const std::string &pathname) : _pathname(pathname) {
  auto result = _aspect.load_file(_pathname.c_str());
  if (!result) {
    std::cout << "XML " << _pathname
              << " parsing error\n  Description: " << result.description()
              << '\n';
  }
}

std::vector<AspectParser::AdvGenPtr>
AspectParser::parseAdviceGenerators() const {
  auto generators = std::vector<AspectParser::AdvGenPtr>();
  auto aspectNode = _aspect.child("aspect");
  for (auto advice : aspectNode.children("advice")) {
    auto type = advice.attribute("type").value();
    auto functionName = advice.child("function-name").text();
    auto returnType = advice.child("return-type").text();
    auto arguments = std::vector<Argument>();
    for (auto argument : advice.children("argument")) {
      auto argType = argument.child("type").text();
      auto name = argument.child("name").text();
      arguments.push_back(Argument(argType.as_string(), name.as_string()));
    }

    if (type == "monitor") {
      generators.push_back(std::make_unique<MonitorGenerator>(
          functionName.as_string(), returnType.as_string(),
          std::move(arguments)));
    }
  }
  return generators;
}
}