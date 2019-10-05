#include <string>
//#include "../../../MyUtils/MyTestFramework/TestFramework.h"
//#include "../../../MyUtils/Profiler/profiler.h"
//#include "TextEditor.h"

#include "test_runner.h"
#include <list>

using namespace std;

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

void TypeText(Editor& editor, const string& text) {
  for(char c : text) {
    editor.Insert(c);
  }
}

void TestEditing() {
  {
    Editor editor;
    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");
    for(size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    editor.Cut(first_part_len);
    for(size_t i = 0; i < text_len - first_part_len; ++i) {
      editor.Right();
    }
    TypeText(editor, ", ");
    editor.Paste();
    editor.Left();
    editor.Left();
    editor.Cut(3);
    
    ASSERT_EQUAL(editor.GetText(), "world, hello");
  }
  {
    Editor editor;
    
    TypeText(editor, "misprnit");
    editor.Left();
    editor.Left();
    editor.Left();
    editor.Cut(1);
    editor.Right();
    editor.Paste();
    
    ASSERT_EQUAL(editor.GetText(), "misprint");
  }
}

void TestReverse() {
  Editor editor;

  const string text = "esreveR";
  for(char c : text) {
    editor.Insert(c);
    editor.Left();
  }
  
  ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
  Editor editor;
  ASSERT_EQUAL(editor.GetText(), "");
  
  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");
  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestEditing);
  RUN_TEST(tr, TestReverse);
  RUN_TEST(tr, TestNoText);
  RUN_TEST(tr, TestEmptyBuffer);
  return 0;
}