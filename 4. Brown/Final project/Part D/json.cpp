#include "json.h"

using namespace std;

namespace Json {

  Document::Document(Node root) : root(move(root)) {
  }

  const Node& Document::GetRoot() const {
    return root;
  }

  Node LoadNode(istream& input);

  Node LoadArray(istream& input) {
    vector<Node> result;

    for (char c; input >> c && c != ']'; ) {
      if (c != ',') {
        input.putback(c);
      }
      result.push_back(LoadNode(input));
    }

    return Node(move(result));
  }

  Node LoadString(istream& input) {
    string line;
    getline(input, line, '"');
    return Node(move(line));
  }

  Node LoadNumber(istream& input) {
    /*int64_t result_integer = 0;
    double is_neg = 1;
    if (input.peek() == '-') {
      input.get();
      is_neg = -1;
    }
    while (isdigit(input.peek())) {
      result_integer *= 10;
      result_integer += input.get() - '0';
    }
    if (input.peek() == '.') {
      double result_fraction = 0;
      int64_t divide = 10;
      input.get();
      while (isdigit(input.peek())) {
        result_fraction += (input.get() - '0') * 1.0 / divide;
        divide *= 10;
      }
      // FOR DEBUG
      auto res = static_cast<double>(is_neg * (static_cast<double>(result_integer) + result_fraction));
      return Node(static_cast<double>(is_neg * (static_cast<double>(result_integer) + result_fraction)));
    }
    // FOR DEBUG
    auto res = static_cast<int64_t >(is_neg * result_integer);
    return Node(static_cast<int64_t >(is_neg * result_integer));*/
    double result;
    input >> result;
    return Node(result);
  }

  bool IsLetter(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z');
  }

  Node LoadBool(istream& input) {
    string line;
    while (IsLetter(input.peek())) {
      line.push_back(input.get());
    }
    if (line == "false") {
      return Node(false);
    }
    else {
      return Node(true);
    }
  }

  Node LoadDict(istream& input) {
    map<string, Node> result;

    for (char c; input >> c && c != '}'; ) {
      if (c == ',') {
        input >> c;
      }

      string key = LoadString(input).AsString();
      input >> c;
      result.emplace(move(key), LoadNode(input));
    }

    return Node(move(result));
  }

  Node LoadNode(istream& input) {
    char c;
    input >> c;

    if (c == '[') {
      return LoadArray(input);
    } else if (c == '{') {
      return LoadDict(input);
    } else if (c == '"') {
      return LoadString(input);
    } else if (c == 'f' || c == 't'){
      input.putback(c);
      return LoadBool(input);
    }
    else {
      input.putback(c);
      return LoadNumber(input);
    }
  }

  Document Load(istream& input) {
    return Document{LoadNode(input)};
  }
}

