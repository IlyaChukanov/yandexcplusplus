//
// Created by ilya on 13.11.2019.
//

#include "ini.h"
#include <iostream>

namespace Ini {

std::unordered_map<std::string, std::string> & Document::AddSection(std::string name) {
  return sections[name];
}

const std::unordered_map<std::string, std::string> & Document::GetSection(const std::string &name) const {
  return sections.at(name);
}

size_t Document::SectionCount() const {
  return sections.size();
}

Document Load(std::istream& input) {
  Document doc;
  Section* curr;
  for(std::string line; std::getline(input, line);) {
    if (line.empty()) continue;
    if (line[0] == '[') {
      auto section_name = line.substr(1, line.size() - 2);
      curr = &doc.AddSection(section_name);
    }
    else {
      auto eq_char = line.find('=');
      auto name = line.substr(0, eq_char);
      auto value = line.substr(eq_char + 1);
      curr->insert({name, value});
    }
  }
  return doc;
}

}
