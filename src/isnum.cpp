#include <regex>
#include "isnum.hpp"

static std::wregex numRegex(L"([+-]?(\\d+(\\.\\d*)?|\\.\\d+)([eE][+-]?\\d+)?)");

bool isNumber(const std::wstring& str) {
    return std::regex_match(str, numRegex);
}