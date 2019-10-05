//
// Created by ilya on 05.10.2019.
//

#ifndef YANDEXCPLUSPLUS_3_RED_4RD_WEEK_5_TASK_TEXTEDITOR_H_
#define YANDEXCPLUSPLUS_3_RED_4RD_WEEK_5_TASK_TEXTEDITOR_H_

#include <list>

class Editor {
 public:
  // Реализуйте конструктор по умолчанию и объявленные методы
  Editor() {
    carriage_ = raw_data_.begin();
  }

  void Left() {
    if (carriage_ != raw_data_.begin()) {
      --carriage_;
    }
  }

  void Right() {
    if (carriage_ != raw_data_.end()) {
      ++carriage_;
    }
  }

  void Insert(char token) {
    raw_data_.insert(carriage_, token);
  }

  void Cut(size_t tokens = 1) {
    tokens = CheckSize(tokens);
    std::list<char>::iterator first_area = carriage_, end_area = carriage_;
    for (size_t i = 0; i < tokens; ++i) {
      end_area++;
      Right();
    }
    bufer_.clear();
    if (first_area != end_area) {
      bufer_.splice(bufer_.begin(), raw_data_, first_area, end_area);
    }
  }

  void Copy(size_t tokens = 1) {
    tokens = CheckSize(tokens);
    std::list<char>::iterator first_area = carriage_, end_area = carriage_;
    for (size_t i = 0; i < tokens; ++i) {
      end_area++;
    }
    if (first_area == end_area) {
      bufer_.clear();
    }
    else {
      bufer_ = {first_area, end_area};
    }
  }

  void Paste() {
    raw_data_.insert(carriage_, bufer_.begin(), bufer_.end());
  }

  string GetText() const {
    return {raw_data_.begin(), raw_data_.end()};
  }

 private:

  size_t CheckSize(size_t tokens) {
    auto to_end = std::distance(carriage_, raw_data_.end());
    return (tokens > to_end) ? to_end : tokens;
  }

  std::list<char> bufer_;
  std::list<char> raw_data_;
  std::list<char>::iterator carriage_;
};

#endif //YANDEXCPLUSPLUS_3_RED_4RD_WEEK_5_TASK_TEXTEDITOR_H_
