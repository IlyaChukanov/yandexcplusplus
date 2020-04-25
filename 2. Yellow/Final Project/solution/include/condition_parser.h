#ifndef SECONDTWEEK_YELLOW_FINAL_PROJECT_CONDITION_PARSE_H_
#define SECONDTWEEK_YELLOW_FINAL_PROJECT_CONDITION_PARSE_H_

#include "node.h"

#include <memory>
#include <iostream>

Date ParseDate(std::istream& input);

std::string ParseEvent(std::istream& input);

std::shared_ptr<Node> ParseCondition(std::istream& is);

#endif //SECONDTWEEK_YELLOW_FINAL_PROJECT_CONDITION_PARSE_H_
