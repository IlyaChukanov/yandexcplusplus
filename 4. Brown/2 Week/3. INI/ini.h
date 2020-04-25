//
// Created by ilya on 13.11.2019.
//

#ifndef YANDEXCPLUSPLUS_4_BROWN_2_WEEK_3_TASK_INI_H_
#define YANDEXCPLUSPLUS_4_BROWN_2_WEEK_3_TASK_INI_H_

#include <string>
#include <unordered_map>

namespace Ini {
using Section = std::unordered_map<std::string, std::string>;

class Document {
 public:
  Section& AddSection(std::string name);
  const Section& GetSection(const std::string& name) const;
  size_t SectionCount() const;

 private:
  std::unordered_map<std::string, Section> sections;
};

Document Load(std::istream& input);
}
#endif //YANDEXCPLUSPLUS_4_BROWN_2_WEEK_3_TASK_INI_H_
