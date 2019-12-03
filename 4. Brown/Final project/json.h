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
      return std::get<int64_t>(*this);
    }
    double AsDouble() const {
      std::cerr << "Double problem" << std::endl;
      return std::get<double>(*this);
    }
    double AsBool() const {
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
