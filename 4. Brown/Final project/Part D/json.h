#pragma once

#include <istream>
#include <map>
#include <string>
#include <variant>
#include <vector>
#include <iostream>

namespace Json {

  class Node : std::variant<std::vector<Node>,
                            std::map<std::string, Node>,
                            int64_t,
                            double,
                            bool,
                            std::string> {
  public:
    using variant::variant;

    const auto& AsArray() const {
      return std::get<std::vector<Node>>(*this);
    }
    const auto& AsMap() const {
      return std::get<std::map<std::string, Node>>(*this);
    }
    double AsInt64() const {
      return static_cast<int64_t>(std::get<double>(*this));
    }
    double AsDouble() const {
      /*if (auto check = std::get_if<double>(this)) {
        return *check;
      }
      if (auto check = std::get_if<int64_t>(this)) {
        return static_cast<double>(*check);
      }*/
      return std::get<double>(*this);
    }

    bool AsBool() const {
      return std::get<bool>(*this);
    }
    const auto& AsString() const {
      return std::get<std::string>(*this);
    }
  };

  class Document {
  public:
    explicit Document(Node root);

    const Node& GetRoot() const;

  private:
    Node root;
  };

  Document Load(std::istream& input);
}
