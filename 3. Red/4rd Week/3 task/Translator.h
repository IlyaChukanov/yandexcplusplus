//
// Created by ilya on 04.10.2019.
//

#ifndef YANDEXCPLUSPLUS_3_RED_4RD_WEEK_3_TASK_TRANSLATOR_H_
#define YANDEXCPLUSPLUS_3_RED_4RD_WEEK_3_TASK_TRANSLATOR_H_

#include <string_view>
#include <string>
#include <map>
#include <deque>
#include <vector>

class Translator {
 public:

  Translator() = default;

  void Add(std::string_view source, std::string_view target) {
    const std::string_view source_view = GetClonedView(source);
    const std::string_view target_view = GetClonedView(target);
    forward_translates[source_view] = std::string_view(target_view);
    backward_translates[target_view] = std::string_view(source_view);
  }

  std::string_view TranslateForward(std::string_view source) const {
    auto find = forward_translates.find(std::string(source));
    if (find == forward_translates.end()) {
      return "";
    }
    return find->second;
  }

  std::string_view TranslateBackward(std::string_view target) const {
    auto find = backward_translates.find(std::string(target));
    if (find == backward_translates.end()) {
      return "";
    }
    return find->second;
  }

 private:

  std::string_view GetClonedView(std::string_view s) {
    for (const auto* map_ptr : {&forward_translates, &backward_translates}) {
      const auto it = map_ptr->find(s);
      if (it != map_ptr->end()) {
        return it->first;
      }
    }
    return data.emplace_back(s);
  }

  static std::string_view Translate(const std::map<std::string_view, std::string_view>& dict, std::string_view s) {
    if (const auto it = dict.find(s); it != dict.end()) {
      return it->second;
    } else {
      return {};
    }
  }

  std::deque<std::string> data;
  std::map<std::string_view , std::string_view> forward_translates;
  std::map<std::string_view, std::string_view> backward_translates;
};
#endif //YANDEXCPLUSPLUS_3_RED_4RD_WEEK_3_TASK_TRANSLATOR_H_
