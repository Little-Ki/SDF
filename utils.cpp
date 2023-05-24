#include "utils.h"

std::u32string Utils::utf8Unicode(const std::string& str ) {
  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
  return converter.from_bytes(str);
}


std::string Utils::loadFile(std::string& path) {
  std::ifstream stream(path);
  std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
  return str;
}