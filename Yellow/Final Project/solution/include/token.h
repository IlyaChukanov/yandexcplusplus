#ifndef SECONDTWEEK_YELLOW_FINAL_PROJECT_TOKEN_H_
#define SECONDTWEEK_YELLOW_FINAL_PROJECT_TOKEN_H_

#include <sstream>
#include <vector>

enum class TokenType {
  DATE,
  EVENT,
  COLUMN,
  LOGICAL_OP,
  COMPARE_OP,
  PAREN_LEFT,
  PAREN_RIGHT,
};

struct Token {
  const std::string value;
  const TokenType type;
};

std::vector<Token> Tokenize(std::istream& cl);


#endif //SECONDTWEEK_YELLOW_FINAL_PROJECT_TOKEN_H_

